#include <Servo.h>
#define LEDPIN 13

#define RDiv 57000.0

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
1 Servo motor per finger is used. Nomenclature:
L - Little  - 0
R - Ring    - 1
M - Middle  - 2
P - Pointer - 3
T - Thumb   - 4
One servo object is created per motor, and each servo is controlled by 1 PWM pin on the Arduino

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
The fingers can be in a spectrum of states ranging from the resting position to the gripping position. 
During the calibration step, the resting and gripping positions of the servo motor are assigned. 
The position is updated according to the resistance values measured.  

MPin: stores pin numbers of PWM pins corresponding to the motors
ResPos: stores resting positions corresponding to each finger
GriPos: stores gripping positions corresponding to each finger
MPos  : Motor positions at any given point in time

ResPos and GriPos are to be set during the callibration step

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Pending: Iteration 2:

RPin: stores pin numbers of voltage divider outputs
RMax: stores RMax values
RMin: stores RMin values
Vin : stores voltage input values  

RMax, Rmin to be assigned at calibration step; RMax, RMin, Vin -> MPos

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Testing program 1:
assign L, R, M, P, T the power to toggle finger positions

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Notes:
1. Tape together the pins, design perfboard for final design. Make sure power and ground lines are fine, do not solder in the PWM control ports
2. Final design loop: Read: "for" loop, followed by write: hardcoded
3. Second iteration: Add time averaging of resistance measurements, introduce delay stage
*/

Servo L,R,M,P,T;
const byte MPin[5] = {10,9,6,5,3};    
const byte ResPos[5] = {0, 90, 180, 180, 0};
const byte GriPos[5] = {90,0,80,90,100};
int  MPos[5] = {ResPos[0], ResPos[1], ResPos[2], ResPos[3], ResPos[4]};
byte  m = 0;

// const byte RPin[5] = {A1, A2, A3, A4, A5};
const byte RPin[5] = {A0, A1, A2, A3, A4};
const float RMax[5] = {10000.0, 10000.0, 10000.0, 10000.0, 10000.0};
const float RMin[5] = { 5000.0,  5000.0,  5000.0,  5000.0,  5000.0};
int Vin[5] = {0,0,0,0,0};

int Vavg[5] = {0,0,0,0,0};
int VMax[5] = {20,-15,15,180,10};

char new_state='q';
/*
State list:
  q - blink
  l, r, m, p, t - toggle finger; return to q
*/

boolean newData = false, led_state = false, control_state = false;
long led_count = 0;

int tf = 0;

void setup ( )
{
  // Attach servo motors to PWM pins
  L.attach ( MPin[0] );
  R.attach ( MPin[1] );
  M.attach ( MPin[2] );
  P.attach ( MPin[3] );
  T.attach ( MPin[4] );

  // Bring motors to resting positions
  L.write(ResPos[0]);
  R.write(ResPos[1]);
  M.write(ResPos[2]);
  P.write(ResPos[3]);
  T.write(ResPos[4]);

  Serial.begin(115200);    // Begins serial communications
  pinMode(LEDPIN, OUTPUT); 
}

