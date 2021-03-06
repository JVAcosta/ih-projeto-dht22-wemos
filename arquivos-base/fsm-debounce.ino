



#include "fsm_config.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "jualabs";
const char* password = "jualabsufrpe";
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


// definicao das funcoes relativas a cada estado
event off_state(void) {
  digitalWrite(ledPin, LOW);
  if(read_button())
    return btn_pressed;
  else
    return repeat;
}

event on_state(void) {
  digitalWrite(ledPin, HIGH);
  if(read_button())
    return btn_pressed;
  else
    return repeat;
}


event connectWifi(void) {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  if (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
    return reconnectWifi
  } else {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    return wifiIsConneceted;
  }

event connectServer(void) {
  Serial.print("Attempting MQTT connection...");
  if (client.connect("ESP8266Client","A1E-Gl69HF0deyOv90xCmk1jffsJ7Ujk4U","")) {
      Serial.println("connected");
      client.subscribe("inTopic");
      return serverIsConneceted

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
      return reconnectServer
    }

  
}

event waitEvent(void) {
  
}
event sendTempHumiTime(void) {
  
}
event sendTempHumiBtn(void) {
  
}


// variaveis que armazenam estado atual, evento atual e funcao de tratamento do estado atual
state cur_state = ENTRY_STATE;
event cur_evt;
event (* cur_state_function)(void);

// implementacao de funcoes auxiliares
int read_button() {
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

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  client.setServer(mqtt_server, 1883);
}

void loop() {
  cur_state_function = state_functions[cur_state];
  cur_evt = (event) cur_state_function();
  if (EXIT_STATE == cur_state)
    return;
  cur_state = lookup_transitions(cur_state, cur_evt);
}
