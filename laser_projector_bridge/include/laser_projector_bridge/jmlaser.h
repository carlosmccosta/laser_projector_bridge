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
/* Represented in 32bit range: [-2147483648, 2147483647]                */
/************************************************************************/
struct JMVectorStruct {
	int32_t x;						// 32 bit signed, 0 is center of X axis projection	[-INT_MAX, INT_MAX]
	int32_t y;						// 32 bit signed, 0 is center of Y axis projection	[-INT_MAX, INT_MAX]
	unsigned short r;			// 16 bit unsigned	[0, USHRT_MAX]
	unsigned short g;			// 16 bit unsigned	[0, USHRT_MAX]
	unsigned short b;			// 16 bit unsigned	[0, USHRT_MAX]
	unsigned short i;			// 16 bit unsigned	[0, USHRT_MAX]
	unsigned short deepblue;	// 16 bit unsigned	[0, USHRT_MAX]
	unsigned short yellow;		// 16 bit unsigned	[0, USHRT_MAX]
	unsigned short cyan;		// 16 bit unsigned	[0, USHRT_MAX]
	unsigned short user4;		// 16 bit unsigned	[0, USHRT_MAX]
};



/**
 * @brief Enumerate all output devices.
 * 
 * This function creates or refreshes the list of all output devices that are connected locally or via network.
 * The device list length is returned.
 * 
 * @return The number of devices found, which equal the device list length. A value less then 0 is returned on error.
 */
int jmLaserEnumerateDevices();



/**
 * @brief Get an entry from the devices list.
 *
 * Use this function to retrieve an entry from the devices list. A devices list entry is a null
 * terminated ASCII/UTF-8 encoded string, that uniquely identifies an output device. This
 * might be "NetPort 0815#0" for a NetPort Board, for instance.
 * The entry at the specified index is retrieved and placed into the deviceName buffer allocated
 * by the application.
 * Not more then length bytes will be written to the buffer. If the entry is longer then length
 * bytes then it will be truncated.
 * You can use jmLaserGetDeviceListEntryLength() to query in advance what size the buffer
 * needs to be to hold the device list entry at a specified index.
 * The jmLaserEnumerateDevices() must have been called at least once prior to calling this function.
 *
 * @param[in] index The index of the device list. Valid range is 0 to the length returned by jmLaserGetDeviceListEntryLength() - 1
 * @param[in] device_name Buffer into which the device list entry will be placed. Has to be allocated by the application.
 * @param[in] The length of the deviceName buffer including terminating zero.
 *
 * @return Returns 0 on success.
 * A negative value indicates an error. Possible error codes include:
 * - JMLASER_ERROR_NOT_ENUMERATED
 *   jmLaserEnumerateDevices() has not been called.
 * - JMLASER_ERROR_OUT_OF_RANGE
 *   index is out of range
 */
int __stdcall jmLaserGetDeviceListEntry(unsigned int index, char* device_name, unsigned int length);



/**
 * @brief Get length of a devices list entry.
 *
 * This function may be used to determine what size the buffer for jmLaserGetDeviceListEntry()
 * needs to be to retrieve the device list entry at the specified index.
 * The jmLaserEnumerateDevices() must have been called at least once prior to calling this
 * function.
 *
 * @param[in] index The index of the device list. Valid range is 0 to the length returned by jmLaserGetDeviceListLength() - 1
 * 
 * @return Returns the length of the device list entry at index. A negative value indicates an error.
 * Possible error codes include:
 * - JMLASER_ERROR_NOT_ENUMERATED
 *   jmLaserEnumerateDevices() has not been called.
 * - JMLASER_ERROR_OUT_OF_RANGE
 *   index is out of range
 */
int __stdcall jmLaserGetDeviceListEntryLength(unsigned int index);



