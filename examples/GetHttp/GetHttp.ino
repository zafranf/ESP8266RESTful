/* ESP8266RESTful Simple GET HTTP request
 *
 * by zafranf
 */

#include "ESP8266RESTful.h"

ESP8266RESTful REST("http://ngetes.com");

void setup()
{
  Serial.println();
  Serial.begin(9600);
  REST.begin("ssid", "password");
}

void loop()
{
  int statusCode = REST.get("/get");
  Serial.print("Status code from server: ");
  Serial.println(REST.getStatusCode());
  Serial.print("Response body from server: ");
  Serial.println(REST.getResponse());
  Serial.println();

  /* wait 10 seconds for next loop, don't flood server */
  delay(10000);
}
