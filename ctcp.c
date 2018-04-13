/******************************************************************************
 * ctcp.c
 * ------
 * Implementation of cTCP done here. This is the only file you need to change.
 * Look at the following files for references and useful functions:
 *   - ctcp.h: Headers for this file.
 *   - ctcp_iinked_list.h: Linked list functions for managing a linked list.
 *   - ctcp_sys.h: Connection-related structs and functions, cTCP segment
 *                 definition.
 *   - ctcp_utils.h: Checksum computation, getting the current time.
 *
 *****************************************************************************/

#include "ctcp_sys.h"
#include "ctcp.h"
#include "ctcp_linked_list.h"
#include "ctcp_utils.h"
/**
 * Connection state.
 *
 * Stores per-connection information such as the current sequence number,
 * unacknowledged packets, etc.
 *
 * You should add to this to store other fields you might need.
 */
struct ctcp_state {
  struct ctcp_state *next;  /* Next in linked list */
  struct ctcp_state **prev; /* Prev in linked list */

  conn_t *conn;             /* Connection object -- needed in order to figure
                               out destination when sending */
  linked_list_t *segments;  /* Linked list of segments sent to this connection.
                               It may be useful to have multiple linked lists
                               for unacknowledged segments, segments that
                               haven't been sent, etc. Lab 1 uses the
                               stop-and-wait protocol and therefore does not
                               necessarily need a linked list. You may remove
                               this if this is the case for you */

  /* FIXME: Add other needed fields. */
  int init_seqno;         /*Initial sequence number , set to 1*/
  int curr_seqno;         /*Current Sequnce number*/
  int next_seqno;         /*Next Sequence Number expected*/
  int curr_ackno;         /*Current ACK number*/
  int init_ackno;         /*Initial ACK number,set to 1*/
  int last_ackno;         /*Recently sent out ACK NO*/
  int r_window_size;      /* Reciever Window  size*/
  int s_window_size;      /* Sender Window size*/
  int bytes_sent;         /* Number of Bytes sent*/
  int sliding_window_size; /*the number of bytes on the sliding window*/
  int conn_state;          /*indicates the connection state*/
  int last_timer_call;     /*indicates last time ctcp_timer was called*/
  long min_seen_rtt;       /*indicates the minimum RTT seen*/
  int max_seen_bw;         /*indicates the maximum bw seen*/
  int inflight_data;       /*Indicates the data in flight*/
  int app_limited_until;  /*Indicates if the application is rate limited*/
  long last_packet_send_time; /*Indicates the last time the packet was sent*/
  linked_list_t *unacked_segs; /*linked list of unacked segments*/
  linked_list_t *recv_segs; /*linked list of received segments*/
  linked_list_t *send_q;    /*linked list of segments that have to be sent*/
  FILE *bdp_fp;             /*Filepointer to bdp.txt*/
  ctcp_config_t *cfg;
  struct bbr *bbr;                 /*BBR parameters*/
};

/**
 * Linked list of connection states. Go through this in ctcp_timer() to
 * resubmit segments and tear down connections.
 */
static ctcp_state_t *state_list;

/* FIXME: Feel free to add as many helper functions as needed. Don't repeat
          code! Helper functions make the code clearer and cleaner. */


ctcp_state_t *ctcp_init(conn_t *conn, ctcp_config_t *cfg) {
  /* Connection could not be established. */
  if (conn == NULL) {
    return NULL;
  }

  /* Established a connection. Create a new state and update the linked list
     of connection states. */
  struct ctcp_state *state = calloc(sizeof(ctcp_state_t), 1);
  state->next = state_list;
  state->prev = &state_list;
  if (state_list)
    state_list->prev = &state->next;
  state_list = state;

  /* Set fields. */
  state->conn = conn;
  state->init_seqno = 1;
  state->init_ackno = 1;
  state->curr_seqno = state->init_seqno;
  state->curr_ackno = state->init_ackno;
  state->last_ackno = state->init_ackno;
  state->r_window_size = cfg->recv_window;
  state->s_window_size = cfg->send_window;
  state->bytes_sent=0;
  state->unacked_segs = ll_create();
  state->recv_segs = ll_create();
  state->send_q = ll_create();
  state->conn_state = 0;
  state->sliding_window_size = 0;
  state->cfg = cfg;
  state->next_seqno = state->init_seqno;
  state->inflight_data = 0;
  state->last_timer_call =0;
  state->min_seen_rtt =-1;
  state->max_seen_bw = -1;
  state->app_limited_until=0;
  state->last_packet_send_time =0;
  state->bdp_fp  = fopen("bdp.txt","w");
  /* FIXME: Do any other initialization here. */
  state->bbr = bbr_init();
  state->bbr->cwnd = state->s_window_size;
  state->bbr->mode = BBR_STARTUP;
  return state;
}

