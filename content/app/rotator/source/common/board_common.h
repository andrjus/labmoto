#ifndef __board_common_h
#define  __board_common_h
#include "__robosd_head_begin.h"


#define BOARD_FREE_MASTER_DRIVER_TYPE_NONE 0
#define BOARD_FREE_MASTER_DRIVER_TYPE_PROTO_ABONENT 1
#define BOARD_FREE_MASTER_DRIVER_TYPE_DIRRECT 2

//Конфигурация и состав привода - его назначение,  подключённые датчик и т.д.
//Для каждой версии привода- свои
#include "board_options.h"

//До определяем  пропущенные опции


#ifndef BOARD_MEXO_NET_FLOW_ENABLED
#define BOARD_MEXO_NET_FLOW_ENABLED 0
#endif

#ifndef BOARD_TERMINAL_ENABLED
#define BOARD_TERMINAL_ENABLED 0
#endif

#ifndef BOARD_NET_PROTO_SWITCH_ENABLED
#define BOARD_NET_PROTO_SWITCH_ENABLED 0
#endif

#ifndef BOARD_FREE_MASTER_DRIVER_TYPE
#define BOARD_FREE_MASTER_DRIVER_TYPE BOARD_FREE_MASTER_DRIVER_TYPE_NONE
#endif 

#if BOARD_FREE_MASTER_DRIVER_TYPE == BOARD_FREE_MASTER_DRIVER_TYPE_NONE
#define BOARD_FREEMASTER_ENABLED 0
#else
#define BOARD_FREEMASTER_ENABLED 1
#endif

#define BOARD_MOTOR_CURRENT_SENCE_ENABLED 0

#ifndef BOARD_STARTUP_PAUSE_US
#define BOARD_STARTUP_PAUSE_US 100000
#endif

#ifndef BOARD_SERIAL_RESET_TIMEOUT_US
#define BOARD_SERIAL_RESET_TIMEOUT_US 500000
#endif

#include "__robosd_head_end.h"
#endif
