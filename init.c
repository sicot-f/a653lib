/**
 * @file      init.c
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

#include "apexInit.h"
#include "apex_config.h"

apex_global_config_t global_config = APEX_PARTITION_CONFIG_DEF;
apex_process_config_t APEX_PROCESS_CONFIG[] = APEX_PROCESS_CONFIG_DEF;
apex_sampling_port_config_t APEX_SP_CONFIG[] = APEX_SP_CONFIG_DEF;
apex_queuing_port_config_t APEX_QP_CONFIG[] = APEX_QP_CONFIG_DEF;

int apex_prcs_num = sizeof(APEX_PROCESS_CONFIG)/sizeof(apex_process_config_t);