/**
 * @brief Get device name.
 *
 * This function can be used to get the unique device name for an already open output device
 * handle. This is the same name as has been passed to jmLaserOpenDevice() when opening
 * device and as returned by jmLaserGetDeviceListEntry(). As this name is unique and does not
 * change for when the application is run again, it can be stored in the application configuration
 * to open the same device again next time.
 * This function reads the device name into the buffer pointed to by deviceName.
 * At most length bytes are written. If the device name is longer then length bytes it will be
 * truncated. You can get the length of the device name with jmLaserGetDeviceNameLength()
 * before calling this function.
 * The output device must have been opened with jmLaserOpenDevice() prior to calling this
 * function. Additionally jmLaserEnumerateDevices() must have been called at least once.
 *
 * @param[in] handle A handle for an open output device as returned by jmLaserOpenDevice().
 * @param[in] device_name Buffer that the device's friendly name string will be placed into.
 * @param[in] length Length of the deviceFriendlyName Buffer.
 * 
 * @return Returns 0 on success. A negative value indicates an error. Possible error codes include:
 * - JMLASER_ERROR_NOT_ENUMERATED
 *   jmLaserEnumerateDevices() has not been called.
 * - JMLASER_ERROR_INVALID_HANDLE
 *   The handle parameter is invalid.
 * - JMLASER_ERROR_DEVICE_NOT_OPEN
 *   The device has not been opened with jmLaserOpenDevice() or has been removed.
 */
int __stdcall jmLaserGetDeviceName(int handle, char* device_name, unsigned int length);



/**
 * @brief Get device name length.
 *
 * This function may be called before jmLaserGetDeviceName() to get the length of the device���s
 * name.
 * The output device must have been opened with jmLaserOpenDevice() prior to calling this
 * function. Additionally jmLaserEnumerateDevices() must have been called at least once.
 *
 * @param[in] handle A handle for an open output device as returned by jmLaserOpenDevice().
 * 
 * @return Returns the length of the device name string, including terminating zero. A negative value
 * indicates an error. Possible error codes include:
 * - JMLASER_ERROR_NOT_ENUMERATED
 *   jmLaserEnumerateDevices() has not been called.
 * - JMLASER_ERROR_INVALID_HANDLE
 *   The handle parameter is invalid.
 * - JMLASER_ERROR_DEVICE_NOT_OPEN
 *   The device has not been opened with jmLaserOpenDevice() or has been removed.
 */
int __stdcall jmLaserGetDeviceNameLength(int handle);



/**
 * @brief Get device family name.
 *
 * This function reads the device family name into the buffer pointed to by deviceFamily. If the
 * device family name is longer then length bytes it will be truncated. You can get the length of
 * the family name with jmLaserGetFamilyNameLength() before calling this function. This
 * function takes the device name as returned by jmLaserGetDeviceName() or
 * jmLaserGetDeviceListEntry() as argument. The device does not need to be open.
 * An Example of a family name is "Netport".
 * The jmLaserEnumerateDevices() must have been called at least once prior to calling this
 * function.
 *
 * @param[in] device_name A device name string as returned by jmLaserGetDeviceName() or jmLaserGetDeviceListEntry().
 * @param[in] device_family_name Buffer that the device's family name string will be placed into.
 * @param[in] length Length of the deviceFamilyName Buffer.
 * 
 * @return Returns 0 on success. A negative value indicates an error. Possible error codes include
 * - JMLASER_ERROR_NOT_ENUMERATED
 *   jmLaserEnumerateDevices() has not been called.
 * - JMLASER_ERROR_DEVICE_NOT_FOUND
 *   A device with the name deviceName was not found.
 */
int __stdcall jmLaserGetDeviceFamilyName(char* device_name, char* device_family_name, unsigned int length);



/**
 * @brief Get device family name length.
 *
 * This function may be called before jmLaserGetDeviceFamilyName() to get the length of the device���s
 * family name.
 * The jmLaserEnumerateDevices() must have been called at least once prior to calling this
 * function.
 *
 * @param[in] device_name A device name string as returned by jmLaserGetDeviceName() or jmLaserGetDeviceListEntry().
 *
 * @return Returns the length of the device family name string, including terminating zero.
 *
 * This value may be 0. A negative value indicates an error. Possible error codes include:
 * - JMLASER_ERROR_NOT_ENUMERATED
 *   jmLaserEnumerateDevices() has not been called.
 * - JMLASER_ERROR_DEVICE_NOT_FOUND
 *   A device with the name deviceName was not found.
 */
