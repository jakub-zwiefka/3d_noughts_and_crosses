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

//// AT32U4 port data register pins mapping
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
// AT32U4 port pins to communicate Bluetooth module over USART
#define TXPPin PORTD3; 
#define RXPPin PORTD2; 

//// AT32U4 port data registers of every pin mapped
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
// AT32U4 ports to communicate Bluetooth module over USART
#define TXPort PORTD; 
#define RXPort PORTD; 

//// AT32U4 port data direction register pins mapping
// AT32U4 port data direction pins used to configure bus setting selected group of ledcube columns
#define BUS0DD DDD6	
#define BUS1DD DDB7	
#define BUS2DD DDB6	
#define BUS3DD DDB5	
#define BUS4DD DDB4	
#define BUS5DD DDE6	
#define BUS6DD DDD7	
#define BUS7DD DDC6	
// AT32U4 port data direction pins used to select ledcube columms' group to be configured
#define R0DD DDC7 // Red collumns 1-8
#define R1DD DDF7 // Red collumns 9-16
#define G0DD DDF6 // Green collumns 1-8
#define G1DD DDF5 // Green collumns 9-16
#define B0DD DDF4 // Blue collumns 1-8
#define B1DD DDF1 // Blue collumns 9-16
// AT32U4 port data direction pins used to select ledcube layer to be activated
#define L0DD DDF0 // Layer 0
#define L1DD DDD4 // Layer 1
#define L2DD DDD0 // Layer 2
#define L3DD DDD1 // Layer 3
// AT32U4 port data direction pins used to communicate Bluetooth module over USART
#define TXDD DDD3
#define RXDD DDD2

//// AT32U4 port data direction registers of every pin mapped
// AT32U4 port direction registers to configure bus setting selected group of ledcube columns
#define BUS0DDR DDRD	
#define BUS1DDR DDRB	
#define BUS2DDR DDRB	
#define BUS3DDR DDRB	
#define BUS4DDR DDRB	
#define BUS5DDR DDRE	
#define BUS6DDR DDRD	
#define BUS7DDR DDRC	
// AT32U4 port direction registers to configure pins selecting ledcube columms' group to be configured
#define R0DDR DDRC // Red collumns 1-8
#define R1DDR DDRF // Red collumns 9-16
#define G0DDR DDRF // Green collumns 1-8
#define G1DDR DDRF // Green collumns 9-16
#define B0DDR DDRF // Blue collumns 1-8
#define B1DDR DDRF // Blue collumns 9-16
// AT32U4 port direction registers to configure pins selecting ledcube layer to be activated
#define L0DDR DDRF // Layer 0
#define L1DDR DDRD // Layer 1
#define L2DDR DDRD // Layer 2
#define L3DDR DDRD // Layer 3
// AT32U4 port direction registers to configure pins communicating with Bluetooth module over USART
#define TXDDR DDRD
#define RXDDR DDRD

////Controler modes
#define TEST 0
#define GAME_ON 1
#define GAME_END 2

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
#define TOP_LAYER 3
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

//// Message type constants
//                   Player:Red/Green/Blue   Layer  Column
// (*) MOVE MSG     ->      {01}/{10}/{11} + {--} + {----} (b)
//     OTHER MSGs   ->      {00} + {------} (b)
// Receivable messages
#define INIT_GAME_MSG 0x00
#define EXIT_GAME_MSG 0x01
#define CONFIRM_MOVE_MSG 0x02
#define MOVE_MSG 0x03
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
bool blink_toggle = true;

// Communication
uint8_t received;
uint8_t received_msg_type;
uint8_t message_to_transmit;

// Game
uint8_t moves_left = LAYERS_NUM * COLUMNS_NUM;
uint8_t game_result;

// Control
uint8_t controller_mode = TEST;
bool reset_trigger = false;
bool transmit_trigger = false;
bool react_trigger = false;
bool blink_trigger = false;
bool unconfirmed_move_received = false;

// Next LED to be set in ledcube[][] table with setLed() function
ledcube_point led_to_set = {
    0, // color
    0, // layer
    0, // column
};

