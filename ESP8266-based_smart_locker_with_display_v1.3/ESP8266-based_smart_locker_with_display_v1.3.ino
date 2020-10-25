// ESP8266-based smart locker
// Developed by Maxim Bortnikov
// For more information and for the circuit of this device visit: https://github.com/Northstrix/ESP8266-based_smart_locker_with_display
// In this project I used code from:
// http://www.martyncurrey.com/esp8266-and-the-arduino-ide-part-6-javascript-and-ajax/
// https://codepen.io/ma_suwa/pen/qBBYKNy
// https://codepen.io/chancesq/pen/abNvEev
// https://codepen.io/rgg/pen/rxLmQm
String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
 
String html_1 = R"=====(
<!DOCTYPE html>
<html>
 <head>
 <meta name='viewport' content='width=device-width, initial-scale=1.0'/>
 <meta charset='utf-8'>
    <style>
  * {
  margin: 0;
  padding: 0;
}
html,
body {
  //      background-image: linear-gradient(-120deg, #26E289 0, #1a5fa2 100%, red 200%);
  background-color: #D0D0D0;
  font-family: Segoe UI,Frutiger,Frutiger Linotype,Dejavu Sans,Helvetica Neue,Arial,sans-serif; 
  font-weight: 400;
  box-sizing: border-box;
  height: 100%;
  width: 100%;
}
#Locker_button {
height: 43px;
width:100%;
background-color: Transparent;
background-repeat:no-repeat;
border: none;
color: #EEE;
border-radius: 20px;
cursor:pointer;
overflow: hidden; 

}
.noselect {
  -webkit-touch-callout: none;
    -webkit-user-select: none;
     -khtml-user-select: none;
       -moz-user-select: none;
        -ms-user-select: none;
            user-select: none;
    -webkit-tap-highlight-color: transparent;
}

button {
  width: 70%;
  height: 50px;
  cursor: pointer;
  background-color: #0071E3;
  border: none;
  border-radius: 30px;
  background-image: linear-gradient(-120deg, #0071E3 0, #1a5fa2 100%);
  box-shadow: 0 0 200px rgba(255,255,255,0.0);
  transition: 1s;
  display: flex;
  align-items: center;
  justify-content: center;
}

button:after {
  position: absolute;
  content: '';
  width: 25px;
  height: 25px;
  border-radius: 100%;
  background: transparent;
  box-shadow: 0 0 80px rgba(255,255,255,1);
}

button:hover {
  box-shadow: 0 0 200px rgba(255,255,255,1);
}

button:hover:after {
  animation: animate 3s infinite linear;
}

button:focus {
  outline: none;
}

@keyframes animate {
  0% { transform: translateX(60px)}
  50% {transform: translateX(-60px)}
  100% {transform: translateX(60px)}
}
.buttons {
  display: flex;
  flex-direction: row;
  flex-wrap: wrap;
  justify-content: center;
  text-align: center;
  width: 100%;
  height: 100%;
  margin: 0 auto;
}
.container {
  display: inline-block;
  align-items: center;
  display: flex;
  flex-direction: column;
  justify-content: top;
  text-align: center;
  background-image: linear-gradient(-120deg, #26E289 0, #1a5fa2 100%);
  padding:  40px 0px;
  width: 300px;
  height: 300px;
  margin-top: 10vh;
  margin-left: auto;
  margin-right: auto;
  border-top: solid 1px rgba(255,255,255,.5);
  border-radius: 5px;
  box-shadow: 0px 2px 7px rgba(0,0,0,0.2);
  overflow: hidden;
}
h1 {
  text-align: center;
  color: #EEE;
  letter-spacing: 0.05em;
  margin: 0 0 0.4em;
  font-size: 1em;
}
p {
  text-align: left;
  color: #EEE;
  letter-spacing: 0.05em;
  font-size: 0.8em;
  margin: 0 0 2em;
}    
</style>
 
<script>
    function switchLocker() 
  {
       var button_text = document.getElementById("Locker_button").value;
     if (button_text=="Open the locker")
     {
       document.getElementById("Locker_button").value = "Opening the locker";
       ajaxLoad('LockerON'); 
     }
     else
     {
       document.getElementById("Locker_button").value = "Closing the locker";
       ajaxLoad('LockerOFF');
     }
    }
 
var ajaxRequest = null;
if (window.XMLHttpRequest)  { ajaxRequest =new XMLHttpRequest(); }
else                        { ajaxRequest =new ActiveXObject("Microsoft.XMLHTTP"); }
 
 
function ajaxLoad(ajaxURL)
{
  if(!ajaxRequest){ alert("AJAX is not supported."); return; }
 
  ajaxRequest.open("GET",ajaxURL,true);
  ajaxRequest.onreadystatechange = function()
  {
    if(ajaxRequest.readyState == 4 && ajaxRequest.status==200)
    {
      var ajaxResult = ajaxRequest.responseText;
      if      ( ajaxResult == "The locker is open" )   { document.getElementById("Locker_button").value = "Close the locker"; }
      else if ( ajaxResult == "The locker is closed" )  { document.getElementById("Locker_button").value = "Open the locker"; }
      document.getElementById("reply").innerHTML = ajaxResult;
    }
  }
  ajaxRequest.send();
} 
</script> 
 <title>ESP8266-based smart locker</title>
</head>
<body>
  <div class="buttons">
  <div class="container">
      <h1>ESP8266-based smart locker</h1>
      <p>Developed by Maxim Bortnikov</p>
            <p id = "reply">Press the button to get a response from ESP</p>
            <button class="noselect">
            <input type="button" id = "Locker_button" onclick="switchLocker()" value="Open the locker"  /></button>
  </div>
</div>
</body>
</html>
 
)=====";
 
 
 
#include <ESP8266WiFi.h>
 
#include <ESP8266WiFi.h>       // Include ESP8266WiFi library
#include <Servo.h>             // Include Servo library
#include <SPI.h>               // Include SPI library
#include <Adafruit_GFX.h>      // Include Adafruit graphics library
#include <Adafruit_PCD8544.h>  // Include Adafruit PCD8544 (Nokia 5110) library
Adafruit_PCD8544 display = Adafruit_PCD8544(D4, D3, D2, D1, D0);
Servo servo;
const char* ssid     = "Blackberry";  // Your SSID (Network's name)
const char* password = "seasidehills32r12n"; // Password from your network
WiFiServer server(80);
int opan = 110; // Servo's angle for the open locker
int clan = 0; // Servo's angle for the closed locker
String request = "";
 
void setup() 
{
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
 
 
 
void loop() 
{
    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client)  {  return;  }
 
    // Read the first line of the request
    request = client.readStringUntil('\r');
 
    Serial.print("request: "); Serial.println(request); 
 
    if       ( request.indexOf("LockerON") > 0 )  
             { 
    servo.write(opan);
    client.print( header );
    client.print( "The locker is open" ); 
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
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
    else if  ( request.indexOf("LockerOFF") > 0 ) 
             { 
    servo.write(clan);
    client.print( header );
    client.print( "The locker is closed" );
    digitalWrite(12, LOW);
    digitalWrite(13, HIGH);
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
    else
    {
        client.flush();
        client.print( header );
        client.print( html_1 );    
        delay(5);
    }
 
 
  // The client will actually be disconnected when the function returns and 'client' object is detroyed
}

