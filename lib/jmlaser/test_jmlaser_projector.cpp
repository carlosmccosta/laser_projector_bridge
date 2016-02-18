#include <string>
#include <iostream>
#include <vector>
#include <climits>
#include <laser_projector_bridge/JMLaserProjector.h>


void test_jmlaser_projector_without_enumeration() {
	std::string projector_name = laser_projector_bridge::JMLaserProjector::jmLaserBridgeGetDeviceListEntry(0);
	projector_name = laser_projector_bridge::JMLaserProjector::jmLaserBridgeGetDeviceListEntry(0);
	std::cout << "Projector name: " << projector_name << std::endl;
}


void test_jmlaser_projector_setup(int number_of_projectors) {
	for (int i = 0; i < number_of_projectors; ++i) {
		laser_projector_bridge::JMLaserProjector laser_projector;

		std::cout << ">>> |setupProjector()" << std::endl;
		laser_projector.setupProjector();
		std::cout << laser_projector << "\n\n" << std::endl;

		std::string friendly_name = laser_projector.getProjectorFriendlyName();
		laser_projector.setProjectorFriendlyName(friendly_name + " TestFriendlyName");

		std::cout << ">>> |setupProjectorUsingIndex(i)" << std::endl;
		laser_projector.setupProjectorUsingIndex(i);
		std::cout << laser_projector << "\n\n" << std::endl;

		laser_projector.setProjectorFriendlyName(friendly_name);

		std::cout << ">>> |setupProjectorUsingName(std::string(\"Netlase 1552 #0\"))" << std::endl;
		laser_projector.setupProjectorUsingName(std::string("Netlase 1552 #0"));
		std::cout << laser_projector << "\n\n" << std::endl;

		std::cout << ">>> |setupProjectorUsingFriendlyName(std::string(\"ILP 622 LAN\"))" << std::endl;
		laser_projector.setupProjectorUsingFriendlyName(std::string("ILP 622 LAN"));
		std::cout << laser_projector << "\n\n" << std::endl;
	}
}


void create_laser_output_pattern(std::vector<JMVectorStruct>& points) {
	int position_limit = INT_MAX;
	unsigned short color_limit = USHRT_MAX;
	unsigned short limit_intensity = USHRT_MAX;
	points.push_back(JMVectorStruct{(int)(position_limit * 0.1), (int)(position_limit * 0.1), color_limit, color_limit, color_limit, limit_intensity, color_limit, color_limit, color_limit, color_limit});
	points.push_back(JMVectorStruct{(int)(position_limit * 0.2), (int)(position_limit * 0.2), color_limit, color_limit, color_limit, limit_intensity, color_limit, color_limit, color_limit, color_limit});

	position_limit = SHRT_MAX;
	color_limit = USHRT_MAX;
	limit_intensity = USHRT_MAX;
	points.push_back(JMVectorStruct{(int)(position_limit * 0.1), (int)(position_limit * 0.1), color_limit, color_limit, color_limit, limit_intensity, color_limit, color_limit, color_limit, color_limit});
	points.push_back(JMVectorStruct{(int)(position_limit * 0.2), (int)(position_limit * 0.2), color_limit, color_limit, color_limit, limit_intensity, color_limit, color_limit, color_limit, color_limit});

	position_limit = 1000;
	color_limit = UCHAR_MAX;
	limit_intensity = UCHAR_MAX;
	points.push_back(JMVectorStruct{(int)(position_limit * 0.1), (int)(position_limit * 0.1), color_limit, color_limit, color_limit, limit_intensity, color_limit, color_limit, color_limit, color_limit});
	points.push_back(JMVectorStruct{(int)(position_limit * 0.2), (int)(position_limit * 0.2), color_limit, color_limit, color_limit, limit_intensity, color_limit, color_limit, color_limit, color_limit});

	position_limit = 100;
	color_limit = 100;
	limit_intensity = 100;
	points.push_back(JMVectorStruct{(int)(position_limit * 0.1), (int)(position_limit * 0.1), color_limit, color_limit, color_limit, limit_intensity, color_limit, color_limit, color_limit, color_limit});
	points.push_back(JMVectorStruct{(int)(position_limit * 0.2), (int)(position_limit * 0.2), color_limit, color_limit, color_limit, limit_intensity, color_limit, color_limit, color_limit, color_limit});

	position_limit = 10;
	color_limit = 0;
	limit_intensity = 100;
	points.push_back(JMVectorStruct{(int)(position_limit * 0.1), (int)(position_limit * 0.1), color_limit, color_limit, color_limit, limit_intensity, color_limit, color_limit, color_limit, color_limit});
	points.push_back(JMVectorStruct{(int)(position_limit * 0.2), (int)(position_limit * 0.2), color_limit, color_limit, color_limit, limit_intensity, color_limit, color_limit, color_limit, color_limit});
}


void test_jmlaser_output(int number_of_projectors) {
	std::vector<JMVectorStruct> points;
	create_laser_output_pattern(points);

	for (int i = 0; i < number_of_projectors; ++i) {
		laser_projector_bridge::JMLaserProjector laser_projector;
		std::cout << ">>> |setupProjectorUsingIndex(i)" << std::endl;
		laser_projector.setupProjectorUsingIndex(i);
		laser_projector.startOutput();
		std::cout << laser_projector << "\n\n" << std::endl;
		std::cout << ">>> Sending pattern to projector " << i << std::endl;
		if (laser_projector.sendVectorImageToProjector(points, 10000, 0)) {
			std::cout << ">>> - Pattern was sent successfully" << std::endl;
		} else {
			std::cout << ">>> - Failed to send pattern" << std::endl;
		}
		std::cout << ">>> - Press ENTER to continue..." << std::endl;
		std::string temp;
		std::getline(std::cin, temp);
		laser_projector.stopOutput();
	}
}



int main(int argc, char **argv) {
	std::cout << "===============================================================================" << std::endl;
	std::cout << ">>> Testing laser_projector_bridge::JMLaserProjector" << std::endl;
	std::cout << "===============================================================================\n" << std::endl;

//	test_jmlaser_projector_without_enumeration();

	int number_of_projectors = laser_projector_bridge::JMLaserProjector::jmLaserBridgeEnumerateDevices();
	if (number_of_projectors <= 0) {
		std::cout << ">>> No projectors were found!" << std::endl;
		return -1;
	} else {
//		test_jmlaser_projector_setup(number_of_projectors);
		test_jmlaser_output(number_of_projectors);
	}

	return 0;
}
