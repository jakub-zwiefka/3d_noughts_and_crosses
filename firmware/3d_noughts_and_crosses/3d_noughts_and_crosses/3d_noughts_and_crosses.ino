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

//// AT32U4 port pins mapping
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

//// AT32U4 ports of every pin
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

//// Constants
#define RED 0
#define GREEN 1
#define BLUE 2

#define LAYER0 0
#define LAYER1 1
#define LAYER2 2
#define LAYER3 3

#define LAYERS_NUM 4
#define PLAYERS_NUM 3

#define EXIT_GAME_MSG 0x00
#define RED_WON_MSG 0x01
#define GREEN_WON_MSG 0x02
#define BLUE_WON_MSG 0x03
#define CORRECT_MOVE_MSG 0x04
#define INCORRECT_MOVE_MSG 0x05

////////// STRUCTURES
struct ledcube_point {
    uint8_t color;
    uint8_t layer;
    uint8_t column;
};


////////// VARIABLES
uint32_t current_micros;
uint32_t previous_micros = 0;
uint8_t current_layer = 0;
uint8_t received;
uint8_t command_to_transmit;
uint16_t subperiod = 0;
bool transmit_trigger = false;
bool receive_trigger = false;
bool test_ended = false;

ledcube_point led_to_set = {
    0x00, // color
    0x00, // layer
    0x00, // column
};
//                         Groups of columns:  red:    green:  blue:      Layer:
uint16_t ledcube[LAYERS_NUM][PLAYERS_NUM] = { {0x0000, 0x0000, 0x0000},	  // 0
                                              {0x0000, 0x0000, 0x0000},   // 1
                                              {0x0000, 0x0000, 0x0000},   // 2
                                              {0x0000, 0x0000, 0x0000} }; // 3


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

    digitalWrite(LED_BUILTIN, LOW);

    Serial.begin(9600);
    Serial1.begin(9600);
}


