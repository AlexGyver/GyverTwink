const char index_page[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
<title>Gyver Twink v1.2 - WEBServer</title>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<script> function createXMLHttp() {if (typeof XMLHttpRequest != "undefined") {return new XMLHttpRequest();} else if (window.ActiveXObject) { var aVersions = ["MSXML2.XMLHttp.5.0","MSXML2.XMLHttp.4.0","MSXML2.XMLHttp.3.0","MSXML2.XMLHttp","Microsoft.XMLHttp"];for (var i = 0; i < aVersions.length; i++) {try {var oXmlHttp = new ActiveXObject(aVersions[i]);return oXmlHttp;} catch (oError) {}}throw new Error("Невозможно создать объект XMLHttp.");}}; </script>
<script>var GET = 'GET'; var POST = 'POST'; var HEAD = 'HEAD'; function postAjax(url, type, data, callback) { var oXmlHttp = createXMLHttp();var sBody = data;oXmlHttp.open(type, url, true);oXmlHttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");oXmlHttp.onreadystatechange = function() {if (oXmlHttp.readyState == 4) {callback(oXmlHttp.responseText);}};oXmlHttp.send(sBody);}
function loadserver(){
  
}
</script>
<body>
<style type="text/css">
    input[type="text"] {margin-bottom:8px;font-size:20px;}
    input[type="number"] {margin-bottom:8px;font-size:20px;}
    input[type="checkbox"] {margin-bottom:8px;font-size:20px;}
    input[type="submit"] {width:180px; height:60px;margin-bottom:8px;font-size:20px;}
    div {
        padding-top: 6px;
    }
    input[type="button"] {
        border-radius: 7px;
    }
</style>
<center>
<h3>Gyver Twink v1.4</h3>
<div><input type="button" onclick="postAjax('/seton', POST, '', function(d){})" value="Включить гирлянду"/><input type="button" onclick="postAjax('/setoff', POST, '', function(d){})" value="Выключить гирлянду"/></div>
<div><input type="button" onclick="postAjax('/nextef', POST, '', function(d){})" value="Следующий эффект"/></div>
<div><input type="button" onclick="postAjax('/changevalue?v=15&m=2', POST, '', function(d){})" value="Уменьшить яркость"/><input type="button" onclick="postAjax('/changevalue?v=15&m=1', POST, '', function(d){})" value="Увеличить яркость"/></div>
<div><p><a href="/setting">Настройки гирлянды</a></p></div>
</center>
</body></html>)rawliteral";

const char setting_page[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
<title>Gyver Twink v1.4 - WEBServer. Настройки</title>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<script> function createXMLHttp() {if (typeof XMLHttpRequest != "undefined") {return new XMLHttpRequest();} else if (window.ActiveXObject) { var aVersions = ["MSXML2.XMLHttp.5.0","MSXML2.XMLHttp.4.0","MSXML2.XMLHttp.3.0","MSXML2.XMLHttp","Microsoft.XMLHttp"];for (var i = 0; i < aVersions.length; i++) {try {var oXmlHttp = new ActiveXObject(aVersions[i]);return oXmlHttp;} catch (oError) {}}throw new Error("Невозможно создать объект XMLHttp.");}}; </script>
<script>var GET = 'GET'; var POST = 'POST'; var HEAD = 'HEAD'; function postAjax(url, type, data, callback) { var oXmlHttp = createXMLHttp();var sBody = data;oXmlHttp.open(type, url, true);oXmlHttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");oXmlHttp.onreadystatechange = function() {if (oXmlHttp.readyState == 4) {callback(oXmlHttp.responseText);}};oXmlHttp.send(sBody);}
function loadserver(){
  postAjax('/getparam', POST, '', function(d){
    var ms = d.split(';');
    ledcount.value = parseInt(ms[0]);
    ace.checked = ms[3] == "1";
    pce.value = parseInt(ms[5]);
    aoff.value = ms[6] == "true";
    toff.value = parseInt(ms[7]);
  });
}
setTimeout("loadserver()", 10);
</script>
</head><body>
<style type="text/css">
    input[type="text"] {margin-bottom:8px;font-size:20px;}
    input[type="number"] {margin-bottom:8px;font-size:20px;}
    input[type="checkbox"] {margin-bottom:8px;font-size:20px;}
    input[type="submit"] {width:180px; height:60px;margin-bottom:8px;font-size:20px;}
    div {
        padding-top: 6px;
    }
    input[type="button"] {
        border-radius: 7px;
    }
</style>
<center>
<h3>Gyver Twink v1.4. Настройки</h3>
<form action="/changeset" method="GET">
    Количество светодиодов: <input id="ledcount" type="number" min='0' min='500' name="ledcount"> <br />
    Автоматическая смена эффектов: <input id="ace" type="checkbox" name="ace"> <br />
    Период смена эффектов: <input id="pce" type="number" min='10' max="255" name="pce"> <br />
    Автоматическое выключение: <input id="aoff" type="checkbox" name="aoff"> <br />
    Таймер выключения: <input id="toff" type="number" min="0" max="255" name="toff"> <br />
    <input type="submit" value="Применить"> <br />
</form>
</center>
</body></html>)rawliteral";

void htmlstart(){
  webserver.on("/", [&]() {
    webserver.send(200, "text/html", index_page);
  });
  webserver.on("/setting", [&]() {
    webserver.send(200, "text/html", setting_page);
  });
  webserver.on("/seton", [&]() {
    CP(true);
    webserver.send(200, "text/html", index_page);
  });
  webserver.on("/setoff", [&]() {
    CP(false);
    webserver.send(200, "text/html", index_page);
  });
  webserver.on("/setoff", [&]() {
    webserver.send(200, "text/html", index_page);
  });
  webserver.on("/changevalue", [&]() {
    CB(webserver.arg("v").toInt(), webserver.arg("m").toInt());
    webserver.send(200, "text/html", index_page);
  });
  webserver.on("/getparam", [&]() {
    String tmp = "";
    tmp += String(cfg.ledAm) + ";";
    tmp += String(cfg.power) + ";";
    tmp += String(cfg.bright) + ";";
    tmp += String(cfg.autoCh) + ";";
    tmp += String(cfg.rndCh) + ";";
    tmp += String(cfg.prdCh) + ";";
    tmp += String(cfg.turnOff) + ";";
    tmp += String(cfg.offTmr);
    
    webserver.send(200, "text/html", tmp);
  });
  webserver.on("/changeset", [&]() {
    cfg.ledAm = webserver.arg("ledcount").toInt();
    strip->setLeds(leds, cfg.ledAm);
    
    cfg.autoCh = webserver.arg("ace") == "on";
    if (cfg.autoCh) switchTmr.restart();
    else switchTmr.stop();

    cfg.prdCh = webserver.arg("pce").toInt();
    switchTmr.setPrd(cfg.prdCh * 60000ul);
    if (cfg.autoCh) switchTmr.restart();

    cfg.turnOff = webserver.arg("aoff") == "on";
    if (cfg.turnOff) offTmr.restart();
    else offTmr.stop();

    cfg.offTmr = webserver.arg("toff").toInt();
    offTmr.setPrd(cfg.offTmr * 60000ul);
    if (cfg.turnOff) offTmr.restart();

    if (!cfg.power) strip->showLeds(0);
    EEcfg.updateNow();
    
    webserver.send(200, "text/html", index_page);
  });
  webserver.on("/nextef", [&]() {
    DEBUGLN("next effect");
    switchEff();
    webserver.send(200, "text/html", index_page);
  });
}


void CP(bool pw){
    DEBUGLN("power change");
    cfg.power = pw;
    if (!cfg.power) strip->showLeds(0);
    EEcfg.update();
}

void CB(int changevalue, int m){
  DEBUGLN("change bright");
  if (cfg.power) {
    // меняем безопасно
    int bri = cfg.bright;
    changevalue = changevalue >= 255 || changevalue <= 0 ? 0 : changevalue;
    if(m == 1)
      bri += changevalue;
    else bri -= changevalue;
    bri = constrain(bri, 0, 255);      
    cfg.bright = bri;
    strip->showLeds(bri);
    EEcfg.update();
  }
}
