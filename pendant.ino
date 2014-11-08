#include <Adafruit_NeoPixel.h>
 
#define PIN 0
#define BUTTON 1
 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(16, PIN);


uint32_t tick;
uint8_t mode = 0;
uint8_t ledId = 0;
uint8_t i = 0;
uint8_t full = 0;
boolean forward;
boolean up = true;
uint8_t laststate = false;
uint8_t currentstate = false;
uint8_t rgbColour[3];
uint32_t randColor = 0;
uint32_t m_z;
uint32_t m_w;
uint32_t colors[6] = {0xff0000,0x00ff00,0x0000ff,0xffff00,0x00ffff,0xff00ff};

void setup() {
  pixels.begin();
  pinMode(BUTTON,INPUT);
  digitalWrite(BUTTON,HIGH);
}
 
void loop() {
  // Start off with red.
  rgbColour[0] = 255;
  rgbColour[1] = 0;
  rgbColour[2] = 0; 
  
  for (int decColour = 0; decColour < 3; decColour += 1) {
    int incColour = decColour == 2 ? 0 : decColour + 1;
 
    // cross-fade the two colours.
    for(int i = 0; i < 255; i += 1) {
      rgbColour[decColour] -= 1;
      rgbColour[incColour] += 1;
       switch (mode){
         case 1: 
           bounce(rgbColour);
           break;
         case 2:
           fourSpin(rgbColour);
           break;
         case 3:
           fill(rgbColour,0);
           break;
         case 4:
           fill(rgbColour,1);
           break;
         case 5:
           colorSpin();
           break;
         case 6:
           spark();
           break;
       }
       currentstate = digitalRead(BUTTON); 
       if(currentstate != laststate){
         if(currentstate == HIGH){
           mode++;
           if(mode > 6){
              mode = 1; 
           }  
         }
         if(mode >= 4){
            pixels.setBrightness(10); 
         }else{
            pixels.setBrightness(32); 
         }
         laststate = currentstate;
       }
    }
    
    
    //delay(75);
  }
}

void bounce(uint8_t* rgbColour){
   uint8_t leader = tick%16;
   uint8_t divisor=1;
   for(i=0;i<16;i++)
   {
      if(i < 5){
        if(forward){
          ledId = (leader-i);  
        }else{
           ledId = (leader+i); 
        }
        pixels.setPixelColor(ledId,rgbColour[0]/divisor,rgbColour[1]/divisor,rgbColour[2]/divisor);
        divisor++;  
      }else{
         if(forward){
           ledId = (leader-i+16)%16;
         }else{
           ledId = (leader+i)%16; 
         }
         pixels.setPixelColor(ledId,0);
      }     
    }
  
   pixels.show();
   if(tick == 63){
      forward = false;           
   }
   if(tick == 0){
      forward = true; 
   }
   
   if(forward){
     tick++;  
   }else{
     tick--; 
   }
   delay(50); 
}
//5,9,14,1    7,11,15,3
void fourSpin(uint8_t* rgbColour){
    uint8_t pos = tick%16;
    for(ledId = pos;ledId < pos + 16; ledId++){
      if((ledId-pos)%4==0){
        pixels.setPixelColor(ledId%16,rgbColour[0],rgbColour[1],rgbColour[2]);  
      }else{
        pixels.setPixelColor(ledId%16,0); 
      }
    }
    
    pixels.show();
    tick++;
    delay(75);
}

void colorSpin(){
   pixels.setBrightness(10);
   uint8_t pos = tick%16;
   uint32_t currentColor = 0xFF;
   for(ledId = pos;ledId < pos + 16; ledId++){
     if((ledId-pos)%4==0){
         currentColor <<= 8;
         if(currentColor == 0){
           currentColor = 0xFF; 
         }
      }
      if(8<=(ledId-pos) && ledId-pos<12){
        pixels.setPixelColor(ledId%16,127,0,255);
      }else{
         pixels.setPixelColor(ledId%16,currentColor); 
      }  
    }
    pixels.show();
    tick++;
    delay(50);
}

void fill(uint8_t* rgbColour,bool rand){
  uint8_t ci =0;
  if(up){
      full++;
   }else{
      full--; 
   }
   if(full == 8 || full == 0)
      up = !up;
   if(rand){
     pixels.setPixelColor((i-1)%16,colors[ci]);
   }else{
     pixels.setPixelColor(15, rgbColour[0],rgbColour[1],rgbColour[2]);  
   } 
   for(i=0;i<16;i++){
     if((i<=full) || (i>=(16-full))){
       if(rand){
         ci = random(6);
         pixels.setPixelColor((i-1)%16,colors[ci]);
       }else{
          pixels.setPixelColor((i-1)%16, rgbColour[0],rgbColour[1],rgbColour[2]);  
       }
     }
     else{
       pixels.setPixelColor((i-1)%16,0);
     }
   }
   pixels.show();
   delay(200);
   tick++;
}


void spark(){
   ledId = random(16);
   uint8_t ci = random(6);
   pixels.setPixelColor(ledId,colors[ci]);
   pixels.show();
   delay(20);
   pixels.setPixelColor(ledId,0);
}