int __stdcall jmLaserGetDeviceFamilyNameLength(char* device_name);
//int __userpurge jmLaserGetDeviceFamilyNameLength@<eax>(int a1@<esi>, void *a2);


/**
 * @brief Get user-friendly device name.
 *
 * This function reads the device friendly name into the buffer pointed to by deviceName. If the
 * device name is longer then length bytes it will be truncated. You can get the length of the
 * friendly name with jmLaserGetFriendlyNameLength() before calling this function.
 * This function takes the device name as returned by jmLaserGetDeviceName() or
 * jmLaserGetDeviceListEntry() as argument. The device does not need to be open.
 * The friendly name adds a description to the hard to remember device ID. It is possible, for
 * example, to set a friendly name "Projector left of table" for the device "NetPort". The friendly
 * name should be displayed to the user next to the device id.
 * The friendly name can be set using jmLaserSetFriendlyName().
 * The jmLaserEnumerateDevices() must have been called at least once prior to calling this
 * function.
 *
 * @param[in] device_name A device name string as returned by jmLaserGetDeviceName() or jmLaserGetDeviceListEntry().
 * @param[in] device_friendly_name Buffer that the device's friendly name string will be placed into.
 * @param[in] length Length of the deviceFriendlyName Buffer.
 * 
 * @return Returns 0 on success. A negative value indicates an error. Possible error codes include
 * - JMLASER_ERROR_NOT_ENUMERATED
 *   jmLaserEnumerateDevices() has not been called.
 * - JMLASER_ERROR_DEVICE_NOT_FOUND
 *   A device with the device name was not found.
 */
int __stdcall jmLaserGetFriendlyName(char* device_name, char* device_friendly_name, unsigned int length);



/**
 * @brief Get user-friendly device name length.
 *
 * This function may be called before jmLaserGetFriendlyName() to get the length of the
 * device's friendly name. This function takes the device name as returned by
 * jmLaserGetDeviceName() or jmLaserGetDeviceListEntry() as argument. The device does not
 * need to be open.
 * The jmLaserEnumerateDevices() must have been called at least once prior to calling this
 * function.
 *
 * @param[in] device_name A device name string as returned by jmLaserGetDeviceName() or jmLaserGetDeviceListEntry().
 *
 * @return Returns the length of the device friendly name string, including terminating zero.
 *
 * This value may be 0. A negative value indicates an error. Possible error codes include:
 * - JMLASER_ERROR_NOT_ENUMERATED
 *   jmLaserEnumerateDevices() has not been called.
 * - JMLASER_ERROR_DEVICE_NOT_FOUND
 *   A device with the name deviceName was not found.
 */
int __stdcall jmLaserGetFriendlyNameLength(char* device_name);
//int __userpurge jmLaserGetFriendlyNameLength@<eax>(int a1@<esi>, void *a2);


/**
 * @brief Set user-friendly device name.
 *
 * This function will set the device's friendly name to the ASCII/UTF-8 encoded string the
 * buffer device-FriendlyName points to.
 * The friendly name adds a description to the hard to remember device ID.
 * It is possible, for example, to set a friendly name "Projector left of table" for the device
 * "NetPort". The friendly name should be displayed to the user next to the device id.
 * The friendly name can be read using jmLaserGetFriendlyName() and
 * jmLaserGetFriendlyNameLength().
 *
 * @param[in] handle A handle for an open output device as returned by jmLaserOpenDevice().
 * @param[in] device_friendly_name Buffer that contains the device's friendly name string. This string is ASCII/UTF-8 encoded and zero-terminated.
 *
 * @return Returns 0 on success. A negative value indicates an error. Possible error codes include
 * - JMLASER_ERROR_NOT_ENUMERATED
 *   jmLaserEnumerateDevices() has not been called.
 * - JMLASER_ERROR_INVALID_HANDLE
 *   The handle parameter is invalid.
 * - JMLASER_ERROR_DEVICE_NOT_OPEN
 *   The device has not been opened with jmLaserOpenDevice() or has been removed.
 */
int __stdcall jmLaserSetFriendlyName(int handle, char* device_friendly_name);



