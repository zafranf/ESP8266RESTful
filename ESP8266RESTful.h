#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

class ESP8266RESTful
{

public:
  /* Initialize host */
  ESP8266RESTful(const char *host);

  /* Wifi Setup */
  int begin(const char *ssid, const char *pass);

  /* Do Request */
  int request(const char *method, const char *path, const char *body);

  /* Set a Request Header */
  void setHeader(const char *key, const char *val);

  /* Set to use a secure connection */
  void setSecureConnection(bool secureConn);

  /* Set the fingerprint of SSL certificate to use secure connection */
  void setFingerprint(const char *fingerPrint);

  /* GET request */
  int get(const char *path);

  /* POST request */
  int post(const char *path, const char *body);

  /* PUT request */
  int put(const char *path, const char *body);

  /* PATCH request */
  int patch(const char *path, const char *body);

  /* DELETE request */
  int del(const char *path);
  /* DELETE request with body */
  int del(const char *path, const char *body);

  /* Get response status code */
  int getStatusCode();

  /* Get response payload */
  String getResponse();

private:
  WiFiClient client;
  WiFiClientSecure client_s;
  HTTPClient http;
  const char *host;
  bool is_secure;
  int statusCode;
  String response;
};
