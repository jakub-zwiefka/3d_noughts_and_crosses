/*
 Name:		_3d_noughts_and_crosses.ino
 Created:	10/5/2018 8:30:03 PM
 Author:	morgarath
*/


///////////////////////////////////////////////////////////////////////////////
////////// MACROS
///////////////////////////////////////////////////////////////////////////////

//// Arduino pins mapping
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

//// AT32U4 port pins mapping
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

//// AT32U4 ports of every pin mapped
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

//// Game result constants
#define RED_WON 0
#define GREEN_WON 1
#define BLUE_WON 2
#define DRAW 3

//// Ledcube constraction constants
#define LEDCUBE_SIZE 4
#define LAYERS_NUM LEDCUBE_SIZE
#define COLUMNS_NUM LEDCUBE_SIZE * LEDCUBE_SIZE 
#define COLORS_NUM 3
#define PLAYERS_NUM 3

//// LayerN constants
#define LAYER0 0
#define LAYER1 1
#define LAYER2 2
#define LAYER3 3

//// Color constants
#define RED 0
#define GREEN 1
#define BLUE 2

//// Time constants
#define PERIOD 2500  // period at which to multiplex ledcube layers (us) // f=400Hz
#define TEST_SUBPERIOD 3500  // period of setting on next leds in testLedcube()
#define ANIM_SUBPERIOD 8000  // period of setting on next layers in animations presenting winners

//// Messages constants
//                   Player:Red/Green/Blue   Layer  Column
// (*) MOVE MSG     ->      {01}/{10}/{11} + {--} + {----} (b)
//     OTHER MSGs   ->      {00} + {------} (b)
// Receivable messages
// MOVE_MSG (*)
#define INIT_GAME_MSG 0x00
#define EXIT_GAME_MSG 0x01
#define CONFIRM_MOVE_MSG 0x02
// Transmittable messagaes
#define DRAW_MSG 0x00
#define RED_WON_MSG 0x01
#define GREEN_WON_MSG 0x02
#define BLUE_WON_MSG 0x03
#define CORRECT_MOVE_MSG 0x04
#define INCORRECT_MOVE_MSG 0x05

///////////////////////////////////////////////////////////////////////////////
////////// STRUCTURES
///////////////////////////////////////////////////////////////////////////////
struct ledcube_point {
    uint8_t color;
    uint8_t layer;
    uint8_t column;
};

///////////////////////////////////////////////////////////////////////////////
////////// VARIABLES
///////////////////////////////////////////////////////////////////////////////
// Multiplexing
uint32_t current_micros;
uint32_t previous_micros = 0;
uint8_t current_layer = 0;

// Animations
uint16_t animation_counter1 = 0;
uint16_t animation_counter2 = 0;
uint8_t animation_iterator1 = 0;
uint8_t animation_iterator2 = 0;
bool go_up = true;
bool blink_led_trigger = false;
bool blink_toggle = true;

// Communication
uint8_t received;
uint8_t message_to_transmit;

// Gameplay
uint8_t moves_left = LAYERS_NUM * COLUMNS_NUM;
uint8_t game_result;

// Control
bool reset_trigger = false;
bool transmit_trigger = false;
bool react_trigger = false;
bool test_ended = false;
bool game_ended = false;

// Next LED to be set in ledcube[][] table with setLed() function
ledcube_point led_to_set = {
    0, // color
    0, // layer
    0, // column
};

// LED needing confirmation not yet set in ledcube[][] table with setLed() function
ledcube_point led_to_blink = {
    0, // color
    0, // layer
    0, // column
};

// LED presented on ledcube but not yet set in ledcube[][] table with setLed() function
ledcube_point led_to_confirm = {
    0, // color
    0, // layer
    0, // column
};

// Table of 12 Words in which is held state of LEDs in each layer and color of ledcube
//                         Groups of columns:  red:    green:  blue:      Layer:
uint16_t ledcube[LAYERS_NUM][PLAYERS_NUM] = { {0x0000, 0x0000, 0x0000},	  // 0
                                              {0x0000, 0x0000, 0x0000},   // 1
                                              {0x0000, 0x0000, 0x0000},   // 2
                                              {0x0000, 0x0000, 0x0000} }; // 3


