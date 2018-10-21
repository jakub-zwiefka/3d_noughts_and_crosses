/*
 Name:		_3d_noughts_and_crosses.ino
 Created:	10/5/2018 8:30:03 PM
 Author:	morgarath
*/
// #define hide

////////// MACROS
//// Time constants
#define period 2500  // period at which to multiplex ledcube layers (us) // f=400Hz

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
#define R1Pin A0	// Red	 collumns 9-16
#define G0Pin A1	// Green collumns 1-8
#define G1Pin A2	// Green collumns 9-16
#define B0Pin A3	// Blue	 collumns 1-8
#define B1Pin A4	// Blue	 collumns 9-16
// Arduino pins to select ledcube layer to be activated
#define L0Pin A5	// Layer 0
#define L1Pin 4	    // Layer 1
#define L2Pin 3	    // Layer 2
#define L3Pin 2	    // Layer 3
// Arduino pins to communicate Bluetooth module over UART
#define TXPin 1
#define RXPin 0
#endif // ARDUINO_PINS

//// AT32U4 port pins
#define PORT_PINS
#ifdef PORT_PINS
// AT32U4 port pins to configure bus setting selected group of ledcube columns
#define BUS0PPin PORTD6	
#define BUS1PPin PORTB7	
#define BUS2PPin PORTB6	
#define BUS3PPin PORTB5	
#define BUS4PPin PORTB4	
#define BUS5PPin PORTE6	
#define BUS6PPin PORTD7	
#define BUS7PPin PORTC6	
// AT32U4 port pins to select ledcube columms' group to be configured
#define R0PPin PORTC7 // Red collumns 1-8
#define R1PPin PORTF7 // Red collumns 9-16
#define G0PPin PORTF6 // Green collumns 1-8
#define G1PPin PORTF5 // Green collumns 9-16
#define B0PPin PORTF4 // Blue collumns 1-8
#define B1PPin PORTF1 // Blue collumns 9-16
// AT32U4 port pins to select ledcube layer to be activated
#define L0PPin PORTF0 // Layer 0
#define L1PPin PORTD4 // Layer 1
#define L2PPin PORTD0 // Layer 2
#define L3PPin PORTD1 // Layer 3
// AT32U4 port pins to communicate Bluetooth module over UART
#define TXPPin PORTD3; 
#define RXPPin PORTD2; 
#endif // PORT_PINS

//// Ports of every pin
#define PORTS
#ifdef PORTS
// AT32U4 ports to configure bus setting selected group of ledcube columns
#define BUS0Port PORTD	
#define BUS1Port PORTB	
#define BUS2Port PORTB	
#define BUS3Port PORTB	
#define BUS4Port PORTB	
#define BUS5Port PORTE	
#define BUS6Port PORTD	
#define BUS7Port PORTC	
// AT32U4 ports to select ledcube columms' group to be configured
#define R0Port PORTC // Red	collumns 1-8
#define R1Port PORTF // Red	collumns 9-16
#define G0Port PORTF // Green collumns 1-8
#define G1Port PORTF // Green collumns 9-16
#define B0Port PORTF // Blue collumns 1-8
#define B1Port PORTF // Blue collumns 9-16
// AT32U4 ports to select ledcube layer to be activated
#define L0Port PORTF // Layer 0
#define L1Port PORTD // Layer 1
#define L2Port PORTD // Layer 2
#define L3Port PORTD // Layer 3
// AT32U4 ports to communicate Bluetooth module over UART
#define TXPort PORTD; 
#define RXPort PORTD; 
#endif // PORTS

//// Other
#define RED 0
#define GREEN 1
#define BLUE 2

#define LAYER0 0
#define LAYER1 1
#define LAYER2 2
#define LAYER3 3

#define COLUMN0 0
#define COLUMN1 1
#define COLUMN2 2
#define COLUMN3 3
#define COLUMN4 4
#define COLUMN5 5
#define COLUMN6 6
#define COLUMN7 7
#define COLUMN8 8
#define COLUMN9 9
#define COLUMN10 10
#define COLUMN11 11
#define COLUMN12 12
#define COLUMN13 13
#define COLUMN14 14
#define COLUMN15 15



#define LAYERS_NUM 4

////////// VARIABLES
uint32_t current_micros;
uint32_t previous_micros = 0;
uint8_t active_layer = 0x00;
uint8_t column_to_set = 0;
uint8_t layer_to_set = 0;
uint8_t color_to_set = 0;

//                 Columns:  red:    green:  blue:      Layer:
uint16_t led_cube[4][3] = { {0x0000, 0x0000, 0x0000},	// 0
                           { 0x0000, 0x0000, 0x0000},   // 1
                           { 0x0000, 0x0000, 0x0000},	// 2
                           { 0x0000, 0x0000, 0x0000} };	// 3

// DEBUGGING VARIABLES
uint8_t a = 1;
uint8_t b = 0;
long tic;
long toc;
long interval;
uint8_t buffer;
uint16_t subperiod = 0;

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

    Serial.begin(115200);
}


