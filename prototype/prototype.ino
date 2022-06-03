void setup(){
  Serial.begin(9600);
  Serial.println("Begin");
}

void loop(){
  static int i = 0;
  Serial.print("Hello World ");
  delay(250);
  Serial.print(i++);
  Serial.print('\n');
  delay(250);
}
