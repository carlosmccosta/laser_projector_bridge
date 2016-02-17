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
		projector_handle_(-1),
		projector_output_started_(false) {
	JMLaserProjector::jmLaserBridgeEnumerateDevices();
}

JMLaserProjector::~JMLaserProjector() {
	if (projector_handle_ >= 0) {
		if (projector_output_started_) { jmLaserStopOutput(projector_handle_); }
		jmLaserCloseDevice(projector_handle_);
	}
	jmLaserCloseDll();
}

void JMLaserProjector::resetProjector() {
	if (projector_output_started_) {
		stopOutput();
		projector_output_started_ = false;
	}

	if (projector_handle_ >= 0) {
		closeProjector();
		projector_handle_ = -1;
	}

	projector_list_entry_index_ = 0;
	projector_name_ = std::string("");
	projector_name_from_handle_ = std::string("");
	projector_friendly_name_ = std::string("");
	projector_family_name_ = std::string("");
}

int JMLaserProjector::s_number_of_projectors_ = JMLASER_ERROR_NOT_ENUMERATED;
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </constructors-destructor>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <static functions>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
int JMLaserProjector::jmLaserBridgeEnumerateDevices() {
	if (s_number_of_projectors_ <= 0) {
		s_number_of_projectors_ = jmLaserEnumerateDevices();
	}

	return s_number_of_projectors_;
}

std::string JMLaserProjector::jmLaserBridgeGetDeviceListEntry(unsigned int list_index) {
	int device_name_length = jmLaserGetDeviceListEntryLength(list_index);
	if (device_name_length == JMLASER_ERROR_NOT_ENUMERATED) {
		if (JMLaserProjector::jmLaserBridgeEnumerateDevices() <= 0) { return std::string(""); }
		device_name_length = jmLaserGetDeviceListEntryLength(list_index);
	}
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
	if (projector_name.empty()) { return std::string(""); }

	std::vector<char> projector_name_c(projector_name.begin(), projector_name.end());
	projector_name_c.push_back('\0');
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
	if (projector_name.empty()) { return std::string(""); }
	std::vector<char> projector_name_c(projector_name.begin(), projector_name.end());
	projector_name_c.push_back('\0');
	int device_friendly_name_length = jmLaserGetFriendlyNameLength(projector_name_c.data());
	if (device_friendly_name_length > 0) {
		char *device_friendly_name = new char[device_friendly_name_length];
		if (jmLaserGetFriendlyName(projector_name_c.data(), device_friendly_name, (unsigned int) device_friendly_name_length) == 0) {
			device_friendly_name[device_friendly_name_length - 1] = '\0';
		}
		std::string device_friendly_name_str(device_friendly_name);
		delete[] device_friendly_name;
		return device_friendly_name_str;
	}
	return std::string("");
}

bool JMLaserProjector::jmLaserBridgeSetFriendlyName(int projector_handle, std::string projector_friendly_name) {
	if (projector_friendly_name.empty()) { return false; }
	std::vector<char> projector_friendly_name_c(projector_friendly_name.begin(), projector_friendly_name.end());
	projector_friendly_name_c.push_back('\0');
	return (jmLaserSetFriendlyName(projector_handle, projector_friendly_name_c.data()) == 0);
}

