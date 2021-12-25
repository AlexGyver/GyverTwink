// Исходник приложения GyverTwink
// Написано на коленке, возможно позже переделаю =(
// v1.0 beta
// v1.1 release
// v1.2 - калибровка больше 255, автоматический масштаб интерфейса, поля ввода подвинул наверх, оптимизация от TheAirBlow 
// v1.3 - опять фиксы масштаба
// v1.4 - починил связь с гирляндой

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

// чтобы сбилдить под Android - нужно установить Android mode
// встроенный билдер собирает под SDK версии 29
// я собирал проект в Android Studio под target 32 версии

// масштаб интерфейса
float androidScale/* = 2.8*/;
float pcScale = 1.3;

// ============== LIBRARIES ===============
import processing.video.*;
import hypermedia.net.*;
import ketai.camera.*;
import ketai.net.*;
KetaiCamera Acam;
Capture Wcam;
UDP udp;

// ============== VARIABLES ================
int X = 60;     // размер сетки по X (задан вручную)
int Y;          // размер сетки по Y
int maxX, maxY; // точка максимума в координатах сетки
int size;       // размер "пикселя"
int[][] brMap;  // карта яркости 
PImage frame, ring;
boolean camReady = false;
boolean camStart = false;
String brIP, curIP;
int port = 8888;
boolean searchF, found = false;
byte parseMode = 0;
int actionTmr;
StringList ips = new StringList();

boolean calibF = false;
int calibCount = 0;
int WW, W;
int offs = 30;
String[] file;

// ============== ПРОГРАММА ===============
void settings() {
  if (!androidMode) size(600, 900);
  smooth(8);
}

void setup() {
  androidScale = width/400.0;
  offs = width / 25;
  if (androidMode) W = width/2;
  else W = 300;      
  WW = width-W-offs;

  file = loadStrings("subnet.txt");
  if (file == null) {
    println("Subnet text file is empty");
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
  if (searchF) {
    if (millis() - actionTmr > 800) {
      searchF = false;
      if (ips.size() == 0) ips.append("not found");
      else {
        found = true;
        requestCfg();
      }
    }
  } else ui();

  // Калибровка
  if (calibF) {
    if (millis() - actionTmr > 400) {
      actionTmr = millis();
      if (calibCount == 0) makeMap(0);
      if (calibCount > int(leds.text)) {
        calibF = false;
        sendData(new int[] {3, 2, calibCount/100, calibCount%100, maxX, maxY});
        calibCount = 0;
        return;
      }
      sendData(new int[] {3, 1, calibCount/100, calibCount%100, maxX, maxY});
      calibCount++;
    }
  }
}
