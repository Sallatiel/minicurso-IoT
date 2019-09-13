#include <ESP8266WiFi.h>
#include <String.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "myhome"; 
const char* password = "marleyebolt"; 
int val, LDRpin = A0; 

WiFiClient client;

String writeAPIKey = "UKXTT7UAOM24JY4O"; // write API key for your ThingSpeak Channel

/*******************************************************/
void setup(void) {
  Serial.begin(115200); // inicia a comunicação serial

  // conecta wifi
  WiFi.begin(ssid, password);
  
  Serial.print("Conectando...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
}

/*******************************************************/
void loop(void) {
  val = analogRead(LDRpin); //Read Analog values and Store in val variable
  Serial.println(val); //Print on Serial Monitor
  
  sending_data("1", String(val));
  delay(25000);
}

/*******************************************************/
void sending_data(String field, String data) {
  String site = "http://api.thingspeak.com/update.json?api_key=";      // constroi link
  site.concat(writeAPIKey);
  site.concat("&field");
  site.concat(field);
  site.concat("=");
  site.concat(data);
  
  Serial.println(site); // printa link

  if (WiFi.status() == WL_CONNECTED) { 
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP()); // IP local
    
    HTTPClient http;  //Declara objeto da classe HTTPClient
    http.begin(site); // requisição http
    int httpCode = http.GET();
    if (httpCode > 0) { //Checa se tem dados
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
    }
    http.end();   //Close connection
  }
}
