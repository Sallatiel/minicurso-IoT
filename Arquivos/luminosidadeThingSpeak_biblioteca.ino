#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
 
const char* ssid = "myhome"; // rede wifi
const char* password = "marleyebolt"; // senha wifi
int val, LDRpin = A0; 
   
WiFiClient client;
 
unsigned long myChannelNumber = 773444; // ID do canal
const char * myWriteAPIKey = "UKXTT7UAOM24JY4O"; // chave de escrita
 
void setup(){
 
  Serial.begin(115200);
   
  // conecta wifi
  WiFi.begin(ssid, password);
   
  ThingSpeak.begin(client);
}
  
void loop(){
 
  val = analogRead(LDRpin); //Read Analog values and Store in val variable
  Serial.println(val); //Print on Serial Monitor
  
  // ID do canal, campo, valor, chave de escrita  
  ThingSpeak.writeField(myChannelNumber, 1,val, myWriteAPIKey); //Update in ThingSpeak
  
  delay(25000);
 
}
