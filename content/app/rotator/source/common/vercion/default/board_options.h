/* определять опции так
#define YOUR_OPTION  YOUR_OPTION_VALUE
*/

#if (!defined(__board_option)) && defined(__board_common_h)
#define __board_option
#else
#error error of using board_option.h
#endif

#define BOARD_APP_TICK_PERIOD_US 20

#define BOARD_FREE_MASTER_DRIVER_TYPE BOARD_FREE_MASTER_DRIVER_TYPE_DIRRECT

#ifndef BOARD_MEXO_FLOW_DEFAULT_ADDRESS
#define BOARD_MEXO_FLOW_DEFAULT_ADDRESS 0x01
#endif

#ifdef IMITATION_MODEL
#include <stdint.h>
extern uintptr_t MODULE_ADDRESS;
#define BOARD_VARTABLE_ADDRESS_OFFSET MODULE_ADDRESS
#undef robo_errlog
#define robo_errlog(format, ...) _robo_log(ROBO_LOG_VERB_ERROR, ROBO_LOG_MASK_DISABLED, format "\r\n\t%s\r\n\t%s - %d", __VA_ARGS__, __FUNCTION__, __FILE__, __LINE__)
#else
#define BOARD_VARTABLE_ADDRESS_OFFSET 0
#endif



