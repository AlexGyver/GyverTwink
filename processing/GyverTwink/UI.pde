byte curTab = 0;
TextInput leds = new TextInput();
TextInput subnet = new TextInput();
DropDown dropIP = new DropDown();
DropDown dropEff = new DropDown();
Toggle power = new Toggle();
Toggle auto = new Toggle();
Toggle rnd = new Toggle();
Slider bri = new Slider();
Slider prd = new Slider();
Toggle fav = new Toggle();
Slider scl = new Slider();
Slider spd = new Slider();
String[] effs = {
  "0. Party_grad", 
  "1. Raibow_grad", 
  "2. Stripe_grad", 
  "3. Sunset_grad", 
  "4. Pepsi_grad", 
  "5. Warm_grad", 
  "6. Cold_grad", 
  "7. Hot_grad", 
  "8. Pink_grad", 
  "9. Cyber_grad", 
  "10. RedWhite_grad", 
  "11. Party_noise", 
  "12. Raibow_noise", 
  "13. Stripe_noise", 
  "14. Sunset_noise", 
  "15. Pepsi_noise", 
  "16. Warm_noise", 
  "17. Cold_noise", 
  "18. Hot_noise", 
  "19. Pink_noise", 
  "20. Cyber_noise", 
  "21. RedWhite_noise", 
};

void ui() {
  uiFill();
  // ====== TABS =======
  int w = width / 3;
  int h = w / 2;
  int y = height - h;

  if (IconButton("wrench", 0, y, w, h, curTab == 0)) switchCfg();
  if (IconButton("adjust", w*1, y, w, h, curTab == 1)) switchEffects();
  if (IconButton("camera", w*2, y, w, h, curTab == 2)) switchCalib();

  if (curTab == 0) cfgTab();
  if (curTab == 1) effTab();
  if (curTab == 2) calibTab();
}

void cfgTab() {
  uiGlobalX(offs);
  uiResetStep(50);
  LabelCenter("GyverTwink", 20);
  Divider(width-offs*2);

  //uiResetStep(120);
  Label("Connection:", 15);
  Label("Power:", 15);
  Label("Brightness:", 15);
  Divider(width-offs*2);
  Label("Switch effect:", 15);
  Label("Auto:", 15);
  Label("Random:", 15);
  Label("Period [1-10m]:", 15);

  Divider(width-offs*2);
  Label("LED amount:", 15);
  Label("Subnet:", 15);

  uiResetStep(50);
  uiStep();
  uiStep();
  uiStep();

  if (power.show(WW, uiStep())) sendData(2, 1, int(power.value));
  if (bri.show(0, 255, WW, uiStep(), W)) sendData(2, 2, int(bri.value));
  uiStep();
  if (Button("Next effect", WW, uiStep(), W)) sendData(2, 6);
  if (auto.show(WW, uiStep())) sendData(2, 3, int(auto.value));
  if (rnd.show(WW, uiStep())) sendData(2, 4, int(rnd.value));
  if (prd.show(1, 10, WW, uiStep(), W)) sendData(2, 5, int(prd.value));

  uiStep();
  if (leds.show(WW, uiStep(), W) && androidMode) openKeyboard();
  if (leds.done()) {
    if (androidMode) closeKeyboard();
    int am = int(leds.text);
    sendData(2, 0, am/100, am%100);
  }
  if (subnet.show(WW, uiStep(), W) && androidMode) openKeyboard();
  if (subnet.done()) {
    if (androidMode) closeKeyboard();
    file[0] = subnet.text;
    saveStrings("subnet.txt", file);
  }

  uiResetStep(50);
  uiStep();
  uiStep();
  if (dropIP.show(ips.array(), WW, uiStep(), W-s_height)) {
    curIP = ips.get(dropIP.getSelected());
    requestCfg();
  }
  if (IconButton("sync", WW + W-s_height, uiPrevStep())) startSearch();
}

void effTab() {
  uiGlobalX(offs);
  uiResetStep(50);

  //uiResetStep(100);
  uiGlobalX(offs);
  Label("Effect:", 15);
  Label("Favorite:", 15);
  Label("Scale:", 15);
  Label("Speed:", 15);

  uiResetStep(60);
  uiStep();
  if (fav.show(WW, uiStep())) sendData(4, 1, int(fav.value));
  if (scl.show(0, 255, WW, uiStep(), W)) sendData(4, 2, int(scl.value));
  if (spd.show(0, 255, WW, uiStep(), W)) sendData(4, 3, int(spd.value));

  uiResetStep(50);

  if (androidMode) uiSetScale(androidScale*0.8);
  else uiSetScale(pcScale*0.7);
  if (dropEff.show(effs, WW, uiStep(), W-s_height)) {
    sendData(4, 0, dropEff.selected);
    parseMode = 4;
  }
  if (androidMode) uiSetScale(androidScale);
  else uiSetScale(pcScale);
}

void calibTab() {
  // камера не стартовала в PC режиме
  if (!androidMode && Wcam == null) return;
  
  if (camReady) {
    camReady = false;
    readCam();
    makeMap(1);
    findMax();
  }

  image(frame, (width-frame.width)/2, 0);
  if (calibF) image(ring, (width-frame.width)/2, 0);

  uiResetStep(frame.height + 30);
  uiResetX(0);
  uiGlobalX(0);
  if (Button("Start")) {
    calibF = true;
    sendData(3, 0);
    calibCount = 0;
    actionTmr = millis() + 2000;
  }
  Label(str(calibCount*100/(int(leds.text)+1))+'%', 15, uiPrevX()+15, uiPrevStep());
  if (Button("Stop")) {
    calibF = false;
    sendData(3, 2);
    calibCount = 0;
  }
}

void switchCfg() {
  curTab = 0;
  sendData(2, 7);
  stopCam();
}
void switchEffects() {
  curTab = 1;
  stopCam();
  sendData(4, 0, dropEff.selected);
  parseMode = 4;
}
void switchCalib() {
  curTab = 2;
  startCam();
}
