#include <Bluepad32.h>

ControllerPtr myControllers[BP32_MAX_CONTROLLERS];

// Your gamepad MAC address (const char* TARGET_MAC = "MAC address";)
const char* TARGET_MAC = "03:11:34:24:2D:29";

// Helper: Convert controller MAC to string
void macToStr(const uint8_t btaddr[6], char* out) {
  sprintf(out, "%02X:%02X:%02X:%02X:%02X:%02X",
          btaddr[0], btaddr[1], btaddr[2],
          btaddr[3], btaddr[4], btaddr[5]);
}

// Arduino setup
void setup() {
  
  Serial.begin(115200);
  while (!Serial) { ; }

  Serial.print("Firmware version: ");
  Serial.println(BP32.firmwareVersion());

  // Setup Bluepad32
  BP32.setup(&onConnectedController, &onDisconnectedController);

  // Forget previous pairings (good cleanup)
  BP32.forgetBluetoothKeys();

  Serial.println("Setup complete, waiting for controller...");
}

// Callback when a controller connects
void onConnectedController(ControllerPtr ctl) {
  char macStr[18];
  macToStr(ctl->getProperties().btaddr, macStr);

  // Reject everything except our target MAC
  if (strcmp(macStr, TARGET_MAC) != 0) {
    Serial.print("Rejected foreign controller with MAC: ");
    Serial.println(macStr);

    // Disconnect unwanted device right away
    ctl->disconnect();
    return;
  }

  // Save into slot
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

 
// Main loop
void loop() {
  BP32.update();

  delay(150);
}