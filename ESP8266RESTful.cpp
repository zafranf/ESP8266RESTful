#include "ESP8266RESTful.h"

/* comment to hide log */
#define SHOW_LOG

#ifdef SHOW_LOG
#define LOG_PRINT(string) (Serial.print(string))
#define LOG_PRINTLN(string) (Serial.println(string))
#endif

#ifndef SHOW_LOG
#define LOG_PRINT(string)
#define LOG_PRINTLN(string)
#endif

ESP8266RESTful::ESP8266RESTful()
{
  header_num = 0;
  setSecureConnection(false);
}

ESP8266RESTful::ESP8266RESTful(const char *_host)
{
  host = _host;
  header_num = 0;
  setSecureConnection(false);
}

int ESP8266RESTful::connect(const char *ssid, const char *pass)
{
  WiFi.persistent(false);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  int countDelay = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    if (countDelay == 0)
    {
      LOG_PRINT("Connecting Wifi to SSID ");
      LOG_PRINTLN(WiFi.SSID().c_str());
    }
    delay(500);
    LOG_PRINT(".");
    countDelay++;
    if (countDelay > 10)
    {
      LOG_PRINTLN();
      countDelay = 0;
    }
  }

  LOG_PRINTLN();
  LOG_PRINT("Wifi connected to ");
  LOG_PRINTLN(WiFi.SSID().c_str());
  LOG_PRINT("IP address: ");
  LOG_PRINTLN(WiFi.localIP());

  return WiFi.status();
}

/* Set host */
void ESP8266RESTful::setHost(const char *_host)
{
  host = _host;
}

/* Set request header */
void ESP8266RESTful::setHeader(const char *key, const char *val)
{
  header_keys[header_num] = key;
  header_values[header_num] = val;
  header_num++;
}

/* Attach request header */
void ESP8266RESTful::attachHeaders()
{
  for (int i = 0; i < header_num; i++)
  {
    http.addHeader(header_keys[i], header_values[i]);
  }
}

/* Set use secure connection */
void ESP8266RESTful::setSecureConnection(bool secure)
{
  is_secure = secure;
}

/** 
 * Use web browser to view and copy SHA1 fingerprint of the certificate
 * Ex: CF 05 98 89 CA FF 8E D8 5E 5C E0 C2 E4 F7 E6 C3 C7 50 DD 5C
 */
void ESP8266RESTful::setFingerprint(const char *fingerPrint)
{
  if (is_secure)
  {
    client_s.setFingerprint(fingerPrint);
  }
}

/* ESP8266HTTPClient */
void ESP8266RESTful::setReuse(bool reuse)
{
  // http.setReuse(reuse);
  _reUse = reuse;
}
void ESP8266RESTful::setUserAgent(const String &userAgent)
{
  // http.setUserAgent(userAgent);
  _UserAgent = userAgent;
}
void ESP8266RESTful::setAuthorization(const char *user, const char *password)
{
  // http.setAuthorization(user, password);
  _user = user;
  _password = password;
}
void ESP8266RESTful::setAuthorization(const char *auth)
{
  // http.setAuthorization(auth);
  _auth = auth;
}
void ESP8266RESTful::setTimeout(uint16_t timeout)
{
  // http.setTimeout(timeout);
  _timeout = timeout;
}
void ESP8266RESTful::setFollowRedirects(bool follow)
{
  // http.setFollowRedirects(follow);
  _follow = follow;
}
void ESP8266RESTful::setRedirectLimit(uint16_t limit)
{
  // http.setRedirectLimit(limit);
  _limit = limit;
}
bool ESP8266RESTful::setURL(const String &url)
{
  // return http.setURL(url);
  _url = url;
}
void ESP8266RESTful::useHTTP10(bool useHTTP10)
{
  // http.useHTTP10(useHTTP10);
  _useHttp10 = useHTTP10;
}

