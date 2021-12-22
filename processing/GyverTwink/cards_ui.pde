void mousePressed() {
  pressHandler();
}

void mouseReleased() {
  releaseHandler();
}

void keyPressed() {
  keyHandler();
}
/*---------------------------------------
 UI Components of Cards_UI for Processing
 author: Lucas Cassiano - cassiano@mit.edu
 
 version: 2.0 by AlexGyver
 мне не нравится - потом сделаю свою либу 
 */
// Function list (2.0): https://github.com/GyverLibs/cards_ui
// Old demo: https://web.media.mit.edu/~cassiano/projects/cards_ui/
// Icon-images: https://iconsplace.com/
// Fontawesome Icons: https://fontawesome.com/download

//Colors
private color c_very_dark = color(36, 37, 46);
private color c_dark = color(29, 33, 44);
private color c_mid = color(44, 58, 71);
private color c_light= color(51, 64, 80);

private color c_primary= color(33, 115, 139);
private color c_hover = color(32, 155, 160);

private color c_text_color = color(255);

//Click Options
private boolean clicked = false;
private boolean canClick = true;

//For text Input/Edit
String bufferText = null;
boolean doneText = false;

//Default sizes
private int s_big = 200;
private int s_height = 30;
private int s_med = 100;
private int s_small = 50;
private int s_stroke = 7;

//For Cards
int card_h = 0;
int card_w = 0;
private int card_x = 0;
private int card_y = 0;

private int medFontSize = 15;
private int smallFontSize = 15;
private int largeFontSize = 25;

private int d_medFontSize = 15;
private int d_smallFontSize = 15;
private int d_largeFontSize = 25;

private int d_big = 200;
private int d_height = 30;
private int d_med = 100;
private int d_small = 50;
private int d_stroke = 5;

// pos
private int _pos_y = 0;
private int _step_y = s_height+6;
private int _prevX = 0;
private float _ui_scale = 1.0;
private boolean _drop_open = false;
private int _x_offs = 0;
private boolean _item_changed = false;

// ====================================== STEP =======================================
void uiResetStep(int y) {
  _pos_y = y;
}
int uiStep() {
  _pos_y += _step_y;
  return _pos_y - _step_y;
}
int uiPrevStep() {
  return _pos_y - _step_y;
}
void uiResetX(int x) {
  _prevX = x;
}
int uiPrevX() {
  return _prevX;
}
void uiGlobalX(int x) {
  _x_offs = x;
}
boolean uiChanged() {
  if (_item_changed) {
    _item_changed = false;
    return true;
  }
  return false;
}

// ====================================== SETTINGS =======================================
public void uiSetScale(float scale) {
  _ui_scale = scale;
  s_big = int(d_big * scale);
  s_height = int(d_height * scale);
  s_med = int(d_med * scale);
  s_small = int(d_small * scale);
  s_stroke = int(d_stroke * scale);
  medFontSize = int(d_medFontSize * scale);
  smallFontSize = int(d_smallFontSize * scale);
  largeFontSize = int(d_largeFontSize * scale);
  _step_y = int((d_height+6) * scale);
}
public void uiTextSize(int size) {
  medFontSize = size;
}
public void inputTextSize(int size) {
  smallFontSize = size;
}
public void tooltipTextSize(int size) {
  largeFontSize = size;
}

public void uiDark() {
  c_very_dark = color(36, 37, 46);
  c_dark = color(29, 33, 44);
  c_mid = color(44, 58, 71);
  c_light = color(51, 64, 80);
  c_hover = color(32, 155, 160);
}

public void uiLight() {
  c_very_dark = color(100);
  c_dark = color(150);
  c_mid = color(200);
  c_light = color(250);
  c_hover = color(32, 155, 160);
  c_text_color = color(10);
}

private void EditText(String txt) {
  bufferText = txt;
}

// ====================================== MISC =======================================
void uiFill() {
  background(c_dark);
}

// ====================================== LABEL =======================================
public void LabelBase(String text, int size, int x, int y, boolean center) {
  y += _step_y/2;
  fill(c_text_color);
  textSize(int(size*_ui_scale));
  if (center) textAlign(CENTER, CENTER);
  else textAlign(LEFT, CENTER);
  text(text, x, y);
}

