#include <Servo.h>
#define LEDPIN 13

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

char state='q', new_state;
boolean newData = false, led_state = false;
long led_count = 0;

int pos = 0;    // variable to store the servo position

void setup ( )
{
  myservo.attach ( 9 );  // attaches the servo on pin 9 to the servo object 
  Serial.begin(9600);    // Begins serial communications
  pinMode(LEDPIN, OUTPUT); 
}

void loop() {
  recvOneChar();
  switch(new_state)
  {
    case  'q' : led_blink(); 
                break;

    case  'w' : pos = 180;
                myservo.write(pos);
                delay(5);
                new_state = 'q';
                break;

    case  'a' : if(pos - 10 > 0) pos = pos - 10;
                else  pos = 0;
                myservo.write(pos);
                delay(100);
                new_state = 'q';
                break;
    
    case  's' : pos = 0;
                myservo.write(pos);
                delay(5);
                new_state = 'q';
                break;

    case  'd' : if(pos + 10 < 180) pos = pos + 10;
                else  pos = 180;
                myservo.write(pos);
                delay(100);
                new_state = 'q';
                break;

    case  ' ' : if (pos!=0&&pos!=180) pos = 0;
                else  pos = 180-pos;

                myservo.write(pos);
                delay(1000);
                new_state = 'q';
                break;

    case  'e' : pos = 90;
                myservo.write(pos);
                delay(500);
                new_state = 'q';
                break;

    default   : new_state = 'q';
  }
}

void recvOneChar()
{
 if (Serial.available() > 0)
 {
  new_state = Serial.read();
  newData = true;
 }
}

void led_blink()
{
  if(led_count<100000) led_count = led_count + 1;
  else
  {
    led_count = 0;
    delay(10);
    if(led_state == true)
    {
      led_state = false;
      digitalWrite(LEDPIN,LOW);
    }
    else
    {
      led_state = true;
      digitalWrite(LEDPIN, HIGH);
    }
  }
}