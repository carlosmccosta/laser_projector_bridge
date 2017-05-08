#include <laser_projector_bridge/jmlaser_projector.h>
#include <laser_projector_bridge/vector_image_builder.h>
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


void createLaserOutputPatternSquareFullRange(std::vector<JMVectorStruct> &points) {
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MIN, 0));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MIN, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0      , INT_MIN, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, INT_MIN, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, 0      , USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, INT_MAX, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0      , INT_MAX, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MAX, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, 0      , USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MIN, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MIN, 0));
}


void createLaserOutputPatternSquareScaled(std::vector<JMVectorStruct> &points, double position_scale = 0.9) {
	unsigned short intensity = (unsigned short)(USHRT_MAX * 0.9);
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MIN * position_scale), 0));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MIN * position_scale), intensity));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MAX * position_scale), (int)(INT_MIN * position_scale), intensity));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MAX * position_scale), (int)(INT_MAX * position_scale), intensity));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MAX * position_scale), intensity));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MIN * position_scale), intensity));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MIN * position_scale), 0));
}

void createLaserOutputPatternSquareScaledUsingVectorImageBuilder(laser_projector_bridge::VectorImageBuilder &vector_image_builder, double scale = 1.0) {
	double x_offset = (vector_image_builder.getDrawingAreaWidth() - (vector_image_builder.getDrawingAreaWidth() * scale)) * 0.5;
	double y_offset = (vector_image_builder.getDrawingAreaHeight() - (vector_image_builder.getDrawingAreaHeight() * scale)) * 0.5;
	double x_min = x_offset;
	double x_max = vector_image_builder.getDrawingAreaWidth() - x_offset;
	double y_min = y_offset;
	double y_max = vector_image_builder.getDrawingAreaHeight() - y_offset;
	vector_image_builder.addNewLine(x_min, y_min, x_max, y_min);
	vector_image_builder.addNewLine(x_max, y_min, x_max, y_max);
	vector_image_builder.addNewLine(x_max, y_max, x_min, y_max);
	vector_image_builder.addNewLine(x_min, y_max, x_min, y_min);
}

void createLaserOutputPatternPlusFullRange(std::vector<JMVectorStruct> &points) {
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0,       INT_MIN, 0));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0,       INT_MIN, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0,       INT_MAX, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, 0,       0));
//	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, 0,       USHRT_MAX));
//	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, 0,       USHRT_MAX));
//	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, 0,       0));
}

void createLaserOutputPatternPlusFullRangeUsingVectorImageBuilder(laser_projector_bridge::VectorImageBuilder &vector_image_builder) {
	double half_width = vector_image_builder.getDrawingAreaWidth() * 0.5;
	double half_height = vector_image_builder.getDrawingAreaHeight() * 0.5;
	vector_image_builder.addNewLine(0.0, half_height, vector_image_builder.getDrawingAreaWidth(), half_height);
	vector_image_builder.addNewLine(half_width, 0.0, half_width, vector_image_builder.getDrawingAreaHeight());
}


void createLaserOutputPatternCrossFullRange(std::vector<JMVectorStruct> &points) {
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN,     INT_MIN,     0));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN,     INT_MIN,     USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN / 2, INT_MIN / 2, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0,           0,           USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX / 2, INT_MAX / 2, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX,     INT_MAX,     USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX,     INT_MIN,     0));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX,     INT_MIN,     USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX / 2, INT_MIN / 2, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0,           0,           USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN / 2, INT_MAX / 2, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN,     INT_MAX,     USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN,     INT_MAX,     0));
}

void createLaserOutputPatternCrossFullRangeUsingVectorImageBuilder(laser_projector_bridge::VectorImageBuilder &vector_image_builder) {
	vector_image_builder.addNewLine(INT_MIN, INT_MAX, INT_MAX, INT_MIN);
	vector_image_builder.addNewLine(INT_MIN, INT_MIN, INT_MAX, INT_MAX);
}

void createLaserOutputPatternCrossScaled(std::vector<JMVectorStruct> &points) {
	double position_scale = 0.75;
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MIN * position_scale), 0));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MIN * position_scale), USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0,                               0,                               USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MAX * position_scale), (int)(INT_MAX * position_scale), USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MAX * position_scale), (int)(INT_MAX * position_scale), 0));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MAX * position_scale), (int)(INT_MIN * position_scale), 0));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MAX * position_scale), (int)(INT_MIN * position_scale), USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0,                               0,                               USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MAX * position_scale), USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MAX * position_scale), 0));
}


