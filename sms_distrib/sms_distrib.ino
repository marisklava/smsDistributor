#include <GSM.h>

#define PINNUMBER ""

GSM gsmAccess;
GSM_SMS sms;

const char ownerNumber[20] = "1188";
char senderNumber[20]; 
char c;

String recMessage;

void setup()
{
  Serial.begin(9600);
  while(!Serial);
 
  Serial.println("SMS distributor v1.0");

  boolean notConnected = true;
  while(notConnected)
  {
    if(gsmAccess.begin(PINNUMBER) == GSM_READY)
    {
      notConnected = false;
    }
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }
  Serial.println("GSM initialized");
  Serial.println("Waiting for messages");
}

void loop()
{
  if(sms.available())
  {
    Serial.print("Message received from: ");
   
    sms.remoteNumber(senderNumber, 20);
    Serial.println(senderNumber);

    if(senderNumber == ownerNumber)
    {
      Serial.println(" (Owner)");
     
      if(sms.peek() == '#')
      {
        Serial.println("Discarded SMS");
        sms.flush();
      }
     
      recMessage = "";
     
      while(c = sms.read())
      {
        recMessage += c;
      }
     
      Serial.print("Message: ");
      Serial.println(recMessage);
     
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
