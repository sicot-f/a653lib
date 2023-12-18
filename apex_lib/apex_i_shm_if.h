/**
 * @file      apex_shm_if.h
 * @copyright Airbus Defence and Space
 * @author    nicolaus.baer@airbus.com
 * @date      Tue Oct 24 15:53:18 CEST 2023
 * @brief     apex shared memory handling
 * @details
 */

#ifndef __APEX_SHM_IF_H__
#define __APEX_SHM_IF_H__

#include <semaphore.h>

#include "apexInit.h"
#include "apexType.h"
#include "apexSampling.h"


typedef struct {
  int pid;
  int kill;
  int init;
  int go;
  char name[64];
} partition_info_t;

typedef struct {
  int   size;
  char  data[MAX_Q_PORT_SIZE];
} fifo_msg_header_t;


typedef struct {
  char                    QUEUING_PORT_NAME[34];
  unsigned short          PortId;
  int                     init_done;
  int                     max_elem;
  int                     max_size;
  int                     cur_elem;
  int                     tx_next;
  int                     rx_next;
  sem_t                   sem_lock;
  int                     magic_number;
  fifo_msg_header_t       msg[MAX_Q_PORT_ENTRIES];
} t_queuing_port_shm_data;

typedef struct{
  char                     SAMPLING_PORT_NAME[34];
  unsigned short           PortId;
  VALIDITY_TYPE            LAST_MSG_VALIDITY;  /* message validity */
  MESSAGE_SIZE_TYPE        MAX_MESSAGE_SIZE;   /* port size */
  int                      init_done;
  char                     data[MAX_S_PORT_SIZE];  
} t_sampling_port_shm_data;

typedef struct {
  partition_info_t          partition_info[MAX_PARTITION];
  t_queuing_port_shm_data   q_port_data[MAX_Q_PORT];
  t_sampling_port_shm_data  s_port_data[MAX_S_PORT];
} apex_shm_info_t;

/* function declarations */
int apex_shm_init(void);

void *apex_shm_get( int* shm_id, char* key_file, size_t segsize );
int apex_shm_remove( int* shm_id);

//void *apex_shm_get_ptr(void);

partition_info_t *apex_shm_partitions(void);
t_queuing_port_shm_data *apex_shm_qports(void);
t_sampling_port_shm_data *apex_shm_sports(void);

//unsigned int apex_shm_malloc(size_t size);
// void apex_shm_free(void *ptr);

#endif /* __APEX_SHM_IF_H__ */
