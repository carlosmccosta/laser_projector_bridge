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

//		std::string friendly_name = laser_projector.getProjectorFriendlyName();
//		laser_projector.setProjectorFriendlyName("Test Name");

		std::cout << ">>> |setupProjectorUsingIndex(i)" << std::endl;
		laser_projector.setupProjectorUsingIndex(i);
		std::cout << laser_projector << "\n\n" << std::endl;

//		laser_projector.setProjectorFriendlyName(friendly_name);

		std::cout << ">>> |setupProjectorUsingName(std::string(\"Netlase 1552 #0\"))" << std::endl;
		laser_projector.setupProjectorUsingName(std::string("Netlase 1552 #0"));
		std::cout << laser_projector << "\n\n" << std::endl;

		std::cout << ">>> |setupProjectorUsingFriendlyName(std::string(\"ILP 622 LAN\"))" << std::endl;
		laser_projector.setupProjectorUsingFriendlyName(std::string("ILP 622 LAN"));
		std::cout << laser_projector << "\n\n" << std::endl;
	}
}


void create_laser_output_pattern_square_full_range(std::vector<JMVectorStruct> &points) {
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MIN, 0));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MIN, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, INT_MIN, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, INT_MAX, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MAX, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MIN, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MIN, 0));
}


void create_laser_output_pattern_square_scaled(std::vector<JMVectorStruct> &points) {
	unsigned short intensity = (unsigned short)(USHRT_MAX * 0.5);
	double position_scale = 0.9;
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MIN * position_scale), 0));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MIN * position_scale), intensity));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MAX * position_scale), (int)(INT_MIN * position_scale), intensity));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MAX * position_scale), (int)(INT_MAX * position_scale), intensity));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MAX * position_scale), intensity));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0, 0, intensity));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0, 0, 0));
}


void create_laser_output_pattern_plus_full_range(std::vector<JMVectorStruct> &points) {
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0, INT_MIN, 0));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0, INT_MIN, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0, INT_MAX, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, 0, 0));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, 0, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, 0, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, 0, 0));
}


void create_laser_output_pattern_cross_full_range(std::vector<JMVectorStruct> &points) {
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MIN, 0));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MIN, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN / 2, INT_MIN / 2, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0, 0, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX / 2, INT_MAX / 2, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, INT_MAX, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, INT_MIN, 0));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, INT_MIN, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX / 2, INT_MIN / 2, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0, 0, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN / 2, INT_MAX / 2, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MAX, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MAX, 0));
}


void create_laser_output_pattern_cross_scaled(std::vector<JMVectorStruct> &points) {
	double position_scale = 0.75;
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MIN * position_scale), 0));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MIN * position_scale), USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0, 0, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MAX * position_scale), (int)(INT_MAX * position_scale), USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MAX * position_scale), (int)(INT_MAX * position_scale), 0));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MAX * position_scale), (int)(INT_MIN * position_scale), 0));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MAX * position_scale), (int)(INT_MIN * position_scale), USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0, 0, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MAX * position_scale), USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MAX * position_scale), 0));
}


void create_laser_output_pattern(std::vector<JMVectorStruct>& points) {
	create_laser_output_pattern_square_full_range(points);
	create_laser_output_pattern_square_scaled(points);
	create_laser_output_pattern_plus_full_range(points);
	create_laser_output_pattern_cross_full_range(points);
	create_laser_output_pattern_cross_scaled(points);
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
		if (laser_projector.sendVectorImageToProjector(points, 500, 0)) {
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

	laser_projector_bridge::JMLaserProjector::jmLaserBridgeOpenDll();

	test_jmlaser_projector_without_enumeration();

	int number_of_projectors = laser_projector_bridge::JMLaserProjector::jmLaserBridgeEnumerateDevices();
	if (number_of_projectors <= 0) {
		std::cout << ">>> No projectors were found!" << std::endl;
		return -1;
	} else {
		test_jmlaser_projector_setup(number_of_projectors);
		test_jmlaser_output(number_of_projectors);
	}

	laser_projector_bridge::JMLaserProjector::jmLaserBridgeCloseDll();
	return 0;
}
