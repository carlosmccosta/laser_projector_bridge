#include <laser_projector_bridge/jmlaser_projector.h>
#include <laser_projector_bridge/vector_image_builder.h>
#include <laser_projector_bridge/pattern_builder.h>
#include <climits>
#include <chrono>
#include <thread>


void testJmlaserProjectorWithoutEnumeration() {
	std::string projector_name = laser_projector_bridge::JMLaserProjector::jmLaserBridgeGetDeviceListEntry(0);
	projector_name = laser_projector_bridge::JMLaserProjector::jmLaserBridgeGetDeviceListEntry(0);
	std::cout << "Projector name: " << projector_name << std::endl;
}


void testJmlaserProjectorSetup(unsigned int number_of_projectors) {
	for (unsigned int i = 0; i < number_of_projectors; ++i) {
		laser_projector_bridge::JMLaserProjector laser_projector;

		std::cout << ">>> |setupProjector()" << std::endl;
		laser_projector.setupProjector();
		std::cout << laser_projector << "\n\n" << std::endl;

//		std::string friendly_name = laser_projector.getProjectorFriendlyName();
//		laser_projector.setProjectorFriendlyName("Test Name");

		std::cout << ">>> |setupProjectorUsingIndex(" << i << ")" << std::endl;
		laser_projector.setupProjectorUsingIndex(i);
		std::cout << laser_projector << "\n\n" << std::endl;

//		laser_projector.setProjectorFriendlyName(friendly_name);

		std::cout << ">>> |setupProjectorUsingName(std::string(\"Netlase 1552 #0\"))" << std::endl;
		laser_projector.setupProjectorUsingName(std::string("Netlase 1552 #0"));
		std::cout << laser_projector << "\n\n" << std::endl;

		std::cout << ">>> |setupProjectorUsingFriendlyName(std::string(\"ILP 622 LAN\"))" << std::endl;
		//laser_projector.setupProjectorUsingFriendlyName(std::string("ILP 622 LAN"));
		std::cout << laser_projector << "\n\n" << std::endl;
	}
}


void createLaserOutputPattern(std::vector<JMVectorStruct> &points) {
//	laser_projector_bridge::pattern_builder::createSquareFullRange(points);
//	laser_projector_bridge::pattern_builder::createSquareScaled(points, 0.75);
//	laser_projector_bridge::pattern_builder::createSquareScaled(points, 0.6);
//	laser_projector_bridge::pattern_builder::createSquareScaled(points, 0.5);
//	laser_projector_bridge::pattern_builder::createSquareScaled(points, 0.4);
//	laser_projector_bridge::pattern_builder::createSquareScaled(points, 0.25);
	laser_projector_bridge::pattern_builder::createPlusFullRange(points);
//	laser_projector_bridge::pattern_builder::createCrossFullRange(points);
//	laser_projector_bridge::pattern_builder::createCrossScaled(points);
}


void createLaserOutputPatternUsingVectorImageBuilder(std::vector<JMVectorStruct> &points) {
	laser_projector_bridge::VectorImageBuilder vector_image_builder;
	vector_image_builder.setInterpolationDistanceInProjectorRange((int32_t)(UINT32_MAX * 0.05));
	vector_image_builder.setLineFirstPointIgnoreDistanceSquaredInProjectorRange(std::pow(UINT32_MAX * 0.0013, 2));
	vector_image_builder.setLineFirstPointMergeDistanceSquaredInProjectorRange(std::pow(UINT32_MAX * 0.0005, 2));
	vector_image_builder.setRadialDistortionCoefficientSecondDegreeInvertedUV(0.08);
	vector_image_builder.setRadialDistortionCoefficientSecondDegree(-0.044);
	vector_image_builder.setRadialDistortionCoefficientFourthDegree(-0.007);
	vector_image_builder.setRadialDistortionCoefficientSixthDegree(-0.005);
	vector_image_builder.startNewVectorImage();

	laser_projector_bridge::pattern_builder::createGridInProjectorRange(vector_image_builder, 6, 6, UINT32_MAX / 10, UINT32_MAX / 10, INT32_MIN + (int32_t)((UINT32_MAX / 10) * 3), INT32_MIN + (int32_t)((UINT32_MAX / 10) * 3));

	for (double scale = 0.0; scale <= 1.0; scale += 0.1)
		laser_projector_bridge::pattern_builder::createSquareScaled(vector_image_builder, scale);

	laser_projector_bridge::pattern_builder::createPlusFullRange(vector_image_builder);
	laser_projector_bridge::pattern_builder::createCrossFullRange(vector_image_builder);

	laser_projector_bridge::pattern_builder::createHorizontalDiamondOutsideDrawingArea(vector_image_builder);
	laser_projector_bridge::pattern_builder::createVerticalDiamondOutsideDrawingArea(vector_image_builder);
	laser_projector_bridge::pattern_builder::createCrossOutsideDrawingArea(vector_image_builder);
	laser_projector_bridge::pattern_builder::createPlusOutsideDrawingArea(vector_image_builder);

	vector_image_builder.finishVectorImage();
	points = vector_image_builder.getVectorImagePoints();
}