void ctcp_destroy(ctcp_state_t *state) {
  /* Update linked list. */
  if (state->next)
    state->next->prev = state->prev;

  *state->prev = state->next;
  conn_remove(state->conn);

  /* FIXME: Do any other cleanup here. */
  //drain_sliding_window(state);
  ll_destroy(state->unacked_segs);
  ll_destroy(state->recv_segs);
  ll_destroy(state->send_q);
  free(state);
  end_client();
}
void size_inflight_data(ctcp_state_t *state)
{
  ll_node_t *curr =  state->unacked_segs->head;
  while(curr!=NULL)
  {
    void **node_buf = curr->object;
    ctcp_segment_t *seg = (ctcp_segment_t*)node_buf[0];
    curr =curr->next;
    if(*((int *)(node_buf[2])) == 1)
    {
      state->inflight_data = state->inflight_data + ntohs(seg->len);
    }
  }
}
void drain_sliding_window(ctcp_state_t *state)
{
  int send_availbility = state->sliding_window_size - state->inflight_data ;
  int bdp = (state->bbr->rtt_prop)* state->bbr->btlbw * 2.89;
  //fprintf(stderr,"The data availbility %d\n",send_availbility);
  //fprintf(stderr,"The BDP is %d\n",bdp);
  if(state->inflight_data >= bdp && bdp!=0)
  {
    fprintf(stderr,"Executing return----------------------------------\n"); 
    return;
  }
  if((ll_length(state->send_q)== 0) && (send_availbility ==0))
  {
    state->app_limited_until = state->inflight_data;
    fprintf(stderr, "application has rate limited \n" );
    return;
  }
  //fprintf(stderr, "Drain sliding window invoked\n" );
  ll_node_t *curr = state->unacked_segs->head;
  while(curr!=NULL)
  {
    void **node_buf = curr->object;
    ctcp_segment_t *seg = (ctcp_segment_t*)node_buf[0];
    //fprintf(stderr,"The value of node_buf[2] is  %d\n",*((int *)(node_buf[2])));
    if(*((int *)(node_buf[2])) == 0)
    {
     
      
       *((long *)(node_buf[1])) = state->last_packet_send_time = current_time();
      
      //fprintf((stderr), "packets sent of size %ld\n",sizeof(ctcp_segment_t)+strlen(seg->data)-1);
      //fprintf(stderr,"Going to be transmitting segment of sequence number  %d in drain_sliding_window\n",ntohs(seg->seqno));
      //fprintf(stderr, "Connection send time %ld\n", current_time());
       if(  state->app_limited_until >0   )
       {
        *((int *)(node_buf[4])) = 1;
        fprintf(stderr, "Application rate limited\n");
      }
        usleep(2000);
        while(1)
        {
          
          if(current_time() >= state->bbr->next_packet_send_time)
          {
             //fprintf(stderr,"-----------------Current time %ld\n",current_time());
             //fprintf(stderr,"-----------------The next packet send time is %ld \n",state->bbr->next_packet_send_time);
             state->bbr->phase_data_sent = state->bbr->phase_data_sent + ntohs(seg->len);
             conn_send(state->conn,seg,ntohs(seg->len));
             break;
          }
          else
          {
            //fprintf(stderr,"-----------------Executing Wait \n");
          }
        }
        *((int *)(node_buf[2])) = 1;
        state->inflight_data = state->inflight_data + ntohs(seg->len);
        if((state->max_seen_bw != -1) && (state->max_seen_bw !=0))
        {
            long now = current_time();
            long len = ntohs(seg->len);
            long btl = state->bbr->btlbw;
            double pac = state->bbr->current_pacing_gain;
            state->bbr->next_packet_send_time = current_time() + len /(pac  * btl);
            fprintf(stderr,"Next packet send time is %ld pacing gain %f , bbw %ld  seglen  %ld, cur time %ld \n", state->bbr->next_packet_send_time, state->bbr->current_pacing_gain, btl,len, now);
        }
        else
            state->bbr->next_packet_send_time = 0;
        //fprintf(stderr,"The next packet send time is %ld \n",state->bbr->next_packet_send_time); 
    }
    curr = curr->next;
  }

}
void sliding_window(ctcp_state_t *state)
{
 // fprintf(stderr,"Send Queue Size %d\n",ll_length(state->send_q));

  // while(state->sliding_window_size > state->s_window_size)
  // {
  //   //fprintf(stderr,"Draining sliding window in sliding_window \n");
  //   drain_sliding_window(state);

  // }
  if(ll_length(state->send_q)>0)
  {
    while((state->sliding_window_size < state->s_window_size) &&(ll_length(state->send_q)>0))
    {
      ll_add(state->unacked_segs,ll_front(state->send_q)->object);
      //fprintf(stderr, "Adding to sliding window\n" );
      void **node_buf = (ll_front(state->send_q)->object);
      ctcp_segment_t *seg = (ctcp_segment_t*)node_buf[0];
      //fprintf(stderr,"The value of node_buf[2] in sliding_window is  %d\n",*((int *)(node_buf[2])));
      state->sliding_window_size = state->sliding_window_size + ntohs(seg->len);
      //fprintf(stderr, "Sliding window size %d\n",state->sliding_window_size);
      ll_remove(state->send_q,ll_front(state->send_q));
    }
  }
  //fprintf(stderr,"Length of the send_q is %d\n",ll_length(state->send_q));
}
void transmit_segment(ctcp_state_t *state,char *data,int data_size,int flags,int req_ack)
{
  ctcp_segment_t *send_segment = (ctcp_segment_t*)calloc(sizeof(ctcp_segment_t) + data_size,1);
  int send_segment_size = sizeof(ctcp_segment_t) + data_size;
  memcpy(send_segment->data,data,data_size);
  if(flags!=-1)
    send_segment->flags = flags;
  send_segment->seqno = htonl(state->curr_seqno);
  send_segment->ackno = htonl(state->curr_ackno);
  send_segment->len = htons(send_segment_size);
  send_segment->window= htons(state->s_window_size);
  send_segment->cksum = 0;
  send_segment->cksum = cksum(send_segment,send_segment_size);
  if(req_ack == 1)
  {
    void **arr = malloc(5 * sizeof(void *));
    arr[0] = malloc(sizeof(ctcp_segment_t*));
    arr[1] = malloc(sizeof(long));
    arr[2] = malloc(sizeof(int));
    arr[3] = malloc(sizeof(int));
    arr[4] = malloc(sizeof(int)); /*Indicate if packet is applimited, 1 if its applimited , 0 if its not*/
    arr[0] = send_segment;
    *((int *)(arr[2])) = 0;
    *((int *)(arr[3])) = 0;
    *((int *)(arr[4])) = 0;
    //fprintf((stderr), "Added to send queue\n" );
    ll_add(state->send_q,arr);
    state->curr_seqno = state->curr_seqno + data_size;
    //fprintf(stderr,"Sliding window invoked\n");
    sliding_window(state);
    drain_sliding_window(state);
    //ll_add(state->unacked_segs,arr);
  }
  else
  {
  usleep(2500);
  conn_send(state->conn,send_segment,send_segment_size);
  state->curr_seqno = state->curr_seqno + data_size;
  free(send_segment);
  free(data);   
}  
}
void ctcp_read(ctcp_state_t *state) {
  /* FIXME */
  char stdin_data[MAX_SEG_DATA_SIZE];
  int stdin_read_size;
  int chunk_iter;
  while(1)
  {
  //fprintf(stderr, "called\n" );
  stdin_read_size = conn_input(state->conn,stdin_data,MAX_SEG_DATA_SIZE);
    if(stdin_read_size > state->s_window_size)
    {
      int stdin_chunk_size =state->s_window_size;
      for(chunk_iter=0;chunk_iter<stdin_read_size;chunk_iter=chunk_iter+stdin_chunk_size)
      {
          transmit_segment(state,stdin_data+chunk_iter,stdin_chunk_size,TH_ACK,1);           
      }
  }
  if ((stdin_read_size == -1))
  {
    
    if(ll_length(state->send_q)!=0)
      return;
    //fprintf(stderr, "Here \n");
    if(state->conn_state ==0)
    {
    //fprintf((stderr), "Send FIN \n" ); 
    transmit_segment(state,NULL,0,TH_FIN,0);
    //state->conn_state = 1; 
    //fprintf(stderr, "Destroying ctcp" );
    //ctcp_destroy(state);
    }
    else
    {
      transmit_segment(state,NULL,0,TH_FIN,0);
      ctcp_destroy(state);
    }
    //fclose(state->bdp_fp);
    return;
  }
  if(stdin_read_size == 0)
    break;
  //fprintf(stderr,"transmit_segment invoked\n");
  
  
  transmit_segment(state,stdin_data,stdin_read_size,TH_ACK,1);
  
}
return;
}

