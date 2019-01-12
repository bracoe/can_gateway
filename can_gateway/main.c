#include <atmel_start.h>

void CAN_0_receive_callback(struct can_async_descriptor *const descr)
{
	struct can_message msg;
	uint8_t data[64];
	msg.data = data;
	can_async_read(descr, &msg);	
	can_async_write(&CAN_1, &msg);
	return;
}

void CAN_1_receive_callback(struct can_async_descriptor *const descr)
{
	struct can_message msg;
	uint8_t data[64];
	msg.data = data;
	can_async_read(descr, &msg);	
	can_async_write(&CAN_0, &msg);
	return;
}

void setup_can(){
	can_async_register_callback(&CAN_1, \ 
		CAN_ASYNC_TX_CB, (FUNC_PTR)NULL);
	can_async_enable(&CAN_1);
	
	can_async_register_callback(&CAN_0, \ 
		CAN_ASYNC_TX_CB, (FUNC_PTR)NULL);
	can_async_enable(&CAN_0);
	
	can_async_register_callback(&CAN_0,  \
		CAN_ASYNC_RX_CB, (FUNC_PTR)CAN_0_receive_callback);
	can_async_register_callback(&CAN_1, \
		CAN_ASYNC_RX_CB, (FUNC_PTR)CAN_1_receive_callback);
	
	struct can_filter  filter;
	filter.id   = 0;
	filter.mask = 0;
	can_async_set_filter(&CAN_0, 1, CAN_FMT_EXTID, &filter);
	can_async_set_filter(&CAN_1, 1, CAN_FMT_EXTID, &filter);
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	setup_can();
	
	/* Infinite while */
	while (1) {	
		delay_ms(1);
	}
}