//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
////////// INITIALIZATON
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
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


    clrPortBit(&L0Port, L0PPin);
    clrPortBit(&L1Port, L1PPin);
    clrPortBit(&L2Port, L2PPin);
    clrPortBit(&L3Port, L3PPin);

    clrPortBit(&R0Port, R0PPin);
    clrPortBit(&R1Port, R1PPin);
    clrPortBit(&G0Port, G0PPin);
    clrPortBit(&G1Port, G1PPin);
    clrPortBit(&B0Port, B0PPin);
    clrPortBit(&B1Port, B1PPin);

    Serial1.begin(9600);
}


//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
////////// TASKS
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
void loop()
{
    current_micros = micros();

    // Reset procedure
    if (reset_trigger)
    {
        resetLedcube();
        reset_trigger = false;
    }

    // Initial testing procedure
    if (!test_ended)
    {
        testLedcube(); // Test ledcube when not busy
    }

    // Receiving procedure
    receive(); // Check for new messages and trigger reacting to new ones

    // Reacting to received procedure
    if (react_trigger)
    {
        react();
        react_trigger = false; // Stop reacting till message received
    }

    // Transmitting of reaction outcome procedure
    if (transmit_trigger)
    {
        transmit();
        transmit_trigger = false; // Stop transmitting till needed
    }

    // Present game result procedure
    if (game_ended)
    {
        presentGameResult(); // Visualize on ledcube result of game
    }

    if (blink_led_trigger)
    {
        blinkUnconfirmedLed();
    }

    // Driving ledcube
    if (current_micros - previous_micros >= PERIOD)
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

////////// FUNCTIONS
// Set given port pin to given value (0 or 1)
void modifyPortBit(volatile uint8_t *port, uint8_t pin, uint8_t val)
{
    *port = (*port & ~(1 << pin)) | ((val << pin)&(1 << pin));
}

// Set on given port pin
void setPortBit(volatile uint8_t *port, uint8_t pin)
{
    *port |= (1 << pin);
}

// Set off given port pin
void clrPortBit(volatile uint8_t *port, uint8_t pin)
{
    *port &= !(1 << pin);
}

// Set Bus of ledcube driver with given 8 bits buffer
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

// Set given ledcube layer with last state of ledcube[][] table
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

// Set on given LED
void setLed(uint8_t color, uint8_t layer, uint8_t column)
{
    ledcube[layer][color] |= 0x0001 << column;
}

// Set off given LED
void clrLed(uint8_t color, uint8_t layer, uint8_t column)
{
    ledcube[layer][color] &= ~(0x0001 << column);
}

// Check whether given RGB LED has any Color set on
bool isAvailable(uint8_t layer, uint8_t column)
{
    clrLed(led_to_blink.color, led_to_blink.layer, led_to_blink.column); // Remove unconfirmed move led
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

// Set all LEDs off
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

// Reset ledcube driver and the game, leave testing state
void resetLedcube(void)
{
    clearLedcube();
    animation_counter1 = 0;
    animation_counter2 = 0;
    animation_iterator1 = 0;
    animation_iterator2 = 0;
    blink_led_trigger = false;
    blink_toggle = false;
    go_up = true;
    game_ended = false;
    moves_left = LAYERS_NUM * COLUMNS_NUM;
    led_to_set.color = 0;
    led_to_set.layer = 0;
    led_to_set.column = 0;
    led_to_blink.color = 0;
    led_to_blink.layer = 0;
    led_to_blink.column = 0;
    led_to_confirm.color = 0;
    led_to_confirm.layer = 0;
    led_to_confirm.column = 0;
}

// Test condition of LEDs after power on
void testLedcube(void)
{
    if (animation_counter1 > TEST_SUBPERIOD)
    {
        animation_counter1 = 0;

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
                }
            }
        }
    }
    animation_counter1++;
}

