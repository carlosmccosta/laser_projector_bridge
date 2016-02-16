/**\file JMLaserProjector.cpp
 * \brief Description...
 *
 * @version 1.0
 * @author Carlos Miguel Correia da Costa
 */

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <includes>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#include <laser_projector_bridge/JMLaserProjector.h>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </includes>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



namespace laser_projector_bridge {

// ===============================================================================  <public-section>   ============================================================================
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <constructors-destructor>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
JMLaserProjector::JMLaserProjector() :
		projector_list_entry_index_(0),
		projector_handle_(-1) {
	jmLaserEnumerateDevices();
}

JMLaserProjector::~JMLaserProjector() {
	if (projector_handle_ >= 0) {
		jmLaserStopOutput(projector_handle_);
		jmLaserCloseDevice(projector_handle_);
	}
	jmLaserCloseDll();
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </constructors-destructor>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <static functions>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
std::string JMLaserProjector::jmLaserBridgeGetDeviceListEntry(unsigned int list_index) {
	int device_name_length = jmLaserGetDeviceListEntryLength(list_index);
	if (device_name_length > 0) {
		char *device_name = new char[device_name_length];
		if (jmLaserGetDeviceListEntry(list_index, device_name, (unsigned int) device_name_length) == 0) {
			device_name[device_name_length - 1] = '\0';
		}
		std::string device_name_str(device_name);
		delete[] device_name;
		return device_name_str;
	}
	return std::string("");
}

std::string JMLaserProjector::jmLaserBridgeGetDeviceName(int projector_handle) {
	int device_name_from_handle_length = jmLaserGetDeviceNameLength(projector_handle);
	if (device_name_from_handle_length > 0) {
		char *device_name_from_handle = new char[device_name_from_handle_length];
		if (jmLaserGetDeviceName(projector_handle, device_name_from_handle, (unsigned int) device_name_from_handle_length) == 0) {
			device_name_from_handle[device_name_from_handle_length - 1] = '\0';
		}
		std::string device_name_from_handle_str(device_name_from_handle);
		delete[] device_name_from_handle;
		return device_name_from_handle_str;
	}
	return std::string("");
}

std::string JMLaserProjector::jmLaserBridgeGetDeviceFamilyName(std::string projector_name) {
	std::vector<char> projector_name_c(projector_name.begin(), projector_name.end());
	int device_family_name_length = jmLaserGetDeviceFamilyNameLength(projector_name_c.data());
	if (device_family_name_length > 0) {
		char *device_family_name = new char[device_family_name_length];
		if (jmLaserGetDeviceFamilyName(projector_name_c.data(), device_family_name, (unsigned int) device_family_name_length) == 0) {
			device_family_name[device_family_name_length - 1] = '\0';
		}
		std::string device_family_name_str(device_family_name);
		delete[] device_family_name;
		return device_family_name_str;
	}
	return std::string("");
}

std::string JMLaserProjector::jmLaserBridgeGetFriendlyName(std::string projector_name) {
	std::vector<char> projector_name_c(projector_name.begin(), projector_name.end());
	int device_friendly_name_length = jmLaserGetFriendlyNameLength(projector_name_c.data());
	if (device_friendly_name_length > 0) {
		char *device_friendly_name = new char[device_friendly_name_length];
		if (jmLaserGetFriendlyName(projector_name_c.data(), device_friendly_name, (unsigned int) device_friendly_name_length) == 0) {
			device_friendly_name[device_friendly_name_length - 1] = '\0';
		}
		std::string device_friendly_name_str(device_friendly_name);
		delete[] device_friendly_name;
		return device_friendly_name;
	}
	return std::string("");
}

bool JMLaserProjector::jmLaserBridgeSetFriendlyName(int projector_handle, std::string projector_friendly_name) {
	std::vector<char> projector_friendly_name_c(projector_friendly_name.begin(), projector_friendly_name.end());
	return (jmLaserSetFriendlyName(projector_handle, projector_friendly_name_c.data()) == 0);
}

int JMLaserProjector::jmLaserBridgeOpenDevice(std::string projector_name) {
	std::vector<char> projector_name_c(projector_name.begin(), projector_name.end());
	return jmLaserOpenDevice(projector_name_c.data());
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <static functions/>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <functions>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
bool JMLaserProjector::setupProjector() {
	return false;
}

bool JMLaserProjector::setupProjectorUsingName(std::string projector_name) {
	return false;
}

bool JMLaserProjector::setupProjectorUsingFriendlyName(std::string projector_friendly_name) {
	return false;
}

bool JMLaserProjector::startOutput() {
	return false;
}

bool JMLaserProjector::sendVectorImage(std::vector<JMVectorStruct> points, unsigned int speed, unsigned int repetitions) {
	return false;
}

bool JMLaserProjector::stopOutput() {
	return false;
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <-functions>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ===============================================================================  </public-section>   ===========================================================================

}
