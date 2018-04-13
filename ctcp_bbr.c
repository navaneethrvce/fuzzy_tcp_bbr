#include "ctcp_bbr.h"


struct bbr* bbr_init()
{
	fprintf(stderr,"BBR initiated\n");
	struct bbr *bbr = malloc(sizeof(struct bbr));
	bbr->rtt_cnt = 0;
	bbr->mode = 1;
	bbr->min_rtt_win_sec = 10;
	bbr->rtt_m_win = 10000;
	bbr->btlbw = 1440;
	bbr->current_pacing_gain = bbr_pacing_gain[0];
	bbr->next_packet_send_time = 0;
	bbr->rtt_prop = 200;
	return bbr;
}

void update_bbr_rtt(struct bbr *bbr,long rtt)
{
	if( bbr->mode  == BBR_PROBE_RTT)
	{
		bbr->rtt_prop = rtt;
	}
	if((current_time() - bbr->rtt_change_timestamp >=bbr->rtt_prop))
	{
		if(bbr->mode == BBR_PROBE_BW)
		{
			fprintf(stderr,"Have to change to Probe_RTT because the flow has been sending for rtt_prop period\n");
			bbr->probe_rtt_start_round = bbr->rtt_cnt;
			bbr->mode = BBR_PROBE_RTT;
		}
	}
	if(rtt < bbr->rtt_prop)
	{
		bbr->rtt_prop = rtt;
		if(bbr->mode == BBR_PROBE_BW)
		{
			fprintf(stderr,"Have to change to Probe_RTT because RTT has decreased\n");
			bbr->probe_rtt_start_round = bbr->rtt_cnt;
			bbr->mode = BBR_PROBE_RTT;
		}
		if(bbr->mode == BBR_PROBE_RTT)
		{
			bbr->mode = BBR_STARTUP;
		}
	}

	fprintf(stderr,"RTT Prop value is %d \n",bbr->rtt_prop);

}

float update_bbr_bw(struct bbr *bbr,long rtt,int segment_length)
{
	bbr->rtt_cnt = bbr->rtt_cnt + 1;
	if(rtt!=0)
	{
		float bw = (float)	segment_length/(float)(rtt);
		fprintf(stderr,"The bandwidth in bps %f\n",bw);
		return bw;
	}
	return 0.0;
}

void bbr_onack_received(struct bbr *bbr)
{
	switch(bbr->mode)
	{
		case BBR_STARTUP:
			fprintf(stderr,"Bandwidth 1 is %d\n",bbr->startup_bw_1);
			if((bbr->rtt_cnt %3==0) && (bbr->startup_bw_1 !=0))
			{
				if(( (bbr->startup_bw_3 - bbr->startup_bw_1)/bbr->startup_bw_1 *100) >25)
				{
					bbr->mode = BBR_DRAIN;
					bbr->drain_start_round = bbr->rtt_cnt;
					return;
				}

			}
			if((bbr->rtt_cnt %3) == 0 &&(bbr->startup_bw_1 == 0))
			{
				bbr->mode = BBR_DRAIN;
				bbr->drain_start_round = bbr->rtt_cnt;
				return;
			}
			else
			bbr->current_pacing_gain = bbr_pacing_gain[0];
			fprintf(stderr,"Have to pace the cwnd\n");
			bbr->cwnd =(float) (bbr->rtt_prop) * bbr->btlbw *bbr_pacing_gain[0];
			return ;

		case BBR_DRAIN:
			if( bbr->rtt_cnt - bbr->drain_start_round >=4)
			{
				fprintf(stderr,"Have to leave drain phase\n");
				bbr->probe_bw_phase =2;
				bbr->phase_data_sent =0;
				bbr->mode = BBR_PROBE_BW;
				return;
			}
			fprintf(stderr,"Drain Phase entered\n");
			bbr->current_pacing_gain = bbr_pacing_gain[1];
			bbr->cwnd =  bbr->rtt_prop * bbr->btlbw * bbr_pacing_gain[1];
			return;

		case BBR_PROBE_BW:
			fprintf(stderr,"Probe bandwidth phase entered \n");
			if(bbr->phase_data_sent >= (bbr->btlbw *bbr->rtt_prop/1000))
			{
				fprintf(stderr,"Changing phase in Probe bandwidth\n");
				if(bbr->probe_bw_phase == 8)
					bbr->probe_bw_phase = 1;
				bbr->probe_bw_phase = bbr->probe_bw_phase +1;
				bbr->phase_data_sent = 0;
			}
			bbr->cwnd =  bbr->rtt_prop * bbr->btlbw * bbr_pacing_gain[5];
			bbr->current_pacing_gain = bbr_pacing_gain[bbr->probe_bw_phase];
			return;

		case BBR_PROBE_RTT:
		if(bbr->rtt_cnt - bbr->probe_rtt_start_round >=4)
		{
			fprintf(stderr,"Have to leave Probe rtt phase \n");
			bbr->probe_bw_phase =2;
			bbr->phase_data_sent =0;
			bbr->mode = BBR_PROBE_BW;
		}
			fprintf(stderr,"Probe RTT phase entered \n");
			bbr->current_pacing_gain = bbr_pacing_gain[5];
			bbr->cwnd = bbr->rtt_prop * bbr->btlbw * bbr_pacing_gain[1];
			return;
	}

}
