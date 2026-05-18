#include "RBCX.h"
#include <Arduino.h>
#include <string>

void clear() {
    rb::Manager::get().oled().fill(rb::Oled::Black);
    rb::Manager::get().oled().updateScreen();
}

void waitToNextTest() {
    delay(3000);
    clear();
}

void setup() {
    printf("RB3204-RBCX\n");

    delay(500);

    printf("Init manager\n");
    auto& man = rb::Manager::get();
    man.install();

    man.leds().red(true);

        auto& mpu = rb::Manager::get().mpu();

    mpu.init();
    mpu.sendStart();

    // 1. Počkáme chvíli, než začnou chodit první stabilní data ze senzoru
    delay(1000);

    // 2. Vymažeme stará kalibrační data (nastaví offset na 0)
    mpu.clearCalibrationData();
    delay(100); // Necháme do knihovny natéct čistá data (bez offsetu)

    // 3. Zkalibrujeme! Uloží aktuální odchylku (chybu) jako offset, který se bude nově odečítat.
    // DŮLEŽITÉ: ROBOT V TUTO CHVÍLI MUSÍ BÝT V ABSOLUTNÍM KLIDU!
    mpu.setCalibrationData();
    
    // 4. Vynulujeme celkový úhel osy Z, aby nám hezky začínal od nuly
    mpu.resetAngleZ();

    while (true) {
        printf("MPU - angle: X: %2.2f Y: %2.2f Z: %2.2f\n", mpu.getAngleX(), mpu.getAngleY(), mpu.getAngleZ());
        delay(100);
    }


}

void loop() {}