/**
 * @brief Open output device.
 *
 * This function is used to open an output device. The output device needs to be opened before
 * any of the Laser Output or Input/Output functions can be used.
 * This function takes a unique device name as returned by jmLaserGetDeviceName() or
 * jmLaserGetDeviceListEntry() as its argument. As this name is unique and does not change for
 * when the application is run again, it can be stored in the application configuration to open the
 * same device again next time. When the device is not present any more, you will receive an
 * JMLASER_ERROR_DEVICE_NOT_FOUND Error, though.
 * Only open the devices that you are going to use in your application. If you are done with the
 * device, close it using jmLaserCloseDevice().
 *
 * @param[in] device_name A device name string as returned by jmLaserGetDeviceName() or jmLaserGetDeviceListEntry().
 * 
 * @return Returns a device handle on success. A negative value indicates an error. Possible error codes include
 * - JMLASER_ERROR_NOT_ENUMERATED
 *   jmLaserEnumerateDevices() has not been called.
 * - JMLASER_ERROR_DEVICE_NOT_FOUND
 *   A device with the name deviceName was not found.
 * - JMLASER_ERROR_DEVICE_BUSY
 *   The device could not be opened because it is in use by another application.
 * - JMLASER_ERROR_IO
 *   IO Error
 */
int __stdcall jmLaserOpenDevice(char* device_name);



/**
 * @brief Close output device.
 *
 * Closes the output device the handle points to. After this call the handle will become invalid.
 * The output device will be closed and accessible again by other applications.
 *
 * @param[in] handle A handle for an open output device as returned by jmLaserOpenDevice().
 * 
 * @return Returns 0 on success. A negative value indicates an error. Possible error codes include
 * - JMLASER_ERROR_NOT_ENUMERATED
 *   jmLaserEnumerateDevices() has not been called.
 * - JMLASER_ERROR_INVALID_HANDLE
 *   The handle parameter is invalid.
 * - JMLASER_ERROR_DEVICE_NOT_OPEN
 *   The device has not been opened with jmLaserOpenDevice() or has been removed.
 * - JMLASER_ERROR_IO
 *   IO Error
 */
int __stdcall jmLaserCloseDevice(int handle);



/**
 * @brief Start the laser output.
 *
 * Call this function to start the laser output. If the laser output is already running this function
 * does nothing.
 * The output device must have been opened with jmLaserOpenDevice() prior to calling this
 * function. Additionally jmLaserEnumerateDevices() must have been called at least once.
 *
 * @param[in] handle A handle for an open output device as returned by jmLaserOpenDevice().
 * 
 * @return This function returns 0 on success. A negative value indicates an error. Possible error codes include
 * - JMLASER_ERROR_NOT_ENUMERATED
 *   jmLaserEnumerateDevices() has not been called.
 * - JMLASER_ERROR_INVALID_HANDLE
 *   The handle parameter is invalid.
 * - JMLASER_ERROR_DEVICE_NOT_OPEN
 *   The device has not been opened with jmLaserOpenDevice() or has been removed.
 * - JMLASER_ERROR_IO
 *   IO Error
 */
int __stdcall jmLaserStartOutput(int handle);



