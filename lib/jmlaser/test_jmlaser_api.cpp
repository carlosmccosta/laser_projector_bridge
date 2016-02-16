#include <stdio.h>
#include <jmlaser/jmlaser.h>


bool testGetFriendlyNameAPI(char *device_name, bool print_info, int i) {
	bool success = false;
	int device_friendly_name_length = jmLaserGetFriendlyNameLength(device_name);
	if (device_friendly_name_length > 0) {
		char *device_friendly_name = new char[device_friendly_name_length];
		if (jmLaserGetFriendlyName(device_name, device_friendly_name, (unsigned int) device_friendly_name_length) == 0) {
			device_friendly_name[device_friendly_name_length - 1] = '\0';
			if (print_info) printf("Device %i friendly name: %s\n", i, device_friendly_name);
			success = true;
		}
		delete[] device_friendly_name;
	}
	return success;
}


bool testGetDeviceFamilyNameAPI(char *device_name, bool print_info, int i) {
	bool success = false;
	int device_family_name_length = jmLaserGetDeviceFamilyNameLength(device_name);
	if (device_family_name_length > 0) {
		char *device_family_name = new char[device_family_name_length];
		if (jmLaserGetDeviceFamilyName(device_name, device_family_name, (unsigned int) device_family_name_length) == 0) {
			device_family_name[device_family_name_length - 1] = '\0';
			if (print_info) printf("Device %i family name: %s\n", i, device_family_name);
			success = true;
		}
		delete[] device_family_name;
	}
	return success;
}


int testGetDeviceNameAPI(int device_handle, bool print_info, int i) {
	bool success = false;
	int device_name_from_handle_length = jmLaserGetDeviceNameLength(device_handle);
	if (device_name_from_handle_length > 0) {
		char *device_name_from_handle = new char[device_name_from_handle_length];
		if (jmLaserGetDeviceName(device_handle, device_name_from_handle, (unsigned int) device_name_from_handle_length) == 0) {
			device_name_from_handle[device_name_from_handle_length - 1] = '\0';
			if (print_info) printf("Device %i name using handle: %s\n", i, device_name_from_handle);
			success = true;
		}
		delete[] device_name_from_handle;
	}
	return success;
}


bool testJMLaserAPI(bool print_info = true) {
	int number_of_devices = jmLaserEnumerateDevices();
	if (number_of_devices <= 0) return false;
	int number_successful_projections = 0;

	for (int i = 0; i < number_of_devices; ++i) {
		int device_name_length = jmLaserGetDeviceListEntryLength(i);
		if (device_name_length > 0) {
			char *device_name = new char[device_name_length];
			if (jmLaserGetDeviceListEntry(i, device_name, (unsigned int) device_name_length) == 0) {
				device_name[device_name_length - 1] = '\0';
				if (print_info) printf("Device %i name: %s\n", i, device_name);
				testGetFriendlyNameAPI(device_name, print_info, i);
				testGetDeviceFamilyNameAPI(device_name, print_info, i);
				int device_handle = jmLaserOpenDevice(device_name);
				if (device_handle >= 0) {
					if (print_info) printf("Device %i handle: %i\n", i, device_handle);
					testGetDeviceNameAPI(device_handle, print_info, i);
				}
			}
			delete[] device_name;
		}
	}

	return (number_of_devices == number_successful_projections);
}


int main(int argc, char **argv) {
	if (testJMLaserAPI(true)) {
		return 0;
	} else {
		printf("No laser projector found!\n");
		return -1;
	}
}
