//Professor Lucas - server websocket nodejs + esp8266 - Genius
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

const char* ssid = "FIBRA-A066"; //altere para o nome do wifi
const char* password = "0Z48058288"; // altere para a senha do wifi
WebSocketsClient webSocket;

int ledGreen = D1;
int ledRed = D2;
int ledYellow = D3;

void setup() {
  Serial.begin(115200);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  
  webSocket.begin("192.168.25.13", 3000, "/");  // altere para o IP do servidor

  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
}

void loop() {
  webSocket.loop();
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    String message = String((char*)payload);
    if (message.indexOf("sequence") >= 0) {
      String sequence = message.substring(message.indexOf("[") + 1, message.indexOf("]"));
      for (int i = 0; i < sequence.length(); i++) {
        if (sequence[i] == 'r') {
          digitalWrite(ledRed, HIGH);
          delay(500);
          digitalWrite(ledRed, LOW);
        } else if (sequence[i] == 'g') {
          digitalWrite(ledGreen, HIGH);
          delay(500);
          digitalWrite(ledGreen, LOW);
        } else if (sequence[i] == 'b') {
          digitalWrite(ledYellow, HIGH);
          delay(500);
          digitalWrite(ledYellow, LOW);
        }
      }
    }
  }
}