/* Get response status code */
int ESP8266RESTful::getStatusCode()
{
  return statusCode;
}

/* Get response payload */
String ESP8266RESTful::getResponse()
{
  return response;
}

String ESP8266RESTful::getErrorMessage()
{
  return error_message;
}

/* Do the request */
int ESP8266RESTful::request(const char *method, String path, const String &body)
{
  String url = host;
  url += path;

  int httpCode = 0;
  String payload = "";
  String http_str = is_secure ? "HTTPS" : "HTTP";

  LOG_PRINTLN("[" + http_str + "] begin...");
  if (http.begin(is_secure ? client_s : client, url))
  {
    if (_reUse != NULL)
    {
      http.setReuse(_reUse);
    }
    if (_UserAgent != NULL)
    {
      http.setUserAgent(_UserAgent);
    }
    if (_user != NULL && _password != NULL)
    {
      http.setAuthorization(_user, _password);
    }
    if (_auth != NULL)
    {
      http.setAuthorization(_auth);
    }
    if (_timeout != NULL)
    {
      http.setTimeout(_timeout);
    }
    if (_follow != NULL)
    {
      http.setFollowRedirects(_follow);
    }
    if (_limit != NULL)
    {
      http.setRedirectLimit(_limit);
    }
    if (_url != NULL)
    {
      http.setURL(_url);
    }
    if (_useHttp10 != NULL)
    {
      http.useHTTP10(_useHttp10);
    }

    /* attach header values */
    if (header_num == 0 && method != "GET")
    {
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    }
    else
    {
      attachHeaders();
    }

    /* GET request */
    if (method == "GET")
    {
      LOG_PRINTLN("[" + http_str + "] GET data from " + url);
      httpCode = http.GET();
    }
    /* POST request */
    else if (method == "POST")
    {
      LOG_PRINTLN("[" + http_str + "] POST data to " + url);
      httpCode = http.POST(body);
    }
    /* PUT request */
    else if (method == "PUT")
    {
      LOG_PRINTLN("[" + http_str + "] PUT data to " + url);
      httpCode = http.PUT(body);
    }
    /* PATCH request */
    else if (method == "PATCH")
    {
      LOG_PRINTLN("[" + http_str + "] PATCH data to " + url);
      httpCode = http.PATCH(body);
    }
    /* DELETE request */
    else if (method == "DELETE")
    {
      LOG_PRINTLN("[" + http_str + "] DELETE data from " + url);
      // httpCode = http.DELETE(body);
      httpCode = http.sendRequest("DELETE", body);
    }

    // httpCode will be negative on error
    if (httpCode > 0)
    {
      // HTTP header has been send and Server response header has been handled
      LOG_PRINT("[" + http_str + "] Response code: ");
      LOG_PRINTLN(httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        payload = http.getString();
        LOG_PRINTLN("Response payload:");
        LOG_PRINTLN(payload);
      }
      else
      {
        payload = http.getString();
      }
    }
    else
    {
      error_message = http.errorToString(httpCode).c_str();
      LOG_PRINT("[" + http_str + "] Request failed, error: ");
      LOG_PRINTLN(error_message);
    }
    response = payload;
    statusCode = httpCode;
    http.end();
  }
  else
  {
    LOG_PRINTLN("[" + http_str + "] Unable to connect");
  }

  return httpCode;
}

/* GET request */
int ESP8266RESTful::get(String path)
{
  return request("GET", path, "");
}

/* POST request */
int ESP8266RESTful::post(String path, const String &body)
{
  return request("POST", path, body);
}

/* PUT request */
int ESP8266RESTful::put(String path, const String &body)
{
  return request("PUT", path, body);
}

/* PATCH request */
int ESP8266RESTful::patch(String path, const String &body)
{
  return request("PATCH", path, body);
}

/* DELETE request with body */
int ESP8266RESTful::del(String path, const String &body)
{
  return request("DELETE", path, body);
}