

/*
Реализуем прокси сервер с помощью ESP8266, т.к сам по себе ESP8266 не обладает возможностью выходить во всемирную сеть - он может это делать с 
помощью Wi-Fi роутеров, окружающих его. Таким образом мы можем реализовать прокси-сервер, который будет ожидать подключение клиента и пересылать 
его запрос от себя в на конретный Wi-Fi, получая ответ от него и пересылая ответ обратно клиенту. 
Таким образом мы можем делать паралельные операции (подменну Wi-Fi сети и мониторинг информации, которую клиент отсылает) или мы можем 
наоборот - выступать в роли VPN сервера, который шифрует данные о своих клиентах и сразу же их удаляет, чтобы убрать следы и сделать клиентов 
анонимными.  
*/




#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>


WiFiServer proxyServer(80);  // Сервер на порту 80

void setup() {
  Serial.begin(9600);
  WiFi.begin("your_SSID", "your_PASSWORD");  // Подключаемся к Wi-Fi, через который мы будем общаться с внешним миром
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  proxyServer.begin();  // Запуск прокси-сервера
}

void loop() {
  WiFiClient client = proxyServer.available();  // Ожидание клиента
  if (client) {
    Serial.println("New Client Connected");
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<h1>Hello from ESP8266!</h1>");
    delay(1000);
    client.stop();  // Закрытие соединения
  }
}








/*
Теперь, реализовав простой пример, мы можем перейти дальше и воспользоваться доплнительными библиотеками, которые позволяют корректно отправлять и 
получать запросы и полноценно контактировоать с внешним миром. 
Например, для этого можно использовать библиотеку ESP8266HTTPClient, которая предназначена для работы с HTTP-запросами.
*/



#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* serverName = "http://example.com/api/data";  // Адрес API или веб-страницы

void setup() {
  Serial.begin(9600);
  
  // Все тоже подключение к Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Proxy server: Connecting to Wi-Fi...");
  }
  
  Serial.println("Proxy server: connected to Wi-Fi is successfully!");
  Serial.print("Proxy server: My IP Address (issued via DNS): ");
  Serial.println(WiFi.localIP()); // отображаем наш IP , который был выдан нам DNS-ом.
}

void loop() {
  // Создаем объект HTTP-клиента, через которого мы будем контактировать с реальным сервером 
  HTTPClient http;



  Serial.println("Proxy server: I send a request to a real server");



  // Устанавливаем URL для запроса
  http.begin(serverName);
  
  // Отправляем GET-запрос
  int httpCode = http.GET();  // Получаем HTTP-код ответа
  
  if (httpCode > 0) {  // Если ответ положительный
    String payload = http.getString();  // Чтение данных
    Serial.println("Proxe server: Response received: ");
    Serial.println(payload);  // Выводим полученный ответ
  } else {
    Serial.println("Proxy server: Error on HTTP request");
  }
  
  http.end();  // Закрываем соединение
  delay(10000);  // Пауза перед следующим запросом (10 секунд)
}








/*
Реализация 3.0 - реализованна с паралельной реализацией клиента





Описание используемых методов и функций далее:


БЛОК 1: функция handlePostRequest()

  server.on()
    Метод библиотеки ESP8266WebServer, который задаёт маршрут (адрес URL) и обработчик для запросов.

  "/"
    Указывает маршрут URL. В данном случае это корень сервера (например, http://<esp8266_ip_address>/).


  HTTP_POST
    Указывает тип запроса, который будет обработан. Здесь это запрос POST.
    Можно также указать другие типы, например, HTTP_GET, HTTP_PUT, HTTP_DELETE.


  handlePostRequest()
    Имя функции, которая будет вызываться, если клиент отправит POST-запрос на маршрут /.
    Эта функция реализует логику обработки запроса (например, получение данных, анализ, формирование ответа).


  Алгоритм работы блока:
    Если клиент отправляет POST-запрос на http://<esp8266_ip_address>/, сервер вызывает функцию handlePostRequest().
    Если тип запроса отличается от POST (например, GET), данный маршрут не сработает.







О последующей реализации:
  Так же стоит отметить, что наш клиент-сервер на данном этапе работы очень простой и примитивный: когда клиент подключается к proxy-server мы пытаемся, 
  через заранее подключенный Wi-Fi источник, промониторить определенный сайт (заранее выбранный на стороне прокси-сервера) и отпаравить данные клиенту с сайта. 
  Но это не не адаптивно к реальным проектам и клиентам, когда клиент почти всегда передает GET/POST-запросы, а прокси-сервер должен ему ответить в HTTP формате 
  (если руководствоваться HTTP-протоколом), с HTTP-содержимым которое он получил от реального сервера .

  Давайте для начала добавим возможность выбирирать набор сайтов, которые клиент хочет прочесть, а затем будем реализовывать более сложную систему. Мы все это сделаем 
  через реализацию на стороне клиента POST-запроса, а на сервере нам нужно уметь обработать их.



*/






#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Конфигурация Wi-Fi
const char* ssid = "your_SSID"; // имя сети 
const char* password = "your_PASSWORD"; // пароль сети 

// Создаем сервер на порту 80
ESP8266WebServer server(80);



struct proxy-server
{
  /* data */
};




void handlePostRequest() { // метод для обработки посылаемого клиентом POST-запрос 
  if (server.method() == HTTP_POST) { // ожидаем и обрабатываем только POST_запросы 
    String body = server.arg("plain"); // Получаем тело POST-запроса
    Serial.println("POST request received:");
    Serial.println(body);

    // Анализируем данные (мы должын иметь какой-нибудь леграмент передачи данных. Мы рассчитываем, что клиент будет его соблюдать)
    if (body.indexOf("status") != -1) {
      server.send(200, "text/plain", "Status received!");
    } else {
      server.send(400, "text/plain", "Invalid request!");
    }
  } else {
    server.send(405, "text/plain", "Method Not Allowed");
  }
}




void setup() {
  Serial.begin(9600);

  // Подключение к Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");
  Serial.println("IP Address: " + WiFi.localIP().toString());

  // Устанавливаем обработчик POST-запросов
  server.on("/", HTTP_POST, handlePostRequest);

  // Запуск сервера
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Обработка входящих запросов
  server.handleClient();
}










/*
Теперь было бы интересно реализовать маленький поисковик - прокси-сервер, через который мы бы могли отпарвлять любые запросы, которые мы вводим на Google, например 
и затем получать ответы в том виде, в котором мы захотим и позволит нам наш proxy-server.



*/










#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <stdio.h> // для ввода информации от пользователя 

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }

  Serial.println("Connected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");



    // Чтение HTTP-запроса клиента
    String request = client.readStringUntil('\r');
    Serial.println("Request: ");
    Serial.println(request);

    // Запрос в Google через HTTPS (пример)
    WiFiClientSecure googleClient;
    if (googleClient.connect("www.google.com", 443)) {
      Serial.println("Connected to Google");
      googleClient.print(String("GET /search?q=котята HTTP/1.1\r\n") +
                         "Host: www.google.com\r\n" +
                         "Connection: close\r\n\r\n");

      while (googleClient.connected()) {
        String line = googleClient.readStringUntil('\n');
        Serial.println(line);  // Отладка ответа Google
      }
    }

    // Отправка базового HTML ответа клиенту
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<h1>Search completed! Check Serial Monitor for results.</h1>");
    client.stop();
  }
}
