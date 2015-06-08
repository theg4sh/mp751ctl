#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hidapi.h"

//#define DEBUG 1

int main(int argc, char* argv[]){
	//------------------------------------------------
    int RESULT;
    unsigned int SV;
    unsigned char buf[10];
	unsigned char currentState, cSetState;
    struct hid_device_info *devs, *cur_dev;
    hid_device *handle;
	//---------------------------------------------	
	if (argc == 2) {
		if (strcmp(argv[1], "off") == 0) {
			cSetState=0xff; // original code - set off
		} else if (strcmp(argv[1], "on") == 0) {
			cSetState=0x00; // experimental intuitive - set on
		} else if (strcmp(argv[1], "state") == 0) {
		} else {
			goto PRINT_USAGE;
		}
	} else {
		goto PRINT_USAGE;
	}

	RESULT=0;
	devs = hid_enumerate(0x0, 0x0);
	cur_dev = devs;	

	while (cur_dev) {
		//----------поиск устройства mp751-------------
		if (cur_dev->vendor_id != 0x16c0) {
		    cur_dev = cur_dev->next;
		    continue;
		}

        #ifdef DEBUG
		printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls\n",
				    cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
        #endif

		handle=hid_open_path(cur_dev->path);

		buf[0]=0x1d;
		hid_send_feature_report(handle, buf, 8);

		hid_get_feature_report(handle, buf, 8);

		SV=(buf[3]<<8)+buf[2]; //склеивание номера прошивки
		//printf("  type: %04hx %04hx    SV: %04hx\n", buf[0], buf[1], SV);
		if ((buf[0]==0x1d)&(buf[1]==0x2c)&(SV>=0x0002)) //проверка устройства
		{

			// -- get current state --
		    buf[0]=0xE0;
		    buf[1]=0x04;
		    if (hid_send_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }
        #ifdef DEBUG
			printf("  S: %04hx %04hx %04hx %04hx\n", buf[0], buf[1], buf[2], buf[3]);
        #endif
		    if (hid_get_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }
        #ifdef DEBUG
			printf("  S: %04hx %04hx %04hx %04hx\n", buf[0], buf[1], buf[2], buf[3]);
        #endif
			currentState = buf[2];

			if ((strcmp(argv[1], "state") == 0) || (currentState == cSetState)) {
				printf("  Current state: %s\n", (buf[2] == 0xFF) ? "off" : "on");
				cur_dev = cur_dev->next;
				RESULT=1;
				continue;
			}
			printf("  Set state: %s\n", (cSetState == 0xFF) ? "off" : "on");

		    //------------запись в EEPROM-------------
		    buf[0]=0x0E;
		    buf[1]=0x04;
			buf[2]=cSetState;
		    if (hid_send_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; } //задаем состояние реле
        #ifdef DEBUG
			printf("  S: %04hx %04hx %04hx %04hx\n", buf[0], buf[1], buf[2], buf[3]);
        #endif
		    if (hid_get_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }
        #ifdef DEBUG
			printf("  R: %04hx %04hx %04hx %04hx\n", buf[0], buf[1], buf[2], buf[3]);
        #endif
		    if ((buf[0]!=0x0E)&(buf[1]!=0x04)&(buf[2]!=0x00)){ RESULT=0; goto END_MAIN; }

		    buf[0]=0xE0;
		    buf[1]=0x04;
		    if (hid_send_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }
        #ifdef DEBUG
			printf("  S: %04hx %04hx %04hx %04hx\n", buf[0], buf[1], buf[2], buf[3]);
        #endif
		    if ((buf[0]!=0xE0)&(buf[1]!=0x04)&(buf[2]!=0xff)){RESULT=0; goto END_MAIN; } //проверяем записанное значение

		    buf[0]=0x0E;
		    buf[1]=0x06;
		    buf[2]=0x00;
		    if (hid_send_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }  //устанавливаем значение таймера в 0
        #ifdef DEBUG
			printf("  S: %04hx %04hx %04hx %04hx\n", buf[0], buf[1], buf[2], buf[3]);
        #endif
		    if(hid_get_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }
        #ifdef DEBUG
			printf("  R: %04hx %04hx %04hx %04hx\n", buf[0], buf[1], buf[2], buf[3]);
        #endif
		    if ((buf[0]!=0x0E)&(buf[1]!=0x06)&(buf[2]!=0x00)){ RESULT=0; goto END_MAIN; }

		    buf[0]=0xE0;
		    buf[1]=0x06;
		    if (hid_send_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }
        #ifdef DEBUG
			printf("  S: %04hx %04hx %04hx %04hx\n", buf[0], buf[1], buf[2], buf[3]);
        #endif
		    if ((buf[0]!=0xE0)&(buf[1]!=0x06)&(buf[2]!=0x00)){RESULT=0; goto END_MAIN; } //проверяем записанное значение

		    buf[0]=0x0E;
		    buf[1]=0x07;
		    buf[2]=0x00;
		    if (hid_send_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }  //устанавливаем значение таймера в 0
        #ifdef DEBUG
			printf("  S: %04hx %04hx %04hx %04hx\n", buf[0], buf[1], buf[2], buf[3]);
        #endif
		    if(hid_get_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }
        #ifdef DEBUG
			printf("  R: %04hx %04hx %04hx %04hx\n", buf[0], buf[1], buf[2], buf[3]);
        #endif
		    if ((buf[0]!=0x0E)&(buf[1]!=0x07)&(buf[2]!=0x00)){ RESULT=0; goto END_MAIN; }	 

		    buf[0]=0xE0;
		    buf[1]=0x07;
		    if (hid_send_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }
        #ifdef DEBUG
			printf("  S: %04hx %04hx %04hx %04hx\n", buf[0], buf[1], buf[2], buf[3]);
        #endif
		    if ((buf[0]!=0xE0)&(buf[1]!=0x07)&(buf[2]!=0x00)){RESULT=0; goto END_MAIN; } //проверяем записанное значение

		    buf[0]=0x0E;
		    buf[1]=0x08;
		    buf[2]=0x00;
		    if (hid_send_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }  //устанавливаем значение таймера в 0
        #ifdef DEBUG
			printf("  S: %04hx %04hx %04hx %04hx\n", buf[0], buf[1], buf[2], buf[3]);
        #endif
		    if(hid_get_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }
        #ifdef DEBUG
			printf("  R: %04hx %04hx %04hx %04hx\n", buf[0], buf[1], buf[2], buf[3]);
        #endif
		    if ((buf[0]!=0x0E)&(buf[1]!=0x08)&(buf[2]!=0x00)){ RESULT=0; goto END_MAIN; }		 

		    buf[0]=0xE0;
		    buf[1]=0x08;
		    if (hid_send_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }
        #ifdef DEBUG
			printf("  S: %04hx %04hx %04hx %04hx\n", buf[0], buf[1], buf[2], buf[3]);
        #endif
		    if ((buf[0]!=0xE0)&(buf[1]!=0x08)&(buf[2]!=0x00)){RESULT=0; goto END_MAIN; } //проверяем записанное значение
		    //-----------------настройка таймера----------------------

			buf[0]=0x5A;
			buf[1]=1;     //время до выключения в секундах
			buf[2]=0;
			buf[3]=0;
			if (hid_send_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }
        #ifdef DEBUG
			printf("  S: %04hx %04hx %04hx %04hx\n", buf[0], buf[1], buf[2], buf[3]);
        #endif
			if (hid_get_feature_report(handle, buf, 8)!=8){ RESULT=0; goto END_MAIN; }
        #ifdef DEBUG
			printf("  R: %04hx %04hx %04hx %04hx\n", buf[0], buf[1], buf[2], buf[3]);
        #endif

		    //--------------------------------------------
		    RESULT=1; goto END_MAIN; 
		} else {
		    printf("Device response a wrong values. Break.\n");
		    //break;
		}
		cur_dev = cur_dev->next;
	}

	hid_free_enumeration(devs);

END_MAIN:

    if (RESULT!=1)
	    printf("ERROR\n");
	return RESULT;

PRINT_USAGE:
	printf("Usage %s [on|off|state]\n", argv[0]);
	return 0;
}
