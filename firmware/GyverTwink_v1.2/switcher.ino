void switchEffect() {
  // поиск favourite эффектов
  while (true) {
    if (cfg.randomChange) currentEffect = random(0, ACTIVE_PALETTES * 2);
    else if (++currentEffect >= ACTIVE_PALETTES * 2) currentEffect = 0;
    if (effects[currentEffect].favourite) break;
  }
  DEBUG("[Switch] Switching effect to ");
  DEBUGLN(currentEffect);
}
