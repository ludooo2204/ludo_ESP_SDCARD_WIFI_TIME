

/*
 Branchement ludo ESP32
 * A REFAIRE !!
 *
 *5-cs
 *18-sck
 *19-miso
 *23-mosi      -
 */

#include "SPI.h"
// #include <WiFi.h>
#include <Wire.h>
// #include <HTTPClient.h>

// PERSO
#include "sdcard.h"
// #include "initWifi.h"
#include "DS18B20.h"
#include "PT100.h"
#include "json.h"
#include "clock.h"

void setup()
{
    Serial.begin(115200);

    delay(500);
    init_pt100();

    delay(500);
    init_ds18b20();

    // delay(1000); // Delay needed before calling the WiFi.begin
    // init_wifi();
}

void loop()
{

    delay(1000);

    String dataToSave = getJson(getTimestamp(), getPT100(), getDS18B20(insideThermometer));

    saveToSDcard(dataToSave);

    delay(5000);

    readFile(SD, "/dataEsp32.txt");
    // // deleteFile(SD, "/foo.txt");
}