////////// TASKS
void loop()
{
    current_micros = micros();

    if (current_micros - previous_micros >= period)
    {
        previous_micros = current_micros;
        subperiod++;
        #ifndef hide

        switch (active_layer)
        {
        case LAYER0:
            clrPortBit(&L3Port, L3PPin);
            setLayer(active_layer);
            setPortBit(&L0Port, L0PPin);
            break;

        case LAYER1:
            clrPortBit(&L0Port, L0PPin);
            setLayer(active_layer);
            setPortBit(&L1Port, L1PPin);
            break;

        case LAYER2:
            clrPortBit(&L1Port, L1PPin);
            setLayer(active_layer);
            setPortBit(&L2Port, L2PPin);
            break;

        case LAYER3:
            clrPortBit(&L2Port, L2PPin);
            setLayer(active_layer);
            setPortBit(&L3Port, L3PPin);
            break;

        default:
            break;
        }

        if (++active_layer >= LAYERS_NUM)
        {
            active_layer = 0;
        }

        #endif // !hide

        testLedCube();

        // Testing space
        #ifdef hide
        if (a)
        {
            clrPortBit(&L1Port, L1PPin);
            setPortBit(&L0Port, L0PPin);
            buffer = 0xFF;
            tic = micros();
            setLayer(0);
            toc = micros();
        }
        if (b)
        {
            clrPortBit(&L0Port, L0PPin);
            setPortBit(&L1Port, L1PPin);
            buffer = 0x00;
            tic = micros();
            setLayer(1);
            toc = micros();
        }
        if (a)
        {
            a = 0;
            b = 1;
        }
        else
        {
            a = 1;
            b = 0;
        }
        #endif // hide
    }


}

////////// FUNCTION DEFINITIONS
void modifyPortBit(volatile uint8_t *port, uint8_t bit_pos, uint8_t val)
{
     *port = (*port & ~(1 << bit_pos)) | ((val << bit_pos)&(1 << bit_pos));
}

void setPortBit(volatile uint8_t *port, uint8_t bit_pos)
{
    *port |= (1 << bit_pos);
}

void clrPortBit(volatile uint8_t *port, uint8_t bit_pos)
{
    *port &= !(1 << bit_pos);
}

void setBus(uint8_t buffer)
{
    modifyPortBit(&BUS0Port, BUS0PPin,  (buffer       & 0x01)); 
    modifyPortBit(&BUS1Port, BUS1PPin, ((buffer >> 1) & 0x01)); 
    modifyPortBit(&BUS2Port, BUS2PPin, ((buffer >> 2) & 0x01)); 
    modifyPortBit(&BUS3Port, BUS3PPin, ((buffer >> 3) & 0x01)); 
    modifyPortBit(&BUS4Port, BUS4PPin, ((buffer >> 4) & 0x01)); 
    modifyPortBit(&BUS5Port, BUS5PPin, ((buffer >> 5) & 0x01)); 
    modifyPortBit(&BUS6Port, BUS6PPin, ((buffer >> 6) & 0x01)); 
    modifyPortBit(&BUS7Port, BUS7PPin, ((buffer >> 7) & 0x01));
}

void setLayer(uint8_t layer)
{   
    modifyPortBit(&R0Port, R0PPin, HIGH);
    setBus(led_cube[layer][RED] & 0xFF);
    modifyPortBit(&R0Port, R0PPin, LOW);
    modifyPortBit(&R1Port, R1PPin, HIGH);
    setBus(led_cube[layer][RED] >> 8);
    modifyPortBit(&R1Port, R1PPin, LOW);
    modifyPortBit(&G0Port, G0PPin, HIGH);
    setBus(led_cube[layer][GREEN] & 0xFF);
    modifyPortBit(&G0Port, G0PPin, LOW);
    modifyPortBit(&G1Port, G1PPin, HIGH);
    setBus(led_cube[layer][GREEN] >> 8);
    modifyPortBit(&G1Port, G1PPin, LOW);
    modifyPortBit(&B0Port, B0PPin, HIGH);
    setBus(led_cube[layer][BLUE] & 0xFF);
    modifyPortBit(&B0Port, B0PPin, LOW);
    modifyPortBit(&B1Port, B1PPin, HIGH);
    setBus(led_cube[layer][BLUE] >> 8);
    modifyPortBit(&B1Port, B1PPin, LOW);
}

void updateLedCube(uint8_t layer, uint8_t column, uint8_t color)
{
    led_cube[layer][color] |= 1 << column;
}


bool isAvailable(uint8_t layer, uint8_t column)
{   
    uint32_t led;
    for (int color = 0; color < 3; color++)
    {
        led = (led_cube[layer][color] >> column) & 0x01;
        if (led == 1)
        {
            return false;
        }
    }    
    return true;
}

void testLedCube(void)
{
    if (subperiod > 10)
    {
        if (isAvailable(layer_to_set, column_to_set))
        {
            updateLedCube(layer_to_set, column_to_set, color_to_set);
        }
        column_to_set++;
        if (layer_to_set == 4)
        {
            layer_to_set = 0;
            color_to_set++;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    led_cube[i][j] = 0x0000;
                }
            }
        }
        if (column_to_set == 16)
        {
            column_to_set = 0;
            layer_to_set++;
        }
        if (color_to_set == 3)
        {
            color_to_set = 0;
        }
        subperiod = 0;
    }
}