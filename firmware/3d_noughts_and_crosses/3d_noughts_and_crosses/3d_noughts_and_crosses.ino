/*
 Name:		_3d_noughts_and_crosses.ino
 Created:	10/5/2018 8:30:03 PM
 Author:	morgarath
*/


////////// MACROS
//// Time constants
#define period 600  // period at which to multiplex ledcube layers (ms)

//// Arduino pins mapping
#define ARDUINO_PINS
#ifdef ARDUINO_PINS
// Arduino pins to configure bus setting selected group of ledcube columns
#define BUS0Pin 12	// D12 - PD6
#define BUS1Pin 11	// D11 - PB7
#define BUS2Pin 10	// D10 - PB6
#define BUS3Pin 9	// D9  - PB5
#define BUS4Pin 8	// D8  - PB4
#define BUS5Pin 7	// D7  - PE6
#define BUS6Pin 6	// D6  - PD7
#define BUS7Pin 5	// D5  - PC6
// Arduino pins to select ledcube columms' group to be configured
#define R0Pin 13	// Red	 collumns 1-8
#define R1Pin 14	// Red	 collumns 9-16
#define G0Pin 15	// Green collumns 1-8
#define G1Pin 16	// Green collumns 9-16
#define B0Pin 17	// Blue	 collumns 1-8
#define B1Pin 18	// Blue	 collumns 9-16
// Arduino pins to select ledcube layer to be activated
#define L0Pin 19	// Layer 0
#define L1Pin 4	    // Layer 1
#define L2Pin 3	    // Layer 2
#define L3Pin 2	    // Layer 3
// Arduino pins to communicate Bluetooth module over UART
#define TXPin 1
#define RXPin 0
#endif // ARDUINO_PINS

//// AT32u4 port pins
#define PORT_PINS
#ifdef PORT_PINS
// AT32u4 port pins to configure bus setting selected group of ledcube columns
#define BUS0PPin PORTD6	
#define BUS1PPin PORTB7	
#define BUS2PPin PORTB6	
#define BUS3PPin PORTB5	
#define BUS4PPin PORTB4	
#define BUS5PPin PORTE6	
#define BUS6PPin PORTD7	
#define BUS7PPin PORTC6	
// AT32u4 port pins to select ledcube columms' group to be configured
#define R0PPin PORTC7 // Red	 collumns 1-8
#define R1PPin PORTF7 // Red	 collumns 9-16
#define G0PPin PORTF6 // Green   collumns 1-8
#define G1PPin PORTF5 // Green   collumns 9-16
#define B0PPin PORTF4 // Blue	 collumns 1-8
#define B1PPin PORTF1 // Blue	 collumns 9-16
// AT32u4 port pins to select ledcube layer to be activated
#define L0PPin PORTF0 // Layer 0
#define L1PPin PORTD4 // Layer 1
#define L2PPin PORTD0 // Layer 2
#define L3PPin PORTD1 // Layer 3
// AT32u4 port pins to communicate Bluetooth module over UART
#define TXPPin PORTD3; 
#define RXPPin PORTD2; 
#endif // PORT_PINS

//// Ports of every pin
#define PORTS
#ifdef PORTS
// AT32u4 ports to configure bus setting selected group of ledcube columns
#define BUS0Port PORTD	
#define BUS1Port PORTB	
#define BUS2Port PORTB	
#define BUS3Port PORTB	
#define BUS4Port PORTB	
#define BUS5Port PORTE	
#define BUS6Port PORTD	
#define BUS7Port PORTC	
// AT32u4 ports to select ledcube columms' group to be configured
#define R0Port PORTC // Red	 collumns 1-8
#define R1Port PORTF // Red	 collumns 9-16
#define G0Port PORTF // Green   collumns 1-8
#define G1Port PORTF // Green   collumns 9-16
#define B0Port PORTF // Blue	 collumns 1-8
#define B1Port PORTF // Blue	 collumns 9-16
// AT32u4 ports to select ledcube layer to be activated
#define L0Port PORTF // Layer 0
#define L1Port PORTD // Layer 1
#define L2Port PORTD // Layer 2
#define L3Port PORTD // Layer 3
// AT32u4 ports to communicate Bluetooth module over UART
#define TXPort PORTD; 
#define RXPort PORTD; 
#endif // PORTS