int JMLaserProjector::jmLaserBridgeOpenDevice(std::string projector_name) {
	if (projector_name.empty()) { return false; }
	std::vector<char> projector_name_c(projector_name.begin(), projector_name.end());
	projector_name_c.push_back('\0');
	return jmLaserOpenDevice(projector_name_c.data());
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <static functions/>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <functions>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
bool JMLaserProjector::setupProjector() {
	if (JMLaserProjector::s_number_of_projectors_ > 0) {
		return setupProjectorUsingIndex(0);
	}
	return false;
}

bool JMLaserProjector::setupProjectorUsingName(std::string projector_name) {
	if (JMLaserProjector::s_number_of_projectors_ > 0) {
		for (int i = 0; i < s_number_of_projectors_; ++i) {
			if (JMLaserProjector::jmLaserBridgeGetDeviceListEntry(i) == projector_name) {
				return setupProjectorUsingIndex(i);
			}
		}
	}
	return false;
}

bool JMLaserProjector::setupProjectorUsingFriendlyName(std::string projector_friendly_name) {
	if (JMLaserProjector::s_number_of_projectors_ > 0) {
		for (int i = 0; i < s_number_of_projectors_; ++i) {
			std::string projector_name = JMLaserProjector::jmLaserBridgeGetDeviceListEntry(i);
			if (JMLaserProjector::jmLaserBridgeGetFriendlyName(projector_name) == projector_friendly_name) {
				return setupProjectorUsingIndex(i);
			}
		}
	}
	return false;
}

bool JMLaserProjector::setupProjectorUsingIndex(unsigned int projector_index) {
	resetProjector();
	projector_list_entry_index_ = projector_index;
	projector_name_ = JMLaserProjector::jmLaserBridgeGetDeviceListEntry(projector_index);
	if (!projector_name_.empty()) {
		projector_friendly_name_ = JMLaserProjector::jmLaserBridgeGetFriendlyName(projector_name_);
		projector_family_name_ = JMLaserProjector::jmLaserBridgeGetDeviceFamilyName(projector_name_);
		projector_handle_ = JMLaserProjector::jmLaserBridgeOpenDevice(projector_name_);

		if (projector_handle_ >= 0) {
			projector_name_from_handle_ = JMLaserProjector::jmLaserBridgeGetDeviceName(projector_handle_);
			return true;
		}
	}
	resetProjector();
	return false;
}

bool JMLaserProjector::closeProjector() {
	if (projector_handle_ >= 0) {
		if (jmLaserCloseDevice(projector_handle_) == 0) {
			projector_handle_ = -1;
			return true;
		}
	}
	return false;
}

bool JMLaserProjector::startOutput() {
	if (projector_handle_ >= 0) {
		if (jmLaserStartOutput(projector_handle_) == 0) {
			projector_output_started_ = true;
			return true;
		}
	}
	return false;
}

bool JMLaserProjector::sendVectorImageToProjector(std::vector<JMVectorStruct> points, unsigned int speed, unsigned int repetitions) {
	if (projector_handle_ >= 0 && !points.empty()) {
		int wait_status = jmLaserWaitForDeviceReady(projector_handle_);
		if (wait_status == JMLASER_ERROR_OUTPUT_NOT_STARTED) {
			if (startOutput()) {
				wait_status = jmLaserWaitForDeviceReady(projector_handle_);
			} else {
				return false;
			}
		}

		if (wait_status == JMLASER_DEVICE_READY) {
			jmLaserWriteFrame(projector_handle_, points.data(), points.size(), speed, repetitions);
		}
	}
	return false;
}

bool JMLaserProjector::stopOutput() {
	if (projector_handle_ >= 0 && projector_output_started_) {
		if (jmLaserStopOutput(projector_handle_) == 0) {
			projector_output_started_ = false;
			return true;
		}
	}
	return false;
}

std::ostream& operator<<(std::ostream& os, const JMLaserProjector& jmlp) {
	os << "############################## JMLaserProjector ###############################\n";
	os << "# number_of_projectors:\t\t" 		<< JMLaserProjector::s_number_of_projectors_ << "\n";
	os << "# projector_list_entry_index:\t" 	<< jmlp.projector_list_entry_index_ << "\n";
	os << "# projector_handle:\t\t" 			<< jmlp.projector_handle_ << "\n";
	os << "# projector_name:\t\t" 				<< jmlp.projector_name_ << "\n";
	os << "# projector_name_from_handle:\t" 	<< jmlp.projector_name_from_handle_ << "\n";
	os << "# projector_friendly_name:\t" 		<< jmlp.projector_friendly_name_<< "\n";
	os << "# projector_family_name:\t" 		<< jmlp.projector_family_name_ << "\n";
	os << "# projector_output_started:\t" 	<< jmlp.projector_output_started_ << "\n";
	os << "###############################################################################" << std::endl;
	return os;
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <-functions>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ===============================================================================  </public-section>   ===========================================================================
}
