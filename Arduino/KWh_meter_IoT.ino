#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <FirebaseHttpClient.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define WIFI_SSID "KONTRAKAN ANAK2"
#define WIFI_PASSWORD "bukaopen"

#define FIREBASE_HOST "kwhmeteronline.firebaseio.com"
#define FIREBASE_AUTH "Ymywv7V0Zu1w96MkWbN9rDsRSBpVA2gXRO8NkQLZ"

int relayInput = 2;
const int sensorIn = A0;
int mVperAmp = 66; // use 100 for 20A Module and 66 for 30A Module
float Voltage = 0;
float VRMS = 0;
float AmpsRMS = 0;
float supplyVoltage = 220.0;
float power = 0;
float wh = 0;
float kwh = 0;
unsigned int pF = 95;
unsigned long lastTime = 0;
unsigned long currentTime = 0;
unsigned int tarifKWh = 1352; // Rupiah per KWh
float tagihan = 0;
float currentKwh;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup(){ 
 Serial.begin(9600);
 Wire.begin(D2, D1);
 lcd.begin();
 pinMode(relayInput, OUTPUT);
 delay(1000);
 WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 Serial.print("Connecting to ");
 Serial.println(WIFI_SSID);
 while(WiFi.status() != WL_CONNECTED){
  Serial.print(".");
  delay(500); 
 }
 Serial.println("Connection Success !!");
 Serial.println(WiFi.localIP());
 Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

float getVPP()
{
  float result;
  
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
  int minValue = 1024;          // store min value here
  
   uint32_t start_time = millis();
   while((millis()-start_time) < 1000) //sample for 1 Sec
   {
       readValue = analogRead(sensorIn);
       // see if you have a new maxValue
       if (readValue > maxValue) 
       {
           /*record the maximum sensor value*/
           maxValue = readValue;
       }
       if (readValue < minValue) 
       {
           /*record the maximum sensor value*/
           minValue = readValue;
       }
   }
   
   // Subtract min from max
   result = ((maxValue - minValue) * 5.0)/1024.0;
   return result;
}

void loop(){ 
 digitalWrite(relayInput, HIGH);
 Voltage = getVPP();
 VRMS = (Voltage/2.0) *0.707; 
 AmpsRMS = (VRMS * 1000)/mVperAmp;
 if((AmpsRMS > -0.015) && (AmpsRMS) < 0.15){
  AmpsRMS = 0;
 }
 power = (supplyVoltage * AmpsRMS) * (pF/100.0);
 lastTime = currentTime;
 currentTime = millis();
 wh = wh + power * ((currentTime - lastTime)/3600000.0);
 kwh = wh / 1000;
 tagihan = kwh * tarifKWh;
 currentKwh = Firebase.getFloat("currentkwh");
 currentKwh = currentKwh - kwh;

// if(currentKwh <= 0)
// {
//  currentKwh = 0; 
//  digitalWrite(relayInput, LOW);
// } else {
//  digitalWrite(relayInput, HIGH); 
// }
 
 Serial.print("Irms = ");
 Serial.print(AmpsRMS);
 Serial.println(" Amps RMS");
 Serial.print("Power = ");
 Serial.print(power);
 Serial.println(" Watt");
 Serial.print("Kilo Watt Hours = ");
 Serial.print(kwh);
 Serial.println(" KWh");
 Serial.print("Sisa Kwh = ");
 Serial.print(currentKwh);
 Serial.println(" KWh");
 Serial.print("Tagihan = ");
 Serial.print(tagihan);
 Serial.println(" rupiah");

 lcd.setCursor(0,0);
 lcd.print("Sisa Kwh = ");
 lcd.setCursor(12,0);
 lcd.print(currentKwh);
 lcd.setCursor(0,1);
 lcd.print("Arus = ");
 lcd.setCursor(8,1);
 lcd.print(AmpsRMS);

// lcdInterface();

 if(Firebase.failed()){
  Serial.print("Firebase error: ");
  Serial.println(Firebase.error());
 }
 Firebase.setFloat("arus", AmpsRMS);
 Firebase.setFloat("daya", power);
 Firebase.setFloat("kwh", kwh);
 Firebase.setFloat("tagihan", tagihan);
 Firebase.setFloat("currentkwh", currentKwh);

 delay(1000);
}
