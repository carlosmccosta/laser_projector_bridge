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
		projector_maximum_number_of_vectors_per_frame_(-1),
		projector_minimum_speed_(-1),
		projector_maximum_speed_(-1),
		projector_speed_step_(-1),
		projector_output_started_(false) {
	JMLaserProjector::jmLaserBridgeEnumerateDevices();
}

JMLaserProjector::~JMLaserProjector() {
	if (projector_handle_ >= 0) {
		if (projector_output_started_) { jmLaserStopOutput(projector_handle_); }
		jmLaserCloseDevice(projector_handle_);
	}
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
	projector_maximum_number_of_vectors_per_frame_ = -1;
	projector_minimum_speed_ = -1;
	projector_maximum_speed_ = -1;
	projector_speed_step_ = -1;
	projector_network_address_ = std::string("");
	projector_name_ = std::string("");
	projector_name_from_handle_ = std::string("");
	projector_friendly_name_ = std::string("");
	projector_family_name_ = std::string("");
}

int JMLaserProjector::s_number_of_projectors_ = JMLASER_ERROR_NOT_ENUMERATED;
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </constructors-destructor>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <static functions>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
int JMLaserProjector::jmLaserBridgeOpenDll() {
	return jmLaserOpenDll();
}

int JMLaserProjector::jmLaserBridgeCloseDll() {
	return jmLaserCloseDll();
}

int JMLaserProjector::jmLaserBridgeEnumerateDevices() {
	if (JMLaserProjector::s_number_of_projectors_ <= 0) {
		JMLaserProjector::s_number_of_projectors_ = jmLaserEnumerateDevices();
	} else {
		JMLaserProjector::s_number_of_projectors_ = JMLaserProjector::jmLaserBridgeGetDeviceListLength();
	}
	return JMLaserProjector::s_number_of_projectors_;
}

