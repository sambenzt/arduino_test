#include <Arduino.h>
#include <Ethernet.h>
#include <ArduinoJson.h>
#include "libs/EthernetConnection.h"

EthernetConnection ethernetConnection = EthernetConnection();

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

void loop()
{
  String line = ethernetConnection.readLine();
  int rand = random(100);

  if(line.length() > 0) {
    Serial.println(line);
  }
  
  if (line == "ping")
  {
    ethernetConnection.writeLine("pong");
    ethernetConnection.maintain();
  }

  if (line == "hola")
  {
    ethernetConnection.writeLine("hola como estais?");
    ethernetConnection.maintain();
  }

  if (rand >= 99)
  {
    String num = String(rand);
    String message = " es el num que salio";
    num.concat(message);
    ethernetConnection.writeLine(num);
    ethernetConnection.maintain();
  }

}
