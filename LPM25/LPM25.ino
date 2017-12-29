#include <ArduinoJson.h>
#include <LWiFi.h>

const char* ssid     = "HTCLIN";           // insert your SSID
const char* pass     = "lhtj955124";   // insert your password

WiFiClient client;

const char* server = "opendata.epa.gov.tw";  
const char* resource = "/ws/Data/ATM00625/?$format=json"; 
const char* server2 = "maker.ifttt.com";  
const char* resource2 = "/trigger/LINE1/with/key/fIQj2Hy3PTtj3sxEW-0UUMU3HmufWBfgPY26Q_LQ3_1"; 

char json[12000]; 

int LedR = 11;
int LedG = 12;
int LedB = 13;

void setup(){
  Serial.begin(115200);
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println();
  pinMode(LedR, OUTPUT);
  pinMode(LedG, OUTPUT);
  pinMode(LedB, OUTPUT);
  digitalWrite(LedR, LOW);
  digitalWrite(LedG, LOW);
  digitalWrite(LedB, LOW);
}

void loop(){
  // connect to server  
  while(!client.connect(server, 80)){
    // wait for connecting server...
  }

  //Send request to resource
  client.print("GET ");
  client.print(resource);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(server);
  client.println("Connection: close");
  client.println();

  delay(100);

  //Reading stream
  client.setTimeout(10000);

  //Checking for the end of HTTP stream
  while(!client.find("\r\n\r\n")){
    // wait for finishing header stream reading ...
  }

  // Reading the response string
  client.readBytes(json, 12000);
  
  // process JSON from response string
  DynamicJsonBuffer jsonBuffer;
  JsonArray& rootArray = jsonBuffer.parseArray(json);
  JsonObject& guTingSite = rootArray[0];
  JsonObject& songShanSite = rootArray[19];
  
  const char* site0 = guTingSite["Site"];
  double pm250 = guTingSite["PM25"];
  
  const char* site1 = songShanSite["Site"];
  double pm251 = songShanSite["PM25"];
  
  // Print data to Serial
 
  Serial.print("*** ");
  Serial.print(site1);
  Serial.println(" ***");
  Serial.print("PM25: ");
  Serial.println(pm251);
  Serial.println("--------------------"); 

  client.stop(); // disconnect from server
  
  delay(2000); // the OWM free plan API does NOT allow more then 60 calls per minute


  while(!client.connect(server2, 80)){
      // wait for connecting server...
  }
  
  if(pm251 <= 35){
     digitalWrite(LedG, HIGH);
     digitalWrite(LedR, LOW);
     digitalWrite(LedB, LOW);
  }else if(pm251 >= 36 && pm251 <= 53){
     digitalWrite(LedG, HIGH);
     digitalWrite(LedR, HIGH);
     digitalWrite(LedB, LOW);
  }else if(pm251 >= 54 && pm251 <= 70){
    digitalWrite(LedR, HIGH);
    digitalWrite(LedG, LOW);
    digitalWrite(LedB, LOW);
  }else{
     digitalWrite(LedB, HIGH);
     digitalWrite(LedR, HIGH);
     digitalWrite(LedG, LOW);
  }
  
  
  client.print("GET ");
  client.print(resource2);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(server2);
  client.println("Connection: close");
  client.println();
  delay(1000);
  client.stop();

  
}


