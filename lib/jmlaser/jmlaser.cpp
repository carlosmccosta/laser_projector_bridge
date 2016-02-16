#pragma once

/************************************************************************/
/* DLL Macros                                                           */
/************************************************************************/

#define CALL_CONVENTION __stdcall
#define DLL_DECLSPEC __declspec(dllexport)



/************************************************************************/
/* Macros                                                               */
/************************************************************************/
#define JMLASER_ERROR_NOT_ENUMERATED -1
#define JMLASER_ERROR_INVALID_HANDLE -2
#define JMLASER_ERROR_DEVICE_NOT_OPEN -3
#define JMLASER_ERROR_DEVICE_NOT_FOUND -4
#define JMLASER_ERROR_OUTPUT_NOT_STARTED -5
#define JMLASER_ERROR_INVALID_UNIVERSE -6
#define JMLASER_ERROR_OUT_OF_RANGE -7
#define JMLASER_ERROR_DEVICE_BUSY -8
#define JMLASER_ERROR_IO -9



/************************************************************************/
/* 16bit hardware resolution X/Y deflection value.                      */
/* Represented in 32bit range: -2147483648 - 2147483647                 */
/************************************************************************/
struct JMVectorStruct {;
	int x;
	int y;
	unsigned short r;
	unsigned short g;
	unsigned short b;
	unsigned short i;
	unsigned short deepblue;
	unsigned short yellow;
	unsigned short cyan;
	unsigned short user4;
};


#ifdef __cplusplus
extern "C"
{
#endif

DLL_DECLSPEC int CALL_CONVENTION jmLaserEnumerateDevices() { return 0; }
DLL_DECLSPEC int CALL_CONVENTION jmLaserGetDeviceListEntry(unsigned int index, char* deviceName, unsigned int length) { return 0; }
DLL_DECLSPEC int CALL_CONVENTION jmLaserGetDeviceListEntryLength(unsigned int index) { return 0; }
DLL_DECLSPEC int CALL_CONVENTION jmLaserGetDeviceName(int handle, char* deviceName, unsigned int length) { return 0; }
DLL_DECLSPEC int CALL_CONVENTION jmLaserGetDeviceNameLength(int handle) { return 0; }
DLL_DECLSPEC int CALL_CONVENTION jmLaserGetDeviceFamilyName(char* deviceName, char* deviceFamilyName, unsigned int length) { return 0; }
DLL_DECLSPEC int CALL_CONVENTION jmLaserGetDeviceFamilyNameLength(char* deviceName) { return 0; }
DLL_DECLSPEC int CALL_CONVENTION jmLaserGetFriendlyName(char* deviceName, char* deviceFriendlyName, unsigned int length) { return 0; }
DLL_DECLSPEC int CALL_CONVENTION jmLaserGetFriendlyNameLength(char* deviceName) { return 0; }
DLL_DECLSPEC int CALL_CONVENTION jmLaserSetFriendlyName(int handle, char _deviceFriendlyName) { return 0; }
DLL_DECLSPEC int CALL_CONVENTION jmLaserOpenDevice(char* deviceName) { return 0; }
DLL_DECLSPEC int CALL_CONVENTION jmLaserCloseDevice(int handle) { return 0; }
DLL_DECLSPEC int CALL_CONVENTION jmLaserStartOutput(int handle) { return 0; }
DLL_DECLSPEC int CALL_CONVENTION jmLaserWriteFrame(int handle, JMVectorStruct *vectors, unsigned int count, unsigned int speed, unsigned int repetitions) { return 0; }
DLL_DECLSPEC int CALL_CONVENTION jmLaserWaitForDeviceReady(int handle) { return 0; }
DLL_DECLSPEC int CALL_CONVENTION jmLaserIsDeviceReady(int handle) { return 0; }
DLL_DECLSPEC int CALL_CONVENTION jmLaserStopOutput(int handle) { return 0; }
DLL_DECLSPEC int CALL_CONVENTION jmLaserCloseDll() { return 0; }

#ifdef __cplusplus
} // __cplusplus defined.
#endif
