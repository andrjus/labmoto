#include "rotator.hpp"
rotator rotator::instance;
#if BOARD_FREEMASTER_ENABLED == 1
#include "freemaster/freemaster.hpp"
#endif

#define PS_PREFIX_NAME bridge
#define GENERATOR_PREFIX_NAME generator

#define bridge_POWER_SUPPLY_VOLTAGE_ENABLED 1
#define bridge_POWER_SUPPLY_CURRENT_MEASSURY_ENABLED 0

#define bridge_VOLTAGE_MAX_LIM MEXO_SIGNAL_MAX
#define bridge_VOLTAGE_MIN_LIM MEXO_SIGNAL_MAX
#define bridge_VOLTAGE_RAMP_GAIN MEXO_SIGNAL_MAX

#define generator_MODE_VOLTAGE_ENABLED 1
#define generator_MODE_CURRENT_ENABLED 0
#define generator_MODE_AC_VOLTAGE_ENABLED 1
#define generator_AC_VOLTAGE_OFFSET 0
#define generator_AC_VOLTAGE_MAGNITUDE 0
#define generator_AC_VOLTAGE_FREQ 0
#include "mexo\machine\power\generator.inc.h"
#include "mexo\mexo_ps.impl.inc.h"
#include "mexo\mexo_ps_dev.impl.inc.h"
#include "mexo\machine\power\generator.imp.inc.h"


bridge_t bridge;
generator_t generator;
generator_settings_t generator_settings=
#include "mexo\machine\power\generator.settings.inc.h"
;
/*
bridge_settings_t dc_settings =
#include "mexo/mexo_ps.settings.inc.h"
;*/


#define PWM_BRIDGE_BOOT() rotator::instance.perefery.pwm_power_boot()
#define PWM_BRIDGE_ON() rotator::instance.perefery.pwm_power_on()
#define PWM_BRIDGE_OFF()rotator::instance.perefery.pwm_power_off()
#define PWM_BRIDGE_SHUTDOWN() rotator::instance.perefery.pwm_power_shutdown()
#define PWM_BRIDGE_NAME bridge
#define PWM_BRIDGE_SET_PWM rotator::instance.perefery.pwm_set
#define PWM_BRIDGE_PWM_MAX rotator::instance.perefery.pwm_max
#define PWM_BRIDGE_PWM_SOURCE bridge.voltage
#include "mexo\machine\power\pwm_dc_brifge.impl.inc.h"

//to do
#define DEV_NAME generator
#define DEV_FLOW_TYPE_PREFIX generator
#define FLOW_CMD_DEV_SNAPSHOT_QUERY_ENABLED 0
#define FLOW_CMD_DEV_QUERY_VAR_ENABLED 0
#define FLOW_CMD_DEV_SET_WORK_MODE_ENABLED 0
#include "mexo/net/flow/dev.flow.performers.impl.inc.h"

void rotator::icore::begin_(){
	#define VAR_BUF_COUNT 20
	static robo_varreg_p var_table[VAR_BUF_COUNT] = { PLATFORM_STATIC_ZEROS_STRUCT };
	static robo_vartable_t vartable_ = { 0, VAR_BUF_COUNT, var_table };
	vartable = &vartable_;


	DELEGAT_PRIORITY_RUN(rotator::prioritet_loop_);
	DELEGAT_PERMANENT_RUN(rotator::permanent_loop_);
	DELEGAT_BACKGRAUND_RUN(rotator::background_loop_);

	#if MEXO_DEBUG_SIGNAL_ENABLE == 1
	mexo_instance.debugVerb = MEXO_DEBUG_VERB_ALL;
	#endif

	#if BOARD_FREE_MASTER_DRIVER_TYPE == BOARD_FREE_MASTER_DRIVER_TYPE_DIRRECT
	if( rotator::instance.perefery.serial_count > 0 ){
		robo::freemaster::connect( rotator::instance.perefery.serials[0] );
	}
	#endif
	
	bridge_init(&bridge);
	generator_init(&generator, &generator_settings, &bridge, bridge_run );
	generator_dev_flow_init((mexo_dev_p)&generator);
	DEV_ATTACH((mexo_dev_p)&generator);
}

void rotator::icore::tick(void){
	tick_flag_ = true;
	tick_us_ += BOARD_APP_TICK_PERIOD_US;
	tick_raw_++;
}

void rotator::icore::prioritet_loop_(void){
	rotator::instance.core.tick();
#if BOARD_FREEMASTER_ENABLED == 1
	robo::freemaster::recorder();
#endif
}

void rotator::icore::permanent_loop_(void){
#if BOARD_FREEMASTER_ENABLED == 1 
	robo::freemaster::poll();
#endif
}

void  rotator::icore::background_loop_(void){
}


void rotator::icore::var_attach_(void){
}

#if MEXO_EXTERNAL_INIT == 1
void mexo_ev_init(void){
	rotator::instance.begin();
}
#endif

#if MEXO_DELAY_US == 1
void mexo_ev_delay_us(robo_time_us_t _us){
	rotator::iperefery::delay_us(_us);
}
#endif 