public void LabelCenter(String text, int size, int x, int y) {
  LabelBase(text, size, x, y, true);
}
public void LabelCenter(String text, int size, int x) {
  LabelBase(text, size, x, uiStep(), true);
}
public void LabelCenter(String text, int size) {
  LabelBase(text, size, width/2, uiStep(), true);
}
public void Label(String text, int size, int x, int y) {
  LabelBase(text, size, x, y, false);
}
public void Label(String text, int size, int x) {
  LabelBase(text, size, x, uiStep(), false);
}
public void Label(String text, int size) {
  int x;
  if (_x_offs != 0) x = _x_offs;
  else x = width/2;
  LabelBase(text, size, x, uiStep(), false);
}

// ====================================== TOOLTIP =======================================
//X and Y are the position of the point of the triangle
public void Tooltip(String text, int x, int y) {
  textSize(largeFontSize);
  int w = (int)textWidth(text);
  int h = 50;
  int tw = 14; //triangle width
  int th = 15; //triangle height
  noStroke();
  //Shadow
  fill(0, 0, 0, 15);
  rect(5+x-w/2, 5+y-th-h, w, h, 2);
  triangle(5+x-tw/2, 5+y-th, 5+x, 5+y, 5+x+tw/2, 5+y-th);
  //Color
  fill(c_very_dark);
  rect(x-w/2, y-th-h, w, h, 2);
  triangle(x-tw/2, y-th, x, y, x+tw/2, y-th);
  //Text
  textSize(medFontSize);
  fill(255);
  textAlign(CENTER, CENTER);
  text(text, x-w/2, y-th-h, w, h);
  //triangle(
}

// ====================================== DIVIDER =======================================
public void Divider(int x, int y, int w) {
  noStroke();
  fill(c_light);
  rect(x, y+_step_y/2, w, s_stroke/2, s_stroke/4);
}
public void Divider(int y, int w) {
  Divider((width-w)/2, y, w);
}
public void Divider(int w) {
  Divider((width-w)/2, uiStep(), w);
}


// ====================================== BUTTON =======================================
//Basic Text Button
public boolean Button(String text, int x, int y, int w, int h) {
  _prevX = x + w;
  stroke(c_very_dark);
  if (mouseX >= x && mouseX <= x+w && 
    mouseY >= y && mouseY <= y+h && !_drop_open) {
    fill(c_hover);
    rect(x, y, w, h);
    fill(c_text_color);
    textSize(medFontSize);
    textAlign(CENTER, CENTER);
    text(text, x, y, w, h);
    if (clicked && canClick) {
      fill(c_light);
      rect(x, y, w, h);
      text(text, x, y, w, h);
      canClick = false;
      return true;
    }
  } else {
    fill(c_light);
    rect(x, y, w, h);
    fill(c_text_color);
    textSize(medFontSize);
    textAlign(CENTER, CENTER);
    text(text, x, y, w, h);
    return false;
  }

  return false;
}

boolean Button(String text) {
  int x;
  if (_x_offs != 0) x = _x_offs;
  else x = (width-s_med)/2;
  return Button(text, x, uiStep(), s_med, s_height);
}

boolean Button(String text, int x) {
  return Button(text, x, uiStep(), s_med, s_height);
}

boolean Button(String text, int x, int y, int w) {
  return Button(text, x, y, w, s_height);
}

//Basic Text Button
boolean Button(String text, int x, int y) {
  return Button(text, x, y, s_med, s_height);
}

//Basic Text Button
boolean Button(String text, int x, int y, String t) {
  return Button(text, x, y, s_med, s_height, t);
}

//Button With Tooltip
boolean Button(String text, int x, int y, int w, int h, String tooltip) {
  _prevX = x + w;
  if (mouseX >= x && mouseX <= x+w && 
    mouseY >= y && mouseY <= y+h && !_drop_open) {
    Tooltip(tooltip, x+w/2, y-1);
    fill(c_hover);
    rect(x, y, w, h);
    fill(c_text_color);
    textSize(medFontSize);
    textAlign(CENTER, CENTER);
    text(text, x, y, w, h);
    if (clicked && canClick) {
      fill(c_light);
      rect(x, y, w, h);
      text(text, x, y, w, h);
      canClick = false;
      return true;
    }
  } else {
    fill(c_light);
    rect(x, y, w, h);
    fill(c_text_color);
    textSize(medFontSize);
    textAlign(CENTER, CENTER);
    text(text, x, y, w, h);
    return false;
  }

  return false;
}