int ctcp_check_corruption(ctcp_segment_t *segment)
{
  int csum = segment->cksum;
  segment->cksum =0;
  //fprintf(stderr, "Received checksum %0x\n",csum);
  //fprintf(stderr,"Received packet len %ld\n",sizeof(ctcp_segment_t)+strlen(segment->data));
  if(csum == cksum(segment,sizeof(ctcp_segment_t)+strlen(segment->data)))
    return 1;
  else return 0;
}

void ctcp_receive(ctcp_state_t *state, ctcp_segment_t *segment, size_t len) {

  //fprintf(stderr,"Received packet of len %d\n",(int)len);
  //print_hdr_ctcp(segment);
  int dup_flag =0;

  /*Check for packet corruption*/
  if(ctcp_check_corruption(segment) == 0)
  {
    fprintf(stderr, "Packet corrupted\n" );
    free(segment);
    return;
  }

  if(((segment->flags & TH_FIN) == TH_FIN) &&(state->conn_state ==0))
  {
    state->conn_state = 1;
    transmit_segment(state,NULL,0,TH_ACK,0);
    conn_output(state->conn,segment->data,0);
    return;
  }
  else if(state->conn_state == 1)
  {
    ctcp_destroy(state);
    return;
  }

  if((segment->flags & TH_ACK) == TH_ACK)
  {

      if(state->conn_state == 1)
      {
        fprintf(stderr, "Destroyed state here\n");
        ctcp_destroy(state);
      }
      while(ll_length(state->unacked_segs)>0)
      {
        //fprintf(stderr, "UNACK queue length %d\n",ll_length(state->unacked_segs));
        ll_node_t *seg_node = ll_front(state->unacked_segs);
        void **node_buf = (seg_node->object);
        ctcp_segment_t *seg = (ctcp_segment_t*)node_buf[0];
        //fprintf(stderr, "Segment stored seqno %d\n", ntohl(seg->seqno));
        //fprintf(stderr, "Segment recievd ackno %d\n", ntohl(segment->ackno));
        if(ntohl(seg->seqno) < ntohl(segment->ackno))
        {
          //fprintf(stderr, "Free\n" );
          //fprintf(stderr, "Packet was sent at %ld\n",*((long*)node_buf[1]));
          //fprintf(stderr,"Current time is %ld\n",current_time());
          fprintf(stderr,"RTT --------------------------------------------------------------------------------------------------------------------  %ld \n",current_time() - *((long*)node_buf[1]));
          long rtt = current_time() - *((long*)node_buf[1]);
          if(state->min_seen_rtt ==-1)
          {
            state->min_seen_rtt = rtt;
            state->bbr->rtt_prop = rtt;
          }
          else
          {
            if(rtt <= state->min_seen_rtt)
            {
              state->min_seen_rtt = rtt;
              if(state->bbr->mode == BBR_PROBE_BW)
              {
                fprintf(stderr,"Have to change to STARTUP\n");
                state->bbr->mode = BBR_STARTUP;
              }
            }
            
          }
          if(state->bbr->rtt_cnt == 0)
          {
            state->max_seen_bw = state->bbr->btlbw = update_bbr_bw(state->bbr,rtt,ntohs(seg->len));  
          }
          else
          {
             if(*((long*)node_buf[4]) ==0)
             {
            fprintf(stderr, "Updating bandwidth\n" ); 
            fprintf(stderr,"state->max_seen_bw %d\n",state->max_seen_bw);
            float bw = update_bbr_bw(state->bbr,rtt,ntohs(seg->len));
            fprintf(state->bdp_fp,"%ld,%ld\n",current_time(),state->bbr->btlbw*rtt*8);
            fflush(state->bdp_fp);
            fprintf(stderr, "bw %f\n",bw );
            if(state->max_seen_bw < bw)
            {
              fprintf(stderr,"Updating values of startup bw\n");
              state->max_seen_bw = bw;
              state->bbr->startup_bw_3 = state->bbr->startup_bw_2;
              state->bbr->startup_bw_2 = state->bbr->startup_bw_1;
              state->bbr->startup_bw_1 = bw;

            }
            if(state->bbr->rtt_cnt %10 == 0)
            {

                if(state->bbr->btlbw < state->max_seen_bw)
                  state->bbr->btlbw = state->max_seen_bw;
            }
            //fprintf(stderr," BBR Bottleneck bandwidth %d\n",state->bbr->btlbw);
          }
          else
          {
            fprintf(stderr,"Application Rate limited \n");
          }
        }
          
          bbr_onack_received(state->bbr);
          fprintf(stderr,"The congestion window is %d\n",state->bbr->cwnd);
          state->s_window_size =state->cfg->send_window= state->bbr->cwnd;
          //fprintf(stderr,"The new congestion window %d\n",state->bbr->cwnd);
          if(state->app_limited_until > 0 )
          {
            state->app_limited_until = state->app_limited_until - ntohs(seg->len);
          }
          state->sliding_window_size = state->sliding_window_size - ntohs(seg->len);
          state->inflight_data = state->inflight_data - ntohs(seg->len);
          free(ll_remove(state->unacked_segs,seg_node));
          if(ll_length(state->send_q) > 0)
          {
            sliding_window(state);
            //fprintf(stderr, "UNACK queue length %d\n",ll_length(state->unacked_segs));
            //fprintf(stderr,"State sliding window size %d\n",state->sliding_window_size);
            drain_sliding_window(state);
          }
        }
        else
        {
          //fprintf(stderr, "Break\n" );
          break;
        }
      }
  }

  int data_len = ntohs(segment->len)-sizeof(ctcp_segment_t);
  state->curr_ackno = ntohl(segment->seqno) + data_len;
  if(ntohl(segment->seqno) <   state->last_ackno)
  {
   fprintf((stderr), "Duplicate Packet Detected\n");
   dup_flag = 1;
  }
  else
  {
    state->last_ackno = state->curr_ackno;
    dup_flag =0;
  }
  char *segment_data_buf = (char*)malloc(sizeof(char)*MAX_SEG_DATA_SIZE);
  strncpy(segment_data_buf,segment->data,data_len);
  //fprintf((stderr), "Comparing segment length  %d  and %ld",ntohs(segment->len),sizeof(ctcp_segment_t));
  if(ntohs(segment->len) > sizeof(ctcp_segment_t))
  { 
    if(dup_flag ==0)
    {
      //fprintf((stderr), "Sending Acknowlegement\n");
      transmit_segment(state,NULL,0,TH_ACK,0);
      conn_output(state->conn,segment_data_buf,data_len);
    }
  }
   free(segment);
   free(segment_data_buf);
}

