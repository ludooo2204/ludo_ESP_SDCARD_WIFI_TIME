#include <ArduinoJson.h>

String getJson(time_t timestamp, float pt100, float ds18b20)
{

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
    doc["ds18b20"] = ds18b20;

    // Add an array.
    //
    // JsonArray data = doc.createNestedArray("data");
    // data.add(48.756080);
    // data.add(21.123);

    // Serialize JSON document
    String json;
    serializeJson(doc, json);

    Serial.println(json);
    return json;
}