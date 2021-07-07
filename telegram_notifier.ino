#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>


const char* ssid = "Sweet Home WiFi";
const char* password = "unauthorised";

// Initialize Telegram BOT
#define BOTtoken ""

#define FIA_PIN D5
#define AMIR_PIN D6
#define MUHA_PIN D7
#define MASTER_PIN D3

#define BUZZER_PIN D8

#define FIA_ID "63366981"
#define AMIR_ID "75312313"
#define MUHA_ID "880509673"



#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Check if user is authorised
bool isAuthorised(String userID){
  String user_ids[3] = {"63366981","75312313","880509673"};
  for (int i = 0; i < sizeof(user_ids); i++)
  {
    if (user_ids[i] == userID){
      return true;
    }
    return false;
  }
}

void my_tone(uint8_t _pin, unsigned int frequency, unsigned long duration) {
//  pinMode (_pin, OUTPUT );
//  analogWriteFreq(frequency);
//  analogWrite(_pin,500);
//  delay(duration);
//  analogWrite(_pin,0);


  
  digitalWrite(BUZZER_PIN, HIGH);
  delay(duration);
  digitalWrite(BUZZER_PIN, LOW);
}

void notify(String user_id){
  my_tone(BUZZER_PIN, 1000, 200);
  Serial.print("Sending message to "+user_id+"...");
  bot.sendMessage(user_id, "Mami panggil!");
  Serial.println("SENT!");
  my_tone(BUZZER_PIN, 2000, 100);
  delay(100);
  my_tone(BUZZER_PIN, 2000, 100);
}




void setup() {
  Serial.begin(115200);


  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif

  pinMode(MASTER_PIN, INPUT_PULLUP);
  pinMode(FIA_PIN, INPUT_PULLUP);
  pinMode(AMIR_PIN, INPUT_PULLUP);
  pinMode(MUHA_PIN, INPUT_PULLUP);
  
  pinMode(BUZZER_PIN, OUTPUT);
  my_tone(BUZZER_PIN, 1000, 1000);
  digitalWrite(D0, 1);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  
  while (WiFi.status() != WL_CONNECTED) {
      Serial.print("\n\n\nConnecting to WiFi..");
      WiFi.begin(ssid, password);
     for (int i=0; i<10; i++){
      delay(1000);
      Serial.print(".");
    }
    digitalWrite(D0, 0);

  }
  
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
}

void loop() {
  while (WiFi.status() != WL_CONNECTED) {
      digitalWrite(D0, 1);
      Serial.print("\n\n\nConnecting to WiFi..");
      WiFi.begin(ssid, password);
     for (int i=0; i<10; i++){
      delay(1000);
      Serial.print(".");
    }
  }
  
  digitalWrite(D0, 0);

  int buttons[4] = {digitalRead(FIA_PIN),digitalRead(AMIR_PIN),digitalRead(MUHA_PIN),digitalRead(MASTER_PIN)};
  if (buttons[0]==LOW){
    notify(FIA_ID);
    delay(150);
  }
  if (buttons[1]==LOW){
    notify(AMIR_ID);
    delay(150);

  }
  if (buttons[2]==LOW){
    notify(MUHA_ID);
    delay(150);
  }
  if (buttons[3]==LOW){
    notify(FIA_ID);
    notify(AMIR_ID);
    notify(MUHA_ID);
    delay(150);
  }

}
