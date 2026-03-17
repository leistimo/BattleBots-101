#include <Bluepad32.h>

ControllerPtr myControllers[BP32_MAX_CONTROLLERS];

// Your Android gamepad MAC address
const char* TARGET_MAC = "03:11:34:24:2D:29";

// Arduino setup function
void setup() {


  Serial.begin(115200);
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
  
   // Print misc buttons (triggers, special buttons)
    Serial.print("Misc Buttons: 0x");
    Serial.print(gamepad->miscButtons(), HEX);

    if (gamepad->miscButtons() & 0x4) {
        Serial.println(" Start");
        }

    if (gamepad->miscButtons() & 0x2) {
        Serial.println(" Select");
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

  delay(150);
}