// ====================================== FONT AWESOME =======================================
//https://fontawesome.com/v5.15/icons?d=gallery&p=2
/*--Font Awesome Icons--
 based on: 
 https://github.com/encharm/Font-Awesome-SVG-PNG
 this Method loads the .svg files from /svg/ folder
 more icons can be added at the folder, and called by its names
 ----------------*/
//This hashmap is used to avoid reload the same icon multiple times
HashMap<String, PShape> usedIcons = new HashMap<String, PShape>();

public void Icon(String name, int x, int y, int w, int h) {
  _prevX = x + w;
  if (usedIcons.get(name)==null) {
    try {
      PShape i = loadShape(name+".svg");
      usedIcons.put(name, i);
    }
    catch(Exception e) {
      println("CARD_UI - ERROR: svg icon not found");
    }
  }
  PShape i = usedIcons.get(name);
  if (w == h && i.width != i.height) {
    if (i.width > i.height) {
      int H = int(w * i.height / i.width);
      y += (h - H) / 2;
      h = H;
    } else {
      int W = int(h * i.width / i.height);
      x += (w - W) / 2;
      w = W;
    }
  }
  shape(i, x, y, w, h);
}
public void Icon(String name, int x, int y, int w) {
  Icon(name, x, y, w, w);
}

// ====================================== IMAGE BUTTON =======================================
boolean ImageButton(PImage img, int x, int y, int w, int h, boolean select) {
  _prevX = x + w;
  int p = min(w, h) / 10;
  int W = w-2*p, H = h-2*p;

  int dW = W;
  int dH = W * img.height / img.width;
  if (dH > H) {
    dH = H;
    dW = H * img.width / img.height;
  }

  int ix = x + (w - dW) / 2;
  int iy = y + (h - dH) / 2;

  if (mouseX >= x && mouseX <= x+w && 
    mouseY >= y && mouseY <= y+h && !_drop_open) {
    fill(c_hover);
    rect(x, y, w, h);
    image(img, ix, iy, dW, dH);
    if (clicked && canClick) {
      fill(c_light);
      rect(x, y, w, h);
      image(img, ix, iy, dW, dH);
      canClick = false;
      return true;
    }
  } else {
    if (select) fill(c_dark);
    else fill(c_light);
    rect(x, y, w, h);
    image(img, ix, iy, dW, dH);
    return false;
  }
  return false;
}

boolean ImageButton(PImage img, int x, int y, int w, int h) {
  return ImageButton(img, x, y, w, h, false);
}

boolean ImageButton(PImage img, int x, int y) {
  return ImageButton(img, x, y, s_height, s_height, false);
}

boolean ImageButton(PImage img, int x, int y, boolean select) {
  return ImageButton(img, x, y, s_height, s_height, select);
}

boolean ImageButton(PImage img, int x) {
  return ImageButton(img, x, uiStep(), s_height, s_height, false);
}

boolean ImageButton(PImage img, int x, boolean select) {
  return ImageButton(img, x, uiStep(), s_height, s_height, select);
}

boolean ImageButton(PImage img) {
  int x;
  if (_x_offs != 0) x = _x_offs;
  else x = (width-s_height)/2;
  return ImageButton(img, x, uiStep(), s_height, s_height, false);
}

boolean ImageButton(PImage img, boolean select) {
  return ImageButton(img, (width-s_height)/2, uiStep(), s_height, s_height, select);
}

// ====================================== ICON BUTTON =======================================
boolean IconButton(String icon, int x, int y, int w, int h, boolean select) {
  _prevX = x + w;
  int p=min(w, h)/9;
  int ix=x+p;
  int iy=y+p;
  int iw=min(w, h)-2*p;

  if (w > h) ix = x+(w-iw)/2;
  else if (w < h) iy = y+(h-iw)/2;

  if (mouseX >= x && mouseX <= x+w && 
    mouseY >= y && mouseY <= y+h && !_drop_open) {
    fill(c_hover);
    rect(x, y, w, h);
    Icon(icon, ix, iy, iw);
    if (clicked && canClick) {
      fill(c_light);
      rect(x, y, w, h);
      Icon(icon, ix, iy, iw);
      canClick = false;
      return true;
    }
  } else {
    if (select) fill(c_dark);
    else fill(c_light);
    rect(x, y, w, h);
    Icon(icon, ix, iy, iw);
    return false;
  }
  return false;
}

boolean IconButton(String icon, int x, int y, int w, int h) {
  return IconButton(icon, x, y, w, h, false);
}

