#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "ctcp_sys.h"
#include "ctcp_utils.h"

#define BW_SCALE 24
#define BBR_SCALE 8
#define BBR_UNIT (1 << BBR_SCALE)
#define BW_UNIT (1 << BW_SCALE)


static const int bbr_high_gain  = BBR_UNIT * 2885 / 1000 + 1;

static const int bbr_drain_gain = BBR_UNIT * 1000 / 2885;

static const int bbr_cwnd_gain  = BBR_UNIT * 2;


struct bbr
{
	uint32_t rtt_cnt;				/*Maintain count of rtts elapsed, if 10 of them have elapsed update the BBR Bandwidth*/
	uint32_t mode;
	uint32_t	min_rtt_us;	        /* min RTT in min_rtt_win_sec window */
	uint32_t	min_rtt_stamp;	        /* timestamp of min_rtt_us */
	uint32_t	probe_rtt_done_stamp;   /* end time for BBR_PROBE_RTT mode */
	uint32_t 	min_rtt_win_sec;	
	int32_t 	rtt_m_win;				/*total time for which min RTT must be considered ,expressed in ms*/
	int32_t		rtt_prop;				/*RTT prop value*/
	uint32_t 	cwnd;					/*Congestion window which is equal to the sender buffer size initially*/
	uint32_t	startup_bw_1;			/*Startup needs to find best of 3 non rate limited,this is the oldest*/
	uint32_t	startup_bw_2;			/*Startup needs to find best of 3 non rate limited,this is the second*/
	uint32_t	startup_bw_3;			/*Startup needs to find best of 3 non rate limited,this is the latest*/	
	uint32_t	btlbw;					/*Bottleneck bandwidth updated every 10 rtt*/	
	float 		current_pacing_gain;	/*Current pacing gain*/
	long		next_packet_send_time;	/*Next packet send time*/
	int 		drain_start_round;		/*Start of drain round*/
	uint32_t 	phase_data_sent;		/*Data sent during the phase in Probe BW*/
	uint32_t	probe_bw_phase;			/*Indicates which phase in probe bw*/
	long		rtt_change_timestamp;	/*indicates when the rtt_prop has been updated*/
	int 		probe_rtt_start_round;	/*Start of Probe_RTT round*/
};

enum bbr_mode {
	BBR_STARTUP,	/* ramp up sending rate rapidly to fill pipe */
	BBR_DRAIN,	/* drain any queue created during startup */
	BBR_PROBE_BW,	/* discover, share bw: pace around estimated bw */
	BBR_PROBE_RTT,	/* cut inflight to min to probe min_rtt */
};


static const float bbr_pacing_gain[] = {
	2.89,	/* probe for more available bw */
	1/2.89,	/* drain queue and/or yield bw to other flows */
	1.25, 0.75,1,	/* cruise at 1.0*bw to utilize pipe, */
	1,1,1,1	/* without creating excess queue... */
};
struct bbr* bbr_init();
void update_bbr_rtt(struct bbr *bbr,long rtt);
float update_bbr_bw(struct bbr *bbr,long rtt,int segment_length);
void bbr_onack_received(struct bbr *bbr);