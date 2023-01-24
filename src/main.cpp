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

String val = "0";

void loop()
{

  String line = ethernetConnection.readLine();

  if (line.length() == 0)
    return;

  DynamicJsonDocument doc(128);
  deserializeJson(doc, line); //{"command": "ping"}
  String command = doc["command"];
  // Serial.println(command);

  if (command == "null")
  {
    Serial.println("invalid JSON:");
    Serial.println(line);
  }

  else if (command == "ping")
  {
    if (val == "0")
    {
      val = "1";
    }
    else
    {
      val = "0";
    }

    DynamicJsonDocument doc(128);
    String result;
    doc["command"] = "pong";
    serializeJson(doc, result); //{"command": "pong"}
    ethernetConnection.writeLine(result);
    Serial.println(val);
  }

  else if (command == "step-length")
  {
    DynamicJsonDocument res(128);
    String result;
    res["code"] = "MSG551";
    res["command"] = "step-length";
    res["module"] = doc["module"];
    res["step-length"] = doc["step-length"];
    res["status"] = "ok";
    serializeJson(res, result);
    ethernetConnection.writeLine(result);
    Serial.print("response: ");
    Serial.println("step-length");
  }

  else if (command == "elevator")
  {
    delay(2000);
    DynamicJsonDocument res(128);
    String result;
    res["code"] = "MSG201";
    res["command"] = "elevator-reaches-height";
    serializeJson(res, result);
    ethernetConnection.writeLine(result);
    Serial.print("response: ");
    Serial.println("elevator-reaches-height");
  }

  else if (command == "expend")
  {
    delay(4000);
    DynamicJsonDocument res(128);
    String result;
    res["code"] = "MSG502";
    res["command"] = "expended";
    res["module"] = doc["module"];
    res["quantity"] = doc["quantity"];
    serializeJson(res, result);
    ethernetConnection.writeLine(result);
    Serial.print("response: ");
    Serial.println("expended");
  }

  else if (command == "conveyor")
  {
    delay(4000);
    DynamicJsonDocument res(128);
    String result;
    res["code"] = "MSG202";
    res["command"] = "conveyor-stop";
    serializeJson(res, result);
    ethernetConnection.writeLine(result);
    Serial.print("response: ");
    Serial.println("conveyor-stop");
  }

  ethernetConnection.maintain();
}