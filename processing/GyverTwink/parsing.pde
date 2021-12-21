void receive(byte[] ubuf) {
  if (ubuf[0] != 'G' || ubuf[1] != 'T') return;
  int[] data = new int[10];
  for (int i = 0; i < ubuf.length - 2; i++) {
    data[i] = int(ubuf[i+2]);
    //print(data[i], ',');
  }
  //println();

  if (parseMode != data[0]) return;

  switch (data[0]) {
  case 0:  // поиск
    String ip = brIP.substring(0, brIP.lastIndexOf('.')+1) + str(data[1]);
    if (!ips.hasValue(ip)) ips.append(ip);
    break;

  case 1:  // 
    searchF = false;
    leds.text = str(data[1] * 100 + data[2]);
    power.value = boolean(data[3]);
    bri.value = data[4];
    auto.value = boolean(data[5]);
    rnd.value = boolean(data[6]);
    prd.value = data[7];
    break;

  case 4:
    fav.value = boolean(data[1]);
    scl.value = data[2];
    spd.value = data[3];
    break;
  }

  //data = subset(data, 0, data.length-2);
  //String message = new String(data);
  //println(message);
  //String str = serial.readStringUntil('\n').trim();  // читаем строку до \n и подрезаем
  //String data[] = str.split(",");                    // разбиваем строку
  //int[] data = int(split(str, ','));  // парсить можно сразу в int[]!
}