boolean IconButton(String icon, int x, int y) {
  return IconButton(icon, x, y, s_height, s_height, false);
}

boolean IconButton(String icon, int x, int y, boolean select) {
  return IconButton(icon, x, y, s_height, s_height, select);
}

boolean IconButton(String icon, int x) {
  return IconButton(icon, x, uiStep(), s_height, s_height, false);
}

boolean IconButton(String icon, int x, boolean select) {
  return IconButton(icon, x, uiStep(), s_height, s_height, select);
}

boolean IconButton(String icon) {
  int x;
  if (_x_offs != 0) x = _x_offs;
  else x = (width-s_height)/2;
  return IconButton(icon, x, uiStep(), s_height, s_height, false);
}

boolean IconButton(String icon, boolean select) {
  return IconButton(icon, (width-s_height)/2, uiStep(), s_height, s_height, select);
}

// ====================================== TEXT INPUT =======================================
public class TextInput {
  String text = "";
  boolean active = false;
  String hint = "";
  String label = "";

  public TextInput() {
  }

  public TextInput(String t) {
    this.hint = t;
  }

  public TextInput(String t, String l) {
    this.hint = t;
    this.label = l;
  }
  
  public TextInput(String t, boolean val) {
    this.text = t;
  }

  boolean done = false;
  public boolean done() {
    if (done) {
      done = false;
      return true;
    } 
    return false;
  }

  //Text Input
  public boolean show(int x, int y, int w, int h) {
    _prevX = x + w;
    boolean in = false;
    fill(200);
    textSize(smallFontSize);
    textAlign(LEFT, BOTTOM);
    text(label, x, y-21, w, 20);
    if (active) {
      //Edit Text
      fill(c_dark);
      stroke(c_light);
      rect(x, y, w, h);
      noStroke();
      fill(c_text_color);
      textSize(medFontSize);
      textAlign(CENTER, CENTER);
      text = bufferText;
      text(text, x, y, w, h);

      if (mouseX >= x && mouseX <= x+w && 
        mouseY >= y && mouseY <= y+h && !_drop_open) {
        //Inside
      } else {
        if (clicked) {
          doneText = true;
          //canClick = true;
          active=false;
        }
      }

      if (doneText) {
        done = true;
        text = bufferText;
        active = false;
        doneText = false;
      }
    } else if (mouseX >= x && mouseX <= x+w && 
      mouseY >= y && mouseY <= y+h && !_drop_open) {
      fill(c_hover);
      rect(x, y, w, h);
      fill(c_text_color);
      textSize(medFontSize);
      textAlign(CENTER, CENTER);
      text(text, x, y, w, h);
      if (clicked && canClick) {
        fill(c_light);
        rect(x, y, w, h);
        fill(255);
        text(text, x, y, w, h);
        EditText(text);
        canClick = false;
        active = true;
        in = true;
      }
    } else {
      fill(c_light);
      stroke(c_dark);
      rect(x, y, w, h);
      fill(c_text_color);
      textSize(medFontSize);
      textAlign(CENTER, CENTER);
      text(text, x, y, w, h);
      active = false;
    }
    if (text.length() == 0) {
      fill(150);
      textSize(medFontSize);
      textAlign(CENTER, CENTER);
      text(hint, x, y, w, h);
    }
    return in;
  }

  public boolean show(int x, int y, int w) {
    return show(x, y, w, s_height);
  }

  public boolean show(int x, int w) {
    return show(x, uiStep(), w, s_height);
  }

  public boolean show(int x) {
    return show(x, uiStep(), s_med, s_height);
  }

  public boolean show() {
    int x;
    if (_x_offs != 0) x = _x_offs;
    else x = (width-s_med)/2;
    return show(x, uiStep(), s_med, s_height);
  }

  public String getText() {
    return text;
  }
}


// ====================================== CARD =======================================
//c_mid
public void beginCard(String card_title, int x, int y, int w, int h) {
  _prevX = x + w;
  noStroke();
  //Shadow
  fill(0, 0, 0, 15);
  rect(x+5, y+5, w, h);
  fill(c_light);
  rect(x, y, w, 40, 2, 2, 0, 0);
  textSize(medFontSize);
  textAlign(CENTER, CENTER);
  fill(c_text_color);
  text(card_title, x, y, w, 40);
  fill(c_mid);

  rect(x, y+40, w, h-40, 0, 0, 2, 2);

  card_h = h-40;
  card_w = w;
  card_x = x;
  card_y = y+40;
  //uiLight();
}

