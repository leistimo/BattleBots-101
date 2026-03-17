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

int ButtonPress = 0;
//int R_joystick = 0;


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

  display.println("Press Buttons...");//loading data prior to display
  display.display();             //Update the OLED display 

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
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Right Trigger");//loading data prior to display
        display.display();             //Update the OLED display 
        ButtonPress = 1;
        
      }

      if (gamepad->buttons() & 0x40) {
        Serial.println("Left Trigger");
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Left Trigger");//loading data prior to display
        display.display();             //Update the OLED display 
        ButtonPress = 1;
      }

      if (gamepad->buttons() & 0x20) {
        Serial.println("Right Bumper");
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Right Bumper");//loading data prior to display
        display.display();             //Update the OLED display 
        ButtonPress = 1;
      }

      if (gamepad->buttons() & 0x10) {
        Serial.println("Left Bumper");
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Left Bumper");//loading data prior to display
        display.display();             //Update the OLED display 
        ButtonPress = 1;
      }

      if (gamepad->buttons() & 0x1) {
        Serial.println("A");
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("A button");//loading data prior to display
        display.display();             //Update the OLED display 
        ButtonPress = 1;
      }

      if (gamepad->buttons() & 0x2) {
        Serial.println("B");
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("B button");//loading data prior to display
        display.display();             //Update the OLED display 
        ButtonPress = 1;
      }

      if (gamepad->buttons() & 0x4) {
        Serial.println("X");
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("X button");//loading data prior to display
        display.display();             //Update the OLED display 
        ButtonPress = 1;
      }

      if (gamepad->buttons() & 0x8) {
        Serial.println("Y");
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Y button");//loading data prior to display
        display.display();             //Update the OLED display 
        ButtonPress = 1;
      }

      if (gamepad->buttons() & 0x200) {
        Serial.println("R_Joy Press");
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Right Joy Press");//loading data prior to display
        display.display();             //Update the OLED display 
        ButtonPress = 1;
      }

      if (gamepad->buttons() & 0x100) {
        Serial.println("L_Joy Press");
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Left Joy Press");//loading data prior to display
        display.display();             //Update the OLED display 
        ButtonPress = 1;
      }

  // Print DPAD
    Serial.print("DPad: 0x");
    Serial.print(gamepad->dpad(), HEX);

    if (gamepad->dpad() & 0x2) {
          Serial.println(" Down");
          display.clearDisplay();
          display.setCursor(0, 0);
          display.println("Dpad Down");//loading data prior to display
          display.display();             //Update the OLED display 
          ButtonPress = 1;
        }

    if (gamepad->dpad() & 0x4) {
          Serial.println(" Down");
          display.clearDisplay();
          display.setCursor(0, 0);
          display.println("Dpad Right");//loading data prior to display
          display.display();             //Update the OLED display 
          ButtonPress = 1;
        }

    if (gamepad->dpad() & 0x1) {
          Serial.println(" Up");
          display.clearDisplay();
          display.setCursor(0, 0);
          display.println("Dpad Up");//loading data prior to display
          display.display();             //Update the OLED display 
          ButtonPress = 1;
        }

    if (gamepad->dpad() & 0x8) {
          Serial.println(" Left");
          display.clearDisplay();
          display.setCursor(0, 0);
          display.println("Dpad Left");//loading data prior to display
          display.display();             //Update the OLED display 
          ButtonPress = 1;
        }

    //Print misc buttons (triggers, special buttons)
    Serial.print("Misc Buttons: 0x");
    Serial.print(gamepad->miscButtons(), HEX);

    if (gamepad->miscButtons() & 0x4) {
          Serial.println(" Start");
          display.clearDisplay();
          display.setCursor(0, 0);
          display.println("Start button");//loading data prior to display
          display.display();             //Update the OLED display 
          ButtonPress = 1;
        }

    if (gamepad->miscButtons() & 0x2) {
          Serial.println(" Select");
          display.clearDisplay();
          display.setCursor(0, 0);
          display.println("Select button");//loading data prior to display
          display.display();             //Update the OLED display 
          ButtonPress = 1;
        }


  //Print joystick axes
    Serial.print("Left Stick: X=");
    Serial.print(gamepad->axisX());

    if (abs(gamepad->axisX()) > 1) {
          //Serial.println(" Left Joy X");
          display.clearDisplay();
          display.setCursor(0, 0);
          display.print("Left Joy X = ");//loading data prior to display
          display.println(gamepad->axisX());
          display.display();             //Update the OLED display 
          ButtonPress = 1;
        }

    Serial.print("Left Stick: Y=");
    Serial.print(gamepad->axisY());

    if (abs(gamepad->axisY()) > 1) {
          //Serial.println(" Left Joy X");
          display.clearDisplay();
          display.setCursor(0, 0);
          display.print("Left Joy Y = ");//loading data prior to display
          display.println(gamepad->axisY());
          display.display();             //Update the OLED display 
          ButtonPress = 1;
        }

    //////
    Serial.print("Right Stick: X=");
    Serial.print(gamepad->axisRX());

    if (abs(gamepad->axisRX()) > 1) {
          //Serial.println(" Left Joy X");
          display.clearDisplay();
          display.setCursor(0, 0);
          display.print("Right Joy X = ");//loading data prior to display
          display.println(gamepad->axisRX());
          display.display();             //Update the OLED display 
          ButtonPress = 1;
        }

    Serial.print("Right Stick: Y=");
    Serial.print(gamepad->axisRY());

    if (abs(gamepad->axisRY()) > 1) {
          //Serial.println(" Left Joy X");
          display.clearDisplay();
          display.setCursor(0, 0);
          display.print("Right Joy Y = ");//loading data prior to display
          display.println(gamepad->axisRY());
          display.display();             //Update the OLED display 
          ButtonPress = 1;
        }    
      
    Serial.println("-------------------------");
    
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

  delay(250);
  
  if (ButtonPress==0) {
    
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Press Button");//loading data prior to display
    //display.println(gamepad->axisRY());
    display.display();             //Update the OLED display 
  }  
  ButtonPress = 0;
  
}