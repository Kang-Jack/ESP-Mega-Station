#include "LASERK_PMS5003S.h"

LASERK_PMS5003S::LASERK_PMS5003S(SoftwareSerial *serial){
  sofSeri = serial;
  Seri = sofSeri;
}

LASERK_PMS5003S::LASERK_PMS5003S(HardwareSerial *serial){
  hwSeri = serial;
  Seri = hwSeri;
}

int LASERK_PMS5003S::begin(){
  if(hwSeri){hwSeri->begin(UART_BAUD);return 0;}
  if(sofSeri){sofSeri->begin(UART_BAUD);return 0;}
  return -1;
}

void LASERK_PMS5003S::send(byte cmd,byte dataH,byte dataL){
  Packet[2] = cmd;
  Packet[3] = dataH;
  Packet[4] = dataL;
  LRC = 0;
  foreach(Packet){
    Seri->write(row);
    LRC += row;
  }
  Seri->write((LRC >> 8) & 0xff);
  Seri->write(LRC & 0xff);
}

void LASERK_PMS5003S::sleep(){
  send(PMS_CMD_SLEEP,0x00,PMS_CMD_SLEEP_SLEEP);
}

void LASERK_PMS5003S::wakeUp(){
  send(PMS_CMD_SLEEP,0x00,PMS_CMD_SLEEP_WAKEUP);
}

void LASERK_PMS5003S::setMode(byte mode){
  send(PMS_CMD_MODE,0x00,mode);
  while(Seri->available() > 0){
    Seri->read();
  }
}

void LASERK_PMS5003S::request(){
  send(PMS_CMD_READ,0x00,0x00);
}

int LASERK_PMS5003S::read(unsigned long timeout){
  static unsigned long start;
  static int cnt;
  static unsigned int check;
  cnt = 0;
  check = 0;
  start = millis();
  while(Seri->available() < 32){
     hwSeri->println("timeout false");
     if(millis() - start > timeout){return false;}
  }
  while(Seri->available() > 0){
    byte c = Seri->read();
    buffer[31-(cnt)] = c;
    if(cnt < 30){
      check += (unsigned int)c;
    }
    cnt++;
    cnt = cnt % 32;
  }

  if(reinterpret_cast<unsigned int *> (buffer)[15] != 0x424D ||
      reinterpret_cast<unsigned int *> (buffer)[0] != check){
      hwSeri->println("return false 2");
    return false;
  }
  memcpy(data,buffer,sizeof(buffer));
  return true;
}

double LASERK_PMS5003S::getForm(){
  return data[DATA_FORMALDE]/1000.0;
}

unsigned int LASERK_PMS5003S::getPcs(double pcs){
  switch((int)(pcs*10)){
    case 100: return data[DATA_PCS100ug];
    case 50 : return data[DATA_PCS50ug];
    case 25 : return data[DATA_PCS25ug];
    case 10 : return data[DATA_PCS10ug];
    case 5  : return data[DATA_PCS05ug];
    case 3  : return data[DATA_PCS03ug];
  }
  return 0;
}

unsigned int LASERK_PMS5003S::getPmAto(double pm){
  switch((int)(pm*10)){
    case 100: return data[DATA_PM100ATO];
    case 25 : return data[DATA_PM25ATO];
    case 10 : return data[DATA_PM10ATO];
  }
  return 0;
}

unsigned int LASERK_PMS5003S::getPmCf1(double pm){
  switch((int)(pm*10)){
    case 100: return data[DATA_PM100CF1];
    case 25 : return data[DATA_PM25CF1];
    case 10 : return data[DATA_PM10CF1];
  }
  return 0;
}
