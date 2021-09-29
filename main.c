#define F_CPU 8000000UL

#include <avr/io.h>

void PORT_INIT(void);
void SPI_MASTER_INIT(void);
char SPI_DATA_EXCHANGE(char databyte);

int main(void){
	DDRA = 0xFF;
	
	PORT_INIT();
	SPI_MASTER_INIT();
	
	char receive = SPI_DATA_EXCHANGE('H');
	
	PORTA = receive;
}

/*!
 *	@brief Initialize ports.
 */

void PORT_INIT(void){
	DDRB |= (1<<PB4)|(1<<PB5)|(1<<PB7);			///< Set MOSI, SCK and SS pins to output.
	DDRB &= ~(1<<PB6);					///< Set MISO pin to input.
	PORTB &= ~(1<<PB4);					///< Set SS pin logic LOW to turn on the slave device.
}

/*!
 *	@brief Initialize SPI in master mode.
 */

void SPI_MASTER_INIT(void){
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);			///< Enable SPI, select Master SPI mode and set SCK frequency to Fosc/16.
}

/*!
 *	@brief Exchange a databyte over SPI.
 *	@param Databyte to be sent.
 *	@return Databyte received.
 */

char SPI_DATA_EXCHANGE(char databyte){
	SPDR = databyte;					///< Load the data byte to the SPDR shift register to transmit.
	while(!(SPSR & (1<<SPIF)));				///< Wait until the full data byte is received.
	return SPDR;						///< Return the received databyte.
}
