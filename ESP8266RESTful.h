#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

class ESP8266RESTful
{

public:
  /* Initialize host */
  ESP8266RESTful();
  ESP8266RESTful(const char *host);

  /* Wifi setup */
  int connect(const char *ssid, const char *pass);

  /* Set a host */
  void setHost(const char *host);

  /* Set a request header */
  void setHeader(const char *key, const char *val);

  /* Set to use a secure connection */
  void setSecureConnection(bool secureConn);

  /* Set the fingerprint of SSL certificate to use secure connection */
  void setFingerprint(const char *fingerPrint);

  /* Get response status code */
  int getStatusCode();

  /* Get response payload */
  String getResponse();

  /* Get response payload */
  String getErrorMessage();

  /* Do Request */
  int request(const char *method, const char *path, const String &body);

  /* GET request */
  int get(const char *path);

  /* POST request */
  int post(const char *path, const String &body);

  /* PUT request */
  int put(const char *path, const String &body);

  /* PATCH request */
  int patch(const char *path, const String &body);

  /* DELETE request */
  int del(const char *path);
  /* DELETE request with body */
  int del(const char *path, const String &body);

private:
  WiFiClient client;
  WiFiClientSecure client_s;
  HTTPClient http;
  const char *host;
  bool is_secure;
  int statusCode;
  String response;
  String error_message;
};
