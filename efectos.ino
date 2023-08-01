
void updatePixel(int pixel, int hue, int sat, int val) {
  if (val<0)
    val=0;
  else if (val >255)
    val=255;
  if (pixel>=0 && pixel < 400) {  
    frameBuffer[pixel][0]=hue;
    frameBuffer[pixel][1]=sat;
    frameBuffer[pixel][2]=val;
  }
}

void colorPleno(uint8_t hue=0, uint8_t sat=0, uint8_t val=0  ) {
    for (int y=0; y<20; y++) {
      for (int x=0; x<20; x++) {
        int l=getBufferIndex(x, y);
        updatePixel(l, hue, sat, val);
      }
   }
  
}

void borrar() {
  uint8_t hue=200;
  uint8_t sat=200;
  uint8_t val=200;
    for (int y=0; y<20; y++) {
      for (int x=0; x<20; x++) {
        int l=getBufferIndex(x, y);
        hue=getLienzoH(x,y);
        sat=getLienzoS(x,y);
        val=getLienzoV(x,y)/2;
        updatePixel(l, hue, sat, val);
      }
   }
  
}

// APAGADO
void efecto0() {
    Serial.println("EFECTO 0"); 

  while (efectoIdx==0)  {
    colorPleno(0,0,0);
    
    delay(50);
    serialCheck();
  }
}



void efecto1() {
  Serial.println("EFECTO 1"); 

  while (efectoIdx==1) {
    if (color==0)
      borrar();
    else if (color==1)
      colorPleno (255,0,255);
    else {
      colorPleno(50*color,100,100);
    }
    delay(50);
    
    serialCheck();
  }
}


void efecto2() {
  uint8_t hue=0;
  uint8_t sat=0;
  uint8_t val=0;
  int count=0;
    Serial.println("EFECTO 2"); 
  
  while (efectoIdx==2) {
    for (int y=0; y<20; y++) {
      for (int x=0; x<20; x++) {
        int l=getBufferIndex(x, y);
        hue=getLienzoH(x,y);
        sat=getLienzoS(x,y);
        val=getLienzoV(x,y)/2;
        updatePixel(l, hue, sat, val);
      }
   }
  newBr=((BRIGHTNESS)*cubicwave8(count))/255;
  
  count++;
  delay (sp*10);
  serialCheck();
  }
  newBr=BRIGHTNESS;
}


// LIENZO ROTAR
/* flag= FOA
 * encontrar una manera de aumentar y disminur el centro
 * reconocimiento de color por camara y saltar a la parte de ese color del efecto
 * 
 */
void efecto3() {
  //SACAR LO QUE DA MARRON
  uint8_t hue=0;
  int sat=0;
  int val=0;
  unsigned long effectTimer=millis();
  int count=0;
  borrar();
  Serial.println("EFECTO 3"); 

  for (int n=0; n<400; n++) {
    int s=lienzoHSV2[n][1];
    
    lienzoHSV2[n][0]=lienzoHSV[n][0];
    
    lienzoHSV2[n][1]=lienzoHSV[n][1];
    
    lienzoHSV2[n][2]=lienzoHSV[n][2];

    int Radio=valorRadialLeds[n];



  }
  while (efectoIdx==3) {
    for (int y=0; y<20; y++) {
      for (int x=0; x<20; x++) {
        int l=getBufferIndex(x, y);

        hue=getLienzoH2(x,y);
        sat=getLienzoS2(x,y);
        val=getLienzoV2(x,y);

        if (val >= 180) {       
           val=180;
        }
        updatePixel(l, hue, sat, val);

      }
   }

  
  //FastLED.show();
  if (millis()-effectTimer > 100) { //10000
    effectTimer=millis();
    count++;
    if (count>=250)
      count=0;
      
    Serial.println(count);
  }
  for (int n=0; n<400; n++) {
    int h=lienzoHSV2[n][0];
    int s=lienzoHSV2[n][1];
    int v=lienzoHSV2[n][2];
    int b=count ; //20
    
    if (count == 0) {
      lienzoHSV2[n][0]=lienzoHSV2[n][0]+3;
    }
    else if (h<b || h > b+10)
    {
      lienzoHSV2[n][0]=lienzoHSV2[n][0]+3;        
    }
    //se encarga de cambiar el hue
    /*if(v <= parametro_val3){
      lienzoHSV2[n][2]=5;
    }*/
    //aumenta el val "agujero"
        switch(Radio){
      case 10:
        lienzoHSV2[n][1]=10;
        break;
      case 9:     
        lienzoHSV2[n][1]=10;
        break;
      case 8:     
        lienzoHSV2[n][1]=120;
        break;
      case 7:     
        lienzoHSV2[n][1]=150;
        break;        
      case 6:     
        lienzoHSV2[n][1]=170;
        break;
      case 5:     
        lienzoHSV2[n][1]=210;
        break;
      case 4:     
        lienzoHSV2[n][1]=250;
        break;
      case 3:     
        lienzoHSV2[n][1]=250;
        break;
      case 2:     
        lienzoHSV2[n][1]=250;
        break;
      case 1:     
        lienzoHSV2[n][1]=10;
        break;   
    }
    
  }
  //sp es la velocidad global
  //delay (sp*10);
  serialCheck();

  }
}




