#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <ESP32Servo.h> //Servo ESC

Servo ESC1; //name servo ESC1

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

// Arduino setup function
void setup() {
  ESC1.attach(13); //Assign to GPIO13
  ESC1.writeMicroseconds(1000); //neutral ESC Startup PW

  Serial.begin(115200);
  Wire.begin();  // Initialize I2C communication
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  display.display();  // Display AdaFruit Logo
  delay(2000);        // Pause for 2 seconds
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setRotation(0);
  display.setCursor(0, 0);
  
  //display.println("Controller...");//loading data prior to display
  display.display();             //Update the OLED display 
  delay(2000);
 
}

void SHOW_VALUE(const char* label, int value) { 
    Serial.print(label);
    Serial.println(value);

    display.clearDisplay();
    display.setCursor(0,0);
    display.print(label);
    display.println(value);
    display.display();
}


  
// Arduino loop
void loop() {

  for (int pulse = 1000; pulse <= 1700; pulse += 5) {
    ESC1.writeMicroseconds(pulse);
    SHOW_VALUE("Pulse = ", pulse);
    delay(100);
}

for (int pulse = 1700; pulse >= 1000; pulse -= 5) {
    ESC1.writeMicroseconds(pulse);
    SHOW_VALUE("Pulse = ", pulse);
    delay(100);
}

  delay(250);
  
  
}