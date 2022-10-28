/*  
 //Grafi so dosegljivi na : https://thingspeak.com/channels/1912119
  
  Hardware: Arduino Ethernet
  
  !!! IMPORTANT - Modify the secrets.h file for this project with your network connection and ThingSpeak channel details. !!!

  Note:
 - Requires the Ethernet library
  
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
  analyze live data streams in the cloud. Visit https://www.thingspeak.com to sign up for a free account and create a channel.  
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the README.md folder where the library was installed.
  See https://www.mathworks.com/help/thingspeak/index.html for the full ThingSpeak documentation.
  
  For licensing information, see the accompanying license file.
  
  Copyright 2020, The MathWorks, Inc.
*/


#include <Ethernet.h>
#include "secrets.h"
#include "ThingSpeak.h"  // always include thingspeak header file after other header files and custom macros

byte mac[] = SECRET_MAC;

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 0, 1);

EthernetClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char* myWriteAPIKey = SECRET_WRITE_APIKEY;

//Definicija pinov
#define LightSen A0
#define TempSen A1
#define RedLed 2

#define DS 12
#define SHCP 10
#define STCP 11

//Function definitions
float GetTemp();
void setLeds(byte leds);

  byte leds = 0;
void setup() {
Serial.begin(115200);  //Initialize serial
Serial.println("IKT shield & data to ThingSpeak\n");

 //* Ethernet.init(10);     // Most Arduino Ethernet hardware

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1);  // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);

  ThingSpeak.begin(client);  // Initialize ThingSpeak
//*/

  pinMode(RedLed, OUTPUT);

  pinMode(STCP, OUTPUT);
  pinMode(SHCP, OUTPUT);
  pinMode(DS, OUTPUT);
}

void loop() {
//*

  // set the fields with the values
  ThingSpeak.setField(1, GetTemp());
  ThingSpeak.setField(2, analogRead(LightSen));


  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }


//*/

  for(int i = 0; i <= 7; i++)
  {
    setLeds(leds);
    leds = leds << 1;
    leds = leds | B00000001;
  	delay(100);
  }

  for (int i = 0; i < 9; i++) 
  {
    setLeds(leds);
    leds = leds << 1;
    leds = leds & B11111110;
    //Serial.println("bitAdvance"); //For debuging

  	delay(2000);
  }
  
  setLeds(255);
  delay(50);
  setLeds(0);
  delay(200);




  //delay(20000);  // Wait 20 seconds to update the channel again
  //No need for this delay as delay is in for loop
}


float GetTemp() {
#define Vout0 400
#define tc 19.53

  int val = analogRead(TempSen);
  float temp = val * (5000.0 / 1024.0);  //Dobimo mV na analog pinu
  temp = temp - Vout0;                   //Odštejemo konstantno napetost senzorja
  temp = temp / tc;                      //Delimo z temp koeficientom
  return temp;
}


void setLeds(byte leds)
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
