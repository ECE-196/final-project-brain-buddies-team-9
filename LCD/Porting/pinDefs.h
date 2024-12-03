// Pin Definition Header
//set up pins for buttons
//      UP Button
const byte interruptUpPin = 16;
volatile byte upPinState = LOW;
volatile byte upState = LOW;

//      DOWN Button
const byte interruptDownPin = 15;
volatile byte downPinState = LOW;
volatile byte downState = LOW;

//      RIGHT Button
const byte interruptRightPin = 14;
volatile byte rightPinState = LOW;
volatile byte rightState = LOW;

//      LEFT Button
const byte interruptLeftPin = 17;
volatile byte leftPinState = LOW;
volatile byte leftState = LOW;

//      TOP Button
const byte interruptTopPin = 1;
volatile byte topPinState = LOW;
volatile byte topState = LOW;

//      Select Button
const byte interruptSelectPin = 2;
volatile byte selectPinState = LOW;
volatile byte selectState = LOW;

//      Bottom Button
const byte interruptBottomPin = 3;
volatile byte bottomPinState = LOW;
volatile byte bottomState = LOW;
