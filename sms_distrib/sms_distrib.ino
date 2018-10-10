#include <GSM.h>

#define PINNUMBER ""

GSM gsmAccess;
GSM_SMS sms;

String recMessage;
const String pin = "maybe use string instead?";
const String oNum = "+371";
char senderNumber[20]; 
char cmdPrefix = "!";

void setup()
{  
  Serial.begin(9600);
  while(!Serial);
  
  pinMode(13, OUTPUT);
  
  Serial.print("SMS distributor v1.0\n\n");
  Serial.print("Owner: [");
  Serial.print(oNum);
  Serial.print("]\n\n");
  Serial.println("Connecting");
  
  boolean isConnected = false;
  while(!isConnected)
  {
    if(gsmAccess.begin(PINNUMBER) == GSM_READY)
    {
      isConnected = true;      
    }
    else
    {
      Serial.println("Not connected");
      delay(750);
    }
  }
  Serial.print("GSM initialized succesfuly\n");
}

void loop()
{
  if(sms.available())
  {
    sms.remoteNumber(senderNumber, 20);
        
    Serial.print("Message received from: ");   
    Serial.print("[");
    Serial.print(senderNumber);
    Serial.print("]");

    String sNum = senderNumber;
    
    if(sNum == oNum)
    {
      Serial.println(" (Owner)");
     
      if(sms.peek() == '#')
      {
        Serial.println("Discarded SMS");
        sms.flush();
      }

      char c;
      recMessage = "";
     
      while(c = sms.read())
      {
        recMessage += c;
      }
     
      Serial.print("Message: ");
      Serial.println(recMessage);

      if(recMessage[0] == cmdPrefix)
      {
        recMessage.remove(0, 1);
        Serial.println("Command: " + recMessage);
        
        if(recMessage == "ON")
        {
          digitalWrite(13, HIGH);
        }
        else if(recMessage == "OFF")
        {
          digitalWrite(13, LOW);
        }
      }
           
      sms.flush();
      Serial.println("Message deleted from modem memory");
    }
    else
    {
      Serial.println(" (Not owner)");
     
      sms.flush();
      Serial.println("Message deleted");
    }
  }
}

void sendMessage(char number[20], char message[200])
{
  sms.beginSMS(number);
  sms.print(message);
  sms.endSMS();
 
  Serial.print("Sent message to ");
  Serial.println(number);
}
