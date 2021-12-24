// найти разницу
void findMax() {
  int max = 0, maxi = 0;
  for (int i = 0; i < brMap.length; i++) {
    brMap[i][2] = (brMap[i][1] - brMap[i][0]);
    brMap[i][2] = max(brMap[i][2], 0);
    if (max < brMap[i][2]) {
      max = brMap[i][2];
      maxi = i;
    }
  }
  maxX = maxi % X;
  maxY = maxi / X;

  PGraphics buf = createGraphics(frame.width, frame.height);
  buf.beginDraw();
  buf.stroke(0xFFFF0000);
  buf.strokeWeight(2);
  buf.noFill();
  buf.circle(maxX*size+size/2, maxY*size+size/2, 30);
  buf.endDraw();
  ring = buf.get();
}

// создать карту яркости (0 базовая, 1 текущая, 2 разностная)
void makeMap(int i) {
  for (int y = 0; y < Y; y++) {
    for (int x = 0; x < X; x++) {
      int sum = 0;
      for (int yy = 0; yy < size; yy++) {
        for (int xx = 0; xx < size; xx++) {
          int pos = (y*size + yy) * frame.width + (x*size + xx);
          int col = frame.pixels[pos];
          //sum += col >> 16 & 0xFF;
          sum += (col & 0xFF) / 3 + (col >> 8 & 0xFF) / 3 + (col >> 16 & 0xFF) / 3;
        }
      }
      sum /= size * size;
      brMap[y*X + x][i] = sum;
    }
  }
}

// вывести карту яркости (0 базовая, 1 текущая, 2 разностная)
PGraphics drawMap(int m) {
  PGraphics buf = createGraphics(frame.width, frame.height);
  buf.beginDraw();
  buf.noStroke();
  for (int y = 0; y < Y; y++) {
    for (int x = 0; x < X; x++) {
      int col = brMap[y*X + x][m];
      buf.fill(col);
      buf.rect(x*size, y*size, size, size);
    }
  }
  buf.endDraw();
  return buf;
}