void createLaserOutputPattern(std::vector<JMVectorStruct> &points) {
//	createLaserOutputPatternSquareFullRange(points);
//	createLaserOutputPatternSquareScaled(points, 0.75);
//	createLaserOutputPatternSquareScaled(points, 0.6);
//	createLaserOutputPatternSquareScaled(points, 0.5);
//	createLaserOutputPatternSquareScaled(points, 0.4);
//	createLaserOutputPatternSquareScaled(points, 0.25);
	createLaserOutputPatternPlusFullRange(points);
//	createLaserOutputPatternCrossFullRange(points);
//	createLaserOutputPatternCrossScaled(points);
}

void createLaserOutputPatternHorizontalDiamondOutsideDrawingAreaUsingVectorImageBuilder(laser_projector_bridge::VectorImageBuilder &vector_image_builder) {
	double half_width = vector_image_builder.getDrawingAreaWidth() * 0.5;
	double half_hHeight = vector_image_builder.getDrawingAreaHeight() * 0.5;
	vector_image_builder.addNewLine(vector_image_builder.getDrawingAreaWidth() * -0.25, half_hHeight, half_width, vector_image_builder.getDrawingAreaHeight() * 0.75);
	vector_image_builder.addNewLine(half_width, vector_image_builder.getDrawingAreaHeight() * 0.75, vector_image_builder.getDrawingAreaWidth() * 1.25, half_hHeight);
	vector_image_builder.addNewLine(vector_image_builder.getDrawingAreaWidth() * 1.25, half_hHeight, half_width, vector_image_builder.getDrawingAreaHeight() * 0.25);
	vector_image_builder.addNewLine(half_width, vector_image_builder.getDrawingAreaHeight() * 0.25, vector_image_builder.getDrawingAreaWidth() * -0.25, half_hHeight);
}

void createLaserOutputPatternVerticalDiamondOutsideDrawingAreaUsingVectorImageBuilder(laser_projector_bridge::VectorImageBuilder &vector_image_builder) {
	double half_width = vector_image_builder.getDrawingAreaWidth() * 0.5;
	double half_height = vector_image_builder.getDrawingAreaHeight() * 0.5;
	vector_image_builder.addNewLine(half_height, vector_image_builder.getDrawingAreaWidth() * -0.25, vector_image_builder.getDrawingAreaHeight() * 0.75, half_width);
	vector_image_builder.addNewLine(vector_image_builder.getDrawingAreaHeight() * 0.75, half_width, half_height, vector_image_builder.getDrawingAreaWidth() * 1.25);
	vector_image_builder.addNewLine(half_height, vector_image_builder.getDrawingAreaWidth() * 1.25, vector_image_builder.getDrawingAreaHeight() * 0.25, half_width);
	vector_image_builder.addNewLine(vector_image_builder.getDrawingAreaHeight() * 0.25, half_width, half_height, vector_image_builder.getDrawingAreaWidth() * -0.25);
}