void ctcp_output(ctcp_state_t *state) {
  //int bytes_avail = conn_bufspace(state->conn);
  /* FIXME */
}

void ctcp_timer() {
  /* FIXME */
  //fprintf(stderr,"Timer caled\n");
  ctcp_state_t *curr_state = state_list;
  while (curr_state!= NULL) {
    //sliding_window(curr_state);
    //drain_sliding_window(curr_state);
    if(curr_state->last_timer_call ==0)
      curr_state->last_timer_call = current_time();
     if(curr_state->bbr->rtt_m_win > 0) 
     {
       curr_state->bbr->rtt_m_win = curr_state->bbr->rtt_m_win - (current_time() - curr_state->last_timer_call);
       curr_state->last_timer_call = current_time();
     }

     if(curr_state->bbr->rtt_m_win <0)
     {
       fprintf(stderr, "Reached end of 1  RTT measurement period\n" );
       update_bbr_rtt(curr_state->bbr,curr_state->min_seen_rtt);
       if(curr_state->min_seen_rtt < curr_state->bbr->rtt_prop)
       {
        curr_state->bbr->rtt_prop = curr_state->min_seen_rtt;
        curr_state->bbr->rtt_change_timestamp = current_time();
      }
       curr_state->bbr->rtt_m_win = 10000;
     }

    ll_node_t *curr_segment = curr_state->unacked_segs->head;
    while(curr_segment!=NULL)
    {
      //fprintf(stderr, "UNACK queue length in timer %d\n",ll_length(curr_state->unacked_segs));
      void **node_buf = (curr_segment->object);
      if((( current_time() - *((long*)node_buf[1]) > curr_state->cfg->rt_timeout)&& (*((int*)node_buf[3])<5)))
      {
        ctcp_segment_t *seg = (ctcp_segment_t*)node_buf[0];
        *((long*)node_buf[1]) = current_time();
        curr_state->sliding_window_size = curr_state->sliding_window_size - ntohs(seg->len);
        *((int*)node_buf[3]) = *((int*)node_buf[3]) + 1;
        //fprintf(stderr, "Sending from UNACK  list \n");
        conn_send(curr_state->conn,seg,ntohs(seg->len));
      }
      if(*((int*)node_buf[3]) >=5)
      {
        ctcp_destroy(curr_state);
        break;
      }
      curr_segment=curr_segment->next;
    }
    curr_state=curr_state->next;
  }
}
