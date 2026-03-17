#include <Bluepad32.h>

GamepadPtr myGamepad = nullptr;
char macStr[18];   // Global storage for MAC address

// Convert MAC to string
void macToStr(const uint8_t btaddr[6], char* out) {
  sprintf(out, "%02X:%02X:%02X:%02X:%02X:%02X",
          btaddr[0], btaddr[1], btaddr[2],
          btaddr[3], btaddr[4], btaddr[5]);
}

void onConnectedGamepad(GamepadPtr gp) {
    Serial.println("Gamepad connected!");

    myGamepad = gp;

    // Convert MAC to readable string
    macToStr(gp->getProperties().btaddr, macStr);

    Serial.print("Controller MAC: ");
    Serial.println(macStr);
}

void onDisconnectedGamepad(GamepadPtr gp) {
    Serial.println("Gamepad disconnected!");
    myGamepad = nullptr;
}

void setup() {
    Serial.begin(115200);
    Serial.println("Bluepad32 Debug Monitor");

    BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);
}

void loop() {

    BP32.update();

    if (myGamepad && myGamepad->isConnected()) {

        Serial.print("Connected to MAC: ");
        Serial.print(macStr);

        Serial.print("  LX: ");
        Serial.print(myGamepad->axisX());

        Serial.print("  LY: ");
        Serial.print(myGamepad->axisY());

        Serial.print("  RX: ");
        Serial.print(myGamepad->axisRX());

        Serial.print("  RY: ");
        Serial.print(myGamepad->axisRY());

        Serial.print("  Throttle: ");
        Serial.print(myGamepad->throttle());

        Serial.print("  Brake: ");
        Serial.print(myGamepad->brake());

        Serial.print("  Buttons: ");
        Serial.print(myGamepad->buttons(), BIN);

        Serial.print("  Dpad: ");
        Serial.print(myGamepad->dpad(), BIN);

        Serial.println();
    }

    delay(1000);
}