void loop() {
  recvOneChar();
  switch(new_state)
  {
    case  'q' : led_blink(); 
                if(control_state == true)
                {
                  ReadAll();
                  WriteAll();
                  trackfinger(tf);                
                  // PrintState();
                  // delay(10);
                }
                break;

    case  'l' : m = 0;
                if ( (MPos[m]!=ResPos[m]) && (MPos[m]!=GriPos[m]) ) MPos[m] = ResPos[m];
                else  MPos[m] = ResPos[m] + GriPos[m] - MPos[m];

                L.write(MPos[m]);
                delay(500);
                new_state = 'q';
                break;

    case  'r' : m = 1;
                if ( (MPos[m]!=ResPos[m]) && (MPos[m]!=GriPos[m]) ) MPos[m] = ResPos[m];
                else  MPos[m] = ResPos[m] + GriPos[m] - MPos[m];

                R.write(MPos[m]);
                delay(500);
                new_state = 'q';
                break;

    case  'm' : m = 2;
                if ( (MPos[m]!=ResPos[m]) && (MPos[m]!=GriPos[m]) ) MPos[m] = ResPos[m];
                else  MPos[m] = ResPos[m] + GriPos[m] - MPos[m];

                M.write(MPos[m]);
                delay(500);
                new_state = 'q';
                break;

    case  'p' : m = 3;
                if ( (MPos[m]!=ResPos[m]) && (MPos[m]!=GriPos[m]) ) MPos[m] = ResPos[m];
                else  MPos[m] = ResPos[m] + GriPos[m] - MPos[m];

                P.write(MPos[m]);
                delay(500);
                new_state = 'q';
                break;

    case  't' : m = 4;
                if ( (MPos[m]!=ResPos[m]) && (MPos[m]!=GriPos[m]) ) MPos[m] = ResPos[m];
                else  MPos[m] = ResPos[m] + GriPos[m] - MPos[m];

                T.write(MPos[m]);
                delay(500);
                new_state = 'q';
                break;

    case  'c' : calibrate();
                new_state = 'q';
                break;    

    case  'u' : control_state = !control_state;
                new_state = 'q';
                break;    

    case  'x' : control_state = false;
                for(int i = 0 ; i < 5 ; ++ i) MPos[i] = ResPos[i];
                WriteAll();
                PrintState();
                delay(10);
                new_state = 'q';
                break;    

    default   : // PrintState();
                trackfinger(tf);
                Serial.println(Vin[0]);
                if(control_state == true)
                {
                  ReadAll();
                  WriteAll();
                  // PrintState();
                  // delay(10);
                }
                new_state = 'q';
                break;
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

void PrintState()
{
  Serial.print("|");
  for( m=0 ; m < 5 ; ++m ) 
  {
    Serial.print(MPos[m]);
    Serial.print('|');
  }
  Serial.print("\n");
}

// EXPERIMENTAL SECTION AHEAD

void ReadAll()
{
  // for(int m = 0 ; m < 5 ; ++ m) ReadRes(m); 
  long  v_sum[5] = {0,0,0,0,0};
  for(int i = 0 ; i < 16 ; ++ i)
  {
    for(int m = 0 ; m < 5 ; ++ m)
    {
      v_sum[m] += analogRead(RPin[m]) - Vavg[m];
      delay(1);
    }        
  }
  
  for(int m = 0 ; m < 5 ; ++ m)
  {
    Vin[m] = v_sum[m]/16;
    MPos[m] = float(Vin[m])/float(VMax[m]) * (GriPos[m] - ResPos[m]) + ResPos[m];
    if(ResPos[m]>GriPos[m])
    {
      if(MPos[m]>ResPos[m]) MPos[m] = ResPos[m];
     else if(MPos[m]<GriPos[m]) MPos[m] = GriPos[m];
   }
    else
    {
     if(MPos[m]<ResPos[m]) MPos[m] = ResPos[m];
     else if(MPos[m]>GriPos[m]) MPos[m] = GriPos[m];
    }
  }
}

/* OBSOLETE
void ReadRes(int m)
{
  // Paralellize segment
  long  v_sum = 0;
  for(int i = 0 ; i < 16 ; ++ i)
  {
    v_sum += analogRead(RPin[m]) - Vavg[m];
    delay(1);
  }
  
  Vin[m] = v_sum/16;

//  float R = RDiv*(1024-Vin[m])/1024.0;
//  MPos[m] = ResPos[m]+(R-RMin[m])/(RMax[m]-RMin[m])*(GriPos[m]-ResPos[m]);

// MISTAKE
  // MPos[m] = map(Vin[m], 0, VMax[m], ResPos[m], GriPos[m]);
  // MPos[m] = constrain(MPos[m], ResPos[m], GriPos[m]);
  MPos[m] = float(Vin[m])/float(VMax[m]) * (GriPos[m] - ResPos[m]) + ResPos[m];
  if(ResPos[m]>GriPos[m])
  {
    if(MPos[m]>ResPos[m]) MPos[m] = ResPos[m];
    else if(MPos[m]<GriPos[m]) MPos[m] = GriPos[m];
  }
  else
  {
    if(MPos[m]<ResPos[m]) MPos[m] = ResPos[m];
    else if(MPos[m]>GriPos[m]) MPos[m] = GriPos[m];
  }
}
*/

void WriteAll()
{
  L.write(MPos[0]);
 // R.write(MPos[1]);
 // M.write(MPos[2]);
 // P.write(MPos[3]);
 // T.write(MPos[4]);
}

void calibrate()
{
  for(int m = 0 ; m < 5 ; ++ m) Vavg[m] = 0;
  Serial.println("Calibration in progress: Please your hand open and stay still");

  long  v_sum[5] = {0,0,0,0,0};
  for(int i = 0 ; i < 16 ; ++ i)
  {
    for(int m = 0 ; m < 5 ; ++ m)
    {
      v_sum[m] += analogRead(RPin[m]);
    }    
    delay(500);
  }
  
  for(int m = 0 ; m < 5 ; ++ m) Vavg[m] = v_sum[m]/16;

  Serial.println("Calibration complete");
}

void trackfinger(int tf)
{
  //for(int i = 0 ; i < 5 ; ++ i )
  //{
    Serial.print(Vin[tf]);
    Serial.print("\t");
    Serial.print(MPos[tf]);
  // }
  Serial.print("\r\n");
}