/*
  Скетч к проекту "Метеостанция"
  Страница проекта (схемы, описания): d2omg.mozello.com/meteostation
  Исходники на GitHub: https://github.com/DaniilD2/metostation
  Нравится, как написан и закомментирован код? Поддержи автора! https://d2omg.mozello.com/support
  Автор: DaniilD2 Technologies, 2021
  https://d2omg.mozello.com
*/

// ------------------------- НАСТРОЙКИ --------------------
#define PIN_MQ2 A0           // Пин датчика mq 2 "универсальный датчик газов"
uint8_t DHTPin = 2;          // Пин датчика температуры и влажности dht11 //static const uint8_t D4   = 2;
#define PIN_MQ7  0           // Пин датчика mq 7 "датчик угарного газа" //static const uint8_t D3   = 0;

//Укажите SSID & пароль
const char* ssid = "12";           // SSID
const char* password = "12345678"; // Password
  
//библиотеки
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <TroykaMQ.h>
#include <TroykaIMU.h>
#include <TroykaLight.h>
#include "DHT.h"

//объекты
#define DHTTYPE DHT11
MQ2 mq2(PIN_MQ2);
Barometer barometer;
MQ7 mq7(PIN_MQ7);
//инциализация    
ESP8266WebServer server(80);
DHT dht(DHTPin, DHTTYPE);  
TroykaLight sensorLight(12); // Пин датчика освещенности //static const uint8_t D6   = 12;
  
//переменные    
  //dht11        
float Temperature;
float Humidity;
  //mq2
float Ratio;
int Metane;
int LPG;
int Smoke;
int Hydrogen;
  //Барометр
float pressurePascals;
float pressureMillimetersHg;
float altitude;
//float temperature //Функция измерения температуры в барометре (офнута)
  //mq7
int Ratio2; //mq 7
int CarbonMonoxide;
  //light
int lux;
int candles;

void setup() {
  Serial.begin(115200); // Отладка
  delay(100);   
  pinMode(DHTPin, INPUT);
  dht.begin();       // Инциализация
  mq2.calibrate();   // Инциализация 
  barometer.begin(); // Инциализация  
  mq7.cycleHeat();   // Инциализация        
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); // подключаемся к локальной сети Wi-Fi
  while (WiFi.status() != WL_CONNECTED) { // проверка подключения Wi-Fi 
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
  }
  
void loop() {
server.handleClient();
// Непредвиденное обстоятельство мк7 калибровка
 if (!mq7.isCalibrated() && mq7.atHeatCycleEnd()) {
}
mq7.calibrate();
mq7.cycleHeat();
if (mq7.isCalibrated() && mq7.atHeatCycleEnd()) { 
}
sensorLight.read(); //Освещенность читаем
}

void handle_OnConnect() {
// Пишем в переменные показания с датчиков
Temperature = dht.readTemperature();
Humidity = dht.readHumidity();
Ratio = mq2.getRo();
Metane = mq2.readMethane();
LPG = mq2.readLPG();
Smoke = mq2.readSmoke();
Hydrogen = mq2.readHydrogen();
pressurePascals = barometer.readPressurePascals();
pressureMillimetersHg = barometer.readPressureMillimetersHg();
altitude = barometer.readAltitude();
//float temperature = barometer.readTemperatureC();
Ratio2 = mq7.readRatio();
CarbonMonoxide = mq7.readCarbonMonoxide();
lux = sensorLight.getLightLux();
candles = sensorLight.getLightFootCandles();

// Калибровка
pressurePascals =  pressurePascals/0.7622 ;
pressureMillimetersHg = pressureMillimetersHg*13.1192;
altitude = altitude*0.0571;

//Послылаем на сервер
server.send(200, "text/html", SendHTML(Temperature,Humidity,Ratio,Metane,LPG,Smoke,Hydrogen,pressurePascals,pressureMillimetersHg,altitude,Ratio2,CarbonMonoxide,lux,candles)); //можно добавить температуру "temperature" 
}
void handle_NotFound(){
server.send(404, "text/plain", "Not found"); // 404
}

// HTML
String SendHTML(float Temperaturestat,float Humiditystat,float Ratiostat,float Metanestat,float LPGstat,float Smokestat,float Hydrogenstat,float pressurePascalsstat,float pressureMillimetersHgstat,float altitudestat,float Ratio2stat,float CarbonMonoxidestat,float luxstat,float candlesstat){ // температутра: "float temperaturestat"
String ptr = "<!DOCTYPE html> <html>\n";
ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
ptr +="</head>\n";
ptr +="<body>\n";
ptr +="<div id=\"webpage\">\n";
ptr +="<h1>Weather Report</h1>\n";      
ptr +="<p>Temperature: ";
ptr +=(int)Temperaturestat;
ptr +="<p> C ";
ptr +="<p>Humidity: ";
ptr +=(int)Humiditystat;
ptr +="%</p>";
ptr +="<p>Ratio: ";
ptr +=(int)Ratiostat;
ptr +="<p>Metane: ";
ptr +=(int)Metanestat;
ptr +="<p>LPG: ";
ptr +=(int)LPGstat;
ptr +="<p>Smoke: ";
ptr +=(int)Smoke;   
ptr +="<p>Hydrogen: ";   
ptr +=(int)Hydrogenstat; 
ptr +="<p>PressurePascals: ";
ptr +=(int)pressurePascalsstat; 
ptr +="<p>PressureMillimetersHg: ";
ptr +=(int)pressureMillimetersHgstat; 
ptr +="<p>Altitude: ";
ptr +=(int)altitudestat; 

//ptr +="<p>Temperature: ";
//ptr +=(int)temperaturestat; 

ptr +="<p>Ratio 2: //Mq 7: ";
ptr +=(int)Ratio2stat; 
ptr +="<p>CarbonMonoxidee: ";
ptr +=(int)CarbonMonoxidestat; 
ptr +="<p>lux: ";
ptr +=(int)luxstat; 
ptr +="<p>Candles: ";
ptr +=(int)candlesstat; 
ptr +="</div>\n";
ptr +="</body>\n";
ptr +="</html>\n";
return ptr;
}
