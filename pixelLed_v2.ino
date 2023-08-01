
/* 7/20/23
 *Pasar codigo a platformio
 *eliminar lo que no se usa (mantener lo necesario para el efecto 3)
 *hacer tareas con flag FOA 
 *
 *deseado:
 *        PODER CONTROLAR EL CENTRO DEL EFECTO 3
 *        QUE CAMBIE DE COLOR AL DETECTAR COLORES DE GENTE
 *        mostrar sonrisa?
 *        
 * 7/25/23
 *auentar zona de efecto del color
 *hacer una transicion natural?
 *NO HACE FALTA AUMENAR EL VAL EN EL CENTRO DEL EFECTO YA
 *AL DETECTAR SONRISA TIENE QUE BRILLAR MAS
 *  
 */



#include <HTTPClient.h>
#include <WiFi.h>


#include <FastLED.h>
#include "config.h"
#include "frame.h"

int efectoIdx=3;
int efectoIdxLast=0;
int br=BRIGHTNESS;
int newBr=br;
int sp=5;
int globalHue=0;
int color=0;
int parametro_val3=50;

CRGB leds[NUM_LEDS];
//CHSV leds[NUM_LEDS];

uint8_t frameBuffer[FRAME_BUFFER_SIZE][3];

int bufferToPixeledBase(int x, int y) {
  int index=y*20+x;
  return frame2PixelLedMap[index];
}

int getLienzoH(int x, int y) {
  int index=y*20+x;
  return lienzoHSV[index][0];
}
int getLienzoS(int x, int y) {
  int index=y*20+x;
  return lienzoHSV[index][1];
}
int getLienzoV(int x, int y) {
  int index=y*20+x;
  return lienzoHSV[index][2];
}

int getLienzoH2(int x, int y) {
  int index=y*20+x;
  return lienzoHSV2[index][0];
}
int getLienzoS2(int x, int y) {
  int index=y*20+x;
  return lienzoHSV2[index][1];
}
int getLienzoV2(int x, int y) {
  int index=y*20+x;
  return lienzoHSV2[index][2];
}




void IRAM_ATTR leerControl();

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  //FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  
  FastLED.setBrightness( br );
  FastLED.clear();
  FastLED.show(); // display this frame
  delay (200);
  clearFrameBuffer();
  /*
  // SOLO PARA TESTEO DE LOS LEDS
  int tleds=210;
  while (1) {
    for (int n=0; n<tleds; n++) {
      leds[n].setRGB (255, 255, 255);
      FastLED.show();
      delay(10);
  
    }
    for (int n=0; n<tleds; n++) {
      leds[n].setRGB (0, 0, 0);
      FastLED.show();
      delay(10);
    }
  }
  */
  
  xTaskCreatePinnedToCore(
                    updateLedsTask,   /* Function to implement the task */
                    "updateLedsTask", /* Name of the task */
                    10000,      /* Stack size in words */
                    NULL,       /* Task input parameter */
                    0,          /* Priority of the task */
                    NULL,       /* Task handle. */
                    0);  /* Core where the task should run */
  
  pinMode (pRFDATA, INPUT_PULLUP);
  delay(5);
  attachInterrupt(pRFDATA, leerControl, CHANGE);

  Serial.print ("WIFI: ");
  //setupWifiEvents();
  //initWifi();
  Serial.println ();

  
  Serial.println ("START!");
}
unsigned int hue=0;
unsigned int baseHue=0;


void loop() {
  /*while (1) {
    HTTPClient http;
    if (WiFi.status() == WL_CONNECTED) {
      http.begin("http://www.coding-arts.com/pixelled/test.png");
        int httpCode = http.GET();

      // httpCode will be negative on error
      if(httpCode > 0) {
          if(httpCode == HTTP_CODE_OK) {
              String payload = http.getString();
              Serial.println(payload);
          }
      } else {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    
    }
  delay (5000);
  }
  */

  while (1) {
   
    if (efectoIdx==0)
      efecto0();
    else if (efectoIdx==1)
      efecto1();
    else if (efectoIdx==2)
      efecto2();
    else if (efectoIdx==3)
      efecto3();
    else if (efectoIdx==4)
      efecto4();
    else if (efectoIdx==5)
      efecto5();
    else if (efectoIdx==6)
      efecto6();
    else if (efectoIdx==7)
      efecto7();
  }
}
void serialCheck() {
   if (Serial.available()) {
      char ch=Serial.read();
      Serial.println(ch);
      if (ch=='0')
        efectoIdx=0;
      else if (ch=='1')
        efectoIdx=1;
      else if (ch=='2')
        efectoIdx=2;
      else if (ch=='3')
        efectoIdx=3;
      else if (ch=='4')
        efectoIdx=4;
      else if (ch=='5')
        efectoIdx=5;
      else if (ch=='6')
        efectoIdx=6;
      else if (ch=='7')
        efectoIdx=7;
      else if (ch=='+')
        subirSpeed();
      else if (ch=='-')
        bajarSpeed();
      else if (ch=='q')
        subirBrillo();
      else if (ch=='a')
        bajarBrillo();
      else if (ch=='w')
        subirColor();
      else if (ch=='s')
        bajarColor();  
      else if (ch=='k'){
        parametro_val3 += 10;
        Serial.println(parametro_val3);
      }
      else if (ch=='l'){
        parametro_val3 -= 10;
        Serial.println(parametro_val3);        
      }      
   }
}

