/* This file is part of the Pluviam Firmware */

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <saulo.zz@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer (or a wine) in return.
 * Saulo Matte Madalozzo
 * ----------------------------------------------------------------------------
 */
 
// created by Saulo Matte Madalozzo (saulo.zz@gmail.com) Aug, 2013
// modified by Saulo Matte Madalozzo (saulo.zz@gmail.com) May, 2014

//options
#define BLOCK_INTERVAL 10000 // 10 secs - do not change it!
#define BLOCK_COUNT 6
#define INIT_DELAY 60000

unsigned long lastBlockTime = INIT_DELAY;
int blockCount = 0;

boolean blockInfo(unsigned long time){
 if((time - lastBlockTime) >= BLOCK_INTERVAL){
     blockCount++;
     lastBlockTime += BLOCK_INTERVAL;
    return true;
 }else{
   return false;
 }
}

boolean sendInfo(){
 if(blockCount >= BLOCK_COUNT ){
   blockCount = 0;
   return true;
 }else{
   return false;
 }
}

unsigned long getInitDelay(){
  return INIT_DELAY;
}

void timeDebug(unsigned long time){
  Serial.print(F("stats: lastBlockTime "));
  Serial.print(lastBlockTime);
  Serial.print(F("diffBlock "));
  Serial.print(time - lastBlockTime);
  Serial.print(F("lastSendTime "));
  Serial.print(blockCount);
  Serial.print(F(" TIME "));
  Serial.println(time);
}
