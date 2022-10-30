#include "SimplePortal.h"
static DNSServer _SP_dnsServer;
static ESP8266WebServer _SP_server(80);

const char SP_connect_page[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
<meta name="viewport" content="width=device-width, initial-scale=1">
</head><body>
<style type="text/css">
    input[type="text"] {margin-bottom:8px;font-size:20px;}
    input[type="submit"] {width:180px; height:60px;margin-bottom:8px;font-size:20px;}
</style>
<center>
<h3>WiFi settings</h3>
<form action="/connect" method="POST">
    <input type="text" name="ssid" placeholder="SSID">
    <input type="text" name="pass" placeholder="Pass">
    <input type="submit" value="Submit">
</form>
<h3>Switch WiFi mode</h3>
<form action="/ap" method="POST">
    <input type="submit" value="Access Point">
</form>
<form action="/local" method="POST">
    <input type="submit" value="Local Mode">
</form>
<form action="/exit" method="POST">
    <input type="submit" value="Exit Portal">
</form>
</center>
</body></html>)rawliteral";

static bool _SP_started = false;
static byte _SP_status = 0;
PortalCfg portalCfg;

void SP_handleConnect() {
  strcpy(portalCfg.SSID, _SP_server.arg("ssid").c_str());
  strcpy(portalCfg.pass, _SP_server.arg("pass").c_str());
  portalCfg.mode = WIFI_STA;
  _SP_status = 1;
}

void SP_handleAP() {
  portalCfg.mode = WIFI_AP;
  _SP_status = 2;
}

void SP_handleLocal() {
  portalCfg.mode = WIFI_STA;
  _SP_status = 3;
}

void SP_handleExit() {
  _SP_status = 4;
}

String SVGRSSI(int rssi){
	String tmp = F("<?xml version=\"1.0\" ?> <svg version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" style=\"width: 32px; height: 16px;\"> <g>");
	if(rssi >= -100) tmp += F("<rect x=\"1.775\" y=\"13.16\" width=\"4.131\" height=\"14.312\"/>"); //первая палочка
	if(rssi >= -85) tmp += F("<rect x=\"8.648\" y=\"9.895\" width=\"4.13\" height=\"17.578\"/>"); //вторая
	if(rssi >= -75) tmp += F("<rect x=\"15.133\" y=\"5.188\" width=\"4.129\" height=\"22.285\"/>"); //третья
	if(rssi >= -65) tmp += F("<rect x=\"21.567\" y=\"0\" width=\"4.13\" height=\"27.473\"/>"); //четвертая
	tmp += F("</g></svg>");
	return tmp;
}

void WebConfigurator(ESP8266WebServer *server, String SSID, String Password){
	if (!(*server).chunkedResponseModeStart(200, "text/html")) {
		(*server).send(505, F("text/html"), F("HTTP1.1 required"));
		return;
	}
	(*server).sendContent(F("<!Doctype html><html><head><title>Конфигурация модуля</title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><meta charset=\"UTF-8\"><script>function Change(d){ssid.value = d;}</script></head><body>"));
	
	(*server).sendContent(F("<center>"));
	(*server).sendContent(F("<h3>WiFi настройки</h3>"));
	#ifndef NoScanWIFi
	  (*server).sendContent(F("<h3>Доступные WIFI сети</h3>"));
	  int n = WiFi.scanComplete();
	  if(n > 0)
	  {
		for (int i = 0; i < n; ++i)
		{
		  (*server).sendContent(F("<p>"));
		  (*server).sendContent("<a href=\"#\" onclick=\"Change('"+String(WiFi.SSID(i))+"'); return false;\">"+String(WiFi.SSID(i))+""+SVGRSSI(WiFi.RSSI(i))+"</a>");
		  (*server).sendContent(F("</p>"));              
		}
		WiFi.scanDelete();
		WiFi.scanNetworks(true);
	  }
	  else if(n == 0){
		(*server).sendContent(F("<p>Сети WIFI не найдены</p>"));
		WiFi.scanDelete();
		WiFi.scanNetworks(true);
	  }			
	  else if(n == WIFI_SCAN_RUNNING)
		(*server).sendContent(F("<p>Сканирование не завершено, обновите страницу позже</p>")); 
	  else{
		WiFi.scanNetworks(true);
		(*server).sendContent(F("<p>Обновите страницу еще раз</p>"));
	  }
	#endif
	(*server).sendContent(F("<form action=\"/connect\" method=\"POST\">"));
	(*server).sendContent("<input type=\"text\" id=\"ssid\" name=\"ssid\" value=\""+SSID+"\" placeholder=\"SSID\">");
	(*server).sendContent("<input type=\"text\" name=\"pass\" value=\""+Password+"\" placeholder=\"Pass\">");
	(*server).sendContent(F("<input type=\"submit\" value=\"Submit\">"));
	(*server).sendContent(F("</form>"));
	(*server).sendContent(F("<h3>Switch WiFi mode</h3>"));
	(*server).sendContent(F("<form action=\"/ap\" method=\"POST\">"));
	(*server).sendContent(F("<input type=\"submit\" value=\"Access Point\">"));
	(*server).sendContent(F("</form>"));
	(*server).sendContent(F("<form action=\"/local\" method=\"POST\">"));
	(*server).sendContent(F("<input type=\"submit\" value=\"Local Mode\">"));
	(*server).sendContent(F("</form>"));
	(*server).sendContent(F("<form action=\"/exit\" method=\"POST\">"));
	(*server).sendContent(F("<input type=\"submit\" value=\"Exit Portal\"">));
	(*server).sendContent(F("</form>"));
	(*server).sendContent(F("</center>"));
	(*server).sendContent(F("</body></html>"));
	(*server).chunkedResponseFinalize();
}

void portalStart() {
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  IPAddress apIP(SP_AP_IP);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, subnet);
  WiFi.softAP(SP_AP_NAME);
  _SP_dnsServer.start(53, "*", apIP);

  _SP_server.onNotFound([]() {
	 WebConfigurator(&_SP_server, String(portalCfg.SSID), String(portalCfg.pass));
    //_SP_server.send(200, "text/html", SP_connect_page);
  });
  _SP_server.on("/connect", HTTP_POST, SP_handleConnect);
  _SP_server.on("/ap", HTTP_POST, SP_handleAP);
  _SP_server.on("/local", HTTP_POST, SP_handleLocal);
  _SP_server.on("/exit", HTTP_POST, SP_handleExit);
  _SP_server.begin();
  _SP_started = true;
  _SP_status = 0;
}

void portalStop() {
  WiFi.softAPdisconnect();
  _SP_server.stop();
  _SP_dnsServer.stop();
  _SP_started = false;
}

bool portalTick() {
  if (_SP_started) {
    _SP_dnsServer.processNextRequest();
    _SP_server.handleClient();
    yield();
    if (_SP_status) {
      portalStop();
      return 1;
    }
  }
  return 0;
}

void portalRun(uint32_t prd) {
  uint32_t tmr = millis();
  portalStart();
  while (!portalTick()) {
    if (millis() - tmr > prd) {
      _SP_status = 5;
      portalStop();
      break;
    }
  }
}

byte portalStatus() {
  return _SP_status;
}