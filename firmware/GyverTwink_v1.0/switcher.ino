void switchEff() {
  // поиск favorite эффектов
  while (true) {
    if (cfg.rndCh) curEff = random(0, ACTIVE_PALETTES * 2);
    else {
      if (++curEff >= ACTIVE_PALETTES * 2) curEff = 0;
    }
    if (effs[curEff].fav) break;
  }
  DEBUG("switch to: ");
  DEBUGLN(curEff);
}
