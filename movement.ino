#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Установите здесь свои SSID и пароль */
const char* ssid = "SverhNovaya";  // SSID
const char* password = "Proverka1"; // пароль

#define PIN_D1 5  // gpio5 = D1  PWM_A
#define PIN_D2 4  // gpio4 = D2  PWM_B
#define PIN_D3 0  // gpio0 = D3  DA (A- A+) подключается двигатель 
#define PIN_D4 2  // gpio2 = D4  DB (B- B+) подключается двигатель
int RV, LV, Rint, Lint;
String R, L;

String webpage = "<html><head><title>qwer</title></head><body>qwerr<body></html>";

// Объект веб-сервера. Будет прослушивать порт 80 (по умолчанию для HTTP)
ESP8266WebServer server(80);

void setup()
{
  pinMode(PIN_D1, OUTPUT); // инициализируем Pin как выход
  pinMode(PIN_D2, OUTPUT); // инициализируем Pin как выход
  pinMode(PIN_D3, OUTPUT); // инициализируем Pin как выход
  pinMode(PIN_D4, OUTPUT); // инициализируем Pin как выход
  Serial.begin(115200);
  delay(100);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  // подключиться к вашей локальной wi-fi сети
  WiFi.begin(ssid, password);

  // проверить, подключился ли wi-fi модуль к wi-fi сети
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());

  server.on("/genericArgs", handleGenericArgs);  // привязать функцию обработчика к URL-пути

  server.begin();                                // запуск сервера
  Serial.println("HTTP server started");

}

void handleGenericArgs() {

  R = server.arg(0);
  L = server.arg(1);
  Serial.println(R);
  Serial.println(L);
  Rint = R.toInt();
  Lint = L.toInt();


  server.send(200, "text/html", webpage);
  if (Rint < 0) {
    Rint = Rint * -1;
    RV = 1;
  }
  else {
    RV = 0;

  }
  if (Lint < 0) {
    Lint = Lint * -1;
    LV = 1;
  }
  else {
    LV = 0;
  }

  digitalWrite(PIN_D3, LV);
  digitalWrite(PIN_D4, RV);
  analogWrite(PIN_D1, Rint);
  analogWrite(PIN_D2, Lint);
  // movement(Rint, Lint);

}

int movement(int R, int L) {
  if (R < 0) {
    R = R * -1;
    RV = 1;
  }
  else {
    RV = 0;

  }
  if (L < 0) {
    L = L * -1;
    LV = 1;
  }
  else {
    LV = 0;
  }

  digitalWrite(PIN_D3, LV);
  digitalWrite(PIN_D4, RV);
  analogWrite(PIN_D1, R);
  analogWrite(PIN_D2, L);
}

void loop()
{
  server.handleClient();    // обработка входящих запросов

}
