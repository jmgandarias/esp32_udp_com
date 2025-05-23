/*

Test ESP32 WT32-ETH01

Easy example to test flshing and running in the board

Author: Juan M. Gandarias
website: https://jmgandarias.com
email: jmgandarias@uma.es

*/

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  Serial.println("Hello");
  delay(1000);                      // wait for a second
  Serial.println("Good Bye");
  delay(1000);                      // wait for a second
}
