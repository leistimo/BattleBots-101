#include <Bluepad32.h>

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

ControllerPtr myControllers[BP32_MAX_CONTROLLERS];

// Your Android gamepad MAC address
const char* TARGET_MAC = "03:11:34:24:2D:29";

// Arduino setup function
void setup() {

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

  display.println("Message: Enter Values");//loading data prior to display
  display.display();             //Update the OLED display 
//}
  while (!Serial) { ; } // wait for serial

  Serial.print("Firmware version: ");
  Serial.println(BP32.firmwareVersion());

  // BP32 setup with callbacks
  BP32.setup(&onConnectedController, &onDisconnectedController);

  // Optionally forget old keys to prevent reconnect issues
  BP32.forgetBluetoothKeys();

  Serial.println("Setup complete, waiting for controller...");
}

// Callback when a controller connects
void onConnectedController(ControllerPtr ctl) {
  // Filter for your specific MAC
  char macStr[18];
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X",
          ctl->getProperties().btaddr[0], ctl->getProperties().btaddr[1],
          ctl->getProperties().btaddr[2], ctl->getProperties().btaddr[3],
          ctl->getProperties().btaddr[4], ctl->getProperties().btaddr[5]);
  
  if (strcmp(macStr, TARGET_MAC) != 0) {
    Serial.print("Ignoring controller with MAC: ");
    Serial.println(macStr);
    return; // ignore other controllers
  }

  for (int i = 0; i < BP32_MAX_CONTROLLERS; i++) {
    if (myControllers[i] == nullptr) {
      myControllers[i] = ctl;
      Serial.print("Controller connected at index ");
      Serial.println(i);
      Serial.print("MAC: "); Serial.println(macStr);
      break;
    }
  }
}

// Callback when a controller disconnects
void onDisconnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_CONTROLLERS; i++) {
    if (myControllers[i] == ctl) {
      Serial.print("Controller disconnected from index ");
      Serial.println(i);
      myControllers[i] = nullptr;
      break;
    }
  }
}

// Process gamepad input
void processGamepad(ControllerPtr gamepad) {
  
      Serial.print("Buttons: 0x");
      Serial.println(gamepad->buttons(), HEX);

      if (gamepad->buttons() & 0x80) {
        Serial.println("Right Trigger");
      }

      if (gamepad->buttons() & 0x40) {
        Serial.println("Left Trigger");
      }

      if (gamepad->buttons() & 0x20) {
        Serial.println("Right Bumper");
      }

      if (gamepad->buttons() & 0x10) {
        Serial.println("Left Bumper");
      }

      if (gamepad->buttons() & 0x1) {
        Serial.println("A");
      }

      if (gamepad->buttons() & 0x2) {
        Serial.println("B");
      }

      if (gamepad->buttons() & 0x4) {
        Serial.println("X");
      }

      if (gamepad->buttons() & 0x8) {
        Serial.println("Y");
      }

      if (gamepad->buttons() & 0x200) {
        Serial.println("R_Joy Press");
      }

      if (gamepad->buttons() & 0x100) {
        Serial.println("L_Joy Press");
      }

  // Print DPAD
      //Serial.print("DPad: 0x");
      //Serial.print(gamepad->dpad(), HEX);

  // Print joystick axes
      //Serial.print("Left Stick: X=");
      //Serial.print(gamepad->axisX());
      //Serial.print(" Y=");
  //Serial.print(gamepad->axisY());



      //Serial.print("Right Stick: X=");
      //Serial.print(gamepad->axisRX());
      //Serial.print(" Y=");
  //Serial.print(gamepad->axisRY());

   
   // L_joystick = map(gamepad->axisY(), -512, 508, 100, -100);
        
   // R_joystick = map(gamepad->axisRY(), -512, 508, 100, -100);

   // if (abs(L_joystick) <= 20) {
   //   L_joystick = 0;
   // }

   // if (abs(R_joystick) <= 20) {
   //   R_joystick = 0;
   // }

   // if (L_joystick>0){
   //   Serial.print("Left Forward");
   // }
   // else if (L_joystick<0) {
   //   Serial.print("Left Reverse");
   // }
    Serial.println("-------------------------");


  // Print misc buttons (triggers, special buttons)
 // Serial.print("Misc Buttons: 0x");
 // Serial.print(gamepad->miscButtons(), HEX);

  // Optionally print battery and gyro/accel
 // Serial.print("Battery: ");
 // Serial.print(gamepad->battery());

 // Serial.print("Gyro: X=");
 // Serial.print(gamepad->gyroX());
 // Serial.print(" Y=");
 // Serial.print(gamepad->gyroY());
 // Serial.print(" Z=");
 // Serial.print(gamepad->gyroZ());

 // Serial.print("Accel: X=");
 // Serial.print(gamepad->accelX());
 // Serial.print(" Y=");
 // Serial.print(gamepad->accelY());
 // Serial.print(" Z=");
 // Serial.print(gamepad->accelZ());

 
}

// Arduino loop
void loop() {
  BP32.update();

  for (int i = 0; i < BP32_MAX_CONTROLLERS; i++) {
    ControllerPtr ctl = myControllers[i];
    if (ctl && ctl->isConnected() && ctl->isGamepad()) {
      processGamepad(ctl);
    }
  }

  delay(150);
}