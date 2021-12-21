// блок 1 - структура
struct DataBlock {
  int valI;
  byte valB;
};
DataBlock data;

// блок 2 - массив
int arr[10];

#include <EEManager.h>
EEManager mem1(data);
EEManager mem2(arr);

void setup() {
  Serial.begin(9600);
  // для esp8266 не забудь вызвать EEPROM.begin(размер)!

  // первый блок храним с адреса 0
  mem1.begin(0, 'a');

  // второй - следом за ним
  mem2.begin(mem1.nextAddr(), 'a');
}

void loop() {

}