// Check if player with given color has won the game
bool isWinner(uint8_t color)
{
    clrLed(led_to_blink.color, led_to_blink.layer, led_to_blink.column); // Remove unconfirmed move led
    uint16_t mask1;
    uint16_t mask2;
    uint16_t mask3;
    uint16_t mask4;
    bool found_gap1;
    bool found_gap2;
    bool found_gap3;
    bool found_gap4;
    //// (*1) MAPPING OF LEDCUBE LEDS:
    /* Y: 3  2  1  0
        --------------  X:
        | O  O  O  O |  0
        | O  O  O  O |  1
        | O  O  O  O |  2
        | O  O  O  O |  3
        |   <LOGO>   |
        --------------
        All leds in each layer for every color are represented by 1 bit
        in Word (2 Bytes) to which they are mapped in such a way that
        LED's bit has a position calculated as POS = X + 4*Y accordingly
        to the shown ledcube's layout shown above (*1).
    */
    //// WIN CASES:
    for (uint8_t layer = 0; layer < LAYERS_NUM; layer++)
    {
        for (uint8_t row = 0; row < LEDCUBE_SIZE; row++)
        {
            // Vertical lines in horizontal planes
            // (16 cases)
            // (bird's eye view)
            //   0 0 0 1   ||   0 0 1 0   ||   0 1 0 0   ||   1 0 0 0
            //   0 0 0 1   ||   0 0 1 0   ||   0 1 0 0   ||   1 0 0 0
            //   0 0 0 1   ||   0 0 1 0   ||   0 1 0 0   ||   1 0 0 0
            //   0 0 0 1   ||   0 0 1 0   ||   0 1 0 0   ||   1 0 0 0
            mask1 = 0x000F << (4 * row);
            if ((ledcube[layer][color] & mask1) == mask1)
            {
                mask2 = 0;
                return true;
            }
            // Horizontal lines in horizontal planes
            // (16 cases)
            // (bird's eye view)
            //   1 1 1 1   ||   0 0 0 0   ||   0 0 0 0   ||   0 0 0 0 
            //   0 0 0 0   ||   1 1 1 1   ||   0 0 0 0   ||   0 0 0 0
            //   0 0 0 0   ||   0 0 0 0   ||   1 1 1 1   ||   0 0 0 0
            //   0 0 0 0   ||   0 0 0 0   ||   0 0 0 0   ||   1 1 1 1
            mask1 = 0x1111 << row;
            if ((ledcube[layer][color] & mask1) == mask1)
            {
                mask1 = 0;
                return true;
            }
        }
        // Crosswise lines in horizontal planes
        // (8 cases)
        // (bird's eye view)
        //  0 0 0 1   ||   1 0 0 0
        //  0 0 1 0   ||   0 1 0 0
        //  0 1 0 0   ||   0 0 1 0
        //  1 0 0 0   ||   0 0 0 1
        mask1 = 0x1248;
        if ((ledcube[layer][color] & mask1) == mask1)
        {
            mask1 = 0;
            return true;
        }
        mask1 = 0x8421;
        if ((ledcube[layer][color] & mask1) == mask1)
        {
            mask1 = 0;
            return true;
        }
    }

    // Vertical lines in vertical planes - columns
    // (16 cases)
    // (bird's eye view)
    //  0 0 0 1   ||   0 0 0 0   ||   0 0 0 0   ||   0 0 0 0   ||   0 0 1 0   ||   0 0 0 0   ||   0 0 0 0   ||   0 0 0 0
    //  0 0 0 0   ||   0 0 0 1   ||   0 0 0 0   ||   0 0 0 0   ||   0 0 0 0   ||   0 0 1 0   ||   0 0 0 0   ||   0 0 0 0
    //  0 0 0 0   ||   0 0 0 0   ||   0 0 0 1   ||   0 0 0 0   ||   0 0 0 0   ||   0 0 0 0   ||   0 0 1 0   ||   0 0 0 0
    //  0 0 0 0   ||   0 0 0 0   ||   0 0 0 0   ||   0 0 0 1   ||   0 0 0 0   ||   0 0 0 0   ||   0 0 0 0   ||   0 0 1 0

    //  0 1 0 0   ||   0 0 0 0   ||   0 0 0 0   ||   0 0 0 0   ||   1 0 0 0   ||   0 0 0 0   ||   0 0 0 0   ||   0 0 0 0
    //  0 0 0 0   ||   0 1 0 0   ||   0 0 0 0   ||   0 0 0 0   ||   0 0 0 0   ||   1 0 0 0   ||   0 0 0 0   ||   0 0 0 0
    //  0 0 0 0   ||   0 0 0 0   ||   0 1 0 0   ||   0 0 0 0   ||   0 0 0 0   ||   0 0 0 0   ||   1 0 0 0   ||   0 0 0 0
    //  0 0 0 0   ||   0 0 0 0   ||   0 0 0 0   ||   0 1 0 0   ||   0 0 0 0   ||   0 0 0 0   ||   0 0 0 0   ||   1 0 0 0
    for (uint8_t column = 0; column < COLUMNS_NUM; column++)
    {
        found_gap1 = false;
        for (uint8_t hor_layer = 0; hor_layer < LAYERS_NUM; hor_layer++)
        {
            mask1 = 0x0001 << column;
            if ((ledcube[hor_layer][color] & mask1) != mask1)
            {
                found_gap1 = true;
            }
        }
        if (!found_gap1)
        {
            return true;
        }
    }
    // Crosswise lines in vertical planes - for both orthogonal sets of verical planes
    // (16 cases)
    // (side view)
    //  1 0 0 0   ||   0 0 0 1
    //  0 1 0 0   ||   0 0 1 0
    //  0 0 1 0   ||   0 1 0 0
    //  0 0 0 1   ||   1 0 0 0
    for (uint8_t vert_layer = 0; vert_layer < LAYERS_NUM; vert_layer++)
    {
        found_gap1 = false;
        found_gap2 = false;
        found_gap3 = false;
        found_gap4 = false;
        for (uint8_t hor_layer = 0; hor_layer < LAYERS_NUM; hor_layer++)
        {
            mask1 = (0x0001 << (4 * vert_layer + hor_layer));
            mask2 = ((0x0008 << (4 * vert_layer)) >> hor_layer);
            mask3 = (0x0001 << (vert_layer + 4 * hor_layer));
            mask4 = ((0x1000 << vert_layer) >> (4 * hor_layer));
            if ((ledcube[hor_layer][color] & mask1) != mask1)
            {
                found_gap1 = true;
            }
            if ((ledcube[hor_layer][color] & mask2) != mask2)
            {
                found_gap2 = true;
            }
            if ((ledcube[hor_layer][color] & mask3) != mask3)
            {
                found_gap3 = true;
            }
            if ((ledcube[hor_layer][color] & mask4) != mask4)
            {
                found_gap4 = true;
            }
        }
        if (!found_gap1)
        {
            return true;
        }
        if (!found_gap2)
        {
            return true;
        }
        if (!found_gap3)
        {
            return true;
        }
        if (!found_gap4)
        {
            return true;
        }
    }
    // Crosswise lines in vertical and horizontal planes
    // (4 cases)
    // (side view)
    //  1 0 0 0   ||   1 0 0 0   ||   0 0 0 1   ||   0 0 0 1
    //  0 1 0 0   ||   0 1 0 0   ||   0 0 1 0   ||   0 0 1 0
    //  0 0 1 0   ||   0 0 1 0   ||   0 1 0 0   ||   0 1 0 0
    //  0 0 0 1   ||   0 0 0 1   ||   1 0 0 0   ||   1 0 0 0
    // (bird's eye view)
    //  1 0 0 0   ||   0 0 0 1   ||   0 0 0 1   ||   0 0 0 1
    //  0 1 0 0   ||   0 0 1 0   ||   0 0 1 0   ||   0 0 1 0
    //  0 0 1 0   ||   0 1 0 0   ||   0 1 0 0   ||   0 1 0 0
    //  0 0 0 1   ||   1 0 0 0   ||   1 0 0 0   ||   1 0 0 0
    found_gap1 = false;
    found_gap2 = false;
    found_gap3 = false;
    found_gap4 = false;
    for (uint8_t vert_layer = 0; vert_layer < LAYERS_NUM; vert_layer++)
    {
        mask1 = (0x0001 << (5 * vert_layer));
        mask2 = (0x0008 << (3 * vert_layer));
        if ((ledcube[vert_layer][color] & mask1) != mask1)
        {
            found_gap1 = true;
        }
        if ((ledcube[LAYER3 - vert_layer][color] & mask1) != mask1)
        {
            found_gap3 = true;
        }
        if ((ledcube[vert_layer][color] & mask2) != mask2)
        {
            found_gap2 = true;
        }
        if ((ledcube[LAYER3 - vert_layer][color] & mask2) != mask2)
        {
            found_gap4 = true;
        }
    }
    if (!found_gap1)
    {
        return true;
    }
    if (!found_gap2)
    {
        return true;
    }
    if (!found_gap3)
    {
        return true;
    }
    if (!found_gap4)
    {
        return true;
    }

    return false;
}

