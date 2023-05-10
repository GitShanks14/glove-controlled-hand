#include<math.h>

int y[5] = {0,0,0,0,0};
int x = 0;
int N = 1000;

int f_x(int x, int f)
{
  int d;
  f = 2*f + 1;
  d = 1000*float((f*x)%N)/N;
  return d;
  
  // sin(f*2*3.14*x/1000);
}

void setup() {

  Serial.begin(115200);    // Begins serial communications

}

#define interval 10000
unsigned long CurrentMicros = 0;
unsigned long PreviousMicros = 0;

void loop() 
{
  CurrentMicros = micros();
  if( CurrentMicros - PreviousMicros >= interval )
  {
    ReadSig(); 
    PreviousMicros = CurrentMicros;
    for(int m = 0 ; m < 5 ; ++ m)
    {      
      char c1 = (y[m]>>8)&0xFF;
      char c2 = (y[m]&0xFF);
      Serial.print(c2);
      Serial.print(c1);
      
      /*
      char *p = (char*)&(y[m]);
      Serial.write(p,2);
      */

      /*
      Serial.print(y[m]);
      Serial.print("\t");
      */
    }
    // Serial.print("\r\n");
    delayMicroseconds(3);
  }
}

void ReadSig()
{
  x = x + 1;
  for(int j = 0 ; j < 5 ; ++ j) y[j] = f_x(x,j);
}
