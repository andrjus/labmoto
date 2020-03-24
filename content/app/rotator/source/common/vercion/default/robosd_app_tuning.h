#if (!defined(__robo_app_tunning_h)) && defined(__robosd_common_h)
#define __robo_app_tunning_h
#else
#error error of using robo_app_tunning.h
#endif

#define MEXO_SIDE_ENABLED 1

#ifdef IMITATION_MODEL
#define ROBO_DEBUG_LOG_ENABLED 1
#define robo_errlog(format, ...) _robo_log(ROBO_LOG_VERB_ERROR, ROBO_LOG_MASK_DISABLED, format "\r\n\t%s\r\n\t%s - %d", __VA_ARGS__, __FUNCTION__, __FILE__, __LINE__)
#endif

#include "board_common.h"

#if BOARD_FREEMASTER_ENABLED == 1
#define APP_FREEMASTER_ROBO_SERIAL_ENABLED 1
#else
#define APP_FREEMASTER_ROBO_SERIAL_ENABLED 0
#endif

#define APP_TICK_PERIOD_US BOARD_APP_TICK_PERIOD_US
