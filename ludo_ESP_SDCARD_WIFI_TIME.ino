

/*
 Branchement ludo ESP32
 * A REFAIRE !!
 *
 *5-cs
 *18-sck
 *19-miso
 *23-mosi      -
 */

#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <WiFi.h>
#include <Wire.h>
#include "RTClib.h"

#include <ArduinoJson.h>

// CONNEXION INTERNET
#include <HTTPClient.h>
#include "sdcard_functions.h"
#include "initWifi.h"

#include <OneWire.h>
#include <DallasTemperature.h>

// Data DS18B20 temperature
#define ONE_WIRE_BUS 14
String dataPiscine;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;

float tempC;

// DATA pt100
float pt100;
#include <Adafruit_MAX31865.h>

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31865 max1 = Adafruit_MAX31865(33, 19, 23, 18);
// Adafruit_MAX31865 max1 = Adafruit_MAX31865(33);
// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF 430.0

#define RNOMINAL 100.0

File myFile;

RTC_DS3231 rtc;

void setup()
{
    Serial.begin(115200);

    max1.begin(MAX31865_3WIRE); // set to 2WIRE or 4WIRE as necessary
    // put your setup code here, to run once:
    delay(1000);
    sensors.begin();

    if (!sensors.getAddress(insideThermometer, 0))
        Serial.println("Unable to find address for Device 0");
    sensors.setResolution(insideThermometer, 12);

    delay(2000);
    // dataPiscine = String(printTemperature(insideThermometer));
    pt100 = moyenne_pt100(10);
    Serial.println("pt100 boudiou");
    Serial.println("pt100 boudiou");
    Serial.println("pt100 boudiou");
    Serial.println("pt100 boudiou");
    Serial.println(pt100);
    delay(500);
    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        while (1)
            ;
    }

    if (rtc.lostPower())
    {
        Serial.println("RTC lost power, lets set the time!");
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }
    // delay(1000); // Delay needed before calling the WiFi.begin
    // init_wifi();
    delay(500);

    if (!SD.begin())
    {
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        return;
    }
}

void loop()
{

    // sonde piscine
    delay(2000);
    // dataPiscine = String(printTemperature(insideThermometer));

    // pt100

    // uint16_t rtd = max1.readRTD();

    // float ratio = rtd;
    // ratio /= 32768;

    // Check and print any faults
    uint8_t fault = max1.readFault();
    if (fault)
    {
        Serial.print("Fault 0x");
        Serial.println(fault, HEX);
        if (fault & MAX31865_FAULT_HIGHTHRESH)
        {
            Serial.println("RTD High Threshold");
        }
        if (fault & MAX31865_FAULT_LOWTHRESH)
        {
            Serial.println("RTD Low Threshold");
        }
        if (fault & MAX31865_FAULT_REFINLOW)
        {
            Serial.println("REFIN- > 0.85 x Bias");
        }
        if (fault & MAX31865_FAULT_REFINHIGH)
        {
            Serial.println("REFIN- < 0.85 x Bias - FORCE- open");
        }
        if (fault & MAX31865_FAULT_RTDINLOW)
        {
            Serial.println("RTDIN- < 0.85 x Bias - FORCE- open");
        }
        if (fault & MAX31865_FAULT_OVUV)
        {
            Serial.println("Under/Over voltage");
        }
        max1.clearFault();
    }
    //  Serial.println();
    delay(50);

    DateTime now = rtc.now();
    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.print(" ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    Serial.println(now.unixtime(), DEC);
    time_t timestamp = now.unixtime();
    delay(1000);
    // sendData(timestamp);
    DynamicJsonDocument doc(2048);

    // StaticJsonObject allocates memory on the stack, it can be
    // replaced by DynamicJsonDocument which allocates in the heap.
    //
    // DynamicJsonDocument  doc(200);

    // Add values in the document
    //
    // doc["sensor"] = "gps";
    doc["time"] = timestamp;
    doc["pt100"] = pt100;
    doc["ds18b20"] = String(printTemperature(insideThermometer));

    // Add an array.
    //
    // JsonArray data = doc.createNestedArray("data");
    // data.add(48.756080);
    // data.add(21.123);

    // Serialize JSON document
    String json;
    serializeJson(doc, json);
    // Start a new line
    Serial.println();
    Serial.println();

    Serial.println(json);

    myFile = SD.open("/dataEsp32.txt", FILE_APPEND);

    // if the file opened okay, write to it:
    if (myFile)
    {
        myFile.print(json);
        myFile.print(";");
        // myFile.println(random(300));
        // // close the file:
        myFile.close();
        Serial.println("done.");
    }
    else
    {
        // if the file didn't open, print an error:
        Serial.println("error opening test.txt");
    }
    delay(5000);

    readFile(SD, "/dataEsp32.txt");
    // // deleteFile(SD, "/foo.txt");
}

// function to print the temperature for a device
float printTemperature(DeviceAddress deviceAddress)
{
    sensors.requestTemperatures(); // Send the command to get temperatures
    float tempC = sensors.getTempC(deviceAddress);
    delay(1000);
    if (tempC > -10 && tempC < 100)
    {
        //  Serial.println(tempC,1);
        tempC = tempC * tempC * 0.001831 + 0.882817 * tempC + 2.307283;
        Serial.print("Temp corrigé = ");
        Serial.println(tempC, 1);
    }
    else
    {

        Serial.println("bug");
    }
    return tempC;
}

float moyenne_pt100(int nbr_valeur)
{
    float total = 0;
    for (int i = 1; i <= nbr_valeur; i++)
    {

        total = total + max1.temperature(RNOMINAL, RREF);
        delay(10);
    }

    total = total / nbr_valeur;
    Serial.print("pt100 = ");
    Serial.println(total, 3);
    return total;
}