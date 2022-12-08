#include <Ethernet.h>
#include <string.h>
#include "hexadecimalStringToLong.h"
#include "Splitter.h"

#ifndef EthernetConnection_h
#define EthernetConnection_h

class EthernetConnection
{
  private:
    IPAddress ip;
    IPAddress dns;
    IPAddress gateway;
    IPAddress subnet;
    IPAddress localIp;
    byte mac[6];
    EthernetServer server;
    String readString = "";
    int port;

  public:
    EthernetConnection(int serverPort = 23) : server(serverPort) {}

    EthernetConnection(String ip, int serverPort = 23) : server(serverPort)
    {
      this->setIp(ip);
      this->start();
    }

    EthernetConnection(String ip, String dns, int serverPort = 23) : server(serverPort)
    {
      this->setIp(ip);
      this->setDns(dns);
      this->start();
    }

    EthernetConnection(String ip, String dns, String subnet, int serverPort = 23) : server(serverPort)
    {
      this->setIp(ip);
      this->setDns(dns);
      this->setSubnet(subnet);
      this->start();
    }

    EthernetConnection(String ip, String dns, String subnet, String gateway, int serverPort = 23) : server(serverPort)
    {
      this->setIp(ip);
      this->setDns(dns);
      this->setSubnet(subnet);
      this->setGateway(gateway);
      this->start();
    }

    EthernetConnection(String ip, String dns, String subnet, String gateway, String mac, int serverPort = 23) : server(serverPort)
    {
      this->setIp(ip);
      this->setDns(dns);
      this->setSubnet(subnet);
      this->setGateway(gateway);
      this->setMac(mac);
      this->start();
    }

    void setIp(String ip)
    {
      Splitter splitter(ip, '.'); // example ("192.168.1.0", '.') -> ["192", "168", "1", "0"]
      String *array = splitter.array();
      this->ip = IPAddress(
          array[0].toInt(),
          array[1].toInt(),
          array[2].toInt(),
          array[3].toInt()
      );
    }

    void setDns(String dns)
    {
      Splitter splitter(dns, '.');
      String *array = splitter.array();
      this->dns = IPAddress(
          array[0].toInt(),
          array[1].toInt(),
          array[2].toInt(),
          array[3].toInt());
    }

    void setSubnet(String subnet)
    {
      Splitter splitter(subnet, '.');
      String *array = splitter.array();
      this->subnet = IPAddress(
          array[0].toInt(),
          array[1].toInt(),
          array[2].toInt(),
          array[3].toInt());
    }

    void setGateway(String gateway)
    {
      Splitter splitter(gateway, '.');
      String *array = splitter.array();
      this->gateway = IPAddress(
          array[0].toInt(),
          array[1].toInt(),
          array[2].toInt(),
          array[3].toInt());
    }

    void setMac(String mac)
    {
      Splitter splitter(mac, ':'); // example 00:00:5e:00:53:af -> ["00", "00", "5e", "00", "53", "af"]
      String *array = splitter.array();

      for (int i = 0; i < splitter.size(); i++)
      {
        this->mac[i] = hexadecimalStringToLong(array[i]);
      }
    }

    void start()
    {
      Serial.println("Starting");
      
      Ethernet.begin(this->mac, this->ip, this->dns, this->gateway, this->subnet);

      if (Ethernet.hardwareStatus() == EthernetNoHardware)
      {
        Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
        while (true)
        {
          delay(1); // do nothing, no point running without Ethernet hardware
        }
      }

      if (Ethernet.linkStatus() == LinkOFF)
      {
        Serial.println("Ethernet cable is not connected.");
      }

      this->server.begin();
      Serial.print("IP Address: ");
      Serial.println(Ethernet.localIP());
    }

    EthernetServer getServer()
    {
      return this->server;
    }

    EthernetClient getEthernetClient()
    {
      return this->getServer().available();
    }

    void maintain()
    {
      Ethernet.maintain();
    }

    char read()
    {
      EthernetClient ethernetClient = this->getEthernetClient();
      char data;

      if (ethernetClient)
      {
        data = ethernetClient.read();
      }

      return data;
    }

    String readLine()
    {
      String line = "";

      if (this->getEthernetClient())
      {
        char character = this->read();

        if (!(character == '\n' || character == '\r'))
        {
          this->readString += character;
        }

        if ((character == '\n' || character == '\r') && this->readString.length() > 0)
        {
          line.concat(this->readString);
          this->readString = "";
        }
      }

      if (line.length() > 0)
      {
        return line;
      }

      return "";
    }

    void writeLine(String message)
    {
      EthernetClient client = this->getEthernetClient();
      client.flush();
      client.println(message);
    }
};

#endif /*EthernetConnection_h*/