/**
 * @brief Write a frame to the output device.
 *
 * Call this function to write a number of vectors to the output device for laser output.
 * The vectors are stored in an array of JMVectorStruct data structures. The number of vectors
 * that should be written is provided in the count parameter. The speed parameter specifies the
 * sampling rate with which the array of vectors should be output.
 * The maximum number of vectors in a frame is limited; its value differs among different types
 * of output devices. Use jmLaserGetFrameSize() to query the maximum number of vectors that
 * a frame may contain.
 * How often array of vectors is output can be set with the repetitions parameter. A value of 0
 * means, this frame will be output at least once, then repeated indefinitely until a new frame is
 * written. A value larger then 0 will cause the frame to be displayed exactly the specified
 * number of times, then the output will stop until a new frame is written.
 * A frame can only be written if the output device has a free buffer. Use
 * jmLaserWaitForDeviceReady() or jmLaserIsDeviceReady() to query, whether the device can
 * accept a new frame.
 * The output device must have been opened with jmLaserOpenDevice() and the laser output
 * must have been started with jmLaserStartOutput() prior to calling this function. Additionally
 * jmLaserEnumerateDevices() must have been called at least once.
 *
 * @param[in] handle A handle for an open output device as returned by jmLaserOpenDevice().
 * @param[in] An array of JMVectorStruct data structures describing the vector data. The array has count entries.
 * @param[in] count Count of vectors to be written to the device. Range is 1.. the value returned by jmLaserGet-FrameSize().
 * @param[in] speed Sampling rate in points per seconds with which the frame should be output.
 * @param[in] repetitions number of times the frame should be repeated. If this value is 0, the frame will
 * be display at least once and repeat indefinitely until a new frame is written. If
 * this value is larger then 0 the frame will be shown exactly the specified number
 * of times and then the output will stop until a new frame is written.
 * 
 * @return This function returns 0 on success. A negative value indicates an error. Possible error codes include
 * - JMLASER_ERROR_NOT_ENUMERATED
 *   jmLaserEnumerateDevices() has not been called.
 * - JMLASER_ERROR_INVALID_HANDLE
 *   The handle parameter is invalid.
 * - JMLASER_ERROR_DEVICE_NOT_OPEN
 *   The device has not been opened with jmLaserOpenDevice() or has been removed.
 * - JMLASER_ERROR_OUTPUT_NOT_STARTED
 *   The laser output has not been started (successfully) with jmLaserStartOutput().
 * - JMLASER_ERROR_DEVICE_BUSY
 *   Frame could not be written because device is busy.
 * - JMLASER_ERROR_IO
 *   IO Error
 */
int __stdcall jmLaserWriteFrame(int handle, JMVectorStruct *vectors, unsigned int count, unsigned int speed, unsigned int repetitions);



/**
 * @brief Block thread until device can accept a new frame.
 *
 * This function blocks the calling thread until jmLaserWriteFrame() is ready to accept a new
 * frame. Use this function in a multi-threaded application when you have one thread for every
 * output device.
 * The output device must have been opened with jmLaserOpenDevice() and the laser output
 * must have been started with jmLaserStartOutput() prior to calling this function. Additionally
 * jmLaserEnumerateDevices() must have been called at least once.
 *
 * @param[in] handle A handle for an open output device as returned by jmLaserOpenDevice().
 * 
 * @return This function returns JMLASER_DEVICE_READY on success. A negative value indicates
 * an error. Possible error codes include:
 * - JMLASER_ERROR_NOT_ENUMERATED
 *   jmLaserEnumerateDevices() has not been called.
 * - JMLASER_ERROR_INVALID_HANDLE
 *   The handle parameter is invalid.
 * - JMLASER_ERROR_DEVICE_NOT_OPEN
 *   The device has not been opened with jmLaserOpenDevice() or has been removed.
 * - JMLASER_ERROR_OUTPUT_NOT_STARTED
 *   The laser output has not been started (successfully) with jmLaserStartOutput().
 * - JMLASER_ERROR_IO
 *   IO Error
 */
int __stdcall jmLaserWaitForDeviceReady(int handle);



/**
 * @brief Query if the device can accept a new frame.
 *
 * This function is used to query if the output device will be ready to accept a new frame with
 * jmLaserWriteFrame(). Use this function in a single-threaded application to poll whether an
 * output device can accept a new frame. If this is the case, JMLASER_DEVICE_READY is
 * returned, else JMLASER_ERROR_DEVICE_BUSY will be returned.
 * The output device must have been opened with jmLaserOpenDevice() and the laser output
 * must have been started with jmLaserStartOutput() prior to calling this function. Additionally
 * jmLaserEnumerateDevices() must have been called at least once.
 *
 * @param[in] handle A handle for an open output device as returned by jmLaserOpenDevice().
 * 
 * @return This function returns JMLASER_DEVICE_READY or
 * JMLASER_ERROR_DEVICE_BUSY on success. A negative value indicates an error.
 * Possible error codes include:
 * - JMLASER_ERROR_NOT_ENUMERATED
 *   jmLaserEnumerateDevices() has not been called.
 * - JMLASER_ERROR_INVALID_HANDLE
 *   The handle parameter is invalid.
 * - JMLASER_ERROR_DEVICE_NOT_OPEN
 *   The device has not been opened with jmLaserOpenDevice() or has been removed.
 * - JMLASER_ERROR_OUTPUT_NOT_STARTED
 *   The laser output has not been started (successfully) with jmLaserStartOutput().
 * - JMLASER_ERROR_IO
 *   IO Error
 */
