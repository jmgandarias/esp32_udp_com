/*

Test ESP32 WT32-ETH01 UDP communication

UDP communication with the ESP32 over ethernet

Author: Juan M. Gandarias
website: https://jmgandarias.com
email: jmgandarias@uma.es

*/

#define DEBUG_ETHERNET_WEBSERVER_PORT Serial

// Debug Level from 0 to 4
#define _ETHERNET_WEBSERVER_LOGLEVEL_ 3

#include <WebServer_WT32_ETH01.h>

// Select the IP address according to your local network
IPAddress myIP(192, 168, 1, 1);
IPAddress myGW(192, 168, 1, 1);
IPAddress mySN(255, 255, 255, 0);

// PC IP Address
IPAddress remoteIp(192, 168, 1, 2);

// Google DNS Server IP
IPAddress myDNS(8, 8, 8, 8);

unsigned int localPort = 1883; // 10002;  // local port to listen on

char packetBuffer[255];     // buffer to hold incoming packet

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;

// Define the preiod of messages in ms
int period_ms = 1;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;

  // Using this if Serial debugging is not necessary or not using Serial port
  // while (!Serial && (millis() < 3000));

  Serial.print("\nStarting UDPSendReceive on " + String(ARDUINO_BOARD));
  Serial.println(" with " + String(SHIELD_TYPE));
  Serial.println(WEBSERVER_WT32_ETH01_VERSION);

  // To be called before ETH.begin()
  WT32_ETH01_onEvent();

  // bool begin(uint8_t phy_addr=ETH_PHY_ADDR, int power=ETH_PHY_POWER, int mdc=ETH_PHY_MDC, int mdio=ETH_PHY_MDIO,
  //            eth_phy_type_t type=ETH_PHY_TYPE, eth_clock_mode_t clk_mode=ETH_CLK_MODE);
  // ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_PHY_TYPE, ETH_CLK_MODE);
  ETH.begin(ETH_PHY_TYPE, ETH_PHY_ADDR, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_PHY_POWER, ETH_CLK_MODE);

  // Static IP, leave without this line to get IP via DHCP
  // bool config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1 = 0, IPAddress dns2 = 0);
  ETH.config(myIP, myGW, mySN, myDNS);

  WT32_ETH01_waitForConnect();

  Serial.println(F("\nStarting connection to server..."));
  // if you get a connection, report back via serial:
  Udp.begin(localPort);

  Serial.print(F("Listening on port "));
  Serial.println(localPort);

  // Start the 1kHz UDP task
  xTaskCreatePinnedToCore(
      udpTask,    // Task function
      "UDP Task", // Name
      4096,       // Stack size
      NULL,       // Parameters
      1,          // Priority
      NULL,       // Task handle
      1           // Core
  );
}

void loop()
{
  // Nothing here, everything is handled in the task
}


void udpTask(void *pvParameters)
{
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = period_ms / portTICK_PERIOD_MS;; // 10 ms

  while (true)
  {
    byte ReplyBuffer[] = "Hello at 100 Hz!"; // a string to send back

    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), localPort);
    Udp.write(ReplyBuffer, sizeof(ReplyBuffer));
    Udp.endPacket();

    // if there's data available, read a packet
    int packetSize = Udp.parsePacket();

    if (packetSize)
    {
      Serial.print(F("Received packet of size "));
      Serial.println(packetSize);
      Serial.print(F(", port "));
      Serial.println(Udp.remotePort());

      // read the packet into packetBufffer
      int len = Udp.read(packetBuffer, 255);

      if (len > 0)
      {
        packetBuffer[len] = 0;
      }

      Serial.println(F("Contents:"));
      Serial.println(packetBuffer);
    }

    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}
