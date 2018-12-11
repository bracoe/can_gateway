#include <atmel_start.h>
#include "examples/driver_examples.h"

uint32_t can_test_id = 0x45A;

void CAN_1_send_message(){
	struct can_message msg;
	//struct can_filter  filter;
	uint8_t            send_data[4];
	send_data[0] = 0x41;
	send_data[1] = 0x42;
	send_data[2] = 0x43;
	send_data[3] = 0x44;

	msg.id   = can_test_id;
	msg.type = CAN_TYPE_DATA;
	msg.data = send_data;
	msg.len  = 4;
	msg.fmt  = CAN_FMT_EXTID;
	
	can_async_write(&CAN_1, &msg);
}

void CAN_0_send_message(){
	struct can_message msg;
	//struct can_filter  filter;
	uint8_t            send_data[4];
	send_data[0] = 0x41;
	send_data[1] = 0x42;
	send_data[2] = 0x43;
	send_data[3] = 0x44;

	msg.id   = can_test_id;
	msg.type = CAN_TYPE_DATA;
	msg.data = send_data;
	msg.len  = 4;
	msg.fmt  = CAN_FMT_EXTID;
	
	can_async_write(&CAN_0, &msg);
}

void CAN_0_send_callback(struct can_async_descriptor *const descr)
{
	(void)descr;
	gpio_toggle_pin_level(Yellow_led);
}
void CAN_0_receive_callback(struct can_async_descriptor *const descr)
{
	struct can_message msg;
	uint8_t            data[64];
	msg.data = data;
	can_async_read(descr, &msg);
	gpio_toggle_pin_level(Green_led);
	
	delay_ms(300);
	CAN_1_send_message();
	return;
}

void CAN_1_send_callback(struct can_async_descriptor *const descr)
{
	(void)descr;
	gpio_toggle_pin_level(Yellow_led);
}
void CAN_1_receive_callback(struct can_async_descriptor *const descr)
{
	struct can_message msg;
	uint8_t            data[64];
	msg.data = data;
	can_async_read(descr, &msg);
	gpio_toggle_pin_level(Green_led);
	
	delay_ms(300);
	CAN_0_send_message();
	return;
}



void setup_can(){
	can_async_register_callback(&CAN_1, CAN_ASYNC_TX_CB, (FUNC_PTR)CAN_1_send_callback);
	can_async_enable(&CAN_1);
	
	can_async_register_callback(&CAN_0, CAN_ASYNC_TX_CB, (FUNC_PTR)CAN_0_send_callback);
	can_async_enable(&CAN_0);
	
	can_async_register_callback(&CAN_0, CAN_ASYNC_RX_CB, (FUNC_PTR)CAN_0_receive_callback);
	can_async_register_callback(&CAN_1, CAN_ASYNC_RX_CB, (FUNC_PTR)CAN_1_receive_callback);
	
	
	struct can_filter  filter;
	filter.id   = can_test_id;
	filter.mask = 0;
	can_async_set_filter(&CAN_0, 1, CAN_FMT_EXTID, &filter);
	can_async_set_filter(&CAN_1, 1, CAN_FMT_EXTID, &filter);
	

}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	setup_can();
	
	/* Replace with your application code */
	while (1) {
		if(gpio_get_pin_level(button) == false){
			CAN_1_send_message();	
		}		
		delay_ms(500);
	}
}
