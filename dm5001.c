#define INITDATA 	(uint8_t)(0xFF)
#define MINUS 		(uint8_t)(0x01)

enum{
	false = 0,
	true = !false,
	one_pf = 0,
	two_pf = 1,
	three_pf = 2
};

static uint8_t flag_irq = false;
static uint8_t flag_convers = false;

static uint8_t idx = 0;
static uint8_t buf[3] = {0};

//!!!CHANGE UART IRQ!!
/*void USARTx_IRQHandler(void){
if(idx == 3) {
	flag_irq = false;
	flag_convers = true; //get start find data
	idx = 0;
}

if(flag_irq){
	//buf[idx] = UART->DR;
	idx++;
}

/*if(USART->DR == INITDATA || flag_irq == false) 
	flag_irq = true;
}
}*/

float dm5001_data_to_float(uint8_t* buf){

	static const float fp[3] = {1000.0, 100.0, 10.0};
	float float_val;
	int gap_val;

	if(flag_convers){
	uint8_t sign = 0x01 & (buf[0]>>4);	//find sign
	uint8_t point = 0x0F & buf[0];		//find point
	gap_val = (buf[1]>>4) * 1000 + (buf[1] & 0x0F) * 100 + (buf[2]>>4) * 10 + buf[2] & 0x0F;
	float_val = 0.0;
	switch(point){
		case 0x00: float_val = (float) (gap_val / fp[one_pf]); break;
		case 0x01: float_val = (float) (gap_val / fp[two_pf]); break;
		case 0x02: float_val = (float) (gap_val / fp[three_pf]); break;
		case 0x03: float_val = (float) gap_val; break;
		default: break;
	}

	if(sign == MINUS && gap_val < 0) float_val *= -1.0;
	flag_convers = false;
	return float_val;
	}
	else return 0.0;
}
