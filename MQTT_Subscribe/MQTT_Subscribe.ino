/*
 MQTT subscriber example
 
  - connects to an MQTT server
  - subscribes to the topic "inTopic"
*/

#include <PubSubClient.h>
#include <ESP8266WiFi.h>

const char* ssid = "Pi_AP";
const char* password = "Raspberry"; 
//const char *ssid =	"xxxxxxxx";		// cannot be longer than 32 characters!
//const char *pass =	"yyyyyyyy";		//

// Update these with values suitable for your network.
//IPAddress server(192, 168, 32, 109);
char* server = "192.168.42.1";

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  int i = 0; 
  char message_buff[100];
  for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  
  String msgString = String(message_buff);
  
  Serial.print("Subscribe ");
  Serial.write(topic);
  Serial.print(" : ");
  //Serial.write(payload,length);
  Serial.println(msgString);
  
  if(msgString == "0") {
    digitalWrite(16, LOW);
  }
  else {
    digitalWrite(16, HIGH);
  }
} 
//void callback(const MQTT::Publish& pub) {
//  Serial.print(pub.topic());
//  Serial.print(" => ");
//  Serial.println(pub.payload_string());
//}
 
//PubSubClient client(server);
WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);
 
void setup()
{
  // Setup console
  pinMode(16, OUTPUT);
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
 
  //client.set_callback(callback);
 
  //WiFi.begin(ssid, pass);
  WiFi.begin(ssid, password);
 
  int retries = 0;
  while ((WiFi.status() != WL_CONNECTED) && (retries < 10)) {
    retries++;
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
  }
 
  if (client.connect("192.168.32.113")) {
    client.subscribe("inTopic");
  }
}
 
void loop()
{
  client.loop();
}