// Read 1 Byte of buffered data from UART
void receive(void)
{
    if (Serial1.available() > 0)
    {
        received = (uint8_t)Serial1.read();
        react_trigger = true;
    }
}

// Transmit next message
void transmit(void)
{
    Serial1.write(message_to_transmit);
}

// Recognize received message and take needed actions
void react(void)
{
    // MOVE Message received
    if (received >> 6 != 0)
    {
        led_to_confirm.color = (received >> 6) - 1;
        led_to_confirm.layer = ((received & 0x30) >> 4);
        led_to_confirm.column = (received & 0x0F);
        // Check if this move doesn't eclipse previous
        if (isAvailable(led_to_confirm.layer, led_to_confirm.column))
        {
            led_to_blink = led_to_confirm;
            blink_led_trigger = true;
        }
        else
        {
            message_to_transmit = INCORRECT_MOVE_MSG;
            transmit_trigger = true; // Trigger feedback to Sender of MOVE message
            blink_led_trigger = false;
            led_to_blink.color = 0;
            led_to_blink.layer = 0;
            led_to_blink.column = 0;
        }
    }
    // Other messages received
    else
    {
        switch (received)
        {
        case INIT_GAME_MSG:
            test_ended = true;
            reset_trigger = true;
            break;
        case EXIT_GAME_MSG:
            test_ended = false;
            reset_trigger = true;
            break;
        case CONFIRM_MOVE_MSG:
            // Check if this move doesn't eclipse previous
            if (isAvailable(led_to_blink.layer, led_to_blink.column))
            {
                led_to_set = led_to_confirm;
                setLed(led_to_set.color, led_to_set.layer, led_to_set.column);
                message_to_transmit = CORRECT_MOVE_MSG;

                blink_led_trigger = false;
                led_to_blink.color = 0;
                led_to_blink.layer = 0;
                led_to_blink.column = 0;

                // Check whether this move ends game
                if (isWinner(led_to_set.color))
                {
                    switch (led_to_set.color)
                    {
                    case RED:
                        game_result = RED_WON;
                        game_ended = true;
                        message_to_transmit = RED_WON_MSG;
                        break;
                    case GREEN:
                        game_result = GREEN_WON;
                        game_ended = true;
                        message_to_transmit = GREEN_WON_MSG;
                        break;
                    case BLUE:
                        game_result = BLUE_WON;
                        game_ended = true;
                        message_to_transmit = BLUE_WON_MSG;
                        break;
                    default:
                        break;
                    }
                }

                // Keep track of leds available to players
                if (--moves_left == 0)
                {
                    game_result = DRAW;
                    game_ended = true;
                    message_to_transmit = DRAW_MSG; // Draw if no more moves possible
                }
            }
            else
            {
                message_to_transmit = INCORRECT_MOVE_MSG;
            }
            transmit_trigger = true; // Trigger feedback to Sender of MOVE message
            break;
        default:
            break;
        }
    }
    react_trigger = false;
}

