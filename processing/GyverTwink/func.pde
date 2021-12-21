void startSearch() {
  int[] ipv4 = int(split(KetaiNet.getIP(), '.'));
  int[] mask = int(split(subnet.text, '.'));
  brIP = "";
  for (int i = 0; i < 4; i++) {
    brIP += ipv4[i] | (mask[i] ^ 0xFF);
    if (i != 3) brIP += '.';
  }
  //println(brIP);

  searchF = true;
  parseMode = 0;

  // выводим однократно
  ips.clear();
  ips.append("searching...");
  dropIP.selected = 0;
  ui();
  ips.clear();
  
  curIP = brIP;
  byte[] packet = {'G', 'T', 0};
  sendData(packet);
  actionTmr = millis();
}

void requestCfg() {
  parseMode = 1;
  sendData(1);
}
void sendData(int b0) {
  int[] data = {'G', 'T', b0};
  sendData(byte(data));
}
void sendData(int b0, int b1) {
  int[] data = {'G', 'T', b0, b1};
  sendData(byte(data));
}
void sendData(int b0, int b1, int b2) {
  int[] data = {'G', 'T', b0, b1, b2};
  sendData(byte(data));
}
void sendData(int b0, int b1, int b2, int b3) {
  int[] data = {'G', 'T', b0, b1, b2, b3};
  sendData(byte(data));
}
void sendData(int b0, int b1, int b2, int b3, int b4) {
  int[] data = {'G', 'T', b0, b1, b2, b3, b4};
  sendData(byte(data));
}
void sendData(int b0, int b1, int b2, int b3, int b4, int b5) {
  int[] data = {'G', 'T', b0, b1, b2, b3, b4, b5};
  sendData(byte(data));
}
void sendData(int b0, int b1, int b2, int b3, int b4, int b5, int b6) {
  int[] data = {'G', 'T', b0, b1, b2, b3, b4, b5, b6};
  sendData(byte(data));
}
void sendData(byte[] data) {
  if (curIP.charAt(0) != 'n') {
    udp.send(data, curIP, port);
    delay(15);
    udp.send(data, curIP, port);
  }
}
