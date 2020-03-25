#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "rotator.hpp"
#include "net\serial\robosd_serial.hpp"
#include "mexo/mexo_events.h"
uint8_t uart_recv_data = 0;
volatile unsigned int *DWT_CYCCNT = (volatile unsigned int *)0xE0001004; //address of the register
volatile unsigned int *DWT_CONTROL = (volatile unsigned int *)0xE0001000; //address of the register
volatile unsigned int *SCB_DEMCR = (volatile unsigned int *)0xE000EDFC; //address of the register 

#if MEXO_DEBUG_SIGNAL_ENABLE == 1
void mexo_ev_debugSignalOn(void){
	HAL_GPIO_WritePin(TP1_GPIO_Port,TP1_Pin,GPIO_PIN_SET);
}
void mexo_ev_debugSignalOff(void){
	HAL_GPIO_WritePin(TP1_GPIO_Port,TP1_Pin,GPIO_PIN_RESET);
}
#endif

#if MEXO_GUARD_ENABLE == 1
/** приостанавливаем работу mexo*/
void * mexo_ev_guard_enter(void){
	uint32_t prim = __get_PRIMASK();
	__disable_irq();
	return (void *) prim;
}
/** продолжаем работу mexo*/
void mexo_ev_guard_leave(void * _context){
	int32_t prim = (uint32_t)_context;
	if(!prim) 
		__enable_irq();
}
#endif

class uart0 :  public robo::net::iserial{
	public:
			virtual robo_size_t available(void){
				return (huart3.Instance->SR & UART_FLAG_RXNE) == UART_FLAG_RXNE ? 1:0;
			}
			virtual robo_size_t space(void){
				return (huart3.Instance->SR & UART_FLAG_TC) == UART_FLAG_TC ? 1 : 0;
			}
			virtual robo_size_t get(robo_byte_p _data, robo_size_t _max_size){
				if(_max_size>0){
					*_data = (uint8_t) huart3.Instance->DR;
					return 1;
				}else{
					return 0;
				}
			}
			virtual robo_result_t put(robo_byte_p _data, robo_size_t _size){
				if(_size == 1){
					huart3.Instance->DR = *_data ;
					return ROBO_SUCCESS;
				} else {
					return ROBO_ERROR;
				}
			}
			
			virtual uint8_t get(void){
					return (uint8_t) huart3.Instance->DR;
			}
			virtual robo_result_t  put(uint8_t _data){
				huart3.Instance->DR = _data;
				return ROBO_SUCCESS;
			}
			virtual void reset(void){
			}

} uart0_;

void rotator::iperefery::var_attach_(robo_vartable_p _vt){
}

void rotator::iperefery::begin_(void){
	static robo::net::iserial *  serials_[1];
	pwm_max = PWM_PERIOD;
	serials_[0] = &(uart0_);
	serials = serials_;
	serial_count = 1;
	
	*SCB_DEMCR = *SCB_DEMCR | 0x01000000; 
	*DWT_CYCCNT = 0; // reset the counter 
	*DWT_CONTROL = *DWT_CONTROL | 1 ; // enable the counter	
}

void rotator::iperefery::start_(void){
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_4);
	__enable_irq();
}
void rotator::iperefery::prioritet_loop_(void){
}
void rotator::iperefery::permanent_loop_(void){
}
void rotator::iperefery::background_loop_(void){
	/*if(uart0_.available()>0){
		if(uart0_.space()>0){
			uart0_.put(uart0_.get());
		}
	}*/
}

void  rotator::iperefery::pwm_power_boot(void){
	TIM1->CCR1 = 0;
	TIM1->CCR2 = 0;
	TIM1->CCR3 = 0;
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
	HAL_GPIO_WritePin(EN1_GPIO_Port,EN1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(EN2_GPIO_Port,EN2_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(EN3_GPIO_Port,EN3_Pin,GPIO_PIN_SET);
}

void rotator::iperefery::pwm_power_on(void){
}

void rotator::iperefery::pwm_power_off(void){
	TIM1->CCR1 = 0;
	TIM1->CCR2 = 0;
	TIM1->CCR3 = 0;
	HAL_GPIO_WritePin(EN1_GPIO_Port,EN1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(EN2_GPIO_Port,EN2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(EN3_GPIO_Port,EN3_Pin,GPIO_PIN_RESET);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
}
void rotator::iperefery::pwm_power_shutdown(void){
}

void rotator::iperefery::pwm_set( uint32_t  * _duty){
		TIM1->CCR1 = *_duty++;
		TIM1->CCR2 = *_duty++;
		TIM1->CCR3 = *_duty;
}

void rotator::iperefery::pwm_set( int  _duty){
	if(_duty>0){
		TIM1->CCR1 = (uint32_t)_duty;
		TIM1->CCR2 = 0;
	} else {
		TIM1->CCR1 = 0;
		TIM1->CCR2 = (uint32_t)(-_duty);
	}
}


void rotator::iperefery::delay_us(uint32_t _us){
	unsigned int start, current; 
	start = *DWT_CYCCNT;
	_us *=72;
	do{
		current = *DWT_CYCCNT;
	} while((current - start) < _us);
}


void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){	
	mexo_priorityRun();	
	mexo_run_step();
}
