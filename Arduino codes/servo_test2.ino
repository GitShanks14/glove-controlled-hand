#include <Servo.h>

#define RDiv 10000.0
#define R0max 4960.0
#define R0min 9.2

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int v   = 0;    // variable to store inputted analog resistance

void setup ( )
{
  myservo.attach ( 9 );  // attaches the servo on pin 9 to the servo object
  Serial.begin (9600);  
}
void loop ( )
{
  //myservo.write(180);
  long  v_sum = 0;
  for(int i = 0 ; i < 16 ; ++ i)
  {
    v_sum += analogRead(A0);
    delay(5);
  }
  
  v = v_sum/16;

  float R = RDiv*(1024-v)/1024.0;

  pos = (R-R0min)/(R0max-R0min)*180;

  Serial.print(v);
  Serial.print("\t");
  Serial.print(R);
  Serial.print("\t");
  Serial.println(pos);


  myservo.write(pos);
  
  /*
  for ( pos = 0 ; pos <= 180 ; pos += 1 ) // goes from 0 degrees to 180 degrees
  {
    // in steps of 1 degree
    myservo.write ( pos );              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 15ms for the servo to reach the position
  }
  delay(5);
  for (pos = 180; pos >= 0; pos -= 1) // goes from 180 degrees to 0 degrees
  {
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 15ms for the servo to reach the position
  }
  delay(5);
  */

}
