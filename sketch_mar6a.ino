#define mq135 A0
#define buzzer D1
int threshold = 499;
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
pinMode(buzzer, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
int airpol_ppm = analogRead(mq135);
Serial.println(airpol_ppm);
if(threshold > 500){
  Serial.println("High Air Poll Zone");
  digitalWrite(buzzer,1);
  delay(100);
}else {
  digitalWrite(buzzer,0);
}
}
