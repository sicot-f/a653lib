/**
 * @file      apexPartition.h
 * @date      Wed Apr  1 17:08:32 2023
 * @brief     a653 partition
 * @details    
 */

#ifndef APEX_PARTITION
#define APEX_PARTITION

typedef enum {
  IDLE = 0,
  COLD_START = 1,
  WARM_START = 2,
  NORMAL = 3
} OPERATING_MODE_TYPE;

typedef APEX_INTEGER PARTITION_ID_TYPE;
typedef APEX_UNSIGNED NUM_CORES_TYPE;

typedef enum {
  NORMAL_START = 0,
  PARTITION_RESTART = 1,
  HM_MODULE_RESTART = 2,
  HM_PARTITION_RESTART = 3
} START_CONDITION_TYPE;

typedef struct {
  SYSTEM_TIME_TYPE PERIOD;
  SYSTEM_TIME_TYPE DURATION;
  PARTITION_ID_TYPE IDENTIFIER;
  LOCK_LEVEL_TYPE LOCK_LEVEL;
  OPERATING_MODE_TYPE OPERATING_MODE;
  START_CONDITION_TYPE START_CONDITION;
  NUM_CORES_TYPE NUM_ASSIGNED_CORES;
} PARTITION_STATUS_TYPE;

extern void GET_PARTITION_STATUS (
 PARTITION_STATUS_TYPE *PARTITION_STATUS, /* out */
 RETURN_CODE_TYPE *RETURN_CODE );         /* out */

extern void SET_PARTITION_MODE (
 OPERATING_MODE_TYPE OPERATING_MODE, /*in */
 RETURN_CODE_TYPE *RETURN_CODE ); /*out*/

#endif
