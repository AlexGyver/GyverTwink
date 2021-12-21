void initCam() {
  if (androidMode) {
    Acam = new KetaiCamera(this, 1280, 720, 30);
    frame = createImage(Acam.height, Acam.width, RGB);
  } else {
    while (Wcam == null) {
      String[] cameras = Capture.list();
      if (cameras.length != 0) Wcam = new Capture(this, cameras[0]);
    }
    frame = createImage(Wcam.height, Wcam.width, RGB);
  }

  size = frame.width / X;
  Y = frame.height / size;
  brMap = new int[X * Y][3];
  ring = frame.copy();
}

void startCam() {
  if (Acam == null || Wcam == null) initCam();
  if (!camStart) {
    if (androidMode) Acam.start();
    else Wcam.start();
  }
  camStart = true;
}

void stopCam() {
  if (camStart) {
    if (androidMode) Acam.stop();
    else Wcam.stop();
  }
  camStart = false;
}

void readCam() {
  PImage buf;
  if (androidMode) buf = Acam;
  else buf = Wcam;

  int am = frame.height * frame.width;
  for (int i = 0; i < am; i++) {
    frame.pixels[(i % frame.height) * frame.width + (am - i) / frame.height] = buf.pixels[i];
  }
  frame.updatePixels();
}

void captureEvent(Capture Wcam) {
  Wcam.read();
  camReady = true;
}

void onCameraPreviewEvent() {
  Acam.read();
  camReady = true;
}
