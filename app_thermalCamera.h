#ifndef APP_THERMALCAMERA_H_
#define APP_THERMALCAMERA_H_













void app_thermalCamera_Initialize() {
  if (!amg.begin()) {
    tft->setTextColor(COLOUR_ERROR, BLACK);
    tft->println("Error Loading Thermal Sensor!");
    tft->setTextColor(COLOUR_DEBUG, BLACK);
    while (1);
  }
  tft->println("Thermal Sensor Loaded");
  delay(250);
}

void app_thermalCamera_load() {
  loadPalette(currentSetting);
  tft->setTextSize(1);
  tft->setCursor(2, 29);
  tft->print("Center Image Temp: ");

  // create colour legend boarder 13x145 pixels in size
  tft->drawFastHLine(157, 54, 16, WHITE);
  tft->drawFastHLine(157, 200, 16, WHITE);
  tft->drawFastVLine(156, 55, 145, WHITE);
  tft->drawFastVLine(173, 55, 145, WHITE);

  for (byte nVal = 56; nVal < 199; nVal++) {
    uint8_t colorIndex = map(nVal, 198, 56, 0, 255);
    colorIndex = constrain(colorIndex, 0, 255);
    tft->drawFastHLine(158, nVal, 14, camColors[colorIndex]);
  }

  app_thermalCamera_reload();
}


void app_thermalCamera_loop() {

}


void app_thermalCamera_reload() {
  int centerTemp = 0;
  //read all the pixels
  amg.readPixels(pixels);
  extrapolateTempRange();
  //
  tft->setCursor(150, 29);
  centerTemp = (pixels[28] + pixels[29] + pixels[37] + pixels[38]);
  if (centerTemp==0){
    tft->print("--");
  }
  else {
    tft->print( String( int( centerTemp  / 4.0  ) ));
    tft->println("c");
  }
  //tft->fillRect(162, 46, 8, 8, BLACK);
  tft->setTextSize(1);
  tft->setCursor(159, 46);
  tft->println( String(MAXTEMP));
  // write Min Temp
  //tft->fillRect(162, 20, 8, 198, BLACK);
  tft->setCursor(159, 203);
  tft->println( String(MINTEMP));
  float dest_2d[INTERPOLATED_ROWS * INTERPOLATED_COLS];

  //interpolate_image(newPixels, 15, 15, dest_2d, INTERPOLATED_ROWS, INTERPOLATED_COLS);
  interpolate_image(pixels, 8, 8, dest_2d, INTERPOLATED_ROWS, INTERPOLATED_COLS);
  uint16_t boxsize = min(150 / INTERPOLATED_COLS, 160 / INTERPOLATED_COLS);
  drawpixels(dest_2d, INTERPOLATED_ROWS, INTERPOLATED_COLS, boxsize, boxsize);
}

void app_thermalCamera_input() {

}


void extrapolateTempRange() {
  uint8_t max = 0.0;
  uint8_t min = 50.0;
  for (uint8_t i = 0; i < 64; i++) {
    if (pixels[i] > max) max = pixels[i];
    if (pixels[i] < min) min = pixels[i];
  }
  maxTempSamples[tempCounter] = max;
  minTempSamples[tempCounter] = min;
  tempCounter++;
  max = 0.0;
  min = 50.0;
  if (tempCounter == 5 || tempCounter == 10 || tempCounter == 15 || tempCounter == 20) {
    for (uint8_t i = 0; i < 20; i++) {
      if (maxTempSamples[i] > max) {
        max = maxTempSamples[i];
      }
      if (minTempSamples[i] < min) {
        min = minTempSamples[i];
      }
    }
    MINTEMP = (int)min - 5;
    MAXTEMP = (int)max + 5;
  }
  if (tempCounter == 20) {
    tempCounter = 0;
  }
}

void interpolatePixels() {
  /*set pixel(o) for every second pixel in every second row from the first
    oxoxoxoxoxoxoxo
    xxxxxxxxxxxxxxx
    oxoxoxoxoxoxoxo*/
  int inc = 0;
  for (byte a = 0 ; a < 8; a++) {
    for (byte b = 0 ; b < 8; b++) {
      newPixels[b + (a * 15) + inc] = pixels[b + (a * 8)];
      inc++;
    }
    inc = inc + 7;
  }
  for (byte c = 1; c < 15; c = c + 2 ) {
    for (byte d = 1; d < 15; d = d + 2 ) {
      /*set avg value for every second value in every second row from the second(o)
        OxOxOxOxOxOxOxO
        xoxoxoxoxoxoxox
        OxOxOxOxOxOxOxO*/
      newPixels[d + (c * 15)] = ((newPixels[d + (c * 15) - 16] + newPixels[d + (c * 15) - 14] + newPixels[d + (c * 15) + 16] + newPixels[d + (c * 15) + 14]) / 4.0);
    }
  }

  for (byte i = 1; i < 225; i = i + 2 ) {
    /*set every remaining pixel to avg of the pixels around it(o)
      OoOoOoOoOoOoOoO
      oOoOoOoOoOoOoOo
      OoOoOoOoOoOoOoO*/
    if (i < 15) { // use left right and bottom pixel
      newPixels[i] = ((newPixels[i - 1] + newPixels[i + 1] + newPixels[i + 15]) / 3.0);
    }
    else if (i % 15 == 0) { // use top, bottom and right pixel
      newPixels[i] = ((newPixels[i - 1] + newPixels[i + 1] + newPixels[i - 15]) / 3.0);
    }
    else if (i % 15 == 14) { // use top, bottom and left pixel
      newPixels[i] = ((newPixels[i + 15] + newPixels[i - 15] + newPixels[i - 1]) / 3.0);
    }
    else if (i > 209) { // use left, right and top pixel
      newPixels[i] = ((newPixels[i - 1] + newPixels[i + 1] + newPixels[i - 15]) / 3.0);
    }
    else { //use, top, bottom, left and right pixel
      newPixels[i] = ((newPixels[i - 15] + newPixels[i + 15] + newPixels[i - 1] + newPixels[i + 1]) / 4.0);
    }
  }
}




#endif /*  APP_THERMALCAMERA_H_ */