void sendMovingHorizontalLine(laser_projector_bridge::JMLaserProjector& laser_projector, int64_t projection_time_per_line_ms = 20000, size_t number_of_lines = 1000, unsigned int projector_speed = 1000) {
	std::vector<JMVectorStruct> points;
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MIN, 0));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MIN, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, INT_MIN, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, INT_MIN, 0));
	int vertical_line_increment = (int)(std::numeric_limits<uint32_t>::max() / number_of_lines);
	for (size_t i = 0; i < number_of_lines; ++i) {
		laser_projector.sendVectorImageToProjector(points, projector_speed, 0);
		std::this_thread::sleep_for(std::chrono::microseconds(projection_time_per_line_ms));
		points[0].y += vertical_line_increment;
		points[1].y += vertical_line_increment;
		points[2].y += vertical_line_increment;
		points[3].y += vertical_line_increment;
	}
	laser_projector.stopOutput();
}


void sendMovingVerticalLine(laser_projector_bridge::JMLaserProjector& laser_projector, int64_t projection_time_per_line_ms = 20000, size_t number_of_lines = 1000, unsigned int projector_speed = 1000) {
	std::vector<JMVectorStruct> points;
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MIN, 0));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MIN, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MAX, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MAX, 0));
	int vertical_line_increment = (int)(std::numeric_limits<uint32_t>::max() / number_of_lines);
	for (size_t i = 0; i < number_of_lines; ++i) {
		laser_projector.sendVectorImageToProjector(points, projector_speed, 0);
		std::this_thread::sleep_for(std::chrono::microseconds(projection_time_per_line_ms));
		points[0].x += vertical_line_increment;
		points[1].x += vertical_line_increment;
		points[2].x += vertical_line_increment;
		points[3].x += vertical_line_increment;
	}
	laser_projector.stopOutput();
}


void testJmlaserOutput(unsigned int number_of_projectors, bool send_test_patterns = true, bool send_moving_horizontal_line = false, bool send_moving_vertical_line = false) {
	std::vector<JMVectorStruct> points;
//	createLaserOutputPattern(points);
	createLaserOutputPatternUsingVectorImageBuilder(points);

	for (unsigned int i = 0; i < number_of_projectors; ++i) {
		laser_projector_bridge::JMLaserProjector laser_projector;
		std::cout << ">>> |setupProjectorUsingIndex(i)" << std::endl;
		laser_projector.setupProjectorUsingIndex(i);
		std::cout << laser_projector << "\n\n" << std::endl;
		std::cout << ">>> Sending pattern to projector " << i << std::endl;
		laser_projector.startOutput();

		if (send_moving_horizontal_line) {
			sendMovingHorizontalLine(laser_projector);
		}

		if (send_moving_vertical_line) {
			sendMovingVerticalLine(laser_projector);
		}

		if (send_test_patterns) {
			if (laser_projector.sendVectorImageToProjector(points, 1000, 0)) {
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
}



int main(int argc, char **argv) {
	std::cout << "===============================================================================" << std::endl;
	std::cout << ">>> Testing laser_projector_bridge::JMLaserProjector" << std::endl;
	std::cout << "===============================================================================\n" << std::endl;

	laser_projector_bridge::JMLaserProjector::jmLaserBridgeOpenDll();

	testJmlaserProjectorWithoutEnumeration();

	int number_of_projectors = laser_projector_bridge::JMLaserProjector::jmLaserBridgeEnumerateDevices();
	if (number_of_projectors <= 0) {
		std::cout << ">>> No projectors were found!" << std::endl;
		return -1;
	} else {
		testJmlaserProjectorSetup((unsigned int)number_of_projectors);
		testJmlaserOutput((unsigned int)number_of_projectors);
	}

	laser_projector_bridge::JMLaserProjector::jmLaserBridgeCloseDll();
	return 0;
}
