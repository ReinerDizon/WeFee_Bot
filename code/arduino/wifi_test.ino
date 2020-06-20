#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "52f3c4e2349b42c68b94579fd313bf1e";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "WefeeWifi";
char pass[] = "WefeeBot";

BLYNK_WRITE(V1){
  // variable declarations
  int x = param[0].asInt();		// x direction value
  int y = param[1].asInt();		// y direction value

  // turning data into percentages
  x /= 100;
  y /= 100;
  
  // left & right wheel percentage calculations
  x = -x;
  int v = (100-abs(x)) * y/100 + y;
  int w = (100-abs(y)) * x/100 + x;
  int r = (v+w) / 2;
  int l = (v-w) / 2;
  
  // sending joystick data onto Atmega328p
  Serial.write(l);
  Serial.write('l');
  Serial.write(r);
  Serial.write('r');
}

void setup(){
  Serial.begin(9600);				// setup UART
  Blynk.begin(auth, ssid, pass);	// setup Wi-Fi authentication
  Serial.write('<');				// send '<' for read ready
}

void loop(){
  Blynk.run();	// run Blynk
}
