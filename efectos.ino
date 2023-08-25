
void updatePixel(int pixel, int hue, int sat, int val) {
  if (val < 0)
    val = 0;
  else if (val > 255)
    val = 255;
  if (pixel >= 0 && pixel < 400) {
    frameBuffer[pixel][0] = hue;
    frameBuffer[pixel][1] = sat;
    frameBuffer[pixel][2] = val;
  }
}

void colorPleno(uint8_t hue = 0, uint8_t sat = 0, uint8_t val = 0  ) {
  for (int y = 0; y < 20; y++) {
    for (int x = 0; x < 20; x++) {
      int l = getBufferIndex(x, y);
      updatePixel(l, hue, sat, val);
    }
  }

}

void borrar() {
  uint8_t hue = 200;
  uint8_t sat = 200;
  uint8_t val = 200;
  for (int y = 0; y < 20; y++) {
    for (int x = 0; x < 20; x++) {
      int l = getBufferIndex(x, y);
      hue = getLienzoH(x, y);
      sat = getLienzoS(x, y);
      val = getLienzoV(x, y) / 2;
      updatePixel(l, hue, sat, val);
    }
  }

}

// APAGADO
void efecto0() {
  Serial.println("EFECTO 0");

  while (efectoIdx == 0)  {
    colorPleno(0, 0, 0);

    delay(50);
    serialCheck();
  }
}



void efecto1() {
  Serial.println("EFECTO 1");

  while (efectoIdx == 1) {
    if (color == 0)
      borrar();
    else if (color == 1)
      colorPleno (255, 0, 255);
    else {
      colorPleno(50 * color, 100, 100);
    }
    delay(50);

    serialCheck();
  }
}


void efecto2() {
  uint8_t hue = 0;
  uint8_t sat = 0;
  uint8_t val = 0;
  int count = 0;
  Serial.println("EFECTO 2");

  while (efectoIdx == 2) {
    for (int y = 0; y < 20; y++) {
      for (int x = 0; x < 20; x++) {
        int l = getBufferIndex(x, y);
        hue = getLienzoH(x, y);
        sat = getLienzoS(x, y);
        val = getLienzoV(x, y) / 2;
        updatePixel(l, hue, sat, val);
      }
    }
    newBr = ((BRIGHTNESS) * cubicwave8(count)) / 255;

    count++;
    delay (sp * 10);
    serialCheck();
  }
  newBr = BRIGHTNESS;
}


