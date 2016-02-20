#pragma once


#ifdef __cplusplus
extern "C"
{
#endif


/************************************************************************/
/* Macros                                                               */
/************************************************************************/
#define JMLASER_DEVICE_READY 1
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
struct JMVectorStruct {
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


__declspec(dllexport) int jmLaserEnumerateDevices() { return 0; }
__declspec(dllexport) int __stdcall jmLaserGetDeviceListEntry(unsigned int index, char* device_name, unsigned int length) { return 0; }
__declspec(dllexport) int __stdcall jmLaserGetDeviceListEntryLength(unsigned int index) { return 0; }
__declspec(dllexport) int __stdcall jmLaserGetDeviceName(int handle, char* device_name, unsigned int length) { return 0; }
__declspec(dllexport) int __stdcall jmLaserGetDeviceNameLength(int handle) { return 0; }
__declspec(dllexport) int __stdcall jmLaserGetDeviceFamilyName(char* device_name, char* device_family_name, unsigned int length) { return 0; }
__declspec(dllexport) int __stdcall jmLaserGetDeviceFamilyNameLength(char* device_name) { return 0; }
//__declspec(dllexport) int __userpurge jmLaserGetDeviceFamilyNameLength@<eax>(int a1@<esi>, void *a2) { return 0; }
__declspec(dllexport) int __stdcall jmLaserGetFriendlyName(char* device_name, char* device_friendly_name, unsigned int length) { return 0; }
__declspec(dllexport) int __stdcall jmLaserGetFriendlyNameLength(char* device_name) { return 0; }
//__declspec(dllexport) int __userpurge jmLaserGetFriendlyNameLength@<eax>(int a1@<esi>, void *a2) { return 0; }
__declspec(dllexport) int __stdcall jmLaserSetFriendlyName(int handle, char* device_friendly_name) { return 0; }
__declspec(dllexport) int __stdcall jmLaserOpenDevice(char* device_name) { return 0; }
__declspec(dllexport) int __stdcall jmLaserCloseDevice(int handle) { return 0; }
__declspec(dllexport) int __stdcall jmLaserStartOutput(int handle) { return 0; }
__declspec(dllexport) int __stdcall jmLaserWriteFrame(int handle, JMVectorStruct *vectors, unsigned int count, unsigned int speed, unsigned int repetitions) { return 0; }
__declspec(dllexport) int __stdcall jmLaserWaitForDeviceReady(int handle) { return 0; }
__declspec(dllexport) int __stdcall jmLaserIsDeviceReady(int handle) { return 0; }
__declspec(dllexport) int __stdcall jmLaserStopOutput(int handle) { return 0; }
__declspec(dllexport) int jmLaserCloseDll() { return 0; }
__declspec(dllexport) int __stdcall jmLaserDeviceIsInUse(char* device_name) { return 0; }
__declspec(dllexport) int __stdcall jmLaserDeviceIsRemote(char* device_name) { return 0; }
__declspec(dllexport) int jmLaserGetDeviceListLength() { return 0; }
__declspec(dllexport) int __stdcall jmLaserGetIsNetworkDevice(char* device_name) { return 0; }
__declspec(dllexport) int __stdcall jmLaserGetMaxFrameSize(int handle) { return 0; }
__declspec(dllexport) int __stdcall jmLaserGetMaxSpeed(int handle) { return 0; }
__declspec(dllexport) int __stdcall jmLaserGetMinSpeed(int handle) { return 0; }
__declspec(dllexport) int __stdcall jmLaserGetNetworkAddress(char* device_name, char* network_address, unsigned int length) { return 0; }
__declspec(dllexport) int __stdcall jmLaserGetNetworkAddressLength(char* device_name) { return 0; }
__declspec(dllexport) int __stdcall jmLaserGetNumColorChannels(int handle) { return 0; }
__declspec(dllexport) int __stdcall jmLaserGetNumUniverses(int handle, bool a2) { return 0; }
__declspec(dllexport) int __stdcall jmLaserGetNumUsedColorChannels(int handle) { return 0; }
__declspec(dllexport) int __stdcall jmLaserGetSpeedStep(int handle) { return 0; }
__declspec(dllexport) int __stdcall jmLaserGetUniverseName(int handle, bool a2, int a3, char* universe_name, unsigned int length) { return 0; }
__declspec(dllexport) int __stdcall jmLaserGetUniverseNameLength(int handle, bool a2, int a3) { return 0; }
__declspec(dllexport) int __stdcall jmLaserGetUniverseNumChannels(int handle, bool a2, int a3) { return 0; }
__declspec(dllexport) int __stdcall jmLaserGetUniverseResolution(int handle, bool a2, int a3) { return 0; }
__declspec(dllexport) int jmLaserOpenDll() { return 0; }
__declspec(dllexport) int __stdcall jmLaserSetNumUsedColorChannels(int a1, int a2) { return 0; }
__declspec(dllexport) int __cdecl jmLaserSetTerminalCallback(void *a1, int a2, int a3) { return 0; }
__declspec(dllexport) int __stdcall jmLaserUniverseRead(int a1, char a2, int a3, int a4, int a5, int a6) { return 0; }
__declspec(dllexport) int __stdcall jmLaserUniverseUpdate(int a1, char a2, int a3) { return 0; }
__declspec(dllexport) int __stdcall jmLaserUniverseWrite(int a1, char a2, int a3, int a4, int a5, int a6) { return 0; }
//__declspec(dllexport) signed int __userpurge jmLaserWriteFrameEL@<eax>(int a1@<ebx>, int a2@<edi>, int a3@<esi>, int a4, int a5, unsigned int a6, char a7, int a8) { return 0; }
//__declspec(dllexport) signed int __userpurge jmLaserWriteFrameNL@<eax>(int a1@<ebx>, int a2@<edi>, int a3@<esi>, int a4, int a5, unsigned int a6, char a7, int a8) { return 0; }



#ifdef __cplusplus
} // __cplusplus defined.
#endif