// LED needing confirmation not yet set in ledcube[][] table with setLed() function
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
////////// INITIALIZATON
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
void setup()
{
    //// Set as outputs pins configuring bus setting selected group of ledcube columns
    //setRegBit(&BUS0DDR, BUS0DD);
    //setRegBit(&BUS1DDR, BUS1DD);
    //setRegBit(&BUS2DDR, BUS2DD);
    //setRegBit(&BUS3DDR, BUS3DD);
    //setRegBit(&BUS4DDR, BUS4DD);
    //setRegBit(&BUS5DDR, BUS5DD);
    //setRegBit(&BUS6DDR, BUS6DD);
    //setRegBit(&BUS7DDR, BUS7DD);

    //// Set as outputs pins selecting ledcube layer to be activated
    //setRegBit(&L0DDR, L0DD);
    //setRegBit(&L1DDR, L1DD);
    //setRegBit(&L2DDR, L2DD);
    //setRegBit(&L3DDR, L3DD);

    //// Set as outputs pins selecting ledcube columms' group to be configured
    //setRegBit(&R0DDR, R0DD);
    //setRegBit(&R1DDR, R1DD);
    //setRegBit(&G0DDR, G0DD);
    //setRegBit(&G1DDR, G1DD);
    //setRegBit(&B0DDR, B0DD);
    //setRegBit(&B1DDR, B1DD);

    //// Set RX as input and TX as output
    //setRegBit(&TXDDR, TXDD);
    //clrRegBit(&RXDDR, RXDD);

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

    // Set initially to Low State lines driving ledcube 
    clrRegBit(&L0Port, L0PPin);
    clrRegBit(&L1Port, L1PPin);
    clrRegBit(&L2Port, L2PPin);
    clrRegBit(&L3Port, L3PPin);
    clrRegBit(&R0Port, R0PPin);
    clrRegBit(&R1Port, R1PPin);
    clrRegBit(&G0Port, G0PPin);
    clrRegBit(&G1Port, G1PPin);
    clrRegBit(&B0Port, B0PPin);
    clrRegBit(&B1Port, B1PPin);

    // Init USART communication
    Serial1.begin(9600);
}


//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
////////// TASKS
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

    // Communicate with player interface device
    communicate();

    // Testing procedure
    if (controller_mode == TEST)
    {
        testLedcube(); // Test ledcube when not busy
    }

    // Present game result procedure
    if (controller_mode == GAME_END)
    {
        presentGameResult(); // Visualize on ledcube result of game
    }

    if (blink_trigger)
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
            clrRegBit(&L3Port, L3PPin);
            setLayer(current_layer);
            setRegBit(&L0Port, L0PPin);
            break;

        case LAYER1:
            clrRegBit(&L0Port, L0PPin);
            setLayer(current_layer);
            setRegBit(&L1Port, L1PPin);
            break;

        case LAYER2:
            clrRegBit(&L1Port, L1PPin);
            setLayer(current_layer);
            setRegBit(&L2Port, L2PPin);
            break;

        case LAYER3:
            clrRegBit(&L2Port, L2PPin);
            setLayer(current_layer);
            setRegBit(&L3Port, L3PPin);
            break;

        default:
            break;
        }

        if (++current_layer >= LAYERS_NUM)
        {
            current_layer = 0;
        }
    }

    if (blink_trigger)
    {
        clrLed(led_to_confirm.color, led_to_confirm.layer, led_to_confirm.column); // Remove unconfirmed move led
    }
}

////////// FUNCTIONS
// Set on given bit of register
void setRegBit(volatile uint8_t *reg, uint8_t bit)
{
    *reg |= (1 << bit);
}

// Set off given bit of register
void clrRegBit(volatile uint8_t *reg, uint8_t bit)
{
    *reg &= !(1 << bit);
}

// Set given bit of register to given value (0 or 1)
void modifyRegBit(volatile uint8_t *reg, uint8_t bit, uint8_t val)
{
    *reg = (*reg & ~(1 << bit)) | ((val << bit)&(1 << bit));
}

// Set Bus of ledcube driver with given 8 bits buffer
void setBus(uint8_t buffer)
{
    modifyRegBit(&BUS0Port, BUS0PPin, (buffer & 0x01));
    modifyRegBit(&BUS1Port, BUS1PPin, ((buffer >> 1) & 0x01));
    modifyRegBit(&BUS2Port, BUS2PPin, ((buffer >> 2) & 0x01));
    modifyRegBit(&BUS3Port, BUS3PPin, ((buffer >> 3) & 0x01));
    modifyRegBit(&BUS4Port, BUS4PPin, ((buffer >> 4) & 0x01));
    modifyRegBit(&BUS5Port, BUS5PPin, ((buffer >> 5) & 0x01));
    modifyRegBit(&BUS6Port, BUS6PPin, ((buffer >> 6) & 0x01));
    modifyRegBit(&BUS7Port, BUS7PPin, ((buffer >> 7) & 0x01));
}

