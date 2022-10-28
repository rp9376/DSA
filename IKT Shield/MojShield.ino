//Definicija pinov
#define LightSen  A0
#define TempSen  A1
#define RedLed 2

#define DS 12
#define SHCP 10
#define STCP 11

float GetTemp();
void SetLeds(byte leds);



void setup() {
  Serial.begin(9600);
  pinMode(RedLed, OUTPUT);
  

  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  

}

// the loop function runs over and over again forever
void loop() {
  /*Serial.print("Temperatura: ");
    Serial.println(GetTemp());

  */
  /*
  int Lightval = analogRead(LightSen);
  Lightval = Lightval/100;
  Serial.println(Lightval);
  Lightval -=2;
  
  byte lucke=0;
  for(;Lightval > 0; Lightval --)
  {
    lucke=0 | B00000001;
    lucke = lucke << 1;
  }
*/
  
  //SetLeds(lucke);
  delay(100);
}

float GetTemp()
{
#define Vout0 400
#define tc 19.53

  int val = analogRead(TempSen);
  float temp = val * (5000.0 / 1024.0); //Dobimo mV na analog pinu
  temp = temp - Vout0;                    //Odštejemo konstantno napetost senzorja
  temp = temp / tc;                        //Delimo z temp koeficientom
  return temp;
}

void SetLeds(byte leds)
{
digitalWrite(DS, LOW);
digitalWrite(SHCP, LOW);
digitalWrite(STCP, LOW);

  
  for (int i = 0; i < 8 ; i++)
  {
    digitalWrite(DS, leds & B00000001);
    digitalWrite(SHCP, HIGH);
    digitalWrite(SHCP, LOW);
    leds = leds >> 1;
  }
  digitalWrite(STCP, HIGH);
  digitalWrite(STCP, LOW);
}
