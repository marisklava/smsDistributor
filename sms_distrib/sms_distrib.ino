#include <GSM.h>

#define pin ""

GSM gsmAccess;
GSM_SMS sms;

String msg;

String owners[] = {"+37111111111", "+37111111112"}

char senderNumber[20]; 
char cmdPrefix = '!';
char msgSep = '|';

void setup()
{  
  Serial.begin(9600); // Start serial connection
  while(!Serial); // Wait for serial
  
  pinMode(13, OUTPUT); // Turn digital 13 into output
  
  Serial.print("SMS distributor v1.0\n\n");
  Serial.print("Owners: [");
  
  for(int i = 0; i < owners.length(); i++)
  {
  Serial.println(owners[i]);
  }
  
  Serial.print("]\n\n");
  Serial.println("Connecting");
  
  boolean isConnected = false;
  while(!isConnected) // While we aren't connected
  {
    if(gsmAccess.begin(pin) == GSM_READY) // Try to connect, takes several seconds
    {
      isConnected = true; // Yeah, it is connected
    }
    else
    {
      Serial.println("Not connected"); // Darn, lets try again
      delay(750); // Lil delay ©
    }
  }
  
  Serial.print("GSM initialized succesfuly\n"); // Claim that we are connected
}

void loop()
{
  if(sms.available()) // We've got a message via GSM
  {
    sms.remoteNumber(senderNumber, 20); // Get sender number
        
    Serial.print("Message received from: ");
    Serial.print("[");
    Serial.print(senderNumber);
    Serial.print("]");

    String sNum = senderNumber; // We convert it to string to make it usable
    
    if(contains(Owners, sNum)) // Is the sender owner?
    {
      Serial.println(" (Owner)");
     
      if(sms.peek() == '#')
      {
        Serial.println("Discarded SMS");
        sms.flush();
      }

      char c;
      msg = "";
     
      while(c = sms.read()) // Read SMS letter-by-letter
      {
        msg += c;
      }

      sms.flush();
      Serial.println("Message deleted from modem memory"); // Cuz we still have it localy
     
      Serial.print("Message: ");
      Serial.println(msg);

      processRequest(msg); 
    }
    else
    {
      Serial.println(" (Not owner)");    
      sms.flush();
      Serial.println("Message deleted");
    }
  }
  
  if(Serial.available()) // We've got a message via serial connection
  {
    String command = "";
    char single;
    
    while(Serial.available()) // Read all available symbols
    {
      single = Serial.read(); // Read single symbol
      if(single == '\n' || single == '\r') // Stop when new line
      {
        break;
      }
      command += single; // Add that one symbol
    }

    if(command != "")
    {
      processRequest(command);
    }
  }
}

void processRequest(String request)
{
  if(request[0] == cmdPrefix) // Is it command?
  {
    request.remove(0, 1); // Remove first symbol
    Serial.println("Command: " + request);
    
    if(request == "on")
    {
      digitalWrite(13, HIGH); // Turn led on
    }
    else if(request == "off")
    {
      digitalWrite(13, LOW); // Turn led off
    }
    else if(request == "cfg")
    {
      Serial.println("SMS distributor config: ");
      Serial.println("   Owner numbers: ");
      
      for(int i = 0; i < owners.length(); i++)
      {
      Serial.println(owners[i]);
      }
      
      Serial.println("   Command prefix: " + cmdPrefix);
      Serial.println("   Message seperator: " + msgSep);
    }
    else
    {
      Serial.println("Unknown command");
    }
  }
  else if(request.indexOf(msgSep) != -1) // Is it a message that we should distribute?
  {        
    while(request[0] != msgSep)
    {}
  }
  else
  {
    Serial.println("Nothing interesting, move along citizen");
  }
  Serial.print("This nibba: [");
  Serial.print(msg[0]);
  Serial.println("]");
}

void sendMessage(char number[20], char message[200])
{
  sms.beginSMS(number);
  sms.print(message);
  sms.endSMS();
 
  Serial.print("Sent message to ");
  Serial.println(number);
}

boolean contains(String Owners[], String Sender) {

for(int i=0; i<Owners.length(); i++){

  if(Owners[i] == Sender) return true;
  
}

return false;
  
}
