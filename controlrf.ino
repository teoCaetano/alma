
enum REMOTE_CMD {PWR=1, MODE_UP=5, SPEED_DOWN=7, DEMO, SPEED_UP, COLOR_UP, MODE_DOWN, BRIGHT_UP, COLOR_DOWN, BRIGHT_DOWN=15};

#define CICLO_ESPERANDO 0 
#define CICLO_PULSO_ALTO 1
#define CICLO_PULSO_BAJO 2
#define PKT_SIZE           3
#define PKT_SIZE_BITS      PKT_SIZE*8


volatile uint8_t pktData[PKT_SIZE+1];
uint16_t address = 0x9A15;
volatile unsigned long lastPacketTime = 0;

volatile unsigned long cicloStart =0;
volatile unsigned long cicloPulsoHigh = 0;
volatile unsigned long cicloLen = 0;
volatile int cicloEstado=CICLO_ESPERANDO;
volatile int dataIdx=0;


void IRAM_ATTR leerControl() {
  bool dataVal = digitalRead (pRFDATA);
 /* if (cicloEstado==CICLO_ESPERANDO && dataVal== LOW) {
    cicloEstado=CICLO_ESPERANDO;
  }*/  
  
  if (cicloEstado==CICLO_ESPERANDO && dataVal== HIGH) {
    cicloEstado=CICLO_PULSO_ALTO;
    cicloStart=micros();
  }  

  /*if (cicloEstado == CICLO_PULSO_BAJO && dataVal == LOW && micros()-cicloStart > 2000) {
    cicloEstado=CICLO_ESPERANDO;
    dataIdx=0;
  }*/
  
  if (cicloEstado == CICLO_PULSO_ALTO && dataVal == LOW) {
    cicloEstado=CICLO_PULSO_BAJO;
    cicloPulsoHigh=micros()-cicloStart;
    //AGREGADO. SI EL PULSO HIGH ES MUY CORTO DESCARTA.
    if (cicloPulsoHigh < 300 || cicloPulsoHigh >1800) {
      cicloEstado=CICLO_ESPERANDO;
      dataIdx=0;
    }
  }

  
  if (cicloEstado == CICLO_PULSO_BAJO && dataVal == HIGH) {
  
    cicloLen=micros()-cicloStart;
    
    cicloEstado=CICLO_PULSO_ALTO;
    cicloStart=micros();
    //Serial.println (cicloLen);
    if (cicloLen < 1700 && cicloLen > 1500) {
      
      if (cicloPulsoHigh > 1050 && cicloPulsoHigh < 1500)
        bitSet (pktData[dataIdx/8], 7-(dataIdx % 8));
      else
        bitClear (pktData[dataIdx/8], 7-(dataIdx % 8));
      dataIdx++;
    } else {
      dataIdx=0;
      cicloEstado=CICLO_ESPERANDO;
    }
      
    
  }
   
  if (dataIdx >=PKT_SIZE_BITS) {
     uint16_t pktAddress=pktData[0];
     pktAddress=(pktAddress << 8) + pktData[1];
     uint8_t cmd=pktData[2];
     if (pktAddress==address) {
      if (millis()-lastPacketTime >= 200) {
        lastPacketTime=millis();
        if (cmd==REMOTE_CMD::PWR) {
          if (efectoIdx==0)
            efectoIdx=efectoIdxLast;
          else {
            efectoIdxLast=efectoIdx;
            efectoIdx=0;
          }
        }
        else if (cmd==REMOTE_CMD::BRIGHT_UP)
          subirBrillo();
        else if (cmd==REMOTE_CMD::BRIGHT_DOWN)
          bajarBrillo();
        else if (cmd==REMOTE_CMD::SPEED_UP)
          subirSpeed();
        else if (cmd==REMOTE_CMD::SPEED_DOWN)
          bajarSpeed();
        else if (cmd==REMOTE_CMD::COLOR_UP)
          subirColor();
        else if (cmd==REMOTE_CMD::COLOR_DOWN)
          bajarColor();
        
        
        else if (cmd==REMOTE_CMD::MODE_DOWN) {
          if (efectoIdx>1)
            efectoIdx--;
        }
        else if (cmd==REMOTE_CMD::MODE_UP) {
          if (efectoIdx<NUM_EFECTOS-1 && efectoIdx > 0)
            efectoIdx++;
        }

        
        //Serial.println (cmd, HEX);
      }
     }
/*     Serial.print (pktData[0], HEX);
     Serial.print (" ");
     Serial.print (pktData[1], HEX);
     Serial.print (":");
     Serial.print (pktData[2], HEX);
     Serial.println();*/
     //pktQueue.put(pktData);   
     //nuevoPaquete=true;
     cicloEstado=CICLO_ESPERANDO;
     dataIdx=0;
  }
 
}

void controlUpdate(unsigned long espera) {
  leerControl();
  unsigned long startTime=millis();
  while (millis()-startTime < espera) {
    leerControl();
  }
}