//// Other
#define RED 0
#define GREEN 1
#define BLUE 2

////////// VARIABLES
uint32_t currentMillis;
uint32_t previousMillis = 0;
uint8_t layer = 0x00;
uint16_t buffer = 0x0000;
//                 Columns:  red     green   blue        Level:
uint16_t ledcube[4][3] = { { 0x1000, 0x0100, 0x0010},	// 0
                           { 0x0100, 0x0010, 0x0001},   // 1
                           { 0x0010, 0x0001, 0x1000},	// 2
                           { 0x0001, 0x1000, 0x0100} };	// 3


////////// FUNCTION DECLARATIONS
inline uint8_t modifyBit(uint8_t operand, uint8_t pos, uint8_t bit);
inline  uint8_t activateLayer(uint8_t operand, uint8_t pos);
inline  uint8_t deactivateLayer(uint8_t operand, uint8_t pos);
void setBus(uint8_t buffer);
void setColumns(void);


////////// INITIALIZATON
void setup()
{
    pinMode(BUS0Pin, OUTPUT);
    pinMode(BUS1Pin, OUTPUT);
    pinMode(BUS2Pin, OUTPUT);
    pinMode(BUS3Pin, OUTPUT);
    pinMode(BUS4Pin, OUTPUT);
    pinMode(BUS5Pin, OUTPUT);
    pinMode(BUS6Pin, OUTPUT);
    pinMode(BUS7Pin, OUTPUT);

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

    pinMode(TXPin, OUTPUT);
    pinMode(RXPin, INPUT);

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
}


////////// CONTINUOUS TASKS
void loop()
{
    currentMillis = millis();

    if (currentMillis - previousMillis >= period)
    {
        

        previousMillis = currentMillis;

        switch (layer)
        {
        case 0:
            // Deactivate previous layer and activate current
            setColumns();
            activateLayer(L3Port, L3PPin);
            deactivateLayer(L0Port, L0PPin);
            
            break;

        case 1:
            setColumns();
            activateLayer(L0Port, L0PPin);
            deactivateLayer(L1Port, L1PPin);
            break;

        case 2:
            setColumns();
            activateLayer(L1Port, L1PPin);
            deactivateLayer(L2Port, L2PPin);
            break;

        case 3:
            setColumns();
            activateLayer(L2Port, L2PPin);
            deactivateLayer(L3Port, L3PPin);
            break;

        default:
            break;
        }

        if (layer++ > 3)
        {
            layer = 0;
        }
    }
}


////////// FUNCTION DEFINITIONS
inline uint8_t modifyBit(uint8_t operand, uint8_t pos, uint8_t bit)
{
    return operand = (operand & ~(1 << pos)) | ((bit << pos)&(1 << pos));
}

inline  uint8_t activateLayer(uint8_t operand, uint8_t pos)
{
    return operand |= (1 << pos);
}

inline  uint8_t deactivateLayer(uint8_t operand, uint8_t pos)
{
    return operand &= !(1 << pos);
}

void setBus(uint8_t buffer)
{
    modifyBit(BUS0Port, BUS0PPin,  (buffer       & 0xFE)); 
    modifyBit(BUS1Port, BUS1PPin, ((buffer >> 1) & 0xFE)); 
    modifyBit(BUS2Port, BUS2PPin, ((buffer >> 2) & 0xFE)); 
    modifyBit(BUS3Port, BUS3PPin, ((buffer >> 3) & 0xFE)); 
    modifyBit(BUS4Port, BUS4PPin, ((buffer >> 4) & 0xFE)); 
    modifyBit(BUS5Port, BUS5PPin, ((buffer >> 5) & 0xFE)); 
    modifyBit(BUS6Port, BUS6PPin, ((buffer >> 6) & 0xFE)); 
    modifyBit(BUS7Port, BUS7PPin, ((buffer >> 7) & 0xFE)); 
}

void setColumns(void)
{
    
}