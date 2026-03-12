#include <Bluepad32.h>

GamepadPtr myGamepad;

void onConnectedGamepad(GamepadPtr gp) {
    Serial.println("Gamepad connected!");
    myGamepad = gp;
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

        Serial.print("LX: ");
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