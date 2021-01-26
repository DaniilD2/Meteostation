# Meteostation
ПРОЕКТ В РАЗАРАБОТКЕ!
A weather station made up of Amperka troyka modules and a nodeMSU with data transmission to the Internet.
There was an idea to create a weather station from cheap ampere modules and the coolest platform for developing NodeMSU for data transmission to the Internet.
Ссылки на амперовские модули:
https://amperka.ru/product/troyka-mq7-gas-sensor - mq7 Датчик газа
https://amperka.ru/product/troyka-mq2-gas-sensor - mq2 Второй датчик газа
https://amperka.ru/product/troyka-barometer - Барометр
https://amperka.ru/product/troyka-temperature-humidity-sensor-dht11 - dht11 Датчик влажности
https://amperka.ru/product/troyka-light-sensor - Датчик света
https://amperkot.ru/msk/catalog/modul_wifi_esp8266_s_usb_nodemcu-24255083.html - Node Msu главный контроллер

Задействованные пины на Node Msu:
3.3 //Питанине
gnd //Питанине
d3  //Mq 7
d4  //dht11
d2  //spi barometr
d1  //sda barometr
a0  //mq2
d6  //фоторезистор
// d7  //анемометр