public void beginCard(int x, int y, int w, int h) {
  noStroke();
  fill(c_mid);

  rect(x, y, w, h);

  card_h = h;
  card_w = w;
  card_x = x;
  card_y = y;
  //uiDark();
}

public void endCard() {
  card_h = 0;
  card_w = 0;
  card_y = 0;
  card_x = 0;
}

// ====================================== TOGGLE =======================================
public class Toggle {
  public Toggle() {
  }

  boolean value = false;

  public boolean show(int x, int y, int w, int h) {
    _prevX = x + w;
    fill(c_dark);
    stroke(c_light);
    rect(x, y, w, h, h);
    noStroke();
    int pos = 0;
    if (value) pos = w-h;
    //Hover
    if (mouseX >= x && mouseX <= x+w && mouseY >= y && mouseY <= y+h && !_drop_open) {
      fill(red(c_hover), green(c_hover), blue(c_hover), 100);  
      circle(x+h/2+pos, y+h/2, h-2);
      fill(c_hover);
      circle(x+h/2+pos, y+h/2, h-s_stroke);
      if (clicked && canClick) {
        value = !value;
        canClick = false;
        return true;
      }
    } 
    //Normal
    else {
      if (value) fill(c_hover);
      else fill(c_light);
      circle(x+h/2+pos, y+h/2, h-s_stroke);
    }

    return false;
  }

  public boolean show(int x, int y) {
    return show(x, y, s_small, s_height);
  }

  public boolean show(int x) {
    return show(x, uiStep(), s_small, s_height);
  }

  public boolean show() {
    int x;
    if (_x_offs != 0) x = _x_offs;
    else x = (width-s_small)/2;
    return show(x, uiStep(), s_small, s_height);
  }

  public boolean show(String text, int x, int y, int w, int h) {
    textSize(medFontSize);
    fill(c_text_color);
    textAlign(LEFT, CENTER);
    text(text, x, y, w, h);
    int pos_x = (int)textWidth(text);
    return show(x+10+pos_x, y, s_small, s_height);
  }

  public boolean show(String text, int x, int y) {
    return show(text, x, y, s_small, s_height);
  }

  public boolean show(String text, int x) {
    return show(text, x, uiStep(), s_small, s_height);
  }

  public boolean show(String text) {
    int x;
    if (_x_offs != 0) x = _x_offs;
    else x = (width-s_small)/2;
    return show(text, x, uiStep(), s_small, s_height);
  }
}

// ====================================== RADIO BUTTON =======================================
public class RadioButton {
  public RadioButton() {
  }
  boolean value = false;

  public boolean show(int x, int y, int w) {
    _prevX = x + w;
    noStroke();
    if (value) fill(c_hover);
    else fill(c_light);

    if (mouseX >= x && mouseX <= x+w && mouseY >= y && mouseY <= y+w && !_drop_open) {  //Hover
      fill(red(c_hover), green(c_hover), blue(c_hover), 100);
      circle(x+w/2, y+w/2, w-2);
      if (value) fill(c_hover);
      else fill(c_light);
      circle(x+w/2, y+w/2, w-s_stroke);
      if (clicked && canClick) {
        value = !value;
        _item_changed = true;
        canClick = false;
        return true;
      }
    } else {
      circle(x+w/2, y+w/2, w-s_stroke);
    }
    return false;
  }

  public boolean show(int x, int y) {
    return show(x, y, s_height);
  }

  public boolean show(int x) {
    return show(x, uiStep(), s_height);
  }

  public boolean show() {
    int x;
    if (_x_offs != 0) x = _x_offs;
    else x = (width-s_height)/2;
    return show(x, uiStep(), s_height);
  }

  public boolean show(String text, int x, int y, int w) {
    textSize(medFontSize);
    fill(c_text_color);
    textAlign(LEFT, CENTER);
    text(text, x, y+w/2);
    int pos_x = (int)textWidth(text);
    return show(x+10+pos_x, y, s_height);
  }

  public boolean show(String text, int x, int y) {
    return show(text, x, y, s_height);
  }

  public boolean show(String text, int x) {
    return show(text, x, uiStep(), s_height);
  }

  public boolean show(String text) {
    return show(text, (width-s_height)/2, uiStep(), s_height);
  }
}

