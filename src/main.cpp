#include <Arduino.h>
#include <Ethernet.h>
#include <ArduinoJson.h>
#include "libs/EthernetConnection.h"

EthernetConnection ethernetConnection = EthernetConnection();

bool encendido = true;

void (*resetFunc)(void) = 0; // declare reset function @ address 0

void setup()
{
  Serial.begin(9600);

  ethernetConnection.setIp("192.168.100.60");
  ethernetConnection.setDns("255.255.255.0");
  ethernetConnection.setMac("00:00:5e:00:53:af");
  ethernetConnection.start();

  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

bool show = false;

void loop()
{
  if (encendido)
  {

    String line = ethernetConnection.readLine();
    DynamicJsonDocument doc(2048);

    if (line.length() > 0)
    {
      // Serial.println(line);

      deserializeJson(doc, line);
      String command = doc["command"];
      Serial.println(command);

      if (command == "null")
      {
        Serial.println("JSON Invalido:");
        Serial.println(line);
      }
    
      else if (command == "ping")
      {
        ethernetConnection.writeLine("pong");
        ethernetConnection.maintain();
      }

      else if (command == "apagar")
      {
        encendido = false;
        delay(1000); // wait for a second
        Serial.println("resetting");
        resetFunc(); // call reset
      }

      else if (command == "delay")
      {
        delay(10000);
        ethernetConnection.writeLine("pong");
        ethernetConnection.maintain();
      }
      
      else if (command == "dispatch")
      {
        DynamicJsonDocument doc(1024);
        String result;
        doc["response"] = "ok";
        doc["line"] = 1;
        delay(1000);
        serializeJson(doc, result);
        ethernetConnection.writeLine(result);

        delay(7000);
        result = "";
        doc["response"] = "ok";
        doc["line"] = 4;
        serializeJson(doc, result);
        ethernetConnection.writeLine(result);
        // This prints:
        // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}

        ethernetConnection.maintain();
      }
    }

  }
}
