// ESP8266-based smart locker
// Developed by Maxim Bortnikov
// For more information and for the circuit of this device visit: https://github.com/Northstrix/ESP8266-based_smart_locker_with_display
// Button template was designed by Julia. You can find it here: https://codepen.io/sfoxy/pen/XpOoJe
#include <ESP8266WiFi.h>       // Include ESP8266WiFi library
#include <Servo.h>             // Include Servo library
#include <SPI.h>               // Include SPI library
#include <Adafruit_GFX.h>      // Include Adafruit graphics library
#include <Adafruit_PCD8544.h>  // Include Adafruit PCD8544 (Nokia 5110) library
Adafruit_PCD8544 display = Adafruit_PCD8544(D4, D3, D2, D1, D0);
Servo servo;
const char* ssid     = "Blackberry";  // Your SSID (Network's name)
const char* password = "seasidehills99"; // Password from your network
WiFiServer server(80);
int opan = 110; // Servo's angle for the open locker
int clan = 0; // Servo's angle for the closed locker
void setup(){
  display.begin();          // Initialise the display
  display.setContrast(60);  // Set the contrast
  display.setRotation(0);   // Set the orientation
  display.clearDisplay();   // Clear the screen (In case something was in it)
  display.setTextSize(1);   // Set the text size 
  display.setTextColor(BLACK); // Set the text color
  display.setCursor(10,8);
  display.println("Connecting");
  display.setCursor(22,18);
  display.println("to the");
  display.setCursor(5,28);
  display.println("Access Point");
  display.display();       // Display the text
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);  // Turn the green LED off
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH); // Light up the red LED
  Serial.begin(115200); // Baudrate
  servo.attach(14); // Assign servo to the D5 pin
  servo.write(clan); // Close the locker
  Serial.print("Connecting to the Newtork"); // This all will be displayed in the Serial Monitor
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected"); 
  server.begin(); // Starting the server
  Serial.println("Server started");
  Serial.print("IP Address of network: ");
  Serial.println(WiFi.localIP()); // Display device's IP address in serial monitor
  Serial.println("/");
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(19,5);
  display.println("Device's");
  display.setCursor(10,15);
  display.println("IP address:");
  display.setCursor(0,25);
  display.println(WiFi.localIP()); // Display device's IP address on display
  display.display();
}

 

