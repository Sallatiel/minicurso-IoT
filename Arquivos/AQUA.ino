#include <GravityTDS.h>

#include <Wire.h>
#include <Adafruit_ADS1015.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <String.h>

#include <OneWire.h>
#include <DallasTemperature.h>

Adafruit_ADS1115 ads(0x48);  /* Use this for the 16-bit version */
GravityTDS gravityTds;

const char* ssid = "myhome"; 
const char* password = "marleyebolt"; 
WiFiClient client;

String writeAPIKey = "5YXBBQMF27H7O1J3"; // write API key for your ThingSpeak Channel

float temperature, tdsValue,tssValue, temperatureValue;
int16_t TDS, TSS;

// Data wire is plugged into pin D1 on the ESP8266 12-E - GPIO 5
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature DS18B20(&oneWire);


void setup(void) 
{
  Serial.begin(115200);
  
  gravityTds.setAref(4.096);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(32768);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization
  
  ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 0.125mV
  //ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV

  ads.begin();

  WiFi.begin(ssid, password);//tds
  Serial.print("Conectando...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");

  DS18B20.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement
}

void loop(void) 
{
  TDS=ads.readADC_SingleEnded(0);
  //temperature=DS18B20.getTempCByIndex(0);
  gravityTds.setPin(TDS);
  gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
  gravityTds.update();  //sample and calculate 
  tdsValue = gravityTds.getTdsValue();  // then get the value
  Serial.print(tdsValue,0);
  Serial.println("ppm");
  tdsValue=map(tdsValue,0,1000,0,100);
  String sTDS = String(tdsValue);
  Serial.print("TDS: ");Serial.println(sTDS);
  Serial.print("adc0: ");Serial.println(TDS);
  sending_data("1", sTDS);
  delay(20000);
  
  TSS=ads.readADC_SingleEnded(1);
  tssValue=map(TSS,1600,29600,0,100); //29600 = 3,7v para Gain_One de0,2V a 3,7V
  String sTSS = String(tssValue);
  Serial.print("TSS: "); Serial.println(sTSS);
  Serial.print("adc1: ");Serial.println(TSS);
  sending_data("2", sTSS);
  delay(20000);

  DS18B20.requestTemperatures();
  temperatureValue = DS18B20.getTempCByIndex(0);
  String sTemperature = String(temperatureValue);
  Serial.print("Temperatura: ");Serial.println(sTemperature);
  Serial.print("ds18b20: ");Serial.println(DS18B20.getTempCByIndex(0));
  sending_data("3", sTemperature);
  delay(20000);
}

// ThinkSpeak
void sending_data(String field, String data) { // Para field, mandar apenas o numero, em formato de string.
  String site = "http://api.thingspeak.com/update?api_key=";
  site.concat(writeAPIKey);
  site.concat("&field");
  site.concat(field);
  site.concat("=");
  site.concat(data);
  
  Serial.println(site);

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    //    Serial.print("Endereço IP: ");
    //    Serial.println(WiFi.localIP());
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin(site);
    int httpCode = http.GET();
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
    }
    http.end();   //Close connection
  }
}
