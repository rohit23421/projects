#include <VirtualWire.h>
int ldr_val;
int alc_val;
int motor=11;
int headlight=13;
const int datain = 12;
void setup()
{
 pinMode(A0,INPUT);
 pinMode(A2,INPUT);
 pinMode(13,OUTPUT);
 pinMode(11,OUTPUT);
 Serial.begin(9600);
vw_set_ptt_inverted(true);
vw_set_rx_pin(datain);
vw_setup(4000);
pinMode(headlight, OUTPUT);
 vw_rx_start();
}
void loop()
{
digitalWrite(headlight,HIGH);
ldr_val=analogRead(A0);
uint8_t buf[VW_MAX_MESSAGE_LEN];
uint8_t buflen = VW_MAX_MESSAGE_LEN;
 if (vw_get_message(buf, &buflen))
    { 
       if(buf[0]=='1' && ldr_val>500)
      {  
      digitalWrite(headlight,LOW);
      }
    }
alc_val=analogRead(A2);
Serial.println(ldr_val);
if (0<alc_val<224)
analogWrite(motor,255);
if(224<alc_val<300)
analogWrite(motor,100);
if(alc_val>300)
analogWrite(motor,LOW);
}
