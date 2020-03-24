#ifndef __manmotor_h
#define __manmotor_h
#include "robosd.hpp"


#define CURRENT_ACP_CH_COUNT 2

class rotator {
public:
	class icore{
		friend class rotator;
		robo_time_us_t tick_us_ = 0;
		uint32_t tick_raw_ = 0;
		bool tick_flag_ = false;
		void begin_(void);
		/*
		1 - �������� � ��������� ����������, ���� ������ realtime  ������.
		��������, ������� �������� ���������
		2 - �������� � ���������� �� ADC, ����� �� ������� ��������
		*/
		void prioritet_loop_(void);
		void permanent_loop_(void);
		void background_loop_(void);

		void var_attach_(void);

		icore(void){};
		~icore(void){};
	public:
		robo_vartable_p vartable = 0;
		uint32_t adc_startupPause;
		void tick(void);
		robo_vartable_p find_vt(int id);
	} core;

	class iperefery{
		friend class rotator;
		iperefery(void){};
		~iperefery(void){};
		void var_attach_(robo_vartable_p);
		void begin_(void);
		void start_(void);
		void prioritet_loop_(void);
		void permanent_loop_(void);
		void background_loop_(void);
	public:
		unsigned int  pwm_max = 0; //������������ �������� ��� (� ������ ���� �����������)
		uint32_t adc_data[CURRENT_ACP_CH_COUNT];
		robo::net::iserial ** serials = 0;
		int serial_count=0;
		//������� ���������� ����������. ����� �������� � �� �������������������  ��������� ��������� mexo , ��� ���� ������� ����������� ������� */

		/*����������, ����� �������� � ������� ���������. � ������� _pwm ��������� �������� ���*/
		void pwm_set(uint32_t *  _duty);
		void pwm_set( int  _duty);
		/*���������� ��������, �������� ���������� ��� ������� �� ���������� ���������.
		������ ����� ����������� ��������� ������� ���� � �����, ���� � �������, �� ������ ����� � �����
		*/
		void pwm_power_boot(void);

		/*������� ��������������� ��������� � ����� ������������� ���������� - ���, ��������������� 0
		(������� � ������ ����� ���� ��� ��������� �������� � 50% ���)
		*/
		void pwm_power_on(void);

		/*
		���� ������� ���, ����� ��� ����� ����������� �����, ������ ��� �������� ��� ���������� � ��������� ������
		����� ���������� ������� ��������������� ��������� �������� ����� ��������� � �����, ��� ������ �������� �����
		*/
		void pwm_power_off(void);

		/*
		� ������� ������� ��� ���������� �������
		������ � ���, ��� ������� ��������������� �������� ���������. ������ ����� ��� ��������
		*/
		void pwm_power_shutdown(void);




#if MEXO_SETTINGS_STORE_ENABLE == 1
#define RDK_SETTINGS_STORE 0
		robo_result_t save(uint8_t _bank, uint8_t * _data, robo_size_t _size);
		robo_result_t load(uint8_t _bank, uint8_t * _data, robo_size_t _size);
		robo_result_t store_reset(uint8_t _bank);
#endif

#if BOARD_MEXO_NET_FLOW_ENABLED == 1
		robo_result_t  flow_set_addr(uint8_t _addr);
#endif


		static void delay_us(uint32_t _us);
	} perefery;


	void begin(void){
		perefery.begin_();
		core.begin_();
		core.var_attach_();
		perefery.var_attach_(core.vartable);
		perefery.start_();
	}
	static rotator instance;
	rotator(void) :core(), perefery(){}
private:
	static robo_result_t prioritet_loop_(){
		instance.perefery.prioritet_loop_();
		instance.core.prioritet_loop_();
		return ROBO_SUCCESS;
	}
	static robo_result_t permanent_loop_(){
		instance.perefery.permanent_loop_();
		instance.core.permanent_loop_();
		return ROBO_SUCCESS;
	}
	static robo_result_t background_loop_(){
		instance.perefery.background_loop_();
		instance.core.background_loop_();
		return ROBO_SUCCESS;
	}
	
};


#endif