// Visualize result of gameplay
void presentGameResult(void)
{
    switch (game_result)
    {
    case DRAW:
        presentDraw();
        break;
    case RED_WON:
        presentWinner(RED);
        break;
    case GREEN_WON:
        presentWinner(GREEN);
        break;
    case BLUE_WON:
        presentWinner(BLUE);
        break;
    default:
        break;
    }
}

// Visualize who is a Winner
void presentWinner(uint8_t color)
{
    if (animation_counter1 > ANIM_SUBPERIOD)
    {
        animation_counter1 = 0;
        clearLedcube();
        ledcube[animation_iterator1][color] = 0xFFFF;

        animation_iterator1 = go_up ? animation_iterator1 + 1 : animation_iterator1 - 1;

        if (animation_iterator1 == 0)
        {
            go_up = true;
        }
        else if (animation_iterator1 >= LAYERS_NUM - 1)
        {
            go_up = false;
        }
    }
    animation_counter1++;
}

// Visualize Draw
void presentDraw(void)
{
    if (animation_counter1 > ANIM_SUBPERIOD)
    {
        animation_counter1 = 0;
        clearLedcube();
        ledcube[animation_iterator1][animation_iterator2] = 0xFFFF;

        animation_iterator1 = go_up ? animation_iterator1 + 1 : animation_iterator1 - 1;

        if (animation_iterator1 == 0)
        {
            go_up = true;
        }
        else if (animation_iterator1 >= LAYERS_NUM - 1)
        {
            go_up = false;
        }
    }
    if (animation_counter2 > 6 * ANIM_SUBPERIOD)
    {
        animation_counter2 = 0;
        if (++animation_iterator2 >= COLORS_NUM)
        {
            animation_iterator2 = 0;
        }
    }
    animation_counter1++;
    animation_counter2++;
}

// Blink Led before move gets confirmed
void blinkUnconfirmedLed()
{
    if (animation_counter1++ > 2 * ANIM_SUBPERIOD)
    {
        blink_toggle = blink_toggle ? false : true;
        animation_counter1 = 0;
    }

    if (blink_toggle)
    {
        setLed(led_to_confirm.color, led_to_confirm.layer, led_to_confirm.column);
    }
    else
    {
        clrLed(led_to_confirm.color, led_to_confirm.layer, led_to_confirm.column);
    }
}