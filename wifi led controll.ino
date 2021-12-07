/*
 * Mish Mash Labs
 * September 2020
 * Webserver with PWM Control of Brightness of LED
*/

// Import libraries that are used
#include <ESP8266WiFi.h>
/*
 * You will need to download the .zip file for the following libraries from the links below
 * Install via Sketch > Include Library > Add .ZIP Library and browse to the download
 */
// https://github.com/me-no-dev/AsyncTCP
//#include <AsyncTCP.h>
// https://github.com/me-no-dev/ESPAsyncWebServer
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>


// Insert your own wifi network SSID and Password
char* ssid = "Hof Grueter";
char* password = "NicoMarl-#2";

const int ledRPin = 4;
const int ledGPin = 5;
const int ledBPin = 0;
boolean fadebutton;

String pwmSliderRValue = "0";
String pwmSliderGValue = "0";
String pwmSliderBValue = "0";
boolean fade = false;
int speed = 1;

// Set Desired PWM Settings
const int frequencyHz = 5000;
const int pwmChannelR = 4;
const int pwmChannelG = 5;
const int pwmChannelB = 0;

const int resolution = 8;

const char* INPUT_PARAMETER = "value";

// Instatiate the AsyncWebServer object on port 80
AsyncWebServer webServer(80);


  void setslidervalue(const char* sliderid){
      webServer.on(sliderid, HTTP_GET, [sliderid] (AsyncWebServerRequest *request) {
     String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(INPUT_PARAMETER)) {
      inputMessage = request->getParam(INPUT_PARAMETER)->value();
      fade = false;
    if(sliderid == "/pwmSliderRed"){      
      pwmSliderRValue = inputMessage;
      analogWrite(pwmChannelR, pwmSliderRValue.toInt());
    }else 
       if(sliderid == "/pwmSliderGreen"){
      pwmSliderGValue = inputMessage;
      analogWrite(pwmChannelG, pwmSliderGValue.toInt());
    }else
        if(sliderid == "/pwmSliderBlue"){
      pwmSliderBValue = inputMessage;
      analogWrite(pwmChannelB, pwmSliderBValue.toInt());
    }
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  }



void colorslider(const char* slider){
    webServer.on(slider, HTTP_GET, [slider] (AsyncWebServerRequest *request) {
    String inputMessage;
    if (request->hasParam(INPUT_PARAMETER)) {
     fade = false;
      inputMessage = request->getParam(INPUT_PARAMETER)->value();
      if(slider == "/allcolorsliderr"){
      pwmSliderRValue = inputMessage;
      analogWrite(pwmChannelR, pwmSliderRValue.toInt());
      }else
      if(slider == "/allcolorsliderg"){
      pwmSliderGValue = inputMessage;
      analogWrite(pwmChannelG, pwmSliderGValue.toInt());
      }else 
      if(slider == "/allcolorsliderb"){
      pwmSliderBValue = inputMessage;
      analogWrite(pwmChannelB, pwmSliderBValue.toInt());
      }

        }
            Serial.println(inputMessage);
    request->send(50, "text/plain", "OK");
  });
  }

// Declare the webpage
// HTML comments look like this <! comment in between here >
const char htmlCode[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html lang="de">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Gartenhaus</title>
    <link href="https://fonts.googleapis.com/css?family=Cardo:400,700|Oswald" rel="stylesheet">
 <style>
   html{
    font-family:oswald;
    background-color:#a4a86c7a; 

  }

  body{
    min-width: 100vh;
    min-height: 100vh;

  }
    .slider { -webkit-appearance: none; margin: 14px; width: 400px; height: 15px; border-radius: 5px; background: #f3f8d3; outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 25px; height: 25px; border-radius: 12px; background: #f74d4d; cursor: pointer;}
    .slider::-moz-range-thumb { width: 25px; height: 25px; border-radius: 12px; background: #c43434; cursor: pointer; } 

    .slidecontainer { margin: 20px;}
    #header{
      margin-left:70px;
      padding-left:15px;
      width:305px;
      border-radius: 30px;
      background-color: #71a0a7;
    }
    #colordisplay {      margin-top: -290px;            margin-left: 480px ;            height:300px;   border-radius: 50px;             width: 150px;          }
    #colordisplay p{   margin: 14px;  margin-left: 50px;    }
    #colorwheel{      margin-left: 14px;      margin-top: 20px;    }
    .colorslider{margin-left: 14px; width: 400px; height: 100px;}

    .buttons{margin-top: 60px;margin-left: 90px; height: 150px; width: 300px; }
    .buttons button{
  margin-left: 30px; 
	border-radius:50px;
	display:inline-block;
	cursor:pointer;
	color:#ffffff;
	font-family:Arial;
	font-size:17px;
	padding:16px 35px;
	text-decoration:none;
}
  .buttons #fadeon{
    background-color:#44c767;
    border:1px solid #18ab29;
    text-shadow:0px 1px 0px #2f6627;}

    .buttons #fadeoff{
    background-color:#c43434;
    border:1px solid #ab1818;
    text-shadow:0px 1px 0px #662727;}

     .buttons #fadeon:active{
    background-color:#07a52f ;
  }
  .buttons #fadeoff:active{
    background-color:#791f1f ;
  }
  .buttons h1{
    margin-left: 55px;
  }


        </style>


  </head>

  <body>

        <h1 id="header">Gartenhaus Beleuchtung</h1>
        <div class="slidecontainer">
        <p>Rot:<span id="Red" >0</span></p>
        <p><input type="range" onchange="updateslider(this.id,'Red')" id="pwmSliderRed" min="0" max="255" value="0" step="1" class="slider"></p>
        <p>Grün:<span id="Green" >0</span></p>
        <p><input type="range" onchange="updateslider(this.id,'Green')" id="pwmSliderGreen" min="0" max="255" value="0" step="1" class="slider"></p>
        <p>Blau:<span id="Blue" >0</span></p>
        <p><input type="range" onchange="updateslider(this.id,'Blue')" id="pwmSliderBlue" min="0" max="255" value="0" step="1" class="slider"></p>


        <div id="colordisplay" style="background-color: #000000;">
        <p>.</p>
        </div>

        <div id="colorwheel">
        <div class="colorpicker"></div>
        <div id="colorindicator"></div>
        <div id="colorpicker"></div>
        </div>

        <div class="buttons">
          <h1>Farbwechsel</h1>
        <button id="fadeon" onclick="updatebutton(this.id)">On</button>
        <button id="fadeoff" onclick="updatebutton(this.id)">Off</button>
      </div>

      <div class="slidecontainer">
        <p>Geschwindigkeit:<span id="speed" >1</span></p>
        <p><input type="range" onchange="updateslider(this.id,'speed')" id="fadeslider" min="1" max="10" value="1" step="1" class="slider"></p>
      </div>


       

      <script src="https://cdn.jsdelivr.net/npm/@jaames/iro@5">      </script>



        <script>
          launchFullScreen(document.documentElement); // the whole page

            let coloridicator = document.getElementById('colorindicator');
            var output = document.getElementById("colordisplay");
            
          const colorPicker = new iro.ColorPicker("#colorpicker",{
            width:400, color: "#fff"
          });
          colorPicker.on('color:change',function(color){
            coloridicator.style.backgroundColor = color.hexString;
            output.style.backgroundColor = color.hexString;
            document.getElementById("pwmSliderRed").value = hexToRgb(color.hexString).r; 
            document.getElementById("Red").innerHTML = hexToRgb(color.hexString).r; 
            document.getElementById("pwmSliderGreen").value = hexToRgb(color.hexString).g;
            document.getElementById("Green").innerHTML = hexToRgb(color.hexString).g; 
            document.getElementById("pwmSliderBlue").value = hexToRgb(color.hexString).b;
            document.getElementById("Blue").innerHTML = hexToRgb(color.hexString).b; 
            
            var httpRequest = new XMLHttpRequest();
            httpRequest.open("GET", "/allcolorsliderr?value="+hexToRgb(color.hexString).r, true);
            httpRequest.send();
            httpRequest = new XMLHttpRequest();
            httpRequest.open("GET", "/allcolorsliderg?value="+hexToRgb(color.hexString).g, true);
            httpRequest.send();
            httpRequest = new XMLHttpRequest();
            httpRequest.open("GET", "/allcolorsliderb?value="+hexToRgb(color.hexString).b, true);
            httpRequest.send();
            
          });


                  function launchFullScreen(element) {
          if(element.requestFullScreen) {
            element.requestFullScreen();
          } else if(element.mozRequestFullScreen) {
            element.mozRequestFullScreen();
          } else if(element.webkitRequestFullScreen) {
            element.webkitRequestFullScreen();
          }
           }

            function updateslider(sliderid,outputid){
            var slider = document.getElementById(sliderid);
            var output = document.getElementById(outputid);

            document.getElementById("colordisplay").style.backgroundColor = 
            "rgb("+document.getElementById("pwmSliderRed").value+", "+document.getElementById("pwmSliderGreen").value+","+document.getElementById("pwmSliderBlue").value+")"; 

            document.getElementById("colordisplay").style.color = 
            "rgb("+document.getElementById("pwmSliderRed").value+", "+document.getElementById("pwmSliderGreen").value+","+document.getElementById("pwmSliderBlue").value+")"; 

            output.innerHTML = slider.value;

            var httpRequest = new XMLHttpRequest();
            httpRequest.open("GET", "/"+sliderid+"?value="+ slider.value, true);
            httpRequest.send();

            slider.oninput = function(){

              document.getElementById("colordisplay").style.backgroundColor = 
            "rgb("+document.getElementById("pwmSliderRed").value+", "+document.getElementById("pwmSliderGreen").value+","+document.getElementById("pwmSliderBlue").value+")"; 
            document.getElementById("colordisplay").style.color = 
            "rgb("+document.getElementById("pwmSliderRed").value+", "+document.getElementById("pwmSliderGreen").value+","+document.getElementById("pwmSliderBlue").value+")"; 

            output.innerHTML = this.value;
            var httpRequest = new XMLHttpRequest();
            httpRequest.open("GET", "/"+sliderid+"?value="+ slider.value, true);
            httpRequest.send();

            }
        }

            function hexToRgb(hex) {
      var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
      return result ? {
        r: parseInt(result[1], 16),
        g: parseInt(result[2], 16),
        b: parseInt(result[3], 16)
      } : null;}

        function updatebutton(id){
          
          var httpRequest = new XMLHttpRequest();
            httpRequest.open("GET", "/fadebutton?value="+id, true);
            httpRequest.send();
        }

        </script>
      </div>
  </body>
</html>

)rawliteral";

// Replaces the placeholder with the button in your web page
String updateButton(const String& var){
  
  if (var == "SLIDERVALUE"){
    return pwmSliderRValue;
  }
  return String();
}

void setup(){


  // Begine Serial Communications over USB
  Serial.begin(115200);
  
  // configure LED PWM functionalitites
  // use ledcSetup(channel, frequency, resolution) to configure the PWM signal
  // ledcSetup(ESP PWM channel not GPIO pin, frequency in Hz, resolution (1-16 bit))
  pinMode(pwmChannelR,OUTPUT);
  pinMode(pwmChannelG,OUTPUT);
  pinMode(pwmChannelB,OUTPUT);
  
  analogWrite(pwmChannelR, pwmSliderRValue.toInt());
  analogWrite(pwmChannelG, pwmSliderGValue.toInt());
  analogWrite(pwmChannelB, pwmSliderBValue.toInt());


  // Connect to your Wi-Fi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print the IP Address of your device
  Serial.println(WiFi.localIP());

  // Detail the route for root / web page
  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", htmlCode, updateButton);
  });

    webServer.on("/fadebutton", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    if (request->hasParam(INPUT_PARAMETER)) {
      inputMessage = request->getParam(INPUT_PARAMETER)->value();
      }
    if(inputMessage == "fadeon"){  
    fade = true;
   }else
    if(inputMessage == "fadeoff"){
      fade = false;
     }
  });

      webServer.on("/fadeslider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    if (request->hasParam(INPUT_PARAMETER)) {
      inputMessage = request->getParam(INPUT_PARAMETER)->value();
            speed = inputMessage.toInt();
      }
  });



  setslidervalue("/pwmSliderRed");
  setslidervalue("/pwmSliderGreen");
  setslidervalue("/pwmSliderBlue");
  colorslider("/allcolorsliderr");
  colorslider("/allcolorsliderg");
  colorslider("/allcolorsliderb");


  // Start server (remembering its on port 80)
  webServer.begin();
}


void loop() {

  while(fade){

 int r, g, b;
 int FADESPEED =15;
  // fade from blue to violet
  for (r = 0; r < 256; r++) { 
    analogWrite(ledRPin, r);
    delay(FADESPEED/speed);
  } 
  // fade from violet to red
  for (b = 255; b > 0; b--) { 
    analogWrite(ledBPin, b);
    delay(FADESPEED/speed);
  } 
  // fade from red to yellow
  for (g = 0; g < 256; g++) { 
    analogWrite(ledGPin, g);
    delay(FADESPEED/speed);
  } 
  // fade from yellow to green
  for (r = 255; r > 0; r--) { 
    analogWrite(ledRPin, r);
    delay(FADESPEED/speed);
  } 
  // fade from green to teal
  for (b = 0; b < 256; b++) { 
    analogWrite(ledBPin, b);
    delay(FADESPEED/speed);
  } 
  // fade from teal to blue
  for (g = 255; g > 0; g--) { 
    analogWrite(ledGPin, g);
    delay(FADESPEED/speed);
  } 
   }
}