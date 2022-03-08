#include <Servo.h>
#include <NewPing.h>
 #define TRIGGER_PIN  10 
 #define ECHO_PIN     11  
 #define MAX_DISTANCE 200 
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
Servo myservo;  
int pos = 0;   
int it = 10;

void setup() {
  myservo.attach(9); 
  Serial.begin(9600);
  delay(3000);
}

void loop() {
  int i = 0;
  int t = 0;
  int a = 0;
  for (i = 0; i < 180; i ++)
  {
    unsigned int uS = sonar.ping();
    myservo.write(i);
    delay(20);
    for (t = 0; t < it; t++)
    {
      uS = sonar.ping();
      a = uS/US_ROUNDTRIP_CM + a;
      delay(30);
    }
    
    a = a / (it-1);
    t = 0;
    Serial.println(a); 
    a = 0;
  }
}
