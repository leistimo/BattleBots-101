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
int ControllerConnected = 0;
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

  //display.println("Controller...");//loading data prior to display
  //display.display();             //Update the OLED display 
  SHOW_MSG("Controller...");
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
      SHOW_MSG("Connected");
      ControllerConnected=1;
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
      SHOW_MSG("Disconnected");
      ControllerConnected=0;
      break;
    }
  }
}

void SHOW_MSG(const char* msg) {
    Serial.println(msg);
    display.clearDisplay();
    display.setCursor(0,0);
    display.println(msg);
    display.display();
    ButtonPress = 1;
}

void JoyStick_MSG(const char* label, int value) {

  //Serial.print(label);
  //Serial.println(value);

  display.clearDisplay();
  display.setCursor(0,0);
  display.print(label);
  display.println(value);
  display.display();

  ButtonPress = 1;
}

// Process gamepad input
void processGamepad(ControllerPtr gamepad) {
  
      Serial.print("Buttons: 0x");
      Serial.println(gamepad->buttons(), HEX);

      if (gamepad->buttons() & 0x80) {
        SHOW_MSG("Right Trigger");
        }

      if (gamepad->buttons() & 0x40) {
        SHOW_MSG("Left Trigger");   
        }

      if (gamepad->buttons() & 0x20) {
        SHOW_MSG("Right Bumper");
        }

      if (gamepad->buttons() & 0x10) {
        SHOW_MSG("Left Bumper");
        }

      if (gamepad->buttons() & 0x1) {
        SHOW_MSG("A button");
        }

      if (gamepad->buttons() & 0x2) {
        SHOW_MSG("B button");
        }

      if (gamepad->buttons() & 0x4) {
        SHOW_MSG("X button");
        }

      if (gamepad->buttons() & 0x8) {
        SHOW_MSG("Y button");
        }

      if (gamepad->buttons() & 0x200) {
        SHOW_MSG("R_Joy Press");
        }

      if (gamepad->buttons() & 0x100) {
        SHOW_MSG("L_Joy Press");
        }

  // Print DPAD
    Serial.print("DPad: 0x");
    Serial.print(gamepad->dpad(), HEX);

    if (gamepad->dpad() & 0x2) {
          SHOW_MSG("Dpad Down");
        }

    if (gamepad->dpad() & 0x4) {
          SHOW_MSG("Dpad Right");
        }

    if (gamepad->dpad() & 0x1) {
        SHOW_MSG("Dpad Up");
        }

    if (gamepad->dpad() & 0x8) {
        SHOW_MSG("Dpad Left");
        }

    //Print misc buttons (triggers, special buttons)
    Serial.print("Misc Buttons: 0x");
    Serial.print(gamepad->miscButtons(), HEX);

    if (gamepad->miscButtons() & 0x4) {
        SHOW_MSG("Start button");
        }

    if (gamepad->miscButtons() & 0x2) {
      SHOW_MSG("Select button");
        }


  //Print joystick axes
    Serial.print("Left Stick: X=");
    Serial.print(gamepad->axisX());

    if (abs(gamepad->axisX()) > 1) {
          JoyStick_MSG("Left Joy X = ", gamepad->axisX());
        }

    Serial.print("Left Stick: Y=");
    Serial.print(gamepad->axisY());

    if (abs(gamepad->axisY()) > 1) {
          JoyStick_MSG("Left Joy Y = ", gamepad->axisY());
        }

    Serial.print("Right Stick: X=");
    Serial.print(gamepad->axisRX());

    if (abs(gamepad->axisRX()) > 1) {
          JoyStick_MSG("Right Joy X = ", gamepad->axisRX());
        }

    Serial.print("Right Stick: Y=");
    Serial.print(gamepad->axisRY());

    if (abs(gamepad->axisRY()) > 1) {
          JoyStick_MSG("Right Joy Y = ", gamepad->axisRY());
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
  if (ControllerConnected==1) {
  
  if (ButtonPress==0) {
    SHOW_MSG("Press Button");
    } }

  ButtonPress = 0;
  
}