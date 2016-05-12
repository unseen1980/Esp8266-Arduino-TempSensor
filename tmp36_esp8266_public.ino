#include<stdlib.h>
#define SSID "Your_SSID"//your network name
#define PASS "Your_Password"//your network password
#define IP "184.106.153.149" // thingspeak.com
#define Baud_Rate 115200 //Another common value is 9600
#define DELAY_TIME 10000 //time in ms between posting data to ThingSpeak
int sensorPin = 17; //the analog pin the TMP36's Vout (sense) pin is connected to
String GET = "GET /update?key=YOUR_KEY=";

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

void loop()
{
 //getting the voltage reading from the temperature sensor
 int reading = analogRead(sensorPin);

 // converting that reading to voltage, for 3.3v arduino use 3.3
 float voltage = reading * 5.0;
 voltage /= 1024.0;

 float temperatureC = (voltage - 0.5) * 100 ;
 Serial.print(temperatureC);
 updated = updateTemp(String(temperatureC));
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