// ====================================== SLIDER =======================================
public class Slider {
  public Slider() {
  }

  public float value = 0;
  public int tmr = 0;
  public boolean flag = false;

  public boolean show(float min, float max, int x, int y, int w, int h) {
    _prevX = x + w;
    boolean chFlag = false;
    noStroke();
    fill(c_light);
    rect(x, y+h/2, w, 4, 2);
    value = constrain(value, min, max);
    float pos = map(value, min, max, 0, w);
    //pos = constrain(pos, min, max);
    fill(c_hover);
    rect(x, y+h/2, pos, 4, 2);

    //Hover
    if (mouseX >= x && mouseX <= x+w && mouseY >= y && mouseY <= y+h && !_drop_open) {
      fill(c_hover);
      if (mousePressed && canClick) {
        pos = mouseX;
        _item_changed = true;
        chFlag = true;
        flag = true;
        value = map(pos, x, x+w, min, max);        
        fill(red(c_hover), green(c_hover), blue(c_hover), 100);
        ellipse(pos, y+h/2, h, h); 
        fill(c_hover);
        ellipse(pos, y+h/2, h-s_stroke, h-s_stroke);
      } else {
        fill(red(c_hover), green(c_hover), blue(c_hover), 50);
        ellipse(pos+x, y+h/2, h, h); 
        fill(c_hover);
        ellipse(pos+x, y+h/2, h-s_stroke, h-s_stroke);
      }
    } 
    //Normal
    else {
      noStroke();
      fill(c_hover);
      ellipse(pos+x, y+h/2, h-s_stroke, h-s_stroke);
    }
    if ((chFlag && millis() - tmr > 50) || (!chFlag && flag)) {
      tmr = millis();
      flag = false;
      return true;
    }
    return false;
  }

  public boolean show(String label, float min, float max, int x, int y, int w, int h) {
    _prevX = x + w;
    int w2 = 0;
    textSize(medFontSize);
    float tw = textWidth(label);
    textSize(medFontSize);
    fill(255);
    textAlign(LEFT, CENTER);
    text(label, x, y, tw, h);
    w2 = (int)(w-tw-15);
    return show(min, max, (int)(tw+x+15), y, w2, h);
  }

  //Minimal Slider
  public boolean show() {
    int x;
    if (_x_offs != 0) x = _x_offs;
    else x = (width-s_height)/2;
    return show(0f, 1f, x, uiStep(), s_big, s_height);
  }

  public boolean show(int x) {
    return show(0f, 1f, x, uiStep(), s_big, s_height);
  }

  public boolean show(int x, int y) {
    return show(0f, 1f, x, y, s_big, s_height);
  }

  public boolean show(String label, int x, int y) {
    return show(label, 0f, 1f, x, y, s_big, s_height);
  }

  public boolean show(String label, int x, int y, int w, int h) {
    return show(label, 0f, 1f, x, y, w, h);
  }

  public boolean show(int x, int y, int w) {
    return show(0f, 1f, x, y, w, s_height);
  }
  public boolean show(int x, int y, int w, int h) {
    return show(0f, 1f, x, y, w, h);
  }

  public boolean show(float min, float max, int x, int y, int w) {
    return show((float) min, (float) max, x, y, w, s_height);
  }
}

// ====================================== HANDLER =======================================
void pressHandler() {
  clicked = true;
}

void releaseHandler() {
  clicked = false;
  canClick = true;
}

void keyHandler() {
  if (keyCode == BACKSPACE) {
    if (bufferText.length() > 0) {
      bufferText = bufferText.substring(0, bufferText.length()-1);
    }
  } else if (keyCode == DELETE) {
    bufferText = "";
  } else if (keyCode != SHIFT && keyCode != ENTER) {
    bufferText = bufferText + key;
  }

  if (keyCode == ' ') {
    bufferText = bufferText.substring(0, bufferText.length()-1);
    bufferText = bufferText + ' ';
  }

  if (keyCode == ENTER) {
    //input = myText;
    //bufferText = "";
    doneText = true;
  }
}

// ====================================== DROPDOWN =======================================
public class DropDown {
  private int selected = 0;
  private boolean open = false;

  public DropDown() {
  }