// LIENZO ROTAR
/* flag= FOA
   encontrar una manera de aumentar y disminur el centro
   reconocimiento de color por camara y saltar a la parte de ese color del efecto

*/
void efecto3() {
  //inicializacion variables cromaticas
  uint8_t hue = 0;
  uint8_t sat = 0;
  uint8_t val = 0;
  //inicializacion variables timer
  unsigned long effectTimer = millis();
  int medioSegundo=0;

  //inicializacion variables brillo - PRUEBA
  int brillop = 5;
  bool bandera_brillo = false;

  //inicializacion variables saturacion dinamica
  int diferenciaEstadosSaturacion = 0;
  bool tipoTransicion; //determina si hay que restar o sumar para llegar al determinado valor de saturacion
  int cantidadManejada = 5;

  //inicializacion variables timer saturacion
  unsigned long tiempoCambioSaturacion = millis();
  int cuantoTiempoCambioSaturacion = 5;

  borrar();
  Serial.println("EFECTO 3");
  //gravo los valores del array constante al array prueba
  for (int n = 0; n < 400; n++) {
    int s = lienzoHSV2[n][1];

    lienzoHSV2[n][0] = lienzoHSV[n][0];

    lienzoHSV2[n][1] = lienzoHSV[n][1];

    lienzoHSV2[n][2] = lienzoHSV[n][2];

    int Radio = valorRadialLeds[n];
    switch (Radio) {
      case 10:
        lienzoHSV2[n][1] = saturacion_10;
        lienzoHSV2[n][2] = 250;
        break;
      case 9:
        lienzoHSV2[n][1] = saturacion_9;
        break;
      case 8:
        lienzoHSV2[n][1] = saturacion_8;
        break;
      case 7:
        lienzoHSV2[n][1] = saturacion_7;
        break;
      case 6:
        lienzoHSV2[n][1] = saturacion_6;
        break;
      case 5:
        lienzoHSV2[n][1] = saturacion_5;
        break;
      case 4:
        lienzoHSV2[n][1] = saturacion_4;
        break;
      case 3:
        lienzoHSV2[n][1] = saturacion_3;
        break;
      case 2:
        lienzoHSV2[n][1] = saturacion_2;
        break;
      case 1:
        lienzoHSV2[n][1] = saturacion_1;
        break;
    }

  }

  //inicio el bucle del efecto
  while (efectoIdx == 3) {
    for (int y = 0; y < 20; y++) {
      for (int x = 0; x < 20; x++) {
        int l = getBufferIndex(x, y);

        hue = getLienzoH2(x, y);
        sat = getLienzoS2(x, y);
        val = getLienzoV2(x, y);

        if (val >= 180) {
          val = 180;
        }
        updatePixel(l, hue, sat, val);

      }
    }


    //FastLED.show();
    if (millis() - effectTimer > 100) { //10
      medioSegundo++;
      if ((enable_efectoReboteSaturacion == true) && (medioSegundo >= 5))
      {
        if ((flag_efectoReboteSaturacion == false) && (saturacion_actual >= 6))
        {
          flag_efectoReboteSaturacion = true;
        }
        if (flag_efectoReboteSaturacion == false)
        {
          saturacion_actual++;
        }
        if (flag_efectoReboteSaturacion == true)
        {
          saturacion_actual--;
        }
        if ((flag_efectoReboteSaturacion == true) && (saturacion_actual <= 0))
        {
          Serial.println("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
          enable_efectoReboteSaturacion = false;
          flag_efectoReboteSaturacion = false;
          saturacion_actual = 0;
        }
        medioSegundo = 0;
      }

      Serial.print("la etapa del efecto saturacion es: ");
      Serial.println(saturacion_actual);
      Serial.print("el enable del efecto saturacion esta en: ");
      Serial.println(enable_efectoReboteSaturacion);

      cuentaEspectro++;
      effectTimer = millis();
      if (flag_timeAfterSerial_efecto3 == false) {
        cuentaEfecto3 = cuentaEfecto3 + 1;
      }

      if (cuentaEfecto3 >= 247) {  //250
        cuentaEfecto3 = 0;
      }
      if (cuentaEspectro >= 40)
      {
        flag_timeAfterSerial_efecto3 = false;
      }
    }
    //efecto rebote
    if (enable_efectoReboteSaturacion == true)
    {
      switch (saturacion_actual) {
        case 1:
          memoria_saturacion_10 = 250;
          memoria_saturacion_9 = 250;
          memoria_saturacion_8 = 250;
          memoria_saturacion_7 = 250;
          memoria_saturacion_6 = 250;
          memoria_saturacion_5 = 250;
          memoria_saturacion_4 = 250;
          memoria_saturacion_3 = 250;
          memoria_saturacion_2 = 10;
          memoria_saturacion_1 = 10;
          break;
        case 2:
          memoria_saturacion_10 = 90;
          memoria_saturacion_9 = 100;
          memoria_saturacion_8 = 100;
          memoria_saturacion_7 = 130;
          memoria_saturacion_6 = 150;
          memoria_saturacion_5 = 190;
          memoria_saturacion_4 = 220;
          memoria_saturacion_3 = 250;
          memoria_saturacion_2 = 10;
          memoria_saturacion_1 = 10;
          break;
        case 3:
          memoria_saturacion_10 = 10;
          memoria_saturacion_9 = 10;
          memoria_saturacion_8 = 20;
          memoria_saturacion_7 = 130;
          memoria_saturacion_6 = 150;
          memoria_saturacion_5 = 190;
          memoria_saturacion_4 = 220;
          memoria_saturacion_3 = 250;
          memoria_saturacion_2 = 10;
          memoria_saturacion_1 = 10;
          break;
        case 4:
          memoria_saturacion_10 = 10;
          memoria_saturacion_9 = 10;
          memoria_saturacion_8 = 20;
          memoria_saturacion_7 = 30;
          memoria_saturacion_6 = 150;
          memoria_saturacion_5 = 190;
          memoria_saturacion_4 = 220;
          memoria_saturacion_3 = 250;
          memoria_saturacion_2 = 10;
          memoria_saturacion_1 = 10;
          break;
        case 5:
          memoria_saturacion_10 = 10;
          memoria_saturacion_9 = 10;
          memoria_saturacion_8 = 20;
          memoria_saturacion_7 = 30;
          memoria_saturacion_6 = 40;
          memoria_saturacion_5 = 190;
          memoria_saturacion_4 = 220;
          memoria_saturacion_3 = 250;
          memoria_saturacion_2 = 10;
          memoria_saturacion_1 = 10;
          break;
        case 6:
          memoria_saturacion_10 = 0;
          memoria_saturacion_9 = 0;
          memoria_saturacion_8 = 0;
          memoria_saturacion_7 = 0;
          memoria_saturacion_6 = 0;
          memoria_saturacion_5 = 0;
          memoria_saturacion_4 = 0;
          memoria_saturacion_3 = 0;
          memoria_saturacion_2 = 10;
          memoria_saturacion_1 = 10;
          break;
        default:
          Serial.println("termino el efecto rebote");
          break;
      }
    }


    for (int n = 0; n < 400; n++) {
      int h = lienzoHSV2[n][0];
      int s = lienzoHSV2[n][1];
      int v = lienzoHSV2[n][2];

      if (cuentaEfecto3 == 0) {
        lienzoHSV2[n][0] = lienzoHSV2[n][0] + random(0, 1);
        int Radio = valorRadialLeds[n];
        switch (Radio) {
          case 10:
            lienzoHSV2[n][1] = saturacion_10 + random(0, 2);
            break;
          case 9:
            lienzoHSV2[n][1] = saturacion_9 + random(0, 2);
            lienzoHSV2[n][2] = lienzoHSV[n][2] - random(0, 3) - 15;
            break;
          case 8:
            lienzoHSV2[n][1] = saturacion_8 + random(0, 2);
            lienzoHSV2[n][2] = lienzoHSV[n][2] - random(0, 3) - 15;
            break;
          case 7:
            lienzoHSV2[n][1] = saturacion_7 + random(0, 2);
            lienzoHSV2[n][2] = lienzoHSV[n][2] - random(0, 3) - 15;
            break;
          case 6:
            lienzoHSV2[n][1] = saturacion_6 + random(0, 2);
            lienzoHSV2[n][2] = lienzoHSV[n][2] - random(0, 2) - 15;
            break;
          case 5:
            lienzoHSV2[n][1] = saturacion_5 + random(0, 2);
            lienzoHSV2[n][2] = lienzoHSV[n][2] - random(0, 3) - 15;
            break;
          case 4:
            lienzoHSV2[n][1] = saturacion_4 + random(0, 2);
            lienzoHSV2[n][2] = lienzoHSV[n][2] - random(0, 3) - 15;
            break;
          case 3:
            lienzoHSV2[n][1] = saturacion_3 + random(0, 2);
            lienzoHSV2[n][2] = lienzoHSV[n][2] + random(0, 3);
            break;
          case 2:
            lienzoHSV2[n][1] = saturacion_2 + random(0, 2);
            break;
          case 1:
            lienzoHSV2[n][1] = saturacion_1 + random(0, 2);
            break;
        }
      }
      else if (h < cuentaEfecto3 || h > cuentaEfecto3 + 3)
      {
        lienzoHSV2[n][0] = lienzoHSV2[n][0] + random(0, 3);
        int Radio = valorRadialLeds[n];
        switch (Radio) {
          case 10:
            lienzoHSV2[n][1] = saturacion_10 + random(0, 2);
            break;
          case 9:
            lienzoHSV2[n][1] = saturacion_9 + random(0, 2);
            lienzoHSV2[n][2] = lienzoHSV[n][2] - random(0, 3) - 15;
            break;
          case 8:
            lienzoHSV2[n][1] = saturacion_8 + random(0, 2);
            lienzoHSV2[n][2] = lienzoHSV[n][2] - random(0, 3) - 15;
            break;
          case 7:
            lienzoHSV2[n][1] = saturacion_7 + random(0, 2);
            lienzoHSV2[n][2] = lienzoHSV[n][2] - random(0, 3) - 15;
            break;
          case 6:
            lienzoHSV2[n][1] = saturacion_6 + random(0, 2);
            lienzoHSV2[n][2] = lienzoHSV[n][2] - random(0, 2) - 15;
            break;
          case 5:
            lienzoHSV2[n][1] = saturacion_5 + random(0, 2);
            lienzoHSV2[n][2] = lienzoHSV[n][2] - random(0, 3) - 15;
            break;
          case 4:
            lienzoHSV2[n][1] = saturacion_4 + random(0, 2);
            lienzoHSV2[n][2] = lienzoHSV[n][2] - random(0, 3) - 15;
            break;
          case 3:
            lienzoHSV2[n][1] = saturacion_3 + random(0, 2);
            lienzoHSV2[n][2] = lienzoHSV[n][2] + random(0, 3);
            break;
          case 2:
            lienzoHSV2[n][1] = saturacion_2 + random(0, 2);
            break;
          case 1:
            lienzoHSV2[n][1] = saturacion_1 + random(0, 2);
            break;
        }
      }


      if (millis() - tiempoCambioSaturacion > cuantoTiempoCambioSaturacion) { //10
        tiempoCambioSaturacion = millis();

        if (memoria_saturacion_10 > saturacion_10)
        {
          saturacion_10 = saturacion_10 + cantidadManejada;
        }
        if (memoria_saturacion_9 > saturacion_9)
        {
          saturacion_9 = saturacion_9 + cantidadManejada;
        }
        if (memoria_saturacion_8 > saturacion_8)
        {
          saturacion_8 = saturacion_8 + cantidadManejada;
        }
        if (memoria_saturacion_7 > saturacion_7)
        {
          saturacion_7 = saturacion_7 + cantidadManejada;
        }
        if (memoria_saturacion_6 > saturacion_6)
        {
          saturacion_6 = saturacion_6 + cantidadManejada;
        }
        if (memoria_saturacion_5 > saturacion_5)
        {
          saturacion_5 = saturacion_5 + cantidadManejada;
        } if (memoria_saturacion_4 > saturacion_4)
        {
          saturacion_4 = saturacion_4 + cantidadManejada;
        }
        if (memoria_saturacion_3 > saturacion_3)
        {
          saturacion_3 = saturacion_3 + cantidadManejada;
        }
        if (memoria_saturacion_2 > saturacion_2)
        {
          saturacion_2 = saturacion_2 + cantidadManejada;
        }
        if (memoria_saturacion_1 > saturacion_1)
        {
          saturacion_1 = saturacion_1 + cantidadManejada;
        }
        if (memoria_saturacion_10 < saturacion_10)
        {
          saturacion_10 = saturacion_10 - cantidadManejada;
        }
        if (memoria_saturacion_9 < saturacion_9)
        {
          saturacion_9 = saturacion_9 - cantidadManejada;
        }
        if (memoria_saturacion_8 < saturacion_8)
        {
          saturacion_8 = saturacion_8 - cantidadManejada;
        }
        if (memoria_saturacion_7 < saturacion_7)
        {
          saturacion_7 = saturacion_7 - cantidadManejada;
        }
        if (memoria_saturacion_6 < saturacion_6)
        {
          saturacion_6 = saturacion_6 - cantidadManejada;
        }
        if (memoria_saturacion_5 < saturacion_5)
        {
          saturacion_5 = saturacion_5 - cantidadManejada;
        }
        if (memoria_saturacion_4 < saturacion_4)
        {
          saturacion_4 = saturacion_4 - cantidadManejada;
        }
        if (memoria_saturacion_3 < saturacion_3)
        {
          saturacion_3 = saturacion_3 - cantidadManejada;
        }
        if (memoria_saturacion_2 < saturacion_2)
        {
          saturacion_2 = saturacion_2 - cantidadManejada;
        }
        if (memoria_saturacion_1 < saturacion_1)
        {
          saturacion_1 = saturacion_1 - cantidadManejada;
        }

      }
    }
    //sp es la velocidad global
    delay (sp * 10);
    /* if(bandera_brillo == false)
       {
         brillop=brillop+1;
       }
       if(brillop>=180)
       {
         bandera_brillo = true;
       }
       if(bandera_brillo == true)
       {
         brillop=brillop-1;
       }
       if(brillop<=30)
       {
         bandera_brillo = false;
       }

       newBr=brillop;
    */
    serialCheck();

  }
}




// CIRCCULOS LATIDO
void efecto4() {
  //inicializacion variables cromaticas
  uint8_t hue = 0;
  uint8_t sat = 0;
  uint8_t val = 0;
  //inicializacion variables timer
  unsigned long effectTimer = millis();

  //inicializacion variables brillo - PRUEBA
  int brillop = 5;
  bool bandera_brillo = false;

  //inicializacion variables saturacion dinamica
  int diferenciaEstadosSaturacion = 0;
  bool tipoTransicion; //determina si hay que restar o sumar para llegar al determinado valor de saturacion
  int cantidadManejada = 5;

  //inicializacion variables timer saturacion
  unsigned long tiempoCambioSaturacion = millis();
  int cuantoTiempoCambioSaturacion = 5;

  borrar();
  Serial.println("EFECTO 4");
  //gravo los valores del array constante al array prueba
  for (int n = 0; n < 400; n++) {
    int s = lienzoHSV2[n][1];

    lienzoHSV2[n][0] = lienzoHSV[n][0];

    lienzoHSV2[n][1] = lienzoHSV[n][1];

    lienzoHSV2[n][2] = lienzoHSV[n][2];

    int Radio = valorRadialLeds[n];
    switch (Radio) {
      case 10:
        lienzoHSV2[n][1] = saturacion_10;
        lienzoHSV2[n][2] = 250;
        break;
      case 9:
        lienzoHSV2[n][1] = saturacion_9;
        break;
      case 8:
        lienzoHSV2[n][1] = saturacion_8;
        break;
      case 7:
        lienzoHSV2[n][1] = saturacion_7;
        break;
      case 6:
        lienzoHSV2[n][1] = saturacion_6;
        break;
      case 5:
        lienzoHSV2[n][1] = saturacion_5;
        break;
      case 4:
        lienzoHSV2[n][1] = saturacion_4;
        break;
      case 3:
        lienzoHSV2[n][1] = saturacion_3;
        break;
      case 2:
        lienzoHSV2[n][1] = saturacion_2;
        break;
      case 1:
        lienzoHSV2[n][1] = saturacion_1;
        break;
    }

  }

  //inicio el bucle del efecto
  while (efectoIdx == 4) {
    for (int y = 0; y < 20; y++) {
      for (int x = 0; x < 20; x++) {
        int l = getBufferIndex(x, y);

        hue = getLienzoH2(x, y);
        sat = getLienzoS2(x, y);
        val = getLienzoV2(x, y);

        if (val >= 180) {
          val = 180;
        }
        updatePixel(l, hue, sat, val);

      }
    }
    for (int n = 0; n < 400; n++) {
      int h = lienzoHSV2[n][0];
      int s = lienzoHSV2[n][1];
      int v = lienzoHSV2[n][2];

      lienzoHSV2[n][0] = cuentaEfecto3;
      int Radio = valorRadialLeds[n];
      switch (Radio) {
        case 10:
          lienzoHSV2[n][1] = saturacion_10;
          break;
        case 9:
          lienzoHSV2[n][1] = saturacion_9;
          break;
        case 8:
          lienzoHSV2[n][1] = saturacion_8;
          break;
        case 7:
          lienzoHSV2[n][1] = saturacion_7;
          break;
        case 6:
          lienzoHSV2[n][1] = saturacion_6;
          break;
        case 5:
          lienzoHSV2[n][1] = saturacion_5;
          break;
        case 4:
          lienzoHSV2[n][1] = saturacion_4;
          break;
        case 3:
          lienzoHSV2[n][1] = saturacion_3;
          break;
        case 2:
          lienzoHSV2[n][1] = saturacion_2;
          break;
        case 1:
          lienzoHSV2[n][1] = saturacion_1;
          break;
      }
      if (millis() - tiempoCambioSaturacion > cuantoTiempoCambioSaturacion) { //10
        tiempoCambioSaturacion = millis();

        if (memoria_saturacion_10 > saturacion_10)
        {
          saturacion_10 = saturacion_10 + cantidadManejada;
        }
        if (memoria_saturacion_9 > saturacion_9)
        {
          saturacion_9 = saturacion_9 + cantidadManejada;
        }
        if (memoria_saturacion_8 > saturacion_8)
        {
          saturacion_8 = saturacion_8 + cantidadManejada;
        }
        if (memoria_saturacion_7 > saturacion_7)
        {
          saturacion_7 = saturacion_7 + cantidadManejada;
        }
        if (memoria_saturacion_6 > saturacion_6)
        {
          saturacion_6 = saturacion_6 + cantidadManejada;
        }
        if (memoria_saturacion_5 > saturacion_5)
        {
          saturacion_5 = saturacion_5 + cantidadManejada;
        } if (memoria_saturacion_4 > saturacion_4)
        {
          saturacion_4 = saturacion_4 + cantidadManejada;
        }
        if (memoria_saturacion_3 > saturacion_3)
        {
          saturacion_3 = saturacion_3 + cantidadManejada;
        }
        if (memoria_saturacion_2 > saturacion_2)
        {
          saturacion_2 = saturacion_2 + cantidadManejada;
        }
        if (memoria_saturacion_1 > saturacion_1)
        {
          saturacion_1 = saturacion_1 + cantidadManejada;
        }
        if (memoria_saturacion_10 < saturacion_10)
        {
          saturacion_10 = saturacion_10 - cantidadManejada;
        }
        if (memoria_saturacion_9 < saturacion_9)
        {
          saturacion_9 = saturacion_9 - cantidadManejada;
        }
        if (memoria_saturacion_8 < saturacion_8)
        {
          saturacion_8 = saturacion_8 - cantidadManejada;
        }
        if (memoria_saturacion_7 < saturacion_7)
        {
          saturacion_7 = saturacion_7 - cantidadManejada;
        }
        if (memoria_saturacion_6 < saturacion_6)
        {
          saturacion_6 = saturacion_6 - cantidadManejada;
        }
        if (memoria_saturacion_5 < saturacion_5)
        {
          saturacion_5 = saturacion_5 - cantidadManejada;
        }
        if (memoria_saturacion_4 < saturacion_4)
        {
          saturacion_4 = saturacion_4 - cantidadManejada;
        }
        if (memoria_saturacion_3 < saturacion_3)
        {
          saturacion_3 = saturacion_3 - cantidadManejada;
        }
        if (memoria_saturacion_2 < saturacion_2)
        {
          saturacion_2 = saturacion_2 - cantidadManejada;
        }
        if (memoria_saturacion_1 < saturacion_1)
        {
          saturacion_1 = saturacion_1 - cantidadManejada;
        }

      }
      //se ejecuta una vez cuando la saturacion_actual cambia

    }
    //sp es la velocidad global
    delay (sp * 10);
    serialCheck();

  }

}


//LLUVIA
void efecto5() {
  Serial.println("EFECTO 5");
  colorPleno ();
  int x = random(0, 20);

  int hue = 10;
  int sat = 100;
  int val = 200;
  int maxVal = 200;
  int cola = 6;
  //int y=0;
  while (efectoIdx == 5) {
    x = random(0, 20);
    colorPleno ();
    for (int y = 0; y < 26; y++) {
      val = maxVal;
      for (int c = 0; c < cola; c++) {
        if (y - c >= 0) {
          int l = getBufferIndex(x, y - c);
          //int l=getBufferIndex(x, yb);

          updatePixel(l, hue, sat, val);
          val = val - (maxVal / (cola));
          if (val < 0)
            val = 0;
        }
      }
      delay (sp * 20);
      serialCheck();
      if (efectoIdx != 5)
        break;
    }
    serialCheck();

    delay(1000);
  }
}
// ESPIRAL
void efecto5b() {
  int pixels = 155;
  int hue = 30;
  int sat = 255;
  int val = 0;
  int head = 0;
  while (efectoIdx == 5) {
    Serial.println("EFECTO 5");
    borrar();
    for (int p = 0; p < pixels; p++) {
      if (efectoIdx != 5)
        break;
      val = 255;
      for (int h = 0; h < 8; h++) {

        int n = p - h;
        if (n < 0)
          break;
        int x = secEspiral[n][0];
        int y = secEspiral[n][1];
        Serial.println (n);
        //int l=bufferToPixeledBase(x, y);

        int l = getBufferIndex(x, y);
        hue = getLienzoH(x, y);
        sat = getLienzoS(x, y);
        updatePixel(l, hue, sat, val);
        val = val - 40;
        if (val < 0)
          val = 0;

      }
      delay (sp * 10);
      serialCheck();

    }
  }

}

void efecto6() {
  int pixels = 318;
  int hue = 30;
  int sat = 255;
  int val = 0;
  int head = 0;
  while (efectoIdx == 6) {
    Serial.println("EFECTO 6");
    borrar();
    for (int p = 0; p < pixels; p++) {
      if (efectoIdx != 6)
        break;
      val = 255;
      for (int h = 0; h < 8; h++) {

        int n = p - h;
        if (n < 0)
          break;
        int x = secCuadrado[n][0];
        int y = secCuadrado[n][1];
        Serial.println (n);
        int l = getBufferIndex(x, y);
        hue = getLienzoH(x, y);
        sat = getLienzoS(x, y);
        updatePixel(l, hue, sat, val);
        val = val - 40;
        if (val < 0)
          val = 0;

      }
      delay (sp * 10);
      serialCheck();

    }
  }

}


// ESPIRAL CUADRADO
void efecto99() {
  int pixels = 155; //318
  uint8_t hue = 30;
  uint8_t sat = 255;
  int val = 0;
  while (efectoIdx == 6) {
    Serial.println("EFECTO 6");
    FastLED.clear();
    for (int n = 0; n < pixels; n++) {
      if (efectoIdx != 5)
        break;
      int x = secEspiral[n][0];
      int y = secEspiral[n][1];
      val = 0;
      while (val < 255) {
        int l = getBufferIndex(x, y);
        hue = getLienzoH(x, y);
        sat = getLienzoS(x, y);
        updatePixel(l, hue, sat, val);
        val += 25;
        delay (sp);
        serialCheck();

      }
    }
  }
}


// PIXEL RANDOM
void efecto7() {
  uint8_t hue, sat, val;
  uint8_t hueval, centroHue = 0;
  Serial.println("EFECTO 7");
  borrar();
  while (efectoIdx == 7) {
    centroHue = random(0, 256);
    for (int n = 0; n < 255; n++) {

      for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 20; x++) {
          int l = getBufferIndex(x, y);
          hue = getLienzoH(x, y);
          sat = getLienzoS(x, y);
          val = getLienzoV(x, y);

          if (hue > centroHue - 80 && hueval < centroHue + 80) {

            hue = hue + n;

          }

          updatePixel(l, hue, sat, val);
        }
      }
      delay(sp * 10);
      serialCheck();

    }
  }
}