int __stdcall jmLaserIsDeviceReady(int handle);



/**
 * @brief Stop the laser output.
 *
 * Call this function just to stop the laser output.
 * The output device must have been opened with jmLaserOpenDevice() and the laser output
 * must have been started with jmLaserStartOutput() prior to calling this function.
 * Additionally jmLaserEnumerateDevices() must have been called at least once.
 *
 * @param[in] handle A handle for an open output device as returned by jmLaserOpenDevice().
 * 
 * @return This function returns 0 on success. A negative value indicates an error. Possible error codes include
 * - JMLASER_ERROR_NOT_ENUMERATED
 *   jmLaserEnumerateDevices() has not been called.
 * - JMLASER_ERROR_INVALID_HANDLE
 *   The handle parameter is invalid.
 * - JMLASER_ERROR_DEVICE_NOT_OPEN
 *   The device has not been opened with jmLaserOpenDevice() or has been removed.
 * - JMLASER_ERROR_OUTPUT_NOT_STARTED
 *   The laser output has not been started (successfully) with jmLaserStartOutput().
 * - JMLASER_ERROR_IO
 *   IO Error
 */
int __stdcall jmLaserStopOutput(int handle);



/**
 * @brief DLL de-initialization.
 *
 * Call jmLaserCloseDll once just before you close the DLL or exit you application program.
 *
 * @return 0 on success, a value less then 0 on error.
 */
int jmLaserCloseDll();


int __stdcall jmLaserDeviceIsInUse(char* device_name);
int __stdcall jmLaserDeviceIsRemote(char* device_name);
int jmLaserGetDeviceListLength();
int __stdcall jmLaserGetIsNetworkDevice(char* device_name);
int __stdcall jmLaserGetMaxFrameSize(int handle);
int __stdcall jmLaserGetMaxSpeed(int handle);
int __stdcall jmLaserGetMinSpeed(int handle);
int __stdcall jmLaserGetNetworkAddress(char* device_name, char* network_address, unsigned int length);
int __stdcall jmLaserGetNetworkAddressLength(char* device_name);
int __stdcall jmLaserGetNumColorChannels(int handle);
int __stdcall jmLaserGetNumUniverses(int handle, bool a2);
int __stdcall jmLaserGetNumUsedColorChannels(int handle);
int __stdcall jmLaserGetSpeedStep(int handle);
int __stdcall jmLaserGetUniverseName(int handle, bool a2, int a3, char* universe_name, unsigned int length);
int __stdcall jmLaserGetUniverseNameLength(int handle, bool a2, int a3);
int __stdcall jmLaserGetUniverseNumChannels(int handle, bool a2, int a3);
int __stdcall jmLaserGetUniverseResolution(int handle, bool a2, int a3);
int jmLaserOpenDll();
int __stdcall jmLaserSetNumUsedColorChannels(int a1, int a2);
int __cdecl jmLaserSetTerminalCallback(void *a1, int a2, int a3);
int __stdcall jmLaserUniverseRead(int a1, char a2, int a3, int a4, int a5, int a6);
int __stdcall jmLaserUniverseUpdate(int a1, char a2, int a3);
int __stdcall jmLaserUniverseWrite(int a1, char a2, int a3, int a4, int a5, int a6);
//signed int __userpurge jmLaserWriteFrameEL@<eax>(int a1@<ebx>, int a2@<edi>, int a3@<esi>, int a4, int a5, unsigned int a6, char a7, int a8);
//signed int __userpurge jmLaserWriteFrameNL@<eax>(int a1@<ebx>, int a2@<edi>, int a3@<esi>, int a4, int a5, unsigned int a6, char a7, int a8);



#ifdef __cplusplus
} // __cplusplus defined.
#endif
