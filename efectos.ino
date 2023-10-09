
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
  //####Serial.println("EFECTO 0");

  while (efectoIdx == 0)  {
    colorPleno(0, 0, 0);

    delay(50);
    serialCheck();
  }
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
  int medioSegundo = 0;

  //inicializacion variables brillo - PRUEBA
  int brillop = 50;
  int cantidadRebotes = 0;
  bool bandera_brillo = false;
  bool bandera_brillo_intermedio = false;
  bool bandera_brillo_max = false;

  //inicializacion variables saturacion dinamica
  int diferenciaEstadosSaturacion = 0;
  bool tipoTransicion; //determina si hay que restar o sumar para llegar al determinado valor de saturacion
  int cantidadManejada = 5;

  //inicializacion variables timer saturacion
  unsigned long tiempoCambioSaturacion = millis();
  int cuantoTiempoCambioSaturacion = 5; //5


  //inicializacion variables timer frame
  unsigned long tiempoCambioFrame = millis();
  int cuantoTiempoCambioFrame = 100; //

  borrar();
  //####Serial.println("EFECTO 3");
  //gravo los valores del array constante al array prueba
  for (int n = 0; n < 400; n++) {
    lienzoHSV2[n][0] = lienzoHSV[n][0];
    lienzoHSV2[n][1] = lienzoHSV[n][1];
    lienzoHSV2[n][2] = lienzoHSV[n][2];

  }

  //inicio el bucle del efecto
  while (efectoIdx == 3) {
    if (enable_serial == true)
    {
      digitalWrite(2, HIGH);
    }
    if (enable_serial == false)
    {
      digitalWrite(2, LOW);
    }
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
    if (millis() - relojMaquinaEstadosEfectos > 1000)
    {
      cuenta_relojMaquinaEstadosEfectos++;
      relojMaquinaEstadosEfectos = millis();
    }
    if (maquina == estadoInicial)
    {
      maquina = humanoDetectado;
    }
    if (maquina == humanoDetectado)
    {
      //Serial.println("Buscando Humano");
      if (humanoPrecionado == true)
      {
        //Serial.println("humano detectado");
        colorActual = NINGUNO;
        colorAnterior = NINGUNO;
        on_humanoDetectado = true;
        humanoPrecionado = false;
        cuenta_relojMaquinaEstadosEfectos = 0;
      }
      if (off_humanoDetectado == true)
      {
        off_humanoDetectado = false;
      }
    }
    if (maquina == estadoColor )
    {
      if (cuenta_relojMaquinaEstadosEfectos >= tiempoRecepcionDeColor)
      {
        maquina = humanoDetectado;
        humanoPrecionado = true;
      }
      if (colorActual == ROJO)
      {
        cuentaEfecto3 = HUE_RED;
        //Serial.println("rojo");
        cuentaEspectro = 0;
        contadorSaturacionColores = 0;
        flag_timeAfterSerial_efecto3 = true;
        cantidadColorAumentado = 2;
        maquina = timerColor;
      }
      if (colorActual == NARANJA)
      {
        cuentaEfecto3 = HUE_ORANGE;
        //Serial.println("naranja");
        cuentaEspectro = 0;
        contadorSaturacionColores = 0;
        flag_timeAfterSerial_efecto3 = true;
        cantidadColorAumentado = 5;
        maquina = timerColor;
      }
      if (colorActual == AMARILLO)
      {
        cuentaEfecto3 = HUE_YELLOW ;
        //Serial.println("amarillo");
        cuentaEspectro = 0;
        contadorSaturacionColores = 0;
        flag_timeAfterSerial_efecto3 = true;
        cantidadColorAumentado = 5;
        maquina = timerColor;
      }
      if (colorActual == VERDE)
      {
        cuentaEfecto3 = HUE_GREEN ;
        //Serial.println("verde");
        cuentaEspectro = 0;
        contadorSaturacionColores = 0;
        flag_timeAfterSerial_efecto3 = true;
        cantidadColorAumentado = 2;
        maquina = timerColor;
      }
      if (colorActual == AZUL)
      {
        cuentaEfecto3 = HUE_BLUE ;
        //Serial.println("azul");
        cuentaEspectro = 0;
        contadorSaturacionColores = 0;
        cantidadColorAumentado = 2;
        flag_timeAfterSerial_efecto3 = true;
        maquina = timerColor;
      }
      if (colorActual == PURPURA)
      {
        cuentaEfecto3 = HUE_PURPLE + 10 ;
        //Serial.println("violeta");
        cuentaEspectro = 0;
        contadorSaturacionColores = 0;
        cantidadColorAumentado = 2;
        flag_timeAfterSerial_efecto3 = true;
        maquina = timerColor;
      }
      if (colorActual == ROSA)
      {
        cuentaEfecto3 = HUE_PINK;
        //Serial.println("rosa");
        cuentaEspectro = 0;
        contadorSaturacionColores = 0;
        cantidadColorAumentado = 2;
        flag_timeAfterSerial_efecto3 = true;
        maquina = timerColor;
      }
      //####Serial.println(cuenta_relojMaquinaEstadosEfectos );

      if (humanoPrecionado == true)
      {
        maquina = humanoDetectado;
      }
      colorAnterior = colorActual;
    }
    if ((maquina == timerColor) && (off_efectoColor == true))
    {
      colorPrecionado = false;
      enable_serial = true;
      if (rebotePrecionado == true)
      {
        off_efectoColor = false;
        maquina = estadoRebote;
        enable_serial = false;
      }
      if (sonrisaPrecionado == true)
      {
        off_efectoColor = false;
        maquina = estadoSonrisa;
        enable_serial = false;
      }
      if ((colorPrecionado == true) && (colorActual != colorAnterior))
      {
        off_efectoReboteSaturacion = false;
        maquina = estadoColor;
        enable_serial = false;
        Serial.println("color");
      }
      if (humanoPrecionado == true)
      {
        maquina = humanoDetectado;
        off_efectoColor = false;
      }
    }
    if (maquina == estadoRebote)
    {
      //Serial.println("rebotee");
      on_efectoReboteSaturacion = true;
      maquina = timerRebote;
    }
    if ((maquina == timerRebote) && (off_efectoReboteSaturacion == true))
    {
      rebotePrecionado = false;
      enable_serial = true;
      if ((colorPrecionado == true) && (colorActual != colorAnterior))
      {
        off_efectoReboteSaturacion = false;
        maquina = estadoColor;
        enable_serial = false;
      }
      if (sonrisaPrecionado == true)
      {
        off_efectoReboteSaturacion = false;
        maquina = estadoSonrisa;
        enable_serial = false;
      }
      if (humanoPrecionado == true)
      {
        off_efectoReboteSaturacion = false;
        maquina = humanoDetectado;
      }
    }
    if (maquina == estadoSonrisa)
    {
      //Serial.println("sonrisaaa");
      on_efectoSonrisa = true;
      maquina = timerSonrisa;
    }
    if ((maquina == timerSonrisa) && (off_efectoSonrisa == true))
    {
      sonrisaPrecionado = false;
      enable_serial = true;
      if (colorPrecionado == true && (colorActual != colorAnterior))
      {
        off_efectoSonrisa = false;
        maquina = estadoColor;
        enable_serial = false;
      }
      if (rebotePrecionado == true)
      {
        off_efectoSonrisa = false;
        maquina = estadoRebote;
        enable_serial = false;
      }
      if (humanoPrecionado == true)
      {
        off_efectoSonrisa = false;
        maquina = humanoDetectado;
      }
    }



    /*
      Reloj cambio color
    */

    //FastLED.show();
    if (millis() - effectTimer > 2000) { //10

      if (cuentaEfecto3 >= 249) {  //250
        cuentaEfecto3 = 0;
      }
      /*
        utilizado para efecto de color, cambio el tiempo cada cuanto cambia el color
      */
      if ((flag_timeAfterSerial_efecto3 == false) && (on_efectoReboteSaturacion == false)) {
        cuentaEfecto3 = cuentaEfecto3 + 10;
      }
      if (flag_timeAfterSerial_efecto3 == true)
      {
        cuentaEspectro++;
      }

      if (cuentaEspectro >= 2)
      {
        off_efectoColor = true;
        flag_timeAfterSerial_efecto3 = false;
        cuentaEspectro = 0;
      }
      effectTimer = millis();
      //####Serial.print("cuenta efecto es:");
      //####Serial.println(cuentaEfecto3);
    }
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    /*
      Reloj maneja los frames de la animacion del efecto expansion
    */
    if (millis() - tiempoCambioFrame > cuantoTiempoCambioFrame) { //10
      medioSegundo++;
      if ((on_efectoReboteSaturacion == true) && (medioSegundo >= 2))
      {
        if ((flag_efectoReboteSaturacion == false) && (saturacion_actual >= 12))
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
          off_efectoReboteSaturacion = true;
          on_efectoReboteSaturacion = false;
          flag_efectoReboteSaturacion = false;
        }
        medioSegundo = 0;
      }
      tiempoCambioFrame = millis();
    }
    //---------------------------------------------------------------------------------------------------------------------------------------------------------
    /*
      Reloj cambio saturacion
    */
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

    //efecto rebote
    switch (saturacion_actual) {
      case 12:
        memoria_saturacion_10 = 220;
        memoria_saturacion_9 = 220;
        memoria_saturacion_8 = 220;
        memoria_saturacion_7 = 220;
        memoria_saturacion_6 = 220;
        memoria_saturacion_5 = 240;
        memoria_saturacion_4 = 240;
        memoria_saturacion_3 = 140;
        memoria_saturacion_2 = 60;
        memoria_saturacion_1 = 0;
        //####Serial.print("frame 12, count:");
        //####Serial.println(saturacion_actual);
        break;
      case 11:
        memoria_saturacion_10 = 200;
        memoria_saturacion_9 = 200;
        memoria_saturacion_8 = 220;
        memoria_saturacion_7 = 220;
        memoria_saturacion_6 = 220;
        memoria_saturacion_5 = 240;
        memoria_saturacion_4 = 240;
        memoria_saturacion_3 = 140;
        memoria_saturacion_2 = 60;
        memoria_saturacion_1 = 0;
        //####Serial.print("frame 11, count:");
        //####Serial.println(saturacion_actual);
        break;
      case 10:
        memoria_saturacion_10 = 190;
        memoria_saturacion_9 = 190;
        memoria_saturacion_8 = 210;
        memoria_saturacion_7 = 220;
        memoria_saturacion_6 = 220;
        memoria_saturacion_5 = 240;
        memoria_saturacion_4 = 240;
        memoria_saturacion_3 = 140;
        memoria_saturacion_2 = 60;
        memoria_saturacion_1 = 0;
        //####Serial.print("frame 10, count:");
        //####Serial.println(saturacion_actual);
        break;
      case 9:
        memoria_saturacion_10 = 160;
        memoria_saturacion_9 = 160;
        memoria_saturacion_8 = 190;
        memoria_saturacion_7 = 220;
        memoria_saturacion_6 = 220;
        memoria_saturacion_5 = 240;
        memoria_saturacion_4 = 240;
        memoria_saturacion_3 = 140;
        memoria_saturacion_2 = 60;
        memoria_saturacion_1 = 0;
        //####Serial.print("frame 9, count:");
        //####Serial.println(saturacion_actual);
        break;
      case 8:
        memoria_saturacion_10 = 160;
        memoria_saturacion_9 = 160;
        memoria_saturacion_8 = 190;
        memoria_saturacion_7 = 220;
        memoria_saturacion_6 = 220;
        memoria_saturacion_5 = 240;
        memoria_saturacion_4 = 240;
        memoria_saturacion_3 = 140;
        memoria_saturacion_2 = 60;
        memoria_saturacion_1 = 0;
        //####Serial.print("frame 8, count:");
        //####Serial.println(saturacion_actual);
        break;
      case 7:
        memoria_saturacion_10 = 150;
        memoria_saturacion_9 = 150;
        memoria_saturacion_8 = 180;
        memoria_saturacion_7 = 220;
        memoria_saturacion_6 = 220;
        memoria_saturacion_5 = 240;
        memoria_saturacion_4 = 240;
        memoria_saturacion_3 = 140;
        memoria_saturacion_2 = 60;
        memoria_saturacion_1 = 0;
        //####Serial.print("frame 7, count:");
        //####Serial.println(saturacion_actual);
        break;
      case 6:
        memoria_saturacion_10 = 130;
        memoria_saturacion_9 = 130;
        memoria_saturacion_8 = 150;
        memoria_saturacion_7 = 180;
        memoria_saturacion_6 = 220;
        memoria_saturacion_5 = 240;
        memoria_saturacion_4 = 240;
        memoria_saturacion_3 = 140;
        memoria_saturacion_2 = 60;
        memoria_saturacion_1 = 0;
        //####Serial.print("frame 6, count:");
        //####Serial.println(saturacion_actual);
        break;
      case 5:
        memoria_saturacion_10 = 20;
        memoria_saturacion_9 = 40;
        memoria_saturacion_8 = 130;
        memoria_saturacion_7 = 150;
        memoria_saturacion_6 = 190;
        memoria_saturacion_5 = 230;
        memoria_saturacion_4 = 230;
        memoria_saturacion_3 = 140;
        memoria_saturacion_2 = 60;
        memoria_saturacion_1 = 0;
        //####Serial.print("frame 5, count:");
        //####Serial.println(saturacion_actual);
        break;
      case 4:
        memoria_saturacion_10 = 20;
        memoria_saturacion_9 = 40;
        memoria_saturacion_8 = 60;
        memoria_saturacion_7 = 130;
        memoria_saturacion_6 = 190;
        memoria_saturacion_5 = 230;
        memoria_saturacion_4 = 230;
        memoria_saturacion_3 = 140;
        memoria_saturacion_2 = 60;
        memoria_saturacion_1 = 0;
        //####Serial.print("frame 4, count:");
        //####Serial.println(saturacion_actual);
        break;
      case 3:
        memoria_saturacion_10 = 20;
        memoria_saturacion_9 = 40;
        memoria_saturacion_8 = 60;
        memoria_saturacion_7 = 180;
        memoria_saturacion_6 = 200;
        memoria_saturacion_5 = 220;
        memoria_saturacion_4 = 220;
        memoria_saturacion_3 = 140;
        memoria_saturacion_2 = 60;
        memoria_saturacion_1 = 0;
        //####Serial.print("frame 3, count:");
        //####Serial.println(saturacion_actual);
        break;
        break;
      case 2:
        memoria_saturacion_10 = 10;
        memoria_saturacion_9 = 30;
        memoria_saturacion_8 = 50;
        memoria_saturacion_7 = 70;
        memoria_saturacion_6 = 200;
        memoria_saturacion_5 = 220;
        memoria_saturacion_4 = 220;
        memoria_saturacion_3 = 140;
        memoria_saturacion_2 = 60;
        memoria_saturacion_1 = 0;
        //####Serial.print("frame 2, count:");
        //####Serial.println(saturacion_actual);
        break;
      case 1:
        memoria_saturacion_10 = 20;
        memoria_saturacion_9 = 40;
        memoria_saturacion_8 = 60;
        memoria_saturacion_7 = 90;
        memoria_saturacion_6 = 130;
        memoria_saturacion_5 = 200;
        memoria_saturacion_4 = 200;
        memoria_saturacion_3 = 140;
        memoria_saturacion_2 = 60;
        memoria_saturacion_1 = 0;
        if (on_efectoReboteSaturacion == true)
        {
          //####Serial.print("frame 1, count:");
          //####Serial.println(saturacion_actual);
        }
        break;
      default:
        //####Serial.println("termino el efecto rebote");
        saturacion_actual = 1;
        break;
    }




    for (int n = 0; n < 400; n++) {
      int h = lienzoHSV2[n][0];
      int s = lienzoHSV2[n][1];
      int v = lienzoHSV2[n][2];
      /*
        Cuenta efecto 3 funciona como el margen en el cual los leds no cambian
      */
      int Radio_s = valorRadialLeds[n];
      /*
        radio_s sirve para bajar el valor de color de los pixeles del radio los cuales son mas fuertes
        confValor11=130
      */
      if (Radio_s == 11)
      {
        lienzoHSV2[n][2] = confValor11;
      }

      if (flag_timeAfterSerial_efecto3 == true)
      {
        if (contadorSaturacionColores <= 20) {
          for (int n = 0; n < 400; n++)
          {
            if (lienzoHSV2[n][1] <= 240)
            {
              lienzoHSV2[n][1] = lienzoHSV2[n][1] + cantidadColorAumentado;
            }
          }
          contadorSaturacionColores++;
        }
      }
      if (h < cuentaEfecto3 - 2 || h > cuentaEfecto3 + 3)
      {
        if (flag_timeAfterSerial_efecto3 == true)
        {
          lienzoHSV2[n][0] = lienzoHSV2[n][0] + 4;
        }
        if (flag_timeAfterSerial_efecto3 == false)
        {
          lienzoHSV2[n][0] = lienzoHSV2[n][0] + 1;
          lienzoHSV2[n][1] = lienzoHSV[n][1];
        }
      }
      if (on_efectoReboteSaturacion == true) {
        int Radio = valorRadialLeds[n];
        switch (Radio) {
          case 11:
            lienzoHSV2[n][1] = saturacion_10 + random(0, 2);
            break;
          case 10:
            lienzoHSV2[n][1] = saturacion_10 + random(0, 2);
            break;
          case 9:
            lienzoHSV2[n][1] = saturacion_9 + random(0, 2);
            break;
          case 8:
            lienzoHSV2[n][1] = saturacion_8 + random(0, 2);
            break;
          case 7:
            lienzoHSV2[n][1] = saturacion_7 + random(0, 2);
            break;
          case 6:
            lienzoHSV2[n][1] = saturacion_6 + random(0, 2);
            break;
          case 5:
            lienzoHSV2[n][1] = saturacion_5 + random(0, 2);
            break;
          case 4:
            lienzoHSV2[n][1] = saturacion_4 + random(0, 2);
            break;
          case 3:
            lienzoHSV2[n][1] = saturacion_3 + random(0, 2);
            break;
          case 2:
            lienzoHSV2[n][1] = saturacion_2 + random(0, 2);
            break;
          case 1:
            lienzoHSV2[n][1] = saturacion_1 + random(0, 2);
            break;
        }
      }
    }
    //sp es la velocidad global
    delay (sp * 10);
    if (on_humanoDetectado == true)
    {
      //####Serial.println(brillop);
      if (flag_brilloMaximo_humanoDetectado == false)
      {
        brillop = brillop + 10;
      }
      if ((brillop >= valor_envioCharFlash ) && (flag_brilloMaximo_humanoDetectado == false))
      {
        if (flag_envioCharFlash == true)
        {
          flag_envioCharFlash = false;
          Serial.print('+');
          maquina = estadoColor;
          //####Serial.print("++++++++++++++++++++++++++++++++++++++++++++");
        }
      }
      if ((flag_brilloMaximo_humanoDetectado == false) && (brillop >= 240))
      {
        flag_brilloMaximo_humanoDetectado = true;
      }
      if (flag_brilloMaximo_humanoDetectado == true)
      {
        brillop = brillop - 10;
      }
      if ((flag_brilloMaximo_humanoDetectado == true) && (brillop <= BRIGHTNESS))
      {
        brillop = BRIGHTNESS;
        flag_brilloMaximo_humanoDetectado = false;
        on_humanoDetectado = false;
        off_humanoDetectado = true;
        flag_envioCharFlash = true;
      }
    }
    if (on_efectoSonrisa == true)
    {
      if ((bandera_brillo == false) && (bandera_brillo_max == false))
      {
        brillop = brillop + 10;
      }
      if ((brillop >= 250) && (bandera_brillo == false))
      {
        bandera_brillo_max = true;
        bandera_brillo_intermedio = false;
      }
      if (bandera_brillo_max == true)
      {
        if (bandera_brillo_intermedio == false)
        {
          brillop = brillop - 10;
        }
        if ((brillop <= 100) && (bandera_brillo_intermedio == false))
        {
          bandera_brillo_intermedio = true;
        }
        if (bandera_brillo_intermedio == true)
        {
          brillop = brillop + 10;
        }
        if ((brillop >= 250) && (bandera_brillo_intermedio == true))
        {
          bandera_brillo_intermedio = false;
          cantidadRebotes++;
          if (cantidadRebotes >= 2)
          {
            bandera_brillo_max = false;
            bandera_brillo = true;
            cantidadRebotes = 0;
          }
        }
      }
      if (bandera_brillo == true)
      {
        brillop = brillop - 5;
      }
      if ((brillop <= BRIGHTNESS) && (bandera_brillo == true))
      {
        on_efectoSonrisa = false;
        bandera_brillo = false;
        off_efectoSonrisa = true;
        //####Serial.println("por favorrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr");
      }

    }
    newBr = brillop;
    serialCheck();
  }
}