void subirSpeed() {
  sp-=1;
  if (sp <0)
    sp=0;
  Serial.print ("SP: "); Serial.println (sp);
}
void bajarSpeed() {
  sp+=1;
  if (sp >10)
    sp=10;
  Serial.print ("SP: "); Serial.println (sp);
}

void subirBrillo() {
  int b=newBr+10;
  if (b > BRIGHTNESS)
    b=BRIGHTNESS;
  newBr=b;
}

void bajarBrillo() {
  int b=newBr-10;
  if (b < 10)
    b=10;
  newBr=b;
}

void subirColor() {
    color++;
    if (color>5)
      color=0;
}

void bajarColor() {
    color--;
    if (color<0)
      color=5;
}


void ledTest2(unsigned long color) {
  for (int l=0; l<NUM_LEDS/3; l+=9) {
    leds[l] = color;
    leds[l+1] = color;
    leds[l+2] = color;
    leds[l+3] = color;
    leds[l+4] = color;
    leds[l+5] = color;
    leds[l+6] = color;
    leds[l+7] = color;
    leds[l+8] = color;
  }
  
  for (int n=0; n<256; n++) {
    FastLED.setBrightness( n );
    FastLED.show(); // display this frame
    delay(5);
  }
  
  for (int n=255; n>=0; n--) {
    FastLED.setBrightness( n );
    FastLED.show(); // display this frame
    delay(5);
  }
}
//----------------------------
void fillBack(unsigned long color) {
  for (int l=0; l<NUM_LEDS; l++) {
    leds[l] = color;
  }
  //FastLED.show(); // display this frame
}


//----------------------------
void ledTest3(unsigned long color, int nLeds) {
  int l=0;
  for (l=0; l<nLeds; l++) {
    leds[l] = color;
  }
  for (; l<NUM_LEDS; l++) {
    leds[l] = 0X000000;
  }

  
  br=BRIGHTNESS;
  
  FastLED.setBrightness( br );
  
  FastLED.show(); // display this frame
  /*
  br++;
  if (br > BRIGHTNESS) {
    br=0;
    Serial.println (millis());
  }*/
  
}


void ledTest(bool dir) {
  int d=0;
  FastLED.clear();
  FastLED.show();

  Serial.println ("BLANCO");
  leds[39]=0x101010;
  leds[40]=0x505050;
  leds[41]=0x101010;
  FastLED.show();
  FastLED.delay(1000);

  
  Serial.println ("AZUL");
  leds[39]=0x000040;
  leds[40]=0x0000FF;
  leds[41]=0x000040;
  FastLED.show();
  FastLED.delay(1000);


    Serial.println ("ROJO");
  leds[39]=0x400000;
  leds[40]=0xFF0000;
  leds[41]=0x400000;
  FastLED.show();
  FastLED.delay(1000);

  Serial.println ("VERDE");

  leds[39]=0x004000;
  leds[40]=0x00FF00;
  leds[41]=0x004000;
  FastLED.show();
  FastLED.delay(1000);

  Serial.println ("");

  Serial.println ("AMARILLO");
  leds[39]=0x404000;
  leds[40]=0xFFFF00;
  leds[41]=0x404000;
  FastLED.show();
  FastLED.delay(1000);
 
  /*
  for (int l=0; l<NUM_LEDS; l+=3) {
    
    FastLED.clear();
    FastLED.show(); // display this frame
     leds[l] = 0xFFFFFF;
    leds[l+1] = 0xFFFFFF;
    leds[l+2] = 0xFFFFFF;
    FastLED.show(); // display this frame
    FastLED.delay(100);
  }
  */
}