void loop(){

  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }
  Serial.println("Waiting for new client");
  while(!client.available())  
  {
    delay(1);
  } 
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  int locker = LOW;
  
      if(request.indexOf("/locker=Open") != -1)

  {
    servo.write(opan);
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
    locker = HIGH;
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(12,2);
    display.println("The locker");
    display.setCursor(20,12);
    display.println("is open");
    display.setCursor(10,22);
    display.println("IP address:");
    display.setCursor(0,32);
    display.println(WiFi.localIP()); // Display device's IP address on display
    display.display();
  }
  if(request.indexOf("/locker=Close") != -1)
  {
    digitalWrite(12, LOW);
    digitalWrite(13, HIGH);
    servo.write(clan);
    locker = LOW;
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(12,2);
    display.println("The locker");
    display.setCursor(15,12);
    display.println("is closed");
    display.setCursor(10,22);
    display.println("IP address:");
    display.setCursor(0,32);
    display.println(WiFi.localIP()); // Display device's IP address on display
    display.display();
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html"); // All below is a web page code written in HTML
  client.println("");
  client.println("<!DOCTYPE html>\n"
"<html>\n"
"  <head>\n"
"    <meta charset=\"utf-8\">\n"
"    <meta name=\"viewport\" content=\"width=device-width\">\n"
"    <title>repl.it</title>\n"
"    <link href=\"style.css\" rel=\"stylesheet\" type=\"text/css\" />\n"
"  </head>\n"
"  <body>\n"
"    <script src=\"script.js\"></script>\n"
"  </body>\n"
"</html></html><!DOCTYPE html>\n"
"<html>\n"
"  <head>\n"
"    <style>\n"
"body {\n"
"  background-color: #121414;\n"
"  background: radial-gradient(ellipse at bottom, #1b2735 0%, #090a0f 100%);\n"
"  height: 100vh;\n"
"  overflow: hidden;\n"
"  display: grid;\n"
"  margin-top: 20px;\n"
"}\n"
"\n"
"h4 {\n"
"  text-align: center;\n"
"  color: #DDD;\n"
"  letter-spacing: 0.05em;\n"
"  font-size: 1em;\n"
"  margin: 0 0 1em;\n"
"}\n"
"\n"
"h41 {\n"
"  text-align: center;\n"
"  color: #DDD;\n"
"  letter-spacing: 0.05em;\n"
"  font-size: 0.7em;\n"
"  margin: 0 0 1em;\n"
"}\n"
"\n"
".btn {\n"
"  border: none;\n"
"  display: block;\n"
"  text-align: center;\n"
"  cursor: pointer;\n"
"  outline: none;\n"
"  overflow: hidden;\n"
"  position: relative;\n"
"  color: #DDD;\n"
"  font-weight: 700;\n"
"  font-size: 15px;\n"
"  background-color: #222;\n"
"  padding: 17px 60px;\n"
"  margin-bottom: 30px;\n"
"  box-shadow: 0 5px 15px rgba(0,0,0,0.20);\n"
"}\n"
"\n"
".btn2 {\n"
"  border: none;\n"
"  display: block;\n"
"  text-align: center;\n"
"  cursor: pointer;\n"
"  text-transform: uppercase;\n"
"  outline: none;\n"
"  overflow: hidden;\n"
"  position: relative;\n"
"  color: #fff;\n"
"  font-weight: 700;\n"
"  font-size: 15px;\n"
"  background-color: #222;\n"
"  padding: 17px 60px;\n"
"  margin-bottom: 30px;\n"
"  box-shadow: 0 5px 15px rgba(0,0,0,0.20);\n"
"}\n"
"\n"
".btn span {\n"
"  position: relative; \n"
"  z-index: 1;\n"
"}\n"
"\n"
".btn2 span {\n"
"  position: relative; \n"
"  z-index: 1;\n"
"}\n"
"\n"
".btn:after {\n"
"  content: \"\";\n"
"  position: absolute;\n"
"  left: 0;\n"
"  top: 0;\n"
"  height: 490%;\n"
"  width: 140%;\n"
"  background: #298149;\n"
"  -webkit-transition: all .5s ease-in-out;\n"
"  transition: all .5s ease-in-out;\n"
"  -webkit-transform: translateX(-98%) translateY(-25%) rotate(45deg);\n"
"  transform: translateX(-98%) translateY(-25%) rotate(45deg);\n"
"}\n"
"\n"
".btn2:after {\n"
"  content: \"\";\n"
"  position: absolute;\n"
"  left: 0;\n"
"  top: 0;\n"
"  height: 490%;\n"
"  width: 140%;\n"
"  background: #81293E;\n"
"  -webkit-transition: all .5s ease-in-out;\n"
"  transition: all .5s ease-in-out;\n"
"  -webkit-transform: translateX(-98%) translateY(-25%) rotate(45deg);\n"
"  transform: translateX(-98%) translateY(-25%) rotate(45deg);\n"
"}\n"
"\n"
".btn:hover:after {\n"
"  -webkit-transform: translateX(-9%) translateY(-25%) rotate(45deg);\n"
"  transform: translateX(-9%) translateY(-25%) rotate(45deg);\n"
"}\n"
"\n"
".btn2:hover:after {\n"
"  -webkit-transform: translateX(-9%) translateY(-25%) rotate(45deg);\n"
"  transform: translateX(-9%) translateY(-25%) rotate(45deg);\n"
"}\n"
"\n"
"\n"
"\n"
"\n"
"    </style>\n"
"    <meta charset=\"utf-8\">\n"
"    <meta name=\"viewport\" content=\"width=device-width\">\n"
"    <title>ESP8266-based smart locker</title>\n"
"      </head>\n"
"  <body>    \n"
"    <table style=\"margin-left:auto;margin-right:auto;text-align:center;max-width: 320px\";>\n"
"  <tr>\n"
"    <th colspan=\"2\"><h4>ESP8266-based smart locker</h4></th>\n"
"  </tr>\n"
"  <tr>\n"
"    <td colspan=\"2\"><h4>Developed by Maxim Bortnikov</h4></td>\n"
"  </tr>\n"
"   <tr>\n");
    if(locker == HIGH)
  {
    client.print("<td colspan=\"2\"><h4>The locker is open</h4></td>");
  }
  else
  {
    client.print("<td colspan=\"2\"><h4>The locker is closed</h4></td>");
  }
 client.println("  </tr>\n"
"     <tr>\n"
"    <td><a href=\"/locker=Open\"\" button class=\"btn\"><span>OPEN</span></button></td>\n"
"    <td><a href=\"/locker=Close\"\" button class=\"btn2\"><span>CLOSE</span></button></td>\n"
"  </tr>\n"
"     <tr>\n"
"    <td colspan=\"2\"><h41>For more information visit: https://github.com/Northstrix/ESP8266-based_smart_locker_with_display</h41></td>\n"
"  </tr>\n"
"  </tr>\n"
"</table>\n"
"  </body>\n"
"</html>");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

}