  public boolean show(String args[], int x, int y, int w) {
    _prevX = x + w;
    int h = s_height;
    w -= h;
    _drop_open = false;
    if (Button(args[selected], x, y, w, h) || Button("", x+w, y, h, h)) open = !open;    
    noStroke();
    if (!open) {
      triangle(x+w+h/4+1, y+0.28*h+1, x+w+h*3/4+1, y+0.28*h+1, x+w+h/2+1, y+0.71*h+1);
      stroke(c_very_dark);
    } else {
      triangle(x+w+h/4+1, y+0.71*h+1, x+w+h*3/4+1, y+0.71*h+1, x+w+h/2+1, y+0.28*h+1);
      stroke(c_very_dark);
      fill(c_light);
      rect(x+0, y+h, w+h, s_height*args.length);
      for (int i=0; i<args.length; i++) {
        if (Button(args[i], x, y + h + h*i, w+h, h)) {
          _drop_open = false;
          open = false;
          selected = i;
          canClick = false;
          return true;
        }
      }
      _drop_open = open;
    }
    return false;
  }

  public boolean show(String args[], int x, int y) {
    return show(args, x, y, maxLen(args)+medFontSize+s_height);
  }

  public boolean show(String args[], int x) {
    return show(args, x, uiStep());
  }

  public boolean show(String args[]) {
    int x;
    if (_x_offs != 0) x = _x_offs;
    else x = (width-maxLen(args)-medFontSize-s_height)/2;
    return show(args, x, uiStep());
  }

  public int getSelected() {
    return selected;
  }

  int maxLen(String args[]) {
    int w = 0;
    textSize(medFontSize);
    for (int i = 0; i < args.length; i++) {
      int tw = (int)textWidth(args[i]);
      w = max(w, tw);
    }
    return w;
  }
}


// ============================== DEPRECATED =============================
//Toggle
public boolean Toggle(boolean value, int x, int y, int w, int h) {
  _prevX = x + w;
  fill(c_dark);
  stroke(c_light);
  rect(x, y, w, h, h);
  noStroke();
  int pos = 0;
  if (value) pos = w-h;
  //Hover
  if (mouseX >= x && mouseX <= x+w && mouseY >= y && mouseY <= y+h && !_drop_open) {
    fill(red(c_hover), green(c_hover), blue(c_hover), 100);  
    circle(x+h/2+pos, y+h/2, h-2);
    fill(c_hover);
    circle(x+h/2+pos, y+h/2, h-s_stroke);
    if (clicked && canClick) {
      value = !value;
      _item_changed = true;
      canClick = false;
      return value;
    }
  } 
  //Normal
  else {
    if (value) fill(c_hover);
    else fill(c_light);
    circle(x+h/2+pos, y+h/2, h-s_stroke);
  }

  return value;
}

public boolean Toggle(boolean value, int x, int y) {
  return Toggle(value, x, y, s_small, s_height);
}

public boolean Toggle(boolean value, int x) {
  return Toggle(value, x, uiStep(), s_small, s_height);
}

public boolean Toggle(boolean value) {
  int x;
  if (_x_offs != 0) x = _x_offs;
  else x = (width-s_small)/2;
  return Toggle(value, x, uiStep(), s_small, s_height);
}

public boolean Toggle(String text, boolean value, int x, int y, int w, int h) {
  textSize(medFontSize);
  fill(c_text_color);
  textAlign(LEFT, CENTER);
  text(text, x, y, w, h);
  int pos_x = (int)textWidth(text);
  return Toggle(value, x+10+pos_x, y, s_small, s_height);
}

public boolean Toggle(String text, boolean value, int x, int y) {
  return Toggle(text, value, x, y, s_small, s_height);
}

public boolean Toggle(String text, boolean value, int x) {
  return Toggle(text, value, x, uiStep(), s_small, s_height);
}

public boolean Toggle(String text, boolean value) {
  int x;
  if (_x_offs != 0) x = _x_offs;
  else x = (width-s_small)/2;
  return Toggle(text, value, x, uiStep(), s_small, s_height);
}


//Toggle
public boolean RadioButton(boolean value, int x, int y, int w) {
  _prevX = x + w;
  noStroke();
  if (value) fill(c_hover);
  else fill(c_light);

  if (mouseX >= x && mouseX <= x+w && mouseY >= y && mouseY <= y+w && !_drop_open) {  //Hover
    fill(red(c_hover), green(c_hover), blue(c_hover), 100);
    circle(x+w/2, y+w/2, w-2);
    if (value) fill(c_hover);
    else fill(c_light);
    circle(x+w/2, y+w/2, w-s_stroke);
    if (clicked && canClick) {
      value = !value;
      _item_changed = true;
      canClick = false;
      return value;
    }
  } else {
    circle(x+w/2, y+w/2, w-s_stroke);
  }
  return value;
}

