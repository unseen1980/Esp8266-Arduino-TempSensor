#include <SPI.h>

#include <Adafruit_MAX31855.h>

#include<stdlib.h>
#define SSID "YOUR SSID"//your network name
#define PASS "YOUR PASSWORD"//your network password
#define IP "184.106.153.149" // thingspeak.com
#define Baud_Rate 115200 //Another common value is 9600
#define DELAY_TIME 10000 //time in ms between posting data to ThingSpeak

int thermoCLK = 11;
int thermoCS = 12;
int thermoDO = 13;
Adafruit_MAX31855 thermocouple(thermoCLK, thermoCS, thermoDO);
String GET = "GET /update?key=YOUR_KEY&field1=";

bool updated;

void setup()
{
  Serial.begin(Baud_Rate);
  Serial.println("AT");
  
  delay(5000);
  
  if(Serial.find("OK")){
    //connect to your wifi netowork
    bool connected = connectWiFi();
    if(!connected){
      Serial.println("failure, not connected");
    }
  }else{
    Serial.println("failure, need to check your values and try again");
  }                     
}
 
void loop()                     // run over and over again
{
double c = thermocouple.readCelsius();
   if (isnan(c)) 
   {
     Serial.print("T/C Problem");
   } 
   else 
   { 
     Serial.print(c);
     updated = updateTemp(String(c));
   }

 delay(DELAY_TIME);
}


bool updateTemp(String temp){
  //initialize your AT command string
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  
  //add IP address and port
  cmd += IP;
  cmd += "\",80";
  
  //connect
  Serial.println(cmd);
  delay(2000);
  if(Serial.find("Error")){
    return false;
  }
  
  //build GET command, ThingSpeak takes Post or Get commands for updates, I use a Get
  cmd = GET;
  cmd += temp;  
  cmd += "\r\n";
  
  //Use AT commands to send data
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  if(Serial.find(">")){
    //send through command to update values
    Serial.print(cmd);
  }else{
    Serial.println("AT+CIPCLOSE");
  }
  
  if(Serial.find("OK")){
    //success! Your most recent values should be online.
    return true;
  }else{
    return false;
  }
}
 
boolean connectWiFi(){
  //set ESP8266 mode with AT commands
  Serial.println("AT+CWMODE=1");
  delay(2000);

  //build connection command
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  
  //connect to WiFi network and wait 5 seconds
  Serial.println(cmd);
  delay(5000);
  
  //if connected return true, else false
  if(Serial.find("OK")){
    return true;
  }else{
    return false;
  }
}