std::string JMLaserProjector::jmLaserBridgeGetDeviceListEntry(unsigned int list_index) {
	if (JMLaserProjector::jmLaserBridgeEnumerateDevices() <= 0) { return std::string(""); }
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
	if (projector_handle < 0) { return std::string(""); }
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

std::string JMLaserProjector::jmLaserBridgeGetDeviceFamilyName(const std::string& projector_name) {
	if (projector_name.empty()) { return std::string(""); }
	if (JMLaserProjector::jmLaserBridgeEnumerateDevices() <= 0) { return std::string(""); }
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

std::string JMLaserProjector::jmLaserBridgeGetFriendlyName(const std::string& projector_name) {
	if (projector_name.empty()) { return std::string(""); }
	if (JMLaserProjector::jmLaserBridgeEnumerateDevices() <= 0) { return std::string(""); }
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

bool JMLaserProjector::jmLaserBridgeSetFriendlyName(int projector_handle, const std::string& projector_friendly_name) {
	if (projector_handle < 0 || projector_friendly_name.empty()) { return false; }
	if (JMLaserProjector::jmLaserBridgeEnumerateDevices() <= 0) { return false; }
	std::vector<char> projector_friendly_name_c(projector_friendly_name.begin(), projector_friendly_name.end());
	projector_friendly_name_c.push_back('\0');
	return (jmLaserSetFriendlyName(projector_handle, projector_friendly_name_c.data()) == 0);
}

int JMLaserProjector::jmLaserBridgeOpenDevice(const std::string& projector_name) {
	if (projector_name.empty()) { return false; }
	if (JMLaserProjector::jmLaserBridgeEnumerateDevices() <= 0) { return false; }
	std::vector<char> projector_name_c(projector_name.begin(), projector_name.end());
	projector_name_c.push_back('\0');
	return jmLaserOpenDevice(projector_name_c.data());
}

int JMLaserProjector::jmLaserBridgeGetMaxFrameSize(int projector_handle) {
	return jmLaserGetMaxFrameSize(projector_handle);
}

int JMLaserProjector::jmLaserBridgeGetDeviceListLength() {
	if (JMLaserProjector::s_number_of_projectors_ <= 0) {
		JMLaserProjector::s_number_of_projectors_ = jmLaserEnumerateDevices();
	}
	return jmLaserGetDeviceListLength();
}

bool JMLaserProjector::jmLaserBridgeGetIsNetworkDevice(const std::string& projector_name) {
	if (projector_name.empty()) { return false; }
	if (JMLaserProjector::jmLaserBridgeEnumerateDevices() <= 0) { return false; }
	std::vector<char> projector_name_c(projector_name.begin(), projector_name.end());
	projector_name_c.push_back('\0');
	return (jmLaserGetIsNetworkDevice(projector_name_c.data()) == 1);
}

std::string JMLaserProjector::jmLaserBridgeGetNetworkAddress(const std::string& projector_name) {
	if (projector_name.empty()) { return std::string(""); }
	if (JMLaserProjector::jmLaserBridgeEnumerateDevices() <= 0) { return std::string(""); }
	std::vector<char> projector_name_c(projector_name.begin(), projector_name.end());
	projector_name_c.push_back('\0');
	int network_address_length = jmLaserGetNetworkAddressLength(projector_name_c.data());
	if (network_address_length > 0) {
		char *network_address = new char[network_address_length];
		if (jmLaserGetNetworkAddress(projector_name_c.data(), network_address, (unsigned int) network_address_length) == 0) {
			network_address[network_address_length - 1] = '\0';
		}
		std::string network_address_str(network_address);
		delete[] network_address;
		return network_address_str;
	}
	return std::string("");
}

int JMLaserProjector::jmLaserBridgeGetMinSpeed(int projector_handle) {
	return jmLaserGetMinSpeed(projector_handle);
}

int JMLaserProjector::jmLaserBridgeGetMaxSpeed(int projector_handle) {
	return jmLaserGetMaxSpeed(projector_handle);
}

int JMLaserProjector::jmLaserBridgeGetSpeedStep(int projector_handle) {
	return jmLaserGetSpeedStep(projector_handle);
}

JMVectorStruct JMLaserProjector::createSingleColorLaserPoint(int x, int y, unsigned short intensity) {
	return JMVectorStruct{x, y, 0, 0, 0, intensity, 0, 0, 0, 0};
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <static functions/>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <functions>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
bool JMLaserProjector::setupProjector() {
	JMLaserProjector::s_number_of_projectors_ = JMLaserProjector::jmLaserBridgeGetDeviceListLength();
	if (JMLaserProjector::s_number_of_projectors_ > 0) {
		return setupProjectorUsingIndex(0);
	}
	return false;
}

bool JMLaserProjector::setupProjectorUsingName(const std::string& projector_name) {
	if (projector_name.empty()) { return false; }
	JMLaserProjector::s_number_of_projectors_ = JMLaserProjector::jmLaserBridgeGetDeviceListLength();
	if (JMLaserProjector::s_number_of_projectors_ > 0) {
		for (int i = 0; i < JMLaserProjector::s_number_of_projectors_; ++i) {
			if (JMLaserProjector::jmLaserBridgeGetDeviceListEntry(i) == projector_name) {
				return setupProjectorUsingIndex(i);
			}
		}
	}
	return false;
}

bool JMLaserProjector::setupProjectorUsingFriendlyName(const std::string& projector_friendly_name) {
	if (projector_friendly_name.empty()) { return false; }
	JMLaserProjector::s_number_of_projectors_ = JMLaserProjector::jmLaserBridgeGetDeviceListLength();
	if (JMLaserProjector::s_number_of_projectors_ > 0) {
		for (int i = 0; i < JMLaserProjector::s_number_of_projectors_; ++i) {
			std::string projector_name = JMLaserProjector::jmLaserBridgeGetDeviceListEntry(i);
			if (JMLaserProjector::jmLaserBridgeGetFriendlyName(projector_name) == projector_friendly_name) {
				return setupProjectorUsingIndex(i);
			}
		}
	}
	return false;
}

bool JMLaserProjector::setupProjectorUsingIndex(unsigned int projector_index) {
	if (projector_index < 0) { return false; }
	resetProjector();
	projector_list_entry_index_ = projector_index;
	projector_name_ = JMLaserProjector::jmLaserBridgeGetDeviceListEntry(projector_index);
	if (!projector_name_.empty()) {
		projector_friendly_name_ = JMLaserProjector::jmLaserBridgeGetFriendlyName(projector_name_);
		projector_family_name_ = JMLaserProjector::jmLaserBridgeGetDeviceFamilyName(projector_name_);
		projector_handle_ = JMLaserProjector::jmLaserBridgeOpenDevice(projector_name_);

		if (projector_handle_ >= 0) {
			projector_name_from_handle_ = JMLaserProjector::jmLaserBridgeGetDeviceName(projector_handle_);
			projector_maximum_number_of_vectors_per_frame_ = JMLaserProjector::jmLaserBridgeGetMaxFrameSize(projector_handle_);
			if (JMLaserProjector::jmLaserBridgeGetIsNetworkDevice(projector_name_)) {
				projector_network_address_ = JMLaserProjector::jmLaserBridgeGetNetworkAddress(projector_name_);
			}
			projector_minimum_speed_ = JMLaserProjector::jmLaserBridgeGetMinSpeed(projector_handle_);
			projector_maximum_speed_ = JMLaserProjector::jmLaserBridgeGetMaxSpeed(projector_handle_);
			projector_speed_step_ = JMLaserProjector::jmLaserBridgeGetSpeedStep(projector_handle_);
			if (projector_maximum_number_of_vectors_per_frame_ > 0 && projector_minimum_speed_ >= 0 && projector_maximum_speed_ > 0 && projector_speed_step_ > 0) { return true; }
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


bool JMLaserProjector::setProjectorFriendlyName(const std::string& projector_friendly_name) {
	if (!projector_friendly_name.empty() && projector_handle_ >= 0) {
		if (jmLaserBridgeSetFriendlyName(projector_handle_, projector_friendly_name)) {
			projector_friendly_name_ = JMLaserProjector::jmLaserBridgeGetFriendlyName(projector_name_);
			return !projector_friendly_name_.empty();
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

bool JMLaserProjector::sendVectorImageToProjector(std::vector<JMVectorStruct>& points, unsigned int speed, unsigned int repetitions) {
	if (projector_handle_ >= 0 && !points.empty() /*&& speed >= projector_minimum_speed_*/ && speed <= projector_maximum_speed_ && repetitions >= 0 && projector_maximum_number_of_vectors_per_frame_ > 0) {
		if (points.size() > (size_t)projector_maximum_number_of_vectors_per_frame_) {
			points.resize((size_t)projector_maximum_number_of_vectors_per_frame_);
		}
		int wait_status = jmLaserWaitForDeviceReady(projector_handle_);
		if (wait_status == JMLASER_ERROR_OUTPUT_NOT_STARTED) {
			if (startOutput()) {
				wait_status = jmLaserWaitForDeviceReady(projector_handle_);
			} else {
				return false;
			}
		}

		if (wait_status == JMLASER_DEVICE_READY) {
			int write_status = jmLaserWriteFrame(projector_handle_, points.data(), points.size(), speed, repetitions);
			return (write_status == 0);
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
	os << "# number_of_projectors:\t\t\t" 				<< JMLaserProjector::s_number_of_projectors_ << "\n";
	os << "# projector_list_entry_index:\t\t" 			<< jmlp.projector_list_entry_index_ << "\n";
	os << "# projector_handle:\t\t\t" 					<< jmlp.projector_handle_ << "\n";
	os << "# maximum_number_of_vectors_per_frame:\t" 	<< jmlp.projector_maximum_number_of_vectors_per_frame_ << "\n";
	os << "# minimum_projection_speed:\t\t" 			<< jmlp.projector_minimum_speed_ << "\n";
	os << "# maximum_projection_speed:\t\t" 			<< jmlp.projector_maximum_speed_ << "\n";
	os << "# projection_speed_step:\t\t" 				<< jmlp.projector_speed_step_ << "\n";
	os << "# network_address:\t\t\t" 					<< jmlp.projector_network_address_ << "\n";
	os << "# projector_name:\t\t\t" 					<< jmlp.projector_name_ << "\n";
	os << "# projector_name_from_handle:\t\t" 			<< jmlp.projector_name_from_handle_ << "\n";
	os << "# projector_friendly_name:\t\t" 				<< jmlp.projector_friendly_name_<< "\n";
	os << "# projector_family_name:\t\t" 				<< jmlp.projector_family_name_ << "\n";
	os << "# projector_output_started:\t\t" 			<< jmlp.projector_output_started_ << "\n";
	os << "###############################################################################" << std::endl;
	return os;
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <-functions>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ===============================================================================  </public-section>   ===========================================================================
}
