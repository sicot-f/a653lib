/**
 * @file      prcs_a.c
 * @author    nicolaus.baer@airbus.com
 * @date      Tue Oct 24 15:53:18 CEST 2023
 * @brief     partition main function
 * @details 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//--------------------
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>




#include "apexLib.h"
#include "apexInit.h"
/* #include "apex_config.h" */

/* apex_global_config_t global_config = APEX_PARTITION_CONFIG_DEF; */
/* apex_process_config_t APEX_PROCESS_CONFIG[] = APEX_PROCESS_CONFIG_DEF; */
/* apex_sampling_port_config_t APEX_SP_CONFIG[] = APEX_SP_CONFIG_DEF; */
/* apex_queuing_port_config_t APEX_QP_CONFIG[] = APEX_QP_CONFIG_DEF; */


void PeriodicProcess(void){

  RETURN_CODE_TYPE return_code;
  MESSAGE_SIZE_TYPE length;
  VALIDITY_TYPE     validity;

  APEX_BYTE data_sp_tx[32] = "sp_test1234";
  APEX_BYTE data_sp_rx[32] = "           ";
  APEX_BYTE data_qp_tx[32] = "qp_test1234";
  APEX_BYTE data_qp_rx[32] = "           ";
  
  SAMPLING_PORT_ID_TYPE sp_id_tx;
  SAMPLING_PORT_ID_TYPE sp_id_rx;
  
  QUEUING_PORT_ID_TYPE qp_id_tx;
  QUEUING_PORT_ID_TYPE qp_id_rx;

  GET_SAMPLING_PORT_ID("SP_Prcs_A to Prcs B             ",
		       &sp_id_rx,
		       &return_code);
  GET_SAMPLING_PORT_ID("SP_Prcs_B to Prcs A             ",
		       &sp_id_tx,
		       &return_code);

  GET_QUEUING_PORT_ID("QP_Prcs_B to Prcs A             ",
		      &qp_id_tx,
		      &return_code);
  GET_QUEUING_PORT_ID("QP_Prcs_A to Prcs B             ",
		      &qp_id_rx,
		      &return_code);

  int index = 0;

  setDebug(5);
  
  while (1){

    length = 0;
    index++;

    sprintf((char *)data_sp_tx,"Hello on SP %d",index);
    sprintf((char *)data_qp_tx,"Hello on QP %d",index);

    READ_SAMPLING_MESSAGE(sp_id_rx,
    			  data_sp_rx,
    			  &length,
    			  &validity,
    			  &return_code);
    
    if (validity == VALID && length > 0){
      
      printDebug(3,"Prcs B: SP we got this : >%s< validity >%d< length >%d<\n",(char *)data_sp_rx,validity,length);
      
      WRITE_SAMPLING_MESSAGE(sp_id_tx,          /* sampling port id */
			     data_sp_tx,        /* pointer to data */
			     32,                /* send size */ 
			     &return_code);     /* return code */     
    }
      

    RECEIVE_QUEUING_MESSAGE(qp_id_rx,        
			    0,                /* time out */
    			    data_qp_rx,          /* pointer to data */
    			    &length,          /* received length */
    			    &return_code);    /* return code */
    if(return_code == NO_ERROR && length !=0){
      
      printDebug(3,"Prcs B: QP we got this : >%s<\n",(char *)data_qp_rx);   

      SEND_QUEUING_MESSAGE(qp_id_tx,
			   data_qp_tx,
			   30, // size
			   0, //timeout
			   &return_code);
      
    }
    
    PERIODIC_WAIT(&return_code);
  }
}


