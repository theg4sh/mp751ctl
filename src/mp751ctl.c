#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hidapi.h"

int main(int argc, char* argv[]){
//------------------------------------------------
        int RESULT;
        unsigned int SV;
        unsigned char buf[10];
        struct hid_device_info *devs, *cur_dev;
        hid_device *handle;
//---------------------------------------------	
	RESULT=0;
	devs = hid_enumerate(0x0, 0x0);
	cur_dev = devs;	
//----------поиск устройства mp751-------------
	while (cur_dev) {
   handle=hid_open_path(cur_dev->path);

   buf[0]=0x1d;
   hid_send_feature_report(handle, buf, 8);
   
   hid_get_feature_report(handle, buf, 8);

   SV=(buf[3]<<8)+buf[2]; //склеивание номера прошивки
   
   if ((buf[0]==0x1d)&(buf[1]==0x2c)&(SV>=0x0002)) //проверка устройства
    {
	//------------запись в EEPROM-------------
     buf[0]=0x0E;
	 buf[1]=0x04;
	 buf[2]=0xff;
	 if (hid_send_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; } //задаем состояние реле

	 if (hid_get_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }
	 if ((buf[0]!=0x0E)&(buf[1]!=0x04)&(buf[2]!=0x00)){ RESULT=0; goto END_MAIN; }
	 
	 buf[0]=0xE0;
	 buf[1]=0x04;
	 if (hid_send_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }
	 if ((buf[0]!=0xE0)&(buf[1]!=0x04)&(buf[2]!=0xff)){RESULT=0; goto END_MAIN; } //проверяем записанное значение

     buf[0]=0x0E;
	 buf[1]=0x06;
	 buf[2]=0x00;
	 if (hid_send_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }  //устанавливаем значение таймера в 0
	 if(hid_get_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }
	 if ((buf[0]!=0x0E)&(buf[1]!=0x06)&(buf[2]!=0x00)){ RESULT=0; goto END_MAIN; }
	 
	 buf[0]=0xE0;
	 buf[1]=0x06;
	 if (hid_send_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }
     if ((buf[0]!=0xE0)&(buf[1]!=0x06)&(buf[2]!=0x00)){RESULT=0; goto END_MAIN; } //проверяем записанное значение	 
	
     buf[0]=0x0E;
	 buf[1]=0x07;
	 buf[2]=0x00;
	 if (hid_send_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }  //устанавливаем значение таймера в 0
	 if(hid_get_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }
	 if ((buf[0]!=0x0E)&(buf[1]!=0x07)&(buf[2]!=0x00)){ RESULT=0; goto END_MAIN; }	 
	 
	 buf[0]=0xE0;
	 buf[1]=0x07;
	 if (hid_send_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }
     if ((buf[0]!=0xE0)&(buf[1]!=0x07)&(buf[2]!=0x00)){RESULT=0; goto END_MAIN; } //проверяем записанное значение
	 
     buf[0]=0x0E;
	 buf[1]=0x08;
	 buf[2]=0x00;
	 if (hid_send_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }  //устанавливаем значение таймера в 0
	 if(hid_get_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }
	 if ((buf[0]!=0x0E)&(buf[1]!=0x08)&(buf[2]!=0x00)){ RESULT=0; goto END_MAIN; }		 
	 
	 buf[0]=0xE0;
	 buf[1]=0x08;
	 if (hid_send_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }
     if ((buf[0]!=0xE0)&(buf[1]!=0x08)&(buf[2]!=0x00)){RESULT=0; goto END_MAIN; } //проверяем записанное значение
//-----------------настройка таймера----------------------

     buf[0]=0x5A;
	 buf[1]=30;     //время до выключения в секундах
	 buf[2]=0;
	 buf[3]=0;
	 if (hid_send_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }
	 if (hid_get_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }
	 
//--------------------------------------------
	RESULT=1; goto END_MAIN; 
	 }
		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);
END_MAIN:
    if (RESULT!=1)
	printf("ERROR\n");
	else printf("OK\n");
	return RESULT;
}
