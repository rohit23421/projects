/*The PERIL Project
 * created in the Design and thinking lab at KIIT University
 * Authors Debagnik Kar, Shivam Chanda, Sayantan Ghosh, Dipanjan Sur
 * Project is licenced by GNU Public Licences
 * GPS TX=2 RX=3
 * GSM TX=4 RX=5
 * LedPin 10
 * Data pin For Transmitter 12
 * 
 * Import Tiny GPS library and Virtual wire library from github.com
 * 
 * for any query email me 1804373@kiit.ac.in
 * 
 * Best results when used in Arduino UNO
 */

#include<SoftwareSerial.h>
#include"TinyGPS++.h"
#include<VirtualWire.h>

SoftwareSerial sgps(2,3);
SoftwareSerial sgsm(4,5);

float ltd;
float lgd;
float lati;
float lng1;

String at="AT+CMGS=\"",ph="<phoneNumber>",at1="\"r";
String at2=at+ph+at1;

const int ledPin=10;
char *data;

TinyGPSPlus gps;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sgps.begin(9600);
  sgsm.begin(9600);
  delay(1000);
  
  pinMode(12,INPUT);
  pinMode(ledPin,OUTPUT);
  vw_set_ptt_inverted(true);
  vw_set_tx_pin(12);
  vw_setup(4000);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  while(sgps.available())       //code for gps setup
  {
    gps.encode(sgps.read());
  }
  if(gps.location.isUpdated())
  {
    ltd=lat1();
    lgd=lon();
  }

  if(digitalRead(12)==HIGH)
  {
    SendSMS(ltd,lgd);         //sending sms
  }
  else
  {
    //code for transmitter
    transmit();               //streaming data
  }
}

void SendSMS(float latitude,float longitude)
{
  sgsm.println("AT+CMGF=1");//text mode on
  delay(500);
  Serial.println(at2);
  Serial.println("AT+CMGS=\"+91<phonenumber>\"\r");
  sgsm.println("AT+CMGS=\"+91<phonenumber>\"\r");//enter proper phone number
  delay(500);
  sgsm.println("Hi Your Vehicle's safety system has been triggered\n It may have been hit by something, Please check the coordinates");
  sgsm.print(latitude);
  sgsm.println(longitude);

  sgsm.println((char)26);//stopping character
  delay(100);
  Serial.println("sent");
}

float lat1()
{
  lati=gps.location.lat(),6;
  return lati;
}

float lon()
{
  lng1=gps.location.lng(),6;
  return lng1;
}

void transmit()
{
  data="1";
  vw_send((uint8_t*)data,strlen(data));
  vw_wait_tx();
  digitalWrite(ledPin,HIGH);
  delay(3000);
  digitalWrite(ledPin,LOW);
  data="0";
  vw_send((uint8_t*)data,strlen(data));
  vw_wait_tx();
  delay(3000);
}
