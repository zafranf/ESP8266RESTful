/* ESP8266RESTful Simple GET HTTPS request
 *
 * by zafranf
 */

#include "ESP8266RESTful.h"

ESP8266RESTful REST("https://ngetes.com");

void setup()
{
  Serial.println();
  Serial.begin(9600);
  REST.connect("ssid", "password");
  REST.setSecureConnection(true);
  REST.setFingerprint("C7 D1 21 64 EB 06 84 56 B9 BA 19 EB 69 74 53 76 1A AE BD 99");
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