// CIRCCULOS LATIDO
void efecto4() {
  
  uint8_t frame0[4][2]={{9,9},{10,9},{9,10},{10,10}};
  uint8_t frame1[8][2]={{9,8},{10,8},{8,9},{11,9},{8,10},{11,10},{9,11},{10,11}};
  uint8_t frame2[12][2]={{9,7},{10,7},{8,8},{11,8},{7,9},{12,9},{7,10},{12,10},{8,11},{11,11},{9,12},{10,12}};
  uint8_t frame3[20][2]={{9,6},{10,6},{7,7},{8,7},{11,7},{12,7},{7,8},{12,8},{6,9},{13,9},{6,10},{13,10},{7,11},{12,11},{7,12},{8,12},{11,12},{12,12},{9,13},{10,13}};
  uint8_t frame4[32][2]={{8,5},{9,5},{10,5},{11,5},{7,6},{8,6},{11,6},{12,6},{6,7},{13,7},{5,8},{6,8},{13,8},{14,8},{5,9},{14,9},{5,10},{14,10},{5,11},{6,11},{13,11},{14,11},{6,12},{13,12},{7,13},{8,13},{11,13},{12,13},{8,14},{9,14},{10,14},{11,14}};
  uint8_t frame5[36][2]={{8,4},{9,4},{10,4},{11,4},{6,5},{7,5},{12,5},{13,5},{5,6},{6,6},{13,6},{14,6},{5,7},{14,7},{4,8},{15,8},{4,9},{15,9},{4,10},{15,10},{4,11},{15,11},{5,12},{14,12},{5,13},{6,13},{13,13},{14,13},{6,14},{7,14},{12,14},{13,14},{8,15},{9,15},{10,15},{11,15}};
  uint8_t frame6[52][2]={{7,3},{8,3},{9,3},{10,3},{11,3},{12,3},{5,4},{6,4},{7,4},{12,4},{13,4},{14,4},{4,5},{5,5},{14,5},{15,5},{4,6},{15,6},{3,7},{4,7},{15,7},{16,7},{3,8},{16,8},{3,9},{16,9},{3,10},{16,10},{3,11},{16,11},{3,12},{4,12},{15,12},{16,12},{4,13},{15,13},{4,14},{5,14},{14,14},{15,14},{5,15},{6,15},{7,15},{12,15},{13,15},{14,15},{7,16},{8,16},{9,16},{10,16},{11,16},{12,16}};
  uint8_t frame7[44][2]={{7,2},{8,2},{9,2},{10,2},{11,2},{12,2},{5,3},{6,3},{13,3},{14,3},{4,4},{15,4},{3,5},{16,5},{3,6},{16,6},{2,7},{17,7},{2,8},{17,8},{2,9},{17,9},{2,10},{17,10},{2,11},{17,11},{2,12},{17,12},{3,13},{16,13},{3,14},{16,14},{4,15},{15,15},{5,16},{6,16},{13,16},{14,16},{7,17},{8,17},{9,17},{10,17},{11,17},{12,17}};
  uint8_t frame8[52][2]={{7,1},{8,1},{9,1},{10,1},{11,1},{12,1},{5,2},{6,2},{13,2},{14,2},{3,3},{4,3},{15,3},{16,3},{3,4},{16,4},{2,5},{17,5},{2,6},{17,6},{1,7},{18,7},{1,8},{18,8},{1,9},{18,9},{1,10},{18,10},{1,11},{18,11},{1,12},{18,12},{2,13},{17,13},{2,14},{17,14},{3,15},{16,15},{3,16},{4,16},{15,16},{16,16},{5,17},{6,17},{13,17},{14,17},{7,18},{8,18},{9,18},{10,18},{11,18},{12,18}};
  uint8_t frame9[56][2]={{7,0},{8,0},{9,0},{10,0},{11,0},{12,0},{5,1},{6,1},{13,1},{14,1},{3,2},{4,2},{15,2},{16,2},{2,3},{17,3},{2,4},{17,4},{1,5},{18,5},{1,6},{18,6},{0,7},{19,7},{0,8},{19,8},{0,9},{19,9},{0,10},{19,10},{0,11},{19,11},{0,12},{19,12},{1,13},{18,13},{1,14},{18,14},{2,15},{17,15},{2,16},{17,16},{3,17},{4,17},{15,17},{16,17},{5,18},{6,18},{13,18},{14,18},{7,19},{8,19},{9,19},{10,19},{11,19},{12,19}};
  uint8_t frames=10;
  uint8_t frameSize[10]={4,8,12,20,32,36,52,44,52,56};
  uint8_t (*framePointer[10])[2]={frame0, frame1, frame2, frame3, frame4, frame5, frame6, frame7, frame8, frame9};
  
  uint8_t hue=10;
  uint8_t sat=255;
  int val=0;

  uint8_t estado=0;

  Serial.println("EFECTO 4");
  while (efectoIdx==4) {
    borrar();
    for (int n=0; n<frames; n++ ) {
          if (efectoIdx!=4)
            break;
          for (int s=0; s<10; s++){            
            for (int p=0; p<frameSize[n]; p++) {
                uint8_t (*ptr)[2] = framePointer[n];
                int x=ptr[p][0];
                int y=ptr[p][1];
                int l=getBufferIndex(x, y);
 
                hue=getLienzoH(x, y);
                sat=getLienzoS(x, y);
                val=getLienzoV(x, y);
                val=(val/2)-(s*2);
                if (val < 40)
                  val=40;
                updatePixel(l, hue, sat, val);
        
            }
            delay (sp*35);
          }
      } 
    delay(100);
  serialCheck();

    for (int n=frames-1; n>=0; n-- ) {
          if (efectoIdx!=4)
            break;
          for (int s=0; s<10; s++){            
            for (int p=0; p<frameSize[n]; p++) {
                uint8_t (*ptr)[2] = framePointer[n];
                int x=ptr[p][0];
                int y=ptr[p][1];
                int l=getBufferIndex(x, y);
 
                hue=getLienzoH(x, y);
                sat=getLienzoS(x, y);
                val=frameBuffer[l][2];
                int valMax=(getLienzoV(x, y)) / 2;
                val+=2;
                if (val > valMax)
                  val=valMax;
                updatePixel(l, hue, sat, val);
        
            }
            delay (sp*35);
          }
      }




      
  }
}


