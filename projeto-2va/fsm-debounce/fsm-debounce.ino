#include <DHT.h>
#include "fsm_config.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "AndroidAP";
const char* password = "numero@1616";
const char* mqtt_server = "things.ubidots.com";
const int buttonPin = D5;    // definicao do pino utilizado pelo botao
const int ledPin = D7;

WiFiClient espClient;
PubSubClient client(espClient);

int i = 0;
long lastMsg = 0;
char msg[50];
int value = 0;
int buttonState = LOW;             // armazena a leitura atual do botao
int lastButtonState = LOW;         // armazena a leitura anterior do botao
unsigned long lastDebounceTime = 0;  // armazena a ultima vez que a leitura da entrada variou
unsigned long debounceDelay = 50;    // tempo utilizado para implementar o debounce
unsigned long previusTime = millis();
unsigned char counter = 0;
unsigned int interval=100;

//Constants
#define DHTPIN D3     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

void sendTempHumi(){
  hum = dht.readHumidity();
  temp= dht.readTemperature();
  Serial.println("Humidity: ");
  Serial.print(hum);
  Serial.println(" %, Temp: ");
  Serial.print(temp);
  Serial.println(" Celsius");
  String humStr = String(hum);
  String tempStr = String(temp);
  client.publish("/v1.6/devices/infra-hardware/temp", "{\"value\":  tempStr }");
  client.publish("/v1.6/devices/infra-hardware/hum", "{\"value\": humStr }");
}

boolean read_button() {
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  lastButtonState = reading;
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        return true;
      }
    }
  }
  return false;
}

// definicao das funcoes relativas a cada estado
event connectWifi_state(void) {
    delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  return wifiIsConnected;
}

event connectServer_state(void) {
  Serial.print("Attempting MQTT connection...");
  if (client.connect("ESP8266Client","A1E-SLO6fJDKYwo4pXd9GewMXFCA9zBPBb","")) {
      delay(10);
      Serial.println("connected");
      client.subscribe("inTopic");
      return serverIsConnected;
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
      return reconnectServer;
    }
}

event waitEvent_state(void) {
  Serial.println("wait event state...");
   if (read_button()){
      //Serial.println(read_button());
      previusTime = millis();
      return btn_pressed;
    }else if ((millis() - previusTime) > 10000 ){
      //Serial.println(millis() - previusTime);
      previusTime = millis();      
      return time10;
      }
    return backToWaitEvent;
}

event sendTempHumiTime_state(void) {
  Serial.print("Send temp/Hum time");
  sendTempHumi();
  if ( WiFi.status() != WL_CONNECTED) {
    return reconnectWifi;
  } else if(!client.connected()){
    return reconnectServer;
  }
  return backToWaitEvent;
}

event sendTempHumiBtn_state(void) {
  Serial.print("Send temp/Hum button");
  sendTempHumi();
  client.publish("/v1.6/devices/infra-hardware/button", "{\"value\": 1}");
    if ( WiFi.status() != WL_CONNECTED) {
    return reconnectWifi;
  } else if(!client.connected()){
    return reconnectServer;
  }
  return backToWaitEvent;
}

// variaveis que armazenam estado atual, evento atual e funcao de tratamento do estado atual
state cur_state = ENTRY_STATE;
event cur_evt;
event (* cur_state_function)(void);

// implementacao de funcoes auxiliares

void setup() {
  //Serial.begin(115200);
  Serial.begin(9600);
  dht.begin();
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  client.setServer(mqtt_server, 1883);
}

void loop() {
  //Serial.println("loop");
  //Serial.println("state in :");
  //Serial.print(cur_state);
  cur_state_function = state_functions[cur_state];
  cur_evt = (event) cur_state_function();
  /*if (EXIT_STATE == cur_state)
    return;*/
  cur_state = lookup_transitions(cur_state, cur_evt);
  //Serial.println("state out :");
  //Serial.print(cur_state);
}