////////// TASKS
void loop()
{
    current_micros = micros();

    if (test_ended)
    {
        receive();
        react();
        transmit();
    }
    else
    {
        testLedcube();
        if (test_ended)
        {
            clearLedcube();
        }
    }

    if (current_micros - previous_micros >= period)
    {
        previous_micros = current_micros;

        switch (current_layer)
        {
        case LAYER0:
            clrPortBit(&L3Port, L3PPin);
            setLayer(current_layer);
            setPortBit(&L0Port, L0PPin);
            break;

        case LAYER1:
            clrPortBit(&L0Port, L0PPin);
            setLayer(current_layer);
            setPortBit(&L1Port, L1PPin);
            break;

        case LAYER2:
            clrPortBit(&L1Port, L1PPin);
            setLayer(current_layer);
            setPortBit(&L2Port, L2PPin);
            break;

        case LAYER3:
            clrPortBit(&L2Port, L2PPin);
            setLayer(current_layer);
            setPortBit(&L3Port, L3PPin);
            break;

        default:
            break;
        }

        if (++current_layer >= LAYERS_NUM)
        {
            current_layer = 0;
        }
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
    modifyPortBit(&BUS0Port, BUS0PPin, (buffer & 0x01));
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
    setBus(ledcube[layer][RED] & 0xFF);
    modifyPortBit(&R0Port, R0PPin, LOW);
    modifyPortBit(&R1Port, R1PPin, HIGH);
    setBus(ledcube[layer][RED] >> 8);
    modifyPortBit(&R1Port, R1PPin, LOW);
    modifyPortBit(&G0Port, G0PPin, HIGH);
    setBus(ledcube[layer][GREEN] & 0xFF);
    modifyPortBit(&G0Port, G0PPin, LOW);
    modifyPortBit(&G1Port, G1PPin, HIGH);
    setBus(ledcube[layer][GREEN] >> 8);
    modifyPortBit(&G1Port, G1PPin, LOW);
    modifyPortBit(&B0Port, B0PPin, HIGH);
    setBus(ledcube[layer][BLUE] & 0xFF);
    modifyPortBit(&B0Port, B0PPin, LOW);
    modifyPortBit(&B1Port, B1PPin, HIGH);
    setBus(ledcube[layer][BLUE] >> 8);
    modifyPortBit(&B1Port, B1PPin, LOW);
}

void setLed(uint8_t color, uint8_t layer, uint8_t column)
{
    ledcube[layer][color] |= 0x0001 << column;
}

bool isAvailable(uint8_t layer, uint8_t column)
{
    uint16_t layer_of_color;
    for (uint8_t color = 0; color < 3; color++)
    {
        layer_of_color = ledcube[layer][color];
        if ((layer_of_color >> column) & 0x0001 == 0x0001)
        {
            return false;
        }
    }
    return true;
}

void clearLedcube(void)
{
    for (uint8_t layer = 0; layer < 4; layer++)
    {
        for (uint8_t color = 0; color < 3; color++)
        {
            ledcube[layer][color] = 0x0000;
        }
    }
}

void testLedcube(void)
{
    if (subperiod > 2000)
    {
        subperiod = 0;

        if ((led_to_set.column == 0) && (led_to_set.layer == 0))
        {
            clearLedcube();
        }

        if (isAvailable(led_to_set.layer, led_to_set.column))
        {
            setLed(led_to_set.color, led_to_set.layer, led_to_set.column);
        }

        led_to_set.column++;
        if (led_to_set.column > 15)
        {
            led_to_set.column = 0;
            led_to_set.layer++;
            if (led_to_set.layer > 3)
            {
                led_to_set.layer = 0;
                led_to_set.color++;
                if (led_to_set.color > 2)
                {
                    led_to_set.color = 0;
                    test_ended = true;
                }
            }
        }
    }
    subperiod++;
}

bool isWinner(uint8_t color)
{
    //// 1) In horizontal layer cases
    for (uint8_t hor_layer = 0; hor_layer < 4; hor_layer++)
    {
        for (uint8_t pos = 0; pos < 4; pos++)
        {
            // Cases: Horizontal
            //   0 0 0 0   ||   0 0 0 0   ||   0 0 0 0   ||   1 1 1 1
            //   0 0 0 0   ||   0 0 0 0   ||   1 1 1 1   ||   0 0 0 0
            //   0 0 0 0   ||   1 1 1 1   ||   0 0 0 0   ||   0 0 0 0
            //   1 1 1 1   ||   0 0 0 0   ||   0 0 0 0   ||   0 0 0 0 
            if (ledcube[hor_layer][color] == 0x000F << (4 * pos))
            {
                return true;
            }
            // Cases: Vertical
            //   0 0 0 1   ||   0 0 1 0   ||   0 1 0 0   ||   1 0 0 0
            //   0 0 0 1   ||   0 0 1 0   ||   0 1 0 0   ||   1 0 0 0
            //   0 0 0 1   ||   0 0 1 0   ||   0 1 0 0   ||   1 0 0 0
            //   0 0 0 1   ||   0 0 1 0   ||   0 1 0 0   ||   1 0 0 0
            if (ledcube[hor_layer][color] == 0x1111 << pos)
            {
                return true;
            }
        }
        // Cases: Crosswise
        //   1 0 0 0   ||   0 0 0 1
        //   0 1 0 0   ||   0 0 1 0
        //   0 0 1 0   ||   0 1 0 0
        //   0 0 0 1   ||   1 0 0 0
        if (ledcube[hor_layer][color] == 0x1248)
        {
            return true;
        }
        if (ledcube[hor_layer][color] == 0x8421)
        {
            return true;
        }

    }

    ////// 2) In vertical layer cases
    //for (uint8_t vert_layer = 0; vert_layer < 4; vert_layer++)
    //{
    //    for (uint8_t pos = 0; pos < 4; pos++)
    //    {
    //        if (ledcube[pos][color] != 0x0001 << (4 * vert_layer))
    //        {
    //            return false;
    //        }
    //    }
    //}

    return false;
}

void receive(void)
{
    if (Serial1.available() > 0)
    {
        received = (uint8_t)Serial1.read();
        receive_trigger = true;
    }
}

void transmit(void)
{
    if (transmit_trigger)
    {
        Serial1.write(command_to_transmit);
        transmit_trigger = false;
    }
}

void react(void)
{
    if (receive_trigger)
    {
        if (received >> 6 != 0)
        {
            led_to_set.color = (received >> 6) - 1;
            led_to_set.layer = ((received & 0x30) >> 4);
            led_to_set.column = (received & 0x0F);
            if (isAvailable(led_to_set.layer, led_to_set.column))
            {
                setLed(led_to_set.color, led_to_set.layer, led_to_set.column);
                command_to_transmit = CORRECT_MOVE_MSG;
                if (isWinner(led_to_set.color))
                {
                    switch (led_to_set.color)
                    {
                    case RED:
                        command_to_transmit = RED_WON_MSG;
                        break;
                    case GREEN:
                        command_to_transmit = GREEN_WON_MSG;
                        break;
                    case BLUE:
                        command_to_transmit = BLUE_WON_MSG;
                        break;
                    default:
                        break;
                    }
                }
                transmit_trigger = true;
            }
            else
            {
                command_to_transmit = INCORRECT_MOVE_MSG;
                transmit_trigger = true;
            }
        }
        else if (received == EXIT_GAME_MSG)
        {
            clearLedcube();
        }
        receive_trigger = false;
    }
}