//LLUVIA
void efecto5() {
  Serial.println("EFECTO 5");
  colorPleno ();
  int x=random(0, 20);
    
  int hue=10;
  int sat=100;
  int val=200;
  int maxVal=200;
  int cola=6;
  //int y=0;
  while (efectoIdx==5) {
    x=random(0, 20);
    colorPleno ();
    for (int y=0; y<26; y++) {
      val=maxVal;
      for (int c=0; c<cola; c++) {
        if (y-c >= 0) {
          int l=getBufferIndex(x, y-c);
          //int l=getBufferIndex(x, yb);
 
          updatePixel(l, hue, sat, val);
          val=val-(maxVal/(cola));
          if (val<0)
            val=0;
        }
      }
      delay (sp*20);
    serialCheck();
    if (efectoIdx!=5)
      break;
    }
  serialCheck();

    delay(1000);
  }
}
// ESPIRAL
void efecto5b() {
  int pixels=155;
  int hue=30;
  int sat=255;
  int val=0;
  int head=0;
  while (efectoIdx==5) {
    Serial.println("EFECTO 5");
    borrar();
    for (int p=0; p<pixels; p++) {
      if (efectoIdx!=5)
        break;
      val=255;
      for (int h=0; h<8; h++) {
                  
          int n=p-h;
          if (n<0)
            break;
          int x=secEspiral[n][0];
          int y=secEspiral[n][1];
          Serial.println (n);     
          //int l=bufferToPixeledBase(x, y);
          
          int l=getBufferIndex(x, y);
          hue=getLienzoH(x, y);
          sat=getLienzoS(x, y);
          updatePixel(l, hue, sat, val);
          val=val-40;
          if (val < 0)
            val=0;
          
      }
      delay (sp*10);
  serialCheck();

      }
    }
  
}