void createLaserOutputPatternCrossOutsideDrawingAreaUsingVectorImageBuilder(laser_projector_bridge::VectorImageBuilder &vector_image_builder) {
	vector_image_builder.addNewLine(vector_image_builder.getDrawingAreaWidth() * -0.25, vector_image_builder.getDrawingAreaHeight() * -0.25, vector_image_builder.getDrawingAreaWidth() * 0.25, vector_image_builder.getDrawingAreaHeight() * 0.25);
	vector_image_builder.addNewLine(vector_image_builder.getDrawingAreaWidth() * -0.25, vector_image_builder.getDrawingAreaHeight() * -0.25, vector_image_builder.getDrawingAreaWidth() * 0.15, vector_image_builder.getDrawingAreaHeight() * 0.25);
	vector_image_builder.addNewLine(vector_image_builder.getDrawingAreaWidth() * -0.25, vector_image_builder.getDrawingAreaHeight() * -0.25, vector_image_builder.getDrawingAreaWidth() * 0.35, vector_image_builder.getDrawingAreaHeight() * 0.25);

	vector_image_builder.addNewLine(vector_image_builder.getDrawingAreaWidth() * -0.25, vector_image_builder.getDrawingAreaHeight() * 1.25, vector_image_builder.getDrawingAreaWidth() * 0.25, vector_image_builder.getDrawingAreaHeight() * 0.75);
	vector_image_builder.addNewLine(vector_image_builder.getDrawingAreaWidth() * -0.25, vector_image_builder.getDrawingAreaHeight() * 1.25, vector_image_builder.getDrawingAreaWidth() * 0.15, vector_image_builder.getDrawingAreaHeight() * 0.75);
	vector_image_builder.addNewLine(vector_image_builder.getDrawingAreaWidth() * -0.25, vector_image_builder.getDrawingAreaHeight() * 1.25, vector_image_builder.getDrawingAreaWidth() * 0.35, vector_image_builder.getDrawingAreaHeight() * 0.75);

	vector_image_builder.addNewLine(vector_image_builder.getDrawingAreaWidth() * 1.25, vector_image_builder.getDrawingAreaHeight() * -0.25, vector_image_builder.getDrawingAreaWidth() * 0.75, vector_image_builder.getDrawingAreaHeight() * 0.25);
	vector_image_builder.addNewLine(vector_image_builder.getDrawingAreaWidth() * 1.25, vector_image_builder.getDrawingAreaHeight() * -0.25, vector_image_builder.getDrawingAreaWidth() * 0.65, vector_image_builder.getDrawingAreaHeight() * 0.25);
	vector_image_builder.addNewLine(vector_image_builder.getDrawingAreaWidth() * 1.25, vector_image_builder.getDrawingAreaHeight() * -0.25, vector_image_builder.getDrawingAreaWidth() * 0.85, vector_image_builder.getDrawingAreaHeight() * 0.25);

	vector_image_builder.addNewLine(vector_image_builder.getDrawingAreaWidth() * 1.25, vector_image_builder.getDrawingAreaHeight() * 1.25, vector_image_builder.getDrawingAreaWidth() * 0.75, vector_image_builder.getDrawingAreaHeight() * 0.75);
	vector_image_builder.addNewLine(vector_image_builder.getDrawingAreaWidth() * 1.25, vector_image_builder.getDrawingAreaHeight() * 1.25, vector_image_builder.getDrawingAreaWidth() * 0.65, vector_image_builder.getDrawingAreaHeight() * 0.75);
	vector_image_builder.addNewLine(vector_image_builder.getDrawingAreaWidth() * 1.25, vector_image_builder.getDrawingAreaHeight() * 1.25, vector_image_builder.getDrawingAreaWidth() * 0.85, vector_image_builder.getDrawingAreaHeight() * 0.75);
}

void createLaserOutputPatternPlusOutsideDrawingAreaUsingVectorImageBuilder(laser_projector_bridge::VectorImageBuilder &vector_image_builder) {
	double half_width = vector_image_builder.getDrawingAreaWidth() * 0.5;
	double half_height = vector_image_builder.getDrawingAreaHeight() * 0.5;
	vector_image_builder.addNewLine(half_width * 0.5, vector_image_builder.getDrawingAreaHeight() * 0.25, half_width * 0.5, vector_image_builder.getDrawingAreaHeight() * 1.25);
	vector_image_builder.addNewLine(vector_image_builder.getDrawingAreaWidth() * 0.25, half_height * 0.5, vector_image_builder.getDrawingAreaWidth() * 1.25, half_height * 0.5);
	vector_image_builder.addNewLine(half_width * 1.5, vector_image_builder.getDrawingAreaHeight() * -0.25, half_width * 1.5, vector_image_builder.getDrawingAreaHeight() * 0.75);
	vector_image_builder.addNewLine(vector_image_builder.getDrawingAreaWidth() * -0.25, half_height * 1.5, vector_image_builder.getDrawingAreaWidth() * 0.75, half_height * 1.5);
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

	for (double scale = 0.0; scale <= 1.0; scale += 0.1)
		createLaserOutputPatternSquareScaledUsingVectorImageBuilder(vector_image_builder, scale);

	createLaserOutputPatternPlusFullRangeUsingVectorImageBuilder(vector_image_builder);
	createLaserOutputPatternCrossFullRangeUsingVectorImageBuilder(vector_image_builder);

	createLaserOutputPatternHorizontalDiamondOutsideDrawingAreaUsingVectorImageBuilder(vector_image_builder);
	createLaserOutputPatternVerticalDiamondOutsideDrawingAreaUsingVectorImageBuilder(vector_image_builder);
	createLaserOutputPatternCrossOutsideDrawingAreaUsingVectorImageBuilder(vector_image_builder);
	createLaserOutputPatternPlusOutsideDrawingAreaUsingVectorImageBuilder(vector_image_builder);

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
