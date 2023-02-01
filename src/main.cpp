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

  else if (command == "principal-door-open")
  {
    Serial.println("execute principal-door-open");
    DynamicJsonDocument res1(128);
    String result1;
    res1["code"] = "MSG151";
    res1["command"] = "principal-door-open";
    res1["status"] = "ok";
    serializeJson(res1, result1);
    ethernetConnection.writeLine(result1);

    delay(5000);
    DynamicJsonDocument res2(128);
    String result2;
    res2["code"] = "MSG103";
    res2["command"] = "principal-door";
    res2["value"] = "opened";
    serializeJson(res2, result2);
    ethernetConnection.writeLine(result2);
    Serial.print("response: ");
    Serial.println("principal-door open");

    delay(2000);
    DynamicJsonDocument res3(128);
    String result3;
    res3["code"] = "MSG103";
    res3["command"] = "principal-door";
    res3["value"] = "closed";
    serializeJson(res3, result3);
    ethernetConnection.writeLine(result3);
    Serial.print("response: ");
    Serial.println("principal-door closed");

    delay(2000);
    DynamicJsonDocument res4(128);
    String result4;
    res4["code"] = "MSG102";
    res4["command"] = "PIR";
    res4["value"] = true;
    serializeJson(res4, result4);
    ethernetConnection.writeLine(result4);
    Serial.print("response: ");
    Serial.println("PIR true");
  }
  
  else if (command == "elevator")
  {
    Serial.println("execute elevator");
    DynamicJsonDocument res1(128);
    String result1;
    res1["code"] = "MSG251";
    res1["command"] = "elevator";
    res1["status"] = "ok";
    serializeJson(res1, result1);
    ethernetConnection.writeLine(result1);

    delay(10000);
    DynamicJsonDocument res2(128);
    String result;
    String value = doc["value"];
    res2["code"] = "MSG201";
    res2["command"] = "elevator-reaches-height";
    serializeJson(res2, result);
    ethernetConnection.writeLine(result);
    Serial.print("response: ");
    Serial.print("elevator-reaches-height ");
    Serial.println(value);
  }

  else if (command == "expend")
  {
    Serial.println("execute expend");
    DynamicJsonDocument res1(128);
    String result1;
    res1["code"] = "MSG552";
    res1["command"] = "expend";
    res1["status"] = "ok";
    serializeJson(res1, result1);
    ethernetConnection.writeLine(result1);

    delay(11000);
    DynamicJsonDocument res2(128);
    String result;
    res2["code"] = "MSG502";
    res2["command"] = "expended";
    res2["module"] = doc["module"];
    res2["quantity"] = doc["quantity"];
    serializeJson(res2, result);
    ethernetConnection.writeLine(result);
    Serial.print("response: ");
    Serial.println("expended");
  }

  else if (command == "conveyor")
  {
    Serial.println("execute conveyor");
    DynamicJsonDocument res1(128);
    String result1;
    res1["code"] = "MSG261";
    res1["command"] = "conveyor";
    res1["status"] = "ok";
    serializeJson(res1, result1);
    ethernetConnection.writeLine(result1);

    delay(10000);
    DynamicJsonDocument res2(128);
    String result;
    res2["code"] = "MSG202";
    res2["command"] = "conveyor-stop";
    serializeJson(res2, result);
    ethernetConnection.writeLine(result);
    Serial.print("response: ");
    Serial.println("conveyor-stop");

    delay(5000);
    DynamicJsonDocument res3(128);
    String result2;
    res3["code"] = "MSG103";
    res3["command"] = "principal-door";
    res3["value"] = "opened";
    serializeJson(res3, result2);
    ethernetConnection.writeLine(result2);
    Serial.print("response: ");
    Serial.println("principal-door open");

    delay(2000);
    DynamicJsonDocument res4(128);
    String result3;
    res4["code"] = "MSG103";
    res4["command"] = "principal-door";
    res4["value"] = "closed";
    serializeJson(res4, result3);
    ethernetConnection.writeLine(result3);
    Serial.print("response: ");
    Serial.println("principal-door closed");
  }

  ethernetConnection.maintain();
}