#include <iostream>
#include <jmlaser.h>

bool listDevices(int ff) {
	bool temp = false;
	int temp1 = 10;
	int temp2 = 5;
	int number_of_devices = jmLaserEnumerateDevices();
	if (number_of_devices <= 0) return false;
	

	for (int i = 0; i < number_of_devices; ++i) {
		int device_name_lenght = jmLaserGetDeviceListEntryLength(i);		
		char* device_name = new char[device_name_lenght + 1];
		if (jmLaserGetDeviceListEntry(i, device_name, device_name_lenght) == 0) {
			std::cout << "Device name: " << device_name << std::endl;
		}
		delete device_name;
	}
	return true;
}


int main(int argc, char** argv) {
	if (listDevices(0)) {
		return 0;
	} else {
		std::cerr << "No laser projector found" << std::endl;
		return -1;
	}
}
