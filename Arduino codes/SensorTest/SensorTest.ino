#define RESPIN A0

#define interval 1000


unsigned long CurrentMicros = 0;
unsigned long PreviousMicros = 0;

unsigned ResVal;

void setup()
{
  // Setup pins
  pinMode(RESPIN,INPUT);

  // Initialize Serial communication
  Serial.begin(115200);
}

void loop()
{

  CurrentMicros = micros();

  if( CurrentMicros - PreviousMicros >= interval )
  {
    PreviousMicros = CurrentMicros;

    ResVal = analogRead(RESPIN);
    char c1 = (ResVal>>8);
    char c2 = ResVal % 256;
    Serial.print(c2);
    Serial.print(c1);
  }  
}