public boolean RadioButton(boolean value, int x, int y) {
  return RadioButton(value, x, y, s_height);
}

public boolean RadioButton(boolean value, int x) {
  return RadioButton(value, x, uiStep(), s_height);
}

public boolean RadioButton(boolean value) {
  int x;
  if (_x_offs != 0) x = _x_offs;
  else x = (width-s_height)/2;
  return RadioButton(value, x, uiStep(), s_height);
}

public boolean RadioButton(String text, boolean value, int x, int y, int w) {
  textSize(medFontSize);
  fill(c_text_color);
  textAlign(LEFT, CENTER);
  text(text, x, y+w/2);
  int pos_x = (int)textWidth(text);
  return RadioButton(value, x+10+pos_x, y, s_height);
}

public boolean RadioButton(String text, boolean value, int x, int y) {
  return RadioButton(text, value, x, y, s_height);
}

public boolean RadioButton(String text, boolean value, int x) {
  return RadioButton(text, value, x, uiStep(), s_height);
}

public boolean RadioButton(String text, boolean value) {
  return RadioButton(text, value, (width-s_height)/2, uiStep(), s_height);
}

//Basic Slider from 0f to 1f
public float Slider(float min, float max, float value, int x, int y, int w, int h) {
  _prevX = x + w;
  noStroke();
  fill(c_light);
  rect(x, y+h/2, w, 4, 2);
  float pos = map(value, min, max, 0, w);
  fill(c_hover);
  rect(x, y+h/2, pos, 4, 2);

  //Hover
  if (mouseX >= x && mouseX <= x+w && mouseY >= y && mouseY <= y+h && !_drop_open) {
    fill(c_hover);
    if (mousePressed && canClick) {
      pos = mouseX;
      _item_changed = true;
      value = map(pos, x, x+w, min, max);
      fill(red(c_hover), green(c_hover), blue(c_hover), 100);
      ellipse(pos, y+h/2, h, h); 
      fill(c_hover);
      ellipse(pos, y+h/2, h-s_stroke, h-s_stroke);
    } else {
      fill(red(c_hover), green(c_hover), blue(c_hover), 50);
      ellipse(pos+x, y+h/2, h, h); 
      fill(c_hover);
      ellipse(pos+x, y+h/2, h-s_stroke, h-s_stroke);
    }
  } 
  //Normal
  else {
    noStroke();
    fill(c_hover);
    ellipse(pos+x, y+h/2, h-s_stroke, h-s_stroke);
  }

  return value;
}

public float Slider(String label, float min, float max, float value, int x, int y, int w, int h) {
  _prevX = x + w;
  int w2 = 0;
  textSize(medFontSize);
  float tw = textWidth(label);
  textSize(medFontSize);
  fill(255);
  textAlign(LEFT, CENTER);
  text(label, x, y, tw, h);
  w2 = (int)(w-tw-15);
  return Slider(min, max, value, (int)(tw+x+15), y, w2, h);
}

//Minimal Slider
public float Slider(float value) {
  int x;
  if (_x_offs != 0) x = _x_offs;
  else x = (width-s_height)/2;
  return Slider(0f, 1f, value, x, uiStep(), s_big, s_height);
}

public float Slider(float value, int x) {
  return Slider(0f, 1f, value, x, uiStep(), s_big, s_height);
}

public float Slider(float value, int x, int y) {
  return Slider(0f, 1f, value, x, y, s_big, s_height);
}

public float Slider(String label, float value, int x, int y) {
  return Slider(label, 0f, 1f, value, x, y, s_big, s_height);
}

public float Slider(String label, float value, int x, int y, int w, int h) {
  return Slider(label, 0f, 1f, value, x, y, w, h);
}

public float Slider(float value, int x, int y, int w) {
  return Slider(0f, 1f, value, x, y, w, s_height);
}
public float Slider(float value, int x, int y, int w, int h) {
  return Slider(0f, 1f, value, x, y, w, h);
}

public float Slider(float min, int max, int value, int x, int y, int w, int h) {
  return Slider((float) min, (float) max, value, x, y, w, h);
}
public float Slider(float min, int max, int value, int x, int y, int w) {
  return Slider((float) min, (float) max, value, x, y, w, s_height);
}