// Set given ledcube layer with last state of ledcube[][] table
void setLayer(uint8_t layer)
{
    modifyRegBit(&R0Port, R0PPin, HIGH);
    setBus(ledcube[layer][RED] & 0xFF);
    modifyRegBit(&R0Port, R0PPin, LOW);
    modifyRegBit(&R1Port, R1PPin, HIGH);
    setBus(ledcube[layer][RED] >> 8);
    modifyRegBit(&R1Port, R1PPin, LOW);
    modifyRegBit(&G0Port, G0PPin, HIGH);
    setBus(ledcube[layer][GREEN] & 0xFF);
    modifyRegBit(&G0Port, G0PPin, LOW);
    modifyRegBit(&G1Port, G1PPin, HIGH);
    setBus(ledcube[layer][GREEN] >> 8);
    modifyRegBit(&G1Port, G1PPin, LOW);
    modifyRegBit(&B0Port, B0PPin, HIGH);
    setBus(ledcube[layer][BLUE] & 0xFF);
    modifyRegBit(&B0Port, B0PPin, LOW);
    modifyRegBit(&B1Port, B1PPin, HIGH);
    setBus(ledcube[layer][BLUE] >> 8);
    modifyRegBit(&B1Port, B1PPin, LOW);
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

// Reset ledcube driver and the game, leave testing state
void resetLedcube(void)
{
    clearLedcube();
    animation_counter1 = 0;
    animation_counter2 = 0;
    animation_iterator1 = 0;
    animation_iterator2 = 0;
    blink_trigger = false;
    blink_toggle = false;
    go_up = true;
    moves_left = LAYERS_NUM * COLUMNS_NUM;
    unconfirmed_move_received = false;
    led_to_set.color = 0;
    led_to_set.layer = 0;
    led_to_set.column = 0;
    led_to_confirm.color = 0;
    led_to_confirm.layer = 0;
    led_to_confirm.column = 0;
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

// Check whether given RGB LED has any Color set on
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

// Receive from USART, react to received messages and transmit response if needed
void communicate(void)
{
    // Receiving procedure
    receive(); // Check for new messages and trigger reacting to new ones

    // Reacting to received procedure
    if (react_trigger)
    {
        react();
        react_trigger = false; // Stop reacting till message received
    }

    // Transmitting reaction outcome procedure
    if (transmit_trigger)
    {
        transmit();
        transmit_trigger = false; // Stop transmitting till needed
    }
}

// Read 1 Byte of buffered data from USART
void receive(void)
{
    if (Serial1.available() > 0)
    {
        received = (uint8_t)Serial1.read();
        react_trigger = true;
    }
}

// Recognize received message and take needed actions
void react(void)
{
    // Decode received message
    decodeReceived();

    if (controller_mode == GAME_ON)
    {
        switch (received_msg_type)
        {
        case MOVE_MSG:
        {
            led_to_confirm.color = (received >> 6) - 1;
            led_to_confirm.layer = ((received & 0x30) >> 4);
            led_to_confirm.column = (received & 0x0F);
            // Check if this move doesn't eclipse previous
            if (isAvailable(led_to_confirm.layer, led_to_confirm.column))
            {
                unconfirmed_move_received = true;
                blink_trigger = true;
                message_to_transmit = CORRECT_MOVE_MSG;
                transmit_trigger = true; // Trigger feedback to Sender of MOVE message
            }
            else
            {
                unconfirmed_move_received = false;
                blink_trigger = false;
                message_to_transmit = INCORRECT_MOVE_MSG;
                transmit_trigger = true; // Trigger feedback to Sender of MOVE message
            }
            break;
        }
        case CONFIRM_MOVE_MSG:
        {
            if (unconfirmed_move_received)
            {
                blink_trigger = false;

                led_to_set = led_to_confirm;
                setLed(led_to_set.color, led_to_set.layer, led_to_set.column);
                message_to_transmit = CORRECT_MOVE_MSG;

                // Check whether this move ends game
                if (isWinner(led_to_set.color))
                {
                    switch (led_to_set.color)
                    {
                    case RED:
                        game_result = RED_WON;
                        controller_mode = GAME_END;
                        message_to_transmit = RED_WON_MSG;
                        break;
                    case GREEN:
                        game_result = GREEN_WON;
                        controller_mode = GAME_END;
                        message_to_transmit = GREEN_WON_MSG;
                        break;
                    case BLUE:
                        game_result = BLUE_WON;
                        controller_mode = GAME_END;
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
                    controller_mode = GAME_END;
                    message_to_transmit = DRAW_MSG; // Draw if no more moves possible
                }

                unconfirmed_move_received = false;
            }
            else
            {
                message_to_transmit = INCORRECT_MOVE_MSG;
            }
            transmit_trigger = true; // Trigger feedback to Sender of MOVE message
            break;
        }
        case EXIT_GAME_MSG:
            controller_mode = TEST;
            reset_trigger = true;
            break;
        default:
            break;
        }
    }
    // Other messages received
    else if (controller_mode == GAME_END)
    {
        switch (received_msg_type)
        {
        case INIT_GAME_MSG:
            controller_mode = GAME_ON;
            reset_trigger = true;
            break;
        case EXIT_GAME_MSG:
            controller_mode = TEST;
            reset_trigger = true;
            break;
        default:
            break;
        }
    }
    else if (controller_mode == TEST)
    {
        switch (received_msg_type)
        {
        case INIT_GAME_MSG:
            controller_mode = GAME_ON;
            reset_trigger = true;
            break;
        default:
            break;
        }
    }
    react_trigger = false;
}

// Check if player with given color has won the game
bool isWinner(uint8_t color)
{
    uint16_t mask;
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
            mask = 0x000F << (LEDCUBE_SIZE * row);
            if ((ledcube[layer][color] & mask) == mask)
            {
                return true;
            }
            // Horizontal lines in horizontal planes
            // (16 cases)
            // (bird's eye view)
            //   1 1 1 1   ||   0 0 0 0   ||   0 0 0 0   ||   0 0 0 0 
            //   0 0 0 0   ||   1 1 1 1   ||   0 0 0 0   ||   0 0 0 0
            //   0 0 0 0   ||   0 0 0 0   ||   1 1 1 1   ||   0 0 0 0
            //   0 0 0 0   ||   0 0 0 0   ||   0 0 0 0   ||   1 1 1 1
            mask = 0x1111 << row;
            if ((ledcube[layer][color] & mask) == mask)
            {
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
        found_gap1 = false;
        found_gap2 = false;
        for (uint8_t row = 0; row < LEDCUBE_SIZE; row++)
        {
            mask = 0x0001 << (LEDCUBE_SIZE + 1) * row;
            if ((ledcube[layer][color] & mask) != mask)
            {
                found_gap1 = true;
            }
            mask = 0x1000 >> (LEDCUBE_SIZE + 1) * row;
            if ((ledcube[layer][color] & mask) != mask)
            {
                found_gap2 = true;
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
        for (uint8_t layer = 0; layer < LAYERS_NUM; layer++)
        {
            mask = 0x0001 << column;
            if ((ledcube[layer][color] & mask) != mask)
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
    for (uint8_t vert_plane = 0; vert_plane < LEDCUBE_SIZE; vert_plane++)
    {
        found_gap1 = false;
        found_gap2 = false;
        found_gap3 = false;
        found_gap4 = false;
        for (uint8_t hor_plane = 0; hor_plane < LEDCUBE_SIZE; hor_plane++)
        {
            mask = (0x0001 << (LEDCUBE_SIZE * vert_plane + hor_plane));
            if ((ledcube[hor_plane][color] & mask) != mask)
            {
                found_gap1 = true;
            }
            if ((ledcube[TOP_LAYER - hor_plane][color] & mask) != mask)
            {
                found_gap2 = true;
            }
            mask = (0x0001 << (vert_plane + LEDCUBE_SIZE * hor_plane));
            if ((ledcube[hor_plane][color] & mask) != mask)
            {
                found_gap3 = true;
            }
            if ((ledcube[TOP_LAYER - hor_plane][color] & mask) != mask)
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
    // Crosswise lines in both vertical and horizontal planes
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
    for (uint8_t plane = 0; plane < LAYERS_NUM; plane++)
    {
        mask = (0x0001 << ((LEDCUBE_SIZE + 1) * plane));
        if ((ledcube[plane][color] & mask) != mask)
        {
            found_gap1 = true;
        }
        if ((ledcube[TOP_LAYER - plane][color] & mask) != mask)
        {
            found_gap2 = true;
        }
        mask = (0x0008 << ((LEDCUBE_SIZE - 1) * plane));
        if ((ledcube[plane][color] & mask) != mask)
        {
            found_gap3 = true;
        }
        if ((ledcube[TOP_LAYER - plane][color] & mask) != mask)
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

// Transmit next message
void transmit(void)
{
    Serial1.write(message_to_transmit);
}

// Set type of received message
void decodeReceived(void)
{
    if (received >> 6 != 0)
    {
        received_msg_type = MOVE_MSG;
    }
    else
    {
        received_msg_type = received;
    }
}

// Visualize result of game
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

// Visualize a Draw
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
