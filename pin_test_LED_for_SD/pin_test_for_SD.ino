
const unsigned int LED_10{10};
const unsigned int LED_11{11};
const unsigned int LED_12{12};
const unsigned int LED_13{13};
void setup() {
  pinMode(LED_10, OUTPUT); //configure pins to be output
  pinMode(LED_11, OUTPUT);
  pinMode(LED_12, OUTPUT);
  pinMode(LED_13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);

  Serial.begin(115200);
  while(!Serial){};
  digitalWrite(LED_12, LOW);
  Serial.println("Pin 12 High");
  digitalWrite(LED_13, LOW);
  Serial.println("Pin 13 High");
  digitalWrite(14, HIGH);
  Serial.println("Pin 14 low");
  digitalWrite(15, HIGH);
  Serial.println("Pin 15 low");

}

void loop() {
//   digitalWrite(LED_13, LOW);
//   digitalWrite(LED_10, HIGH);
//   Serial.println("Pin 10 High");
//   delay(5000);

//   digitalWrite(LED_10, LOW);
//   digitalWrite(LED_11, HIGH);
//   Serial.println("Pin 11 high");
//   delay(5000);

//   digitalWrite(LED_11, LOW);
//   digitalWrite(LED_12, HIGH);
//   Serial.println("Pin 12 High");
//   delay(5000);

//   digitalWrite(LED_12, LOW);
//   digitalWrite(LED_13, HIGH);
//   Serial.println("Pin 13 High");
//   delay(5000);
}
