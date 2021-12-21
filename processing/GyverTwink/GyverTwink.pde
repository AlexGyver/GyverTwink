// Исходник приложения GyverTwink
// Написано на коленке, возможно позже переделаю =(
// v1.0

// ============== ВАЖНО! ===============
// Установить библиотеки из менеджера библиотек:
// (Набросок/Импортировать библиотеку/Добавить библиотеку)
// - Video
// - Ketai

// Установить библиотеки вручную:
// (в documents/processing/libraries)
// - http://ubaa.net/shared/processing/udp/ - download Processing library

// Android/Sketch Permissions установлены
// - CAMERA
// - INTERNET
// - READ_EXTERNAL_STORAGE

// ============== НАСТРОЙКИ ===============
// true - Android режим, false - PC режим
private static final boolean androidMode = false;

// для PC режима раскомментируй две строки ниже. Для Android - закомментируй
void openKeyboard() {}
void closeKeyboard() {}

/// масштаб интерфейса
float androidScale = 3.0;
float pcScale = 1.5;

// ============== ЛИБЫ ===============
import processing.video.*;
Capture Wcam;

//http://ketai.org/reference/ui/
import ketai.camera.*;
import ketai.net.*;
KetaiCamera Acam;

import hypermedia.net.*;
UDP udp;

// ============== ДАТА ===============
int X = 60;    // размер сетки по X (задан вручную)
int Y;         // размер сетки по Y
int maxX, maxY;// точка максимума в координатах сетки

//
int size;      // размер "пикселя"
int[][] brMap; // карта яркости 
PImage frame, ring;
boolean camReady = false;
boolean camStart = false;
String brIP, curIP;
int port = 8888;
boolean searchF = false;
byte parseMode = 0;
int actionTmr;
StringList ips = new StringList();

boolean calibF = false;
int calibCount = 0;
int WW, W;
int offs = 20;
String[] file;
// ============== ПРОГРАММА ===============
void settings() {
  if (!androidMode) size(600, 900);
  smooth(8);
}

void setup() {
  if (androidMode) W = width/2;
  else W = 300;      
  WW = width-W-offs;

  file = loadStrings("subnet.txt");
  if (file == null) {
    println("file null");
    file = new String[1];
    file[0] = "255.255.255.0";
    saveStrings("subnet.txt", file);
  }
  subnet.text = file[0];

  if (androidMode) uiSetScale(androidScale);
  else uiSetScale(pcScale);

  udp = new UDP(this);
  udp.listen(true);
  startSearch();
}

void draw() {
  // поиск устройств блокирует отрисовку интерфейса по таймеру
  if (searchF) {
    if (millis() - actionTmr > 800) {
      searchF = false;
      if (ips.size() == 0) ips.append("not connected");
      else requestCfg();
    }
  } else ui();

  // рутина калибровки
  if (calibF) {
    if (millis() - actionTmr > 500) {
      actionTmr = millis();
      if (calibCount == 0) makeMap(0);
      if (calibCount > int(leds.text)) {
        calibF = false;
        sendData(3, 2, calibCount, maxX, maxY);
        calibCount = 0;
        return;
      }
      sendData(3, 1, calibCount, maxX, maxY);
      calibCount++;
    }
  }
}
