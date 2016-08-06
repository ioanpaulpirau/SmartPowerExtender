/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "JPHomeCJ_WiFiGuest"
#define WLAN_PASS       "kilimanjaro"

/************************* Adafruit.io Setup *********************************/

#define SERVER       "jppcj.go.ro"
#define SERVER_PORT  1883                   // use 8883 for SSL
#define DEVICE_UID   "proto"                // device unique identifier
#define DEVICE_KEY   ""

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiClientSecure for SSL
//WiFiClientSecure client;

// Store the MQTT server, username, and password in flash memory. This is required for using the Adafruit MQTT library.
const char MQTT_SERVER[] PROGMEM    = SERVER;
const char MQTT_USERNAME[] PROGMEM  = DEVICE_UID;
const char MQTT_PASSWORD[] PROGMEM  = DEVICE_KEY;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, SERVER_PORT, MQTT_USERNAME, MQTT_PASSWORD);

/****************************** Feeds ***************************************/

// Notice MQTT paths follow the form: <username>/feeds/<feedname>

// Setup a feed called 'SPEPrototype' for publishing.
const char TEST_PUBLISH_FEED[] PROGMEM = DEVICE_UID "/feeds/test";
Adafruit_MQTT_Publish publisher = Adafruit_MQTT_Publish(&mqtt, TEST_PUBLISH_FEED);

// Setup a feed called 'onoff' for subscribing to changes.
const char TEST_SUBSCRIBE_FEED[] PROGMEM = DEVICE_UID "/feeds/onoff";
Adafruit_MQTT_Subscribe subscriber = Adafruit_MQTT_Subscribe(&mqtt, TEST_SUBSCRIBE_FEED);

/*************************** Helper Functions ************************************/

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}

// Get the time from a ping, See more at: http://www.esp8266.com/viewtopic.php?f=29&t=6007&start=5#sthash.zBe2qWFY.dpuf
String getTime() {
  WiFiClient client;
  while (!!!client.connect("google.com", 80)) {
    Serial.println("connection failed, retrying...");
  }

  client.print("HEAD / HTTP/1.1\r\n\r\n");
 
  while(!!!client.available()) {
     yield();
  }

  while(client.available()){
    if (client.read() == '\n') {    
      if (client.read() == 'D') {    
        if (client.read() == 'a') {    
          if (client.read() == 't') {    
            if (client.read() == 'e') {    
              if (client.read() == ':') {    
                client.read();
                String theDate = client.readStringUntil('\r');
                client.stop();
                return theDate;
              }
            }
          }
        }
      }
    }
  }
} 

/*************************** Sketch Setup and Looper Code ************************************/

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&subscriber);
}

uint32_t x=0;
char text_to_send[100];
char converter[10];

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &subscriber) {
      Serial.print(F("Got: "));
      Serial.println((char *)subscriber.lastread);
    }
  }

  strcpy(text_to_send, getTime().c_str());
  strcat(text_to_send, " - NodeMCU says hello for the ");
  strcat(text_to_send, itoa(x++, converter, 10));
  strcat(text_to_send, "'th time :)");

  // Now we can publish stuff!
  Serial.print(F("\nSending :"));
  Serial.print(text_to_send);
  Serial.print("...");
//  if (! photocell.publish(x++)) {
  if (! publisher.publish(text_to_send)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
}
