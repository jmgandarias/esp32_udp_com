#include <ETH.h>
#include <WiFiUdp.h>

#define ETH_PHY_ADDR 0
#define ETH_PHY_POWER 12
#define ETH_PHY_MDC 23
#define ETH_PHY_MDIO 18
#define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT

IPAddress local_IP(192,168,0,2);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255, 255, 255, 0);


WiFiUDP udp;
const char* udpAddress = "192.168.0.1"; // Replace with your PC's IP
const int udpPort = 1234;

void WiFiEvent(WiFiEvent_t event) {
  if (event == SYSTEM_EVENT_ETH_GOT_IP) {
    Serial.print("ETH IP: ");
    Serial.println(ETH.localIP());
  }
}

void udpTask(void* pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = 1; // 1 tick = 1 ms at default config

  while (true) {
    String message = "Hello at 1kHz!";
    udp.beginPacket(udpAddress, udpPort);
    udp.print(message);
    udp.endPacket();

    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.onEvent(WiFiEvent);
  ETH.config(local_IP, gateway, subnet);
  ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_CLK_MODE);

  // Wait for Ethernet to initialize
  delay(3000);

  // Start the 1kHz UDP task
  xTaskCreatePinnedToCore(
    udpTask,       // Task function
    "UDP Task",    // Name
    4096,          // Stack size
    NULL,          // Parameters
    1,             // Priority
    NULL,          // Task handle
    1              // Core
  );
}

void loop() {
  // Nothing here, everything is handled in the task
}
