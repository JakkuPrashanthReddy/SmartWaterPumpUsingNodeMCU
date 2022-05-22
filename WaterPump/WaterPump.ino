#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <PubSubClient.h>

unsigned long last_time = 0;
unsigned long timer_delay = 15000;

const char *ssid =  "xxxxxxxxxxxx";        //Your Access Point or Personal Hotspot, cannot be longer than 32 characters!
const char *pass =  "xxxxxxxxxxxx";    //Your Access Point or Personal Hotspot password
const char* serverTS = "api.thingspeak.com";
String apiKey = "xxxxxxxxxxxxxxxxx";          //Insert your Channel API Key here
int TRIGGER = D3; //Pin D1 = TRIGGER
int ECHO =D2; //Pin D2 = ECHO
int flag = 0;
int motor  = D6;
void setup() 
{
  Serial.begin(115200);         //Recommended speed is 115200
  pinMode(TRIGGER,OUTPUT);
  pinMode(ECHO,INPUT);
  pinMode(motor,OUTPUT);
  connectWifi();
  }

void loop() 
{
  long duration, inches, cm;
 
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);

  duration = pulseIn(ECHO, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  delay(100);
  if ((millis()-last_time) > timer_delay) 
  {
    sendHeight(cm);
    last_time = millis();
  }
  if(cm >= 140 and flag == 0)
  {
    digitalWrite(motor,LOW);
    flag = 1;
  }
  else if(cm <= 10)
  {
    digitalWrite(motor,HIGH);
    flag = 0;
  }
  delay(500);
}
void connectWifi()
{ 
  Serial.print("Connecting to "+*ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected");
  Serial.println("");  
}//end connect
long microsecondsToInches(long microseconds) 
{ 
  return microseconds / 74 / 2;
}
long microsecondsToCentimeters(long microseconds) 
{
  return microseconds / 29 / 2;
}

 void sendHeight(float cm)
{  
  WiFiClient tclient;
  if (tclient.connect(serverTS, 80)) { // use ip 184.106.153.149 or api.thingspeak.com
    Serial.println("WiFi Client connected ");
    
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(cm);
    postStr += "\r\n\r\n";
    
    tclient.print("POST /update HTTP/1.1\n");
    tclient.print("Host: api.thingspeak.com\n");
    tclient.print("Connection: close\n");
    tclient.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    tclient.print("Content-Type: application/x-www-form-urlencoded\n");
    tclient.print("Content-Length: ");
    tclient.print(postStr.length());
    tclient.print("\n\n");
    tclient.print(postStr);   
  }
  tclient.stop();
}
