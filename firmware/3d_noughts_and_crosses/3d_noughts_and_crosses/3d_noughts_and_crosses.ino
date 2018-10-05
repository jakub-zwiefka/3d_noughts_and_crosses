/*
 Name:		_3d_noughts_and_crosses.ino
 Created:	10/5/2018 8:30:03 PM
 Author:	morgarath
*/

////////// TIME CONSTANTS
#define interval 1000           // interval at which to blink (milliseconds)

////////// PIN CONSTANTS
// Led cube columns configuration bus pins
#define D0Pin 12
#define D1Pin 11
#define D2Pin 10
#define D3Pin 9
#define D4Pin 8
#define D5Pin 7
#define D6Pin 6
#define D7Pin 5
// Led cube columns' color group latches selection pins
#define R0Pin 13
#define R1Pin 14
#define G0Pin 15
#define G1Pin 16
#define B0Pin 17
#define B1Pin 18
// Led cube level activation pins
#define L0Pin 19
#define L1Pin 4
#define L2Pin 3
#define L3Pin 2
// Bluetooth module UART communication pins
//#define TXPin PIN;
//#define RXPin PIN;

////////// VARIABLES
byte level = 0x00;
byte buff0 = 0x00;
byte buff1 = 0x00;
word buff = 0x0000;
unsigned int previousMillis = 0;
//           COLUMNS:  red     green   blue         LEVEL:
word cube[4][3] = { { 0x1000, 0x0100, 0x0010},		// 0
					{ 0x0100, 0x0010, 0x0001},		// 1
					{ 0x0010, 0x0001, 0x1000},		// 2
					{ 0x0001, 0x1000, 0x0100} };	// 3

////////// FUNCTION DECLARATIONS
void setCollumns(void);

////////// INITIAL ACTIONS
void setup()
{
	pinMode(D0Pin, OUTPUT);
	pinMode(D1Pin, OUTPUT);
	pinMode(D2Pin, OUTPUT);
	pinMode(D3Pin, OUTPUT);
	pinMode(D4Pin, OUTPUT);
	pinMode(D5Pin, OUTPUT);
	pinMode(D6Pin, OUTPUT);
	pinMode(D7Pin, OUTPUT);

	pinMode(L0Pin, OUTPUT);
	pinMode(L1Pin, OUTPUT);
	pinMode(L2Pin, OUTPUT);
	pinMode(L3Pin, OUTPUT);

	pinMode(R0Pin, OUTPUT);
	pinMode(R1Pin, OUTPUT);
	pinMode(G0Pin, OUTPUT);
	pinMode(G1Pin, OUTPUT);
	pinMode(B0Pin, OUTPUT);
	pinMode(B1Pin, OUTPUT);

//	pinMode(TXPin, OUTPUT);
//	pinMode(RXPin, INPUT);

	digitalWrite(L0Pin, LOW);
	digitalWrite(L1Pin, LOW);
	digitalWrite(L2Pin, LOW);
	digitalWrite(L3Pin, LOW);

	digitalWrite(R0Pin, LOW);
	digitalWrite(R1Pin, LOW);
	digitalWrite(G0Pin, LOW);
	digitalWrite(G1Pin, LOW);
	digitalWrite(B0Pin, LOW);
	digitalWrite(B1Pin, LOW);

	digitalWrite(D0Pin, LOW);
	digitalWrite(D1Pin, LOW);
	digitalWrite(D2Pin, LOW);
	digitalWrite(D3Pin, LOW);
	digitalWrite(D4Pin, LOW);
	digitalWrite(D5Pin, LOW);
	digitalWrite(D6Pin, LOW);
	digitalWrite(D7Pin, LOW);
}

////////// CONTINUOUS ACTIONS
void loop()
{
	unsigned long currentMillis = millis();

	if (currentMillis - previousMillis >= interval)
	{
		digitalWrite(L0Pin, LOW);
		digitalWrite(L1Pin, LOW);
		digitalWrite(L2Pin, LOW);
		digitalWrite(L3Pin, LOW);

		previousMillis = currentMillis;

		switch (level)
		{
		case 0:
			setCollumns();
			digitalWrite(L0Pin, HIGH);
			break;

		case 1:
			setCollumns();
			digitalWrite(L1Pin, HIGH);
			break;

		case 2:
			setCollumns();
			digitalWrite(L2Pin, HIGH);
			break;

		case 3:
			setCollumns();
			digitalWrite(L3Pin, HIGH);
			break;

		default:
			break;
		}

		level += 1;
		if (level > 3)
		{
			level = 0;
		}

	}
}

