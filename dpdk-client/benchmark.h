#ifndef _MAIN_H_
#define _MAIN_H_

#ifdef RTE_EXEC_ENV_BAREMETAL
#define MAIN _main
#else
#define MAIN main
#endif

/* Following protocol speicific parameters should be same with MEGA */
#define MEGA_PKT_END 0xFFFF
#define MEGA_JOB_GET 0x2
#define MEGA_JOB_SET 0x3
// #define MEGA_JOB_DEL 0x4 // TODO: To be implemented

/* BITS_INSERT_BUF should be same with mega: config->bits_insert_buf */
#define BITS_INSERT_BUF 3  // 2^3 = 8

#define MEGA_MAGIC_NUM_LEN 2
#define MEGA_END_MARK_LEN 2
#define PROTOCOL_TYPE_LEN 2U
#define PROTOCOL_KEYLEN_LEN 2U
#define PROTOCOL_VALLEN_LEN 4U
#define PROTOCOL_HEADER_LEN 8U

#define SET_RESPOND_LEN 2U
#define SET_SUCC 0x13
#define SET_FAIL 0x23

#define GET_RESPOND_TYPE_LEN 2U
#define GET_SUCC 0x12
#define GET_FAIL 0x22

/* ========== Key definitions ========== */

/* if PRELOAD is disabled, the main program should preload locally */
// #define PRELOAD 1

/* Key Distribution, only one can be enabled */
#define DIS_UNIFORM 1
// #define DIS_ZIPF	1

#if defined(DIS_ZIPF)
#define ZIPF_THETA 0.99
#define AFFINITY_MAX_QUEUE 1
/* Core Affinity, zipf distribution needs more cores for calculation */
#define NUM_QUEUE (7U)
#elif defined(DIS_UNIFORM)
#define ZIPF_THETA 0.00
#define AFFINITY_ONE_NODE 1
#define NUM_QUEUE (4U)
#endif

/* Hash Table Load Factor, These should be the same with the main program
 * if PRELOAD is disabled! TODO: avoid mismatches */
#define LOAD_FACTOR 0.0
#define TOTAL_CNT ((((uint64_t)1 << 63) - 1) | ((uint64_t) 1 << 63))//64位全为1
#define QUARTER_CNT (uint64_t)(TOTAL_CNT / NUM_QUEUE)//16位全为1
#define PRELOAD_CNT (uint32_t)(LOAD_FACTOR * TOTAL_CNT)//计算结果是0

#define KEY_LEN 8
#define VALUE_LEN 8
#define SET_LEN (KEY_LEN + VALUE_LEN + 8) //24
#define ETHERNET_MAX_FRAME_LEN 1514

/* choose which workload to use with the above parameters
 * 0 - 100% GET, 1 - 95% GET, 5% SET
 * Only supports 8 byte key/value */
int WORKLOAD_ID = 6;//即最后一种workload

/* 100%GET : 0  Set, 100Get, (42+2) + (12 * 122) = 1508
 * 95% GET : 5  Set, 95 Get, (42+2) + (24*1 + 12*19)*5 = 1304
 * 90% GET : 11 Set, 99 Get, (42+2) + (24*1 + 12*9)*11 = 1496
 * 80% GET : 20 Set, 80 Get, (42+2) + (24*1 + 12*4)*20 = 1484
 * 70% GET : 27 Set, 63 Get, (42+2) + (24*3 + 12*7)*9  = 1448
 * 60% GET : 34 Set, 51 Get, (42+2) + (24*2 + 12*3)*17 = 1472
 * 50% GET : 40 Set, 40 Get, (42+2) + (24*1 + 12*1)*40 = 1484
 */
 /*To do: edit by perfectxhj
value size：8 256 512 768 1024 1280
key size：8 48 88 128 168 208 248
get ratio：0.5 0.6 0.7 0.8 0.9 0.95 1 
头部42+2=44
set: 2+2+4+keysize+valuesize
get: 2+2+keysize
为了减少计算范围，设定更小的范围
value size：8 256 512 768 1024
key size：8 48 88 128 168
get ratio：0.5 0.6 0.7 0.8 0.9 0.95 1 
排列组合的话有125种情况

 */
const unsigned int number_packet_set[8] = {0, 5, 11, 20, 27, 34, 40};//set的个数
const unsigned int number_packet_get[8] = {122, 95, 99, 80, 63, 51, 40};
const unsigned int length_packet[8] = {1508, 1304, 1496, 1484,
                                       1448, 1472, 1484};

#endif /* _MAIN_H_ */
