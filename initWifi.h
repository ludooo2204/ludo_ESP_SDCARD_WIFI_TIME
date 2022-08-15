const char *ssid = "Livebox-0D14";
const char *password = "14372CA5442C0671CF1A21511F";

//********************fonctions WIFI********************
void init_wifi()
{
    WiFi.begin(ssid, password);
    delay(5000);
    while (WiFi.status() != WL_CONNECTED)
    { // Check for the connection
        delay(3000);
        Serial.println("Connecting to WiFi..");
        // ++tentativeWifiCount;
        // if (tentativeWifiCount > 10)
        // {
        //     // esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
        //     // Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
        //     //    " Seconds");

        //     // Serial.println("Going to sleep now");
        //     Serial.flush();
        //     // esp_deep_sleep_start();
        //     // Serial.println("This will never be printed");
        // }
    }
    Serial.println("Connected to the WiFi network");
}

void sendData(time_t timestamp)
{
    Serial.print("voici les timestamp");
    Serial.println(timestamp);
    if (WiFi.status() == WL_CONNECTED)
    { // Check WiFi connection status

        HTTPClient http;
        String dataMeteo;

        // Prepare JSON document
        DynamicJsonDocument doc(2048);

        // StaticJsonObject allocates memory on the stack, it can be
        // replaced by DynamicJsonDocument which allocates in the heap.
        //
        // DynamicJsonDocument  doc(200);

        // Add values in the document
        //
        doc["sensor"] = "gps";
        doc["time"] = 1351824120;

        // Add an array.
        //
        JsonArray data = doc.createNestedArray("data");
        data.add(48.756080);
        data.add(21.123);

        // Serialize JSON document
        String json;
        serializeJson(doc, json);
        // Start a new line
        Serial.println();
        Serial.println();

        Serial.println(json);
        // The above line prints:
        // {
        //   "sensor": "gps",
        //   "time": 1351824120,
        //   "data": [
        //     48.756080,
        //     2.302038
        //   ]
        // }
        // Serial.println("test json");
        // Serial.println(serializeJson(doc, Serial));

        http.begin("https://lomano.fr/apiLudo/esp32"); // Specify destination for HTTP request
        // http.begin("http://192.168.1.20:7000/toto");  // Specify destination for HTTP request
        http.addHeader("Content-Type", "text/plain"); // Specify content-type header

        int httpResponseCode = http.POST(json); // Send the actual POST request
        // int httpResponseCode2 = http.POST(dataMeteo2);   //Send the actual POST request
        if (httpResponseCode > 0)
        {

            String response = http.getString(); // Get the response to the request

            Serial.println(httpResponseCode); // Print return code
            Serial.println(response);         // Print request answer
        }
        else
        {

            Serial.print("Error on sending POST: ");
            Serial.println(httpResponseCode);
        }

        http.end(); // Free resources
    }
    else
    {

        Serial.println("Error in WiFi connection");
    }

    delay(10000); // Send a request every 10 seconds
}