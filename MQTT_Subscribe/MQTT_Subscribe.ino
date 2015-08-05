/*
 MQTT subscriber example
 
  - connects to an MQTT server
  - subscribes to the topic "inTopic"
*/

#include <PubSubClient.h>
#include <ESP8266WiFi.h>
//#include <OneWire.h>
//#include <DallasTemperature.h>

//#define ONE_WIRE_BUS 2
//OneWire oneWire(ONE_WIRE_BUS);
//DallasTemperature sensors(&oneWire);

const char* ssid = "Pi_AP";
const char* password = "Raspberry"; 
//const char* ssid = "worktong514";
//const char* password = "1234567890"; 
//const char *ssid =	"xxxxxxxx";		// cannot be longer than 32 characters!
//const char *pass =	"yyyyyyyy";		//

// Update these with values suitable for your network.
//IPAddress server(192, 168, 32, 109);
char* server = "192.168.42.1";

unsigned int temp_count;
char temp[10];
String red_s,green_s,blue_s;

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  int i = 0; 
  int select_color = 0;
  char message_buff[100];
  for(i = 0; i < length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  
  String msgString = String(message_buff);
  
  Serial.print("Subscribe ");
  Serial.write(topic);
  Serial.print(" : ");
  //Serial.write(payload,length);
  Serial.println(msgString);
  /*
  if(msgString == "0") {
    digitalWrite(16, LOW);
  }
  else {
    digitalWrite(16, HIGH);
  }
  */
  
  for(i = 0; i < (sizeof(msgString)-1); i++) {
    if(msgString[i] != ',') {
      switch(select_color) {
        case 0: red_s = red_s + msgString[i]; break;
        case 1: green_s = green_s + msgString[i]; break;
        case 2: blue_s = blue_s + msgString[i]; break;
      }
    }
    else
      select_color++;
  }
  Serial.println("Red : " + red_s);
  Serial.println("Green : " + green_s);
  Serial.println("Blue : " + blue_s);
  red_s = ""; green_s = ""; blue_s = "";
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
  pinMode(12, OUTPUT); digitalWrite(12, HIGH);
  pinMode(13, OUTPUT); digitalWrite(13, HIGH);
  pinMode(14, OUTPUT); digitalWrite(14, HIGH);
  
  Serial.begin(115200);
  //sensors.begin();
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
 
  if (client.connect("192.168.42.1")) {
    //client.subscribe("home/light");
    client.subscribe("color");
  }
  /*
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  Serial.print("Temp : ");
  Serial.println(String(tempC));
  String(tempC).toCharArray(temp,5);
  client.publish("home/temperature",temp);
  */
}
 
void loop()
{
  /*
  if(temp_count > 65500) {
    sensors.requestTemperatures();
    float tempC = sensors.getTempCByIndex(0);
    Serial.print("Temp : ");
    Serial.println(String(tempC));
    String(tempC).toCharArray(temp,5);
    client.publish("home/temperature",temp);
    temp_count = 0;
  }
  else temp_count++;
  */
  client.loop();
}
