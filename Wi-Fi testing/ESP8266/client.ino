


/*
Пример восоздания простого Wi-Fi подключения, со строны якобы клиента (простой коннект к Wi-Fi).
*/



#include <ESP8266WiFi.h> // библиотека для взаимодействия с Wi-Fi 


const char* ssid = "WiFi-name"; 
const char* password = "password123"; 


void setup() {
  Serial.begin(9600); // настройка скорости передачи данных между ESP8266 и компьютером 


  WiFi.begin(ssid, password); // инициализация начала работы Wi-Fi модуля, через обьект класса ESP8266WiFi.h - подключение к сети
  /*
  Возможные вариации:


  WiFi.status() - проверка состояния подключения: WL_CONNECTED — подключено; WL_DISCONNECTED — не подключено.
  WiFi.isConnected() - в своем роде замена метода "WiFi.status()", уменьшает длинну аргумента условного блока  
  WiFi.localIP() - возвращает адрес, который присвоил нашему устройсву DNS 
  WiFi.disconnect() - отключение от сети 
  WiFi.reconnect() - пересоединение к сети 
  
  */ 


  while (WiFi.status() != WL_CONNECTED ){
    delay(1000);
    Serial.print("Connecting to WiFi...");
  }



  // показываем имя сети и присвоеный нам DNS-ом нам IP-адрес 
  Serial.print("Connected to Wi-Fi: ");
  Serial.println(ssid);
  Serial.print("Result of IP DNS assignment: ");
  Serial.println(WiFi.localIP());





  /* 
  В библиотеке ESP8266WiFi.h есть функции для получения дополнительной информации о текущем Wi-Fi-соединении:
  WiFi.SSID() — возвращает имя сети.
  WiFi.RSSI() — сигнал сети (уровень мощности).
  WiFi.macAddress() — MAC-адрес устройства.
  WiFi.hostname() — имя устройства в сети.
  
  Пример:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("Signal strength (RSSI): ");
  Serial.println(WiFi.RSSI());
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
  
  */
  


}

void loop() {
  // put your main code here, to run repeatedly:

}