int main (int argc, char *argv[]){

  RETURN_CODE_TYPE               return_code;
  PARTITION_STATUS_TYPE          Init_Status;
  RETURN_CODE_TYPE               Init_Process_ret;
  PROCESS_ID_TYPE                Init_periodic_process_ID;
  /*RETURN_CODE_TYPE               Init_Process_start_ret;*/
  PROCESS_ATTRIBUTE_TYPE         periodic_process;
  /*PROCESS_ID_TYPE                Init_aperiodic_process_ID;*/
  
  APEX_INTEGER                   PortId;
  
  int ret_val = 0;

  do {
    //Initialization of partition
    GET_PARTITION_STATUS( &Init_Status, &Init_Process_ret );
  } while (Init_Status.OPERATING_MODE == IDLE);

  /*                    0        1         2         3   */ 
  /*                    01234567890123456789012345678901 */ 
  CREATE_SAMPLING_PORT("SP_Prcs_A to Prcs B             ",
		       32, // Size in Bytes
		       1,  // 0 tx : 1 rx
		       40000000,                     // currently unused
		       &PortId,
		       &return_code
		       );

  /*                    0        1         2         3   */ 
  /*                    01234567890123456789012345678901 */ 
  CREATE_SAMPLING_PORT("SP_Prcs_B to Prcs A             ",
		       32, // Size in Bytes
		       0,  // 0 tx : 1 rx
		       40000000,                     // currently unused
		       &PortId,
		       &return_code
		       );

  /*                   0        1         2         3   */ 
  /*                   01234567890123456789012345678901 */ 
  CREATE_QUEUING_PORT("QP_Prcs_A to Prcs B             ",
  		      32, // Size in Bytes
  		      4,  // number of entries
  		      1,  // 0 tx : 1 rx
  		      FIFO,
  		      &PortId,
  		      &return_code
  		      );

  /*                   0        1         2         3   */ 
  /*                   01234567890123456789012345678901 */ 
  CREATE_QUEUING_PORT("QP_Prcs_B to Prcs A             ",
  		      32, // Size in Bytes
  		      4,  // number of entries
  		      0,  // 0 tx : 1 rx
  		      FIFO,
  		      &PortId,
  		      &return_code
  		      );
  
  //Start initialization
  //InitSamplingPorts();

   //Initialization of the processes
   GET_PARTITION_STATUS( &Init_Status, &Init_Process_ret );
   memset((char*)(&periodic_process.NAME), 0, sizeof(periodic_process.NAME));
   sprintf((char*)(&periodic_process.NAME), "Process B                    ");
   periodic_process.PERIOD = 20000000LL;
   periodic_process.TIME_CAPACITY = 0;
   periodic_process.STACK_SIZE = 0x500000;
   periodic_process.ENTRY_POINT = &PeriodicProcess; //Entrypoint to periodic process
   periodic_process.BASE_PRIORITY = 10 ;
   periodic_process.DEADLINE = SOFT;


   CREATE_PROCESS(&periodic_process, &Init_periodic_process_ID, &Init_Process_ret);

   /* if( Init_Process_ret != NO_ERROR )
      RAISE_APPLICATION_ERROR( APPLICATION_ERROR, errorMsgs[Init_Process_ret], 10, &raiseErrorRet ); */


   //Creation of ports
   /* for(portCnt=0;portCnt<NbOfPorts;portCnt++) */
   /* { */
   /*    CREATE_SAMPLING_PORT(Ports_cfg[portCnt].PortName, */
   /*                         Ports_cfg[portCnt].MsgSize, */
   /*                         Ports_cfg[portCnt].Direction, */
   /*                         Ports_cfg[portCnt].RefreshPeriod, */
   /*                        &(Ports[portCnt].PortID), */
   /*                        &(Ports[portCnt].Create_ret)); */
   /*    Ports[portCnt].size = Ports_cfg[portCnt].MsgSize; */
   /* } */

   //Starting processes
   START(Init_periodic_process_ID, &Init_Process_ret);

   /* if( Init_Process_ret != NO_ERROR )
      RAISE_APPLICATION_ERROR( APPLICATION_ERROR, errorMsgs[Init_Process_ret], 10, &raiseErrorRet ); */


   //Set normal mode
   SET_PARTITION_MODE(NORMAL, &Init_Process_ret);
   /* if( Init_Process_ret != NO_ERROR )
      RAISE_APPLICATION_ERROR( APPLICATION_ERROR, errorMsgs[Init_Process_ret], 10, &raiseErrorRet ); */


  
  printDebug(3,"a653 start prcs (%d)\n",getpid());

  
  while (1){
    apex_act_partition();
    usleep(50);	 
  }
  
  return ret_val;
}