void efecto6() {
  int pixels=318;
  int hue=30;
  int sat=255;
  int val=0;
  int head=0;
  while (efectoIdx==6) {
    Serial.println("EFECTO 6");
    borrar();
    for (int p=0; p<pixels; p++) {
      if (efectoIdx!=6)
        break;
      val=255;
      for (int h=0; h<8; h++) {
                  
          int n=p-h;
          if (n<0)
            break;
          int x=secCuadrado[n][0];
          int y=secCuadrado[n][1];
          Serial.println (n);     
          int l=getBufferIndex(x, y);
          hue=getLienzoH(x, y);
          sat=getLienzoS(x, y);
          updatePixel(l, hue, sat, val);
          val=val-40;
          if (val < 0)
            val=0;
          
      }
      delay (sp*10);
  serialCheck();

      }
    }
  
}


// ESPIRAL CUADRADO
void efecto99() {
  int pixels=155; //318
  uint8_t hue=30;
  uint8_t sat=255;
  int val=0;
  while (efectoIdx==6) {
    Serial.println("EFECTO 6");
    FastLED.clear();
    for (int n=0; n<pixels; n++) {
      if (efectoIdx!=5)
        break;
      int x=secEspiral[n][0];
      int y=secEspiral[n][1];
      val=0;
      while (val < 255) {
        int l=getBufferIndex(x, y);
        hue=getLienzoH(x, y);
        sat=getLienzoS(x, y);
        updatePixel(l, hue, sat, val);
        val+=25;
        delay (sp);
  serialCheck();

      }
    }
  }
}


// PIXEL RANDOM
void efecto7() {
  uint8_t hue, sat, val;
  uint8_t hueval, centroHue=0;
  Serial.println("EFECTO 7"); 
  borrar();
  while (efectoIdx==7) {
   centroHue=random(0, 256);
   for (int n=0; n<255; n++) {
     
     for (int y=0; y<20; y++) {
      for (int x=0; x<20; x++) {
        int l=getBufferIndex(x, y);
        hue=getLienzoH(x,y);
        sat=getLienzoS(x,y);
        val=getLienzoV(x,y);

        if (hue > centroHue-80 && hueval < centroHue+80) {
          
          hue=hue+n;  
        
        }
        
        updatePixel(l, hue, sat, val);
       }
      }
       delay(sp*10);   
  serialCheck();

    }
  }
}
