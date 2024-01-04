#include <Arduino.h>

void setup() {
  Serial.begin(9600);
}
  
void loop() {
  Serial.println("Hello XIAO!");
  delay(2000);
}