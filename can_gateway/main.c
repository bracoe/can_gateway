#include <atmel_start.h>
#include "examples/driver_examples.h"

void CAN_0_send_callback(struct can_async_descriptor *const descr)
{
	(void)descr;
}
void CAN_0_receive_callback(struct can_async_descriptor *const descr)
{
	struct can_message msg;
	uint8_t            data[64];
	msg.data = data;
	can_async_read(descr, &msg);
	gpio_toggle_pin_level(Green_led);
	return;
}

void CAN_0_send_message(){
	
}

void CAN_1_send_callback(struct can_async_descriptor *const descr)
{
	(void)descr;
}
void CAN_1_receive_callback(struct can_async_descriptor *const descr)
{
	struct can_message msg;
	uint8_t            data[64];
	msg.data = data;
	can_async_read(descr, &msg);
	return;
}

void CAN_1_send_message(){
	struct can_message msg;
	//struct can_filter  filter;
	uint8_t            send_data[4];
	send_data[0] = 0x00;
	send_data[1] = 0x01;
	send_data[2] = 0x02;
	send_data[3] = 0x03;

	msg.id   = 0x45A;
	msg.type = CAN_TYPE_DATA;
	msg.data = send_data;
	msg.len  = 4;
	msg.fmt  = CAN_FMT_STDID;
	can_async_register_callback(&CAN_1, CAN_ASYNC_TX_CB, (FUNC_PTR)CAN_1_send_callback);
	can_async_enable(&CAN_1);
	
	can_async_write(&CAN_1, &msg);
	
	gpio_toggle_pin_level(Yellow_led);
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	can_async_register_callback(&CAN_0, CAN_ASYNC_RX_CB, (FUNC_PTR)CAN_0_receive_callback);
	
	struct can_filter  filter;
	filter.id   = 0;
	filter.mask = 0;
	
	can_async_set_filter(&CAN_0, 1, CAN_FMT_STDID, &filter);

	/* Replace with your application code */
	while (1) {
		CAN_1_send_message();
		
		delay_ms(100);
	}
}
