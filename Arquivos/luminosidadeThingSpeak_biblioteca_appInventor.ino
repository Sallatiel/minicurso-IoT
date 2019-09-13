#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// WIFI
const char* ssid = "APtelefonia"; // rede wifi
const char* password = "Projeto360AP"; // senha wifi

// Variáveis
int count, val, LDRpin = A0, LED = 16, ledStatus=0;

// Canal 1
unsigned long myChannelNumber1 = 773444; // ID do
const char * myWriteAPIKey1 = "UKXTT7UAOM24JY4O"; // chave de escrita
const char * myReadAPIKey1 = "FB800MW0GYFK5CXN"; // chave de leitura

// Canal 2
unsigned long myChannelNumber2 = 774516; // ID do canal 2
const char * myWriteAPIKey2 = "PQYQ84TOF21TPHNJ"; // chave de escrita
const char * myReadAPIKey2 = "0PLPFQIR3E3GFHZ7"; // chave de leitura

WiFiClient client;

void setup(){
 
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  // conecta wifi
  WiFi.begin(ssid, password);
   
  ThingSpeak.begin(client);

  delay(100);
  Serial.begin(115200);

}
  
void loop(){
  if(count==10){
    val = analogRead(LDRpin); // Lê valor LDR
    Serial.println(val); //imprimi na serial
    
    // ID do canal, campo, valor, chave de escrita  
    ThingSpeak.writeField(myChannelNumber1, 1,val, myWriteAPIKey1); //Update
    count=0;
  }

  // Lê estatus LED
  ledStatus  = int(ThingSpeak.readIntField(myChannelNumber2, 1, myReadAPIKey2)); 
  Serial.println(ledStatus); // imprimi na serial
  
  digitalWrite(LED, ledStatus); // Controla LED
  
  delay(2000);
  count++;
 
}
