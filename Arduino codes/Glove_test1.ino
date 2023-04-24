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
const float RMax[5] = {10000.0, 10000.0, 10000.0, 10000.0, 10000.0};
const float RMin[5] = { 5000.0,  5000.0,  5000.0,  5000.0,  5000.0};
float R[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
unsigned int Vin[5] = {0,0,0,0,0};

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

  Serial.begin(9600);    // Begins serial communications
}

void loop() {
  ReadAll();
  delay(2000);
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
