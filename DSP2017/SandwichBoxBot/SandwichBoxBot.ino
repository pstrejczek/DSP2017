/*
 Name:		SandwichBoxBot.ino
 Created:	2/28/2017 7:02:30 PM
 Author:	Pawe³ Strejczek
*/

#define D0 16
#define D1 5 // I2C Bus SCL (clock)
#define D2 4 // I2C Bus SDA (data)
#define D3 0
#define D4 2 // Same as "LED_BUILTIN", but inverted logic
#define D5 14 // SPI Bus SCK (clock)
#define D6 12 // SPI Bus MISO 
#define D7 13 // SPI Bus MOSI
#define D8 15 // SPI Bus SS (CS)
#define D9 3 // RX0 (Serial console)
#define D10 1 // TX0 (Serial console)

// the setup function runs once when you press reset or power the board
void setup() {
	pinMode(D0, OUTPUT);
	pinMode(D4, OUTPUT);
}

// the loop function runs over and over again until power down or reset
void loop() {
	digitalWrite(D0, HIGH);
	digitalWrite(D4, LOW);
	delay(1000);
	digitalWrite(D0, LOW);
	digitalWrite(D4, HIGH);
	delay(1000);
}
