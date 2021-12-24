/***
 * Переключить эффект (авто-режим)
 */
void switchEffect() {
  // поиск favorite эффектов
  while (true) {
    if (cfg.rndCh) curEff = random(0, ACTIVE_PALETTES * 2);
    else {
      if (++curEff >= ACTIVE_PALETTES * 2) curEff = 0;
    }
    if (effs[curEff].fav) break;
  }
  DEBUG("[AutoMode] Switching effect to ");
  DEBUGLN(curEff);
}
