
const int LDR_PIN = 14;  // ADC input pin

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("LDR Test Started");
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);

  Serial.print("LDR Value: ");
  Serial.println(ldrValue);  

  delay(500);
}
