#define LEDPIN 13
#define RDiv 57000.0

/*
A1: thumb
A2: little
A3: Ring finger
A4: Pointer
A5: Middle
*/

const byte RPin[5] = {A2, A3, A5, A4, A1};
// const float RMax[5] = {10000.0, 10000.0, 10000.0, 10000.0, 10000.0};
// const float RMin[5] = { 5000.0,  5000.0,  5000.0,  5000.0,  5000.0};
float R[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
unsigned int Vin[5] = {0,0,0,0,0};

unsigned int Vavg[5] = {0,0,0,0,0};

#define interval 10000
unsigned long CurrentMicros = 0;
unsigned long PreviousMicros = 0;

/*
const byte MPin[5] = {10,9,6,5,3};    
const byte ResPos[5] = {0, 90, 180, 180, 0};
const byte GriPos[5] = {90,0,80,90,100};
byte  MPos[5] = {ResPos[0], ResPos[1], ResPos[2], ResPos[3], ResPos[4]};
*/
byte  m = 0;

void setup() {
  // Initializing pins
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);
  pinMode(A5,INPUT);
  pinMode(LEDPIN, OUTPUT);

  Serial.begin(115200);    // Begins serial communications

  calibrate();
}

void loop() {

  CurrentMicros = micros();
  if( CurrentMicros - PreviousMicros >= interval )
  {
    PreviousMicros = CurrentMicros;
    for(int m = 0 ; m < 5 ; ++ m)
    {
      ReadRes(m); 
      char c1 = (Vin[m]>>8);
      char c2 = Vin[m] % 256;
      Serial.print(c2);
      Serial.print(c1);
    }
  }
}

void ReadAll()
{
  for(int m = 0 ; m < 5 ; ++ m) ReadRes(m); 

  /* Debug code */
  Serial.print("|");
  for( m=0 ; m < 5 ; ++m ) 
  {
    Serial.print(Vin[m]);
    Serial.print('|');
  }
  Serial.print("\n");
}

void ReadRes(int m)
{
  Vin[m] = analogRead(RPin[m]) - Vavg[m];

  // R[m]   = RDiv*(1024-Vin[m])/1024.0;  // DEBUG!!!
  // MPos[m] = ResPos[m]+(R[m]-RMin[m])/(RMax[m]-RMin[m])*(GriPos[m]-ResPos[m]);
}

void ReadResAvg(int m)
{
  long  v_sum = 0;
  for(int i = 0 ; i < 16 ; ++ i)
  {
    v_sum += analogRead(RPin[m]);
    delay(5);
  }
  
  Vin[m] = v_sum/16;
  R[m]   = RDiv*(1024-Vin[m])/1024.0;  // DEBUG!!!
  // MPos[m] = ResPos[m]+(R[m]-RMin[m])/(RMax[m]-RMin[m])*(GriPos[m]-ResPos[m]);
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