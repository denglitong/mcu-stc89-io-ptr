#include <reg52.h>

void ConfigUART(unsigned int bard);

unsigned char RECEIVE_SIGNATL = 0;
unsigned char IS_SIGNAL_RECEIVED = 0;

unsigned char *BUFFER_PTR = 0;
unsigned char BUFFER_LEN = 0;

unsigned char buffer1[] = {1,2};
unsigned char buffer2[] = {1,2,3,4};
unsigned char buffer3[] = {1,2,3,4,5,6};
unsigned char buffer4[] = {1,2,3,4,5,6,7,8};

void main() {
	EA = 1;
	ConfigUART(9600);

	while (1) {
		if (IS_SIGNAL_RECEIVED) {
			IS_SIGNAL_RECEIVED = 0;
			switch (RECEIVE_SIGNATL) {
				case 1:
					BUFFER_PTR = buffer1;
					BUFFER_LEN = sizeof(buffer1);	 
					// software enable TI interrupt, 
					// will enter interrupt func immediately
					TI = 1;  
					break;
				case 2:		
					BUFFER_PTR = buffer2;
					BUFFER_LEN = sizeof(buffer2);	 
					TI = 1;   
					break;
				case 3:		 
					BUFFER_PTR = buffer3;
					BUFFER_LEN = sizeof(buffer3);	 
					TI = 1;   
					break;
				case 4:		 
					BUFFER_PTR = buffer4;
					BUFFER_LEN = sizeof(buffer4);	 
					TI = 1;   
					break;
				default:
					break;
			}
		}
	}
}

void ConfigUART(unsigned int baud) {
	// SCON 0101 0000
	// SM1=1, REN=1，串行通信模式
	SCON = 0X50;
	TMOD &= 0x0F;
	TMOD |= 0x20;
	TH1 = 256 - (11059200/12/2/16)/baud;
	TL1 = TH1;
	
	ET1 = 0; // 使能T1作为波特率发生器，而禁止T1作为定时器
	ES = 1; // 使能串口中断
	TR1 = 1; // start T1
}

void InterruptUART() interrupt 4 {
	if (RI) { // Receive Interrupt
		RI = 0;
		IS_SIGNAL_RECEIVED = 1;
		RECEIVE_SIGNATL = SBUF;
	}
	if (TI) { // Transmission
		TI = 0;
		if (BUFFER_LEN) {
			BUFFER_LEN--;
			SBUF = *BUFFER_PTR;
			BUFFER_PTR++;
		}
	}
}