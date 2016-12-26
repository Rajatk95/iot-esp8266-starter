#include "SoftwareSerial.h"
 
#define DEBUG true
#define pin11 11
#define pin12 12
#define pin13 13
#define pin7 4
SoftwareSerial esp8266(2,3); #pins on arduino for interfacing with esp8266

boolean FAIL_8266 = false;

#define BUFFER_SIZE 128
//char buffer[BUFFER_SIZE];

void setup() {
   pinMode(pin11,OUTPUT);
  digitalWrite(pin11,LOW);

  pinMode(pin12,OUTPUT);
  digitalWrite(pin12,LOW);

  pinMode(pin13,OUTPUT);
  digitalWrite(pin13,LOW);

  do{
    Serial.begin(9600);
    esp8266.begin(9600);
  
    //Wait Serial Monitor to start
    while(!Serial);
    Serial.println("--- Start ---");

    esp8266.println("AT+RST");
    delay(1000);
    if(esp8266.find("ready"))
    {
      Serial.println("Module is ready");
      esp8266.println("AT+CWMODE=2");
      delay(1000);
      

      //Get and display my IP
      sendesp8266Cmdln("AT+CIFSR", 1000);  
      //Set multi connections
      sendesp8266Cmdln("AT+CIPMUX=1", 1000);
      sendesp8266Cmdln("AT+CIPSERVER=1,331",1000);
   //     clearesp8266SerialBuffer();
      Serial.println("Server setup finish");
        
      FAIL_8266 = false;
    }else{
      Serial.println("Module have no response.");
      delay(500);
      FAIL_8266 = true;
    }
  }while(FAIL_8266);
  
  digitalWrite(pin7, HIGH);
  esp8266.setTimeout(1000);
}


void loop()
{

  if(esp8266.available()) // check if the esp is sending a message 
  { //Serial.println(esp8266.read());  
 //   clearesp8266SerialBuffer();
    if(esp8266.find("+IPD,"))
    {    
   //   clearesp8266SerialBuffer(); idhar tk no buffer
      delay(1000);
      int connectionId = esp8266.read()-48;
      Serial.print("Connection ID ");
      Serial.println(connectionId);
       esp8266.find("pin=");
    //  Serial.println(esp8266.read()); //44 aaraha hai
      int pinNumber = (esp8266.read()-48)*10; // get first number i.e. if the pin 13 then the 1st number is 1, then multiply to get 10
      pinNumber += (esp8266.read()-48);
      Serial.print("Pin Number ");
      Serial.println(pinNumber);
      
    
      digitalWrite(pinNumber, !digitalRead(pinNumber)); 
      cipSend(String(connectionId),1000);
      closeconn(String(connectionId),1000);
      clearesp8266SerialBuffer();
    }
  }
}
void cipSend(String cid, int waitTime)
{
  esp8266.println("AT+CIPSEND="+cid+",2");
  delay(waitTime);
  esp8266.println("ok");
  delay(waitTime);
  
}
void sendesp8266Cmdln(String cmd, int waitTime)
{
  esp8266.println(cmd);
  delay(waitTime);
//  clearesp8266SerialBuffer();
}
void closeconn(String cmd, int waitTime)
{
  esp8266.println("AT+CIPCLOSE="+String(cmd));
  Serial.println("Closing Connection");
  esp8266.println(cmd);
  delay(waitTime);
}
//Basically same as sendesp8266Cmdln()
//But call esp8266.print() instead of call esp8266.println()
void sendesp8266Data(String data, int waitTime)
{
  esp8266.print(data);
  delay(waitTime);
  clearesp8266SerialBuffer();
}


//Clear and display Serial Buffer for esp8266
void clearesp8266SerialBuffer()
{
//  Serial.println("= clearesp8266SerialBuffer() =");
  while (esp8266.available() > 0) {
    char a = esp8266.read();
    Serial.write(a);
  }
//  Serial.println("=====Done Cleaning======");
}
