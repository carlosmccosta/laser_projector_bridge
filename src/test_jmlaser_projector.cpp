#include <laser_projector_bridge/jmlaser_projector.h>
#include <laser_projector_bridge/vector_image_builder.h>
#include <climits>


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

		std::cout << ">>> |setupProjectorUsingIndex(" << i << ")" << std::endl;
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


void create_laser_output_pattern_square_scaled(std::vector<JMVectorStruct> &points, double position_scale = 0.9) {
	unsigned short intensity = (unsigned short)(USHRT_MAX * 0.9);
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MIN * position_scale), 0));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MIN * position_scale), intensity));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MAX * position_scale), (int)(INT_MIN * position_scale), intensity));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MAX * position_scale), (int)(INT_MAX * position_scale), intensity));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MAX * position_scale), intensity));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MIN * position_scale), intensity));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MIN * position_scale), 0));
}

void CreateLaserOutputPatternSquareScaledUsingVectorImageBuilder(laser_projector_bridge::VectorImageBuilder& vectorImageBuilder, double scale = 1.0) {
	double xOffset = (vectorImageBuilder.getDrawingAreaWidth() - (vectorImageBuilder.getDrawingAreaWidth() * scale)) * 0.5;
	double yOffset = (vectorImageBuilder.getDrawingAreaHeight() - (vectorImageBuilder.getDrawingAreaHeight() * scale)) * 0.5;
	double xMin = xOffset;
	double xMax = vectorImageBuilder.getDrawingAreaWidth() - xOffset;
	double yMin = yOffset;
	double yMax = vectorImageBuilder.getDrawingAreaHeight() - yOffset;
	vectorImageBuilder.AddNewLine(xMin, yMin, xMax, yMin);
	vectorImageBuilder.AddNewLine(xMax, yMin, xMax, yMax);
	vectorImageBuilder.AddNewLine(xMax, yMax, xMin, yMax);
	vectorImageBuilder.AddNewLine(xMin, yMax, xMin, yMin);
}

void create_laser_output_pattern_plus_full_range(std::vector<JMVectorStruct> &points) {
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0,       INT_MIN, 0));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0,       INT_MIN, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0,       INT_MAX, USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, 0,       0));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, 0,       USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, 0,       USHRT_MAX));
	points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, 0,       0));
}

void CreateLaserOutputPatternPlusFullRangeUsingVectorImageBuilder(laser_projector_bridge::VectorImageBuilder& vectorImageBuilder) {
	double halfWidth = vectorImageBuilder.getDrawingAreaWidth() * 0.5;
	double halfHeight = vectorImageBuilder.getDrawingAreaHeight() * 0.5;
	vectorImageBuilder.AddNewLine(0.0, halfHeight, vectorImageBuilder.getDrawingAreaWidth(), halfHeight);
	vectorImageBuilder.AddNewLine(halfWidth, 0.0, halfWidth, vectorImageBuilder.getDrawingAreaHeight());
}


void create_laser_output_pattern_cross_full_range(std::vector<JMVectorStruct> &points) {
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

void CreateLaserOutputPatternCrossFullRangeUsingVectorImageBuilder(laser_projector_bridge::VectorImageBuilder& vectorImageBuilder)
{
	vectorImageBuilder.AddNewLine(INT_MIN, INT_MAX, INT_MAX, INT_MIN);
	vectorImageBuilder.AddNewLine(INT_MIN, INT_MIN, INT_MAX, INT_MAX);
}

void create_laser_output_pattern_cross_scaled(std::vector<JMVectorStruct> &points) {
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


void create_laser_output_pattern(std::vector<JMVectorStruct>& points) {
	create_laser_output_pattern_square_full_range(points);
	create_laser_output_pattern_square_scaled(points, 0.75);
	create_laser_output_pattern_square_scaled(points, 0.6);
	create_laser_output_pattern_square_scaled(points, 0.5);
	create_laser_output_pattern_square_scaled(points, 0.4);
	create_laser_output_pattern_square_scaled(points, 0.25);
	create_laser_output_pattern_plus_full_range(points);
	create_laser_output_pattern_cross_full_range(points);
	create_laser_output_pattern_cross_scaled(points);
}

void CreateLaserOutputPatternHorizontalDiamondOutsideDrawingAreaUsingVectorImageBuilder(laser_projector_bridge::VectorImageBuilder& vectorImageBuilder) {
	double halfWidth = vectorImageBuilder.getDrawingAreaWidth() * 0.5;
	double halfHeight = vectorImageBuilder.getDrawingAreaHeight() * 0.5;
	vectorImageBuilder.AddNewLine(vectorImageBuilder.getDrawingAreaWidth() * -0.25, halfHeight, halfWidth, vectorImageBuilder.getDrawingAreaHeight() * 0.75);
	vectorImageBuilder.AddNewLine(halfWidth, vectorImageBuilder.getDrawingAreaHeight() * 0.75, vectorImageBuilder.getDrawingAreaWidth() * 1.25, halfHeight);
	vectorImageBuilder.AddNewLine(vectorImageBuilder.getDrawingAreaWidth() * 1.25, halfHeight, halfWidth, vectorImageBuilder.getDrawingAreaHeight() * 0.25);
	vectorImageBuilder.AddNewLine(halfWidth, vectorImageBuilder.getDrawingAreaHeight() * 0.25, vectorImageBuilder.getDrawingAreaWidth() * -0.25, halfHeight);
}

void CreateLaserOutputPatternVerticalDiamondOutsideDrawingAreaUsingVectorImageBuilder(laser_projector_bridge::VectorImageBuilder& vectorImageBuilder) {
	double halfWidth = vectorImageBuilder.getDrawingAreaWidth() * 0.5;
	double halfHeight = vectorImageBuilder.getDrawingAreaHeight() * 0.5;
	vectorImageBuilder.AddNewLine(halfHeight, vectorImageBuilder.getDrawingAreaWidth() * -0.25, vectorImageBuilder.getDrawingAreaHeight() * 0.75, halfWidth);
	vectorImageBuilder.AddNewLine(vectorImageBuilder.getDrawingAreaHeight() * 0.75, halfWidth, halfHeight, vectorImageBuilder.getDrawingAreaWidth() * 1.25);
	vectorImageBuilder.AddNewLine(halfHeight, vectorImageBuilder.getDrawingAreaWidth() * 1.25, vectorImageBuilder.getDrawingAreaHeight() * 0.25, halfWidth);
	vectorImageBuilder.AddNewLine(vectorImageBuilder.getDrawingAreaHeight() * 0.25, halfWidth, halfHeight, vectorImageBuilder.getDrawingAreaWidth() * -0.25);
}

void CreateLaserOutputPatternCrossOutsideDrawingAreaUsingVectorImageBuilder(laser_projector_bridge::VectorImageBuilder& vectorImageBuilder) {
	double halfWidth = vectorImageBuilder.getDrawingAreaWidth() * 0.5;
	double halfHeight = vectorImageBuilder.getDrawingAreaHeight() * 0.5;
	vectorImageBuilder.AddNewLine(vectorImageBuilder.getDrawingAreaWidth() * -0.25, vectorImageBuilder.getDrawingAreaHeight() * -0.25, vectorImageBuilder.getDrawingAreaWidth() * 0.25, vectorImageBuilder.getDrawingAreaHeight() * 0.25);
	vectorImageBuilder.AddNewLine(vectorImageBuilder.getDrawingAreaWidth() * -0.25, vectorImageBuilder.getDrawingAreaHeight() * -0.25, vectorImageBuilder.getDrawingAreaWidth() * 0.15, vectorImageBuilder.getDrawingAreaHeight() * 0.25);
	vectorImageBuilder.AddNewLine(vectorImageBuilder.getDrawingAreaWidth() * -0.25, vectorImageBuilder.getDrawingAreaHeight() * -0.25, vectorImageBuilder.getDrawingAreaWidth() * 0.35, vectorImageBuilder.getDrawingAreaHeight() * 0.25);

	vectorImageBuilder.AddNewLine(vectorImageBuilder.getDrawingAreaWidth() * -0.25, vectorImageBuilder.getDrawingAreaHeight() * 1.25, vectorImageBuilder.getDrawingAreaWidth() * 0.25, vectorImageBuilder.getDrawingAreaHeight() * 0.75);
	vectorImageBuilder.AddNewLine(vectorImageBuilder.getDrawingAreaWidth() * -0.25, vectorImageBuilder.getDrawingAreaHeight() * 1.25, vectorImageBuilder.getDrawingAreaWidth() * 0.15, vectorImageBuilder.getDrawingAreaHeight() * 0.75);
	vectorImageBuilder.AddNewLine(vectorImageBuilder.getDrawingAreaWidth() * -0.25, vectorImageBuilder.getDrawingAreaHeight() * 1.25, vectorImageBuilder.getDrawingAreaWidth() * 0.35, vectorImageBuilder.getDrawingAreaHeight() * 0.75);

	vectorImageBuilder.AddNewLine(vectorImageBuilder.getDrawingAreaWidth() * 1.25, vectorImageBuilder.getDrawingAreaHeight() * -0.25, vectorImageBuilder.getDrawingAreaWidth() * 0.75, vectorImageBuilder.getDrawingAreaHeight() * 0.25);
	vectorImageBuilder.AddNewLine(vectorImageBuilder.getDrawingAreaWidth() * 1.25, vectorImageBuilder.getDrawingAreaHeight() * -0.25, vectorImageBuilder.getDrawingAreaWidth() * 0.65, vectorImageBuilder.getDrawingAreaHeight() * 0.25);
	vectorImageBuilder.AddNewLine(vectorImageBuilder.getDrawingAreaWidth() * 1.25, vectorImageBuilder.getDrawingAreaHeight() * -0.25, vectorImageBuilder.getDrawingAreaWidth() * 0.85, vectorImageBuilder.getDrawingAreaHeight() * 0.25);

	vectorImageBuilder.AddNewLine(vectorImageBuilder.getDrawingAreaWidth() * 1.25, vectorImageBuilder.getDrawingAreaHeight() * 1.25, vectorImageBuilder.getDrawingAreaWidth() * 0.75, vectorImageBuilder.getDrawingAreaHeight() * 0.75);
	vectorImageBuilder.AddNewLine(vectorImageBuilder.getDrawingAreaWidth() * 1.25, vectorImageBuilder.getDrawingAreaHeight() * 1.25, vectorImageBuilder.getDrawingAreaWidth() * 0.65, vectorImageBuilder.getDrawingAreaHeight() * 0.75);
vectorImageBuilder.AddNewLine(vectorImageBuilder.getDrawingAreaWidth() * 1.25, vectorImageBuilder.getDrawingAreaHeight() * 1.25, vectorImageBuilder.getDrawingAreaWidth() * 0.85, vectorImageBuilder.getDrawingAreaHeight() * 0.75);
}

void CreateLaserOutputPatternPlusOutsideDrawingAreaUsingVectorImageBuilder(laser_projector_bridge::VectorImageBuilder& vectorImageBuilder) {
	double halfWidth = vectorImageBuilder.getDrawingAreaWidth() * 0.5;
	double halfHeight = vectorImageBuilder.getDrawingAreaHeight() * 0.5;
	vectorImageBuilder.AddNewLine(halfWidth * 0.5, vectorImageBuilder.getDrawingAreaHeight() * 0.25, halfWidth * 0.5, vectorImageBuilder.getDrawingAreaHeight() * 1.25);
	vectorImageBuilder.AddNewLine(vectorImageBuilder.getDrawingAreaWidth() * 0.25, halfHeight * 0.5, vectorImageBuilder.getDrawingAreaWidth() * 1.25, halfHeight * 0.5);
	vectorImageBuilder.AddNewLine(halfWidth * 1.5, vectorImageBuilder.getDrawingAreaHeight() * -0.25, halfWidth * 1.5, vectorImageBuilder.getDrawingAreaHeight() * 0.75);
	vectorImageBuilder.AddNewLine(vectorImageBuilder.getDrawingAreaWidth() * -0.25, halfHeight * 1.5, vectorImageBuilder.getDrawingAreaWidth() * 0.75, halfHeight * 1.5);
}


void CreateLaserOutputPatternUsingVectorImageBuilder(std::vector<JMVectorStruct>& points)
{
	laser_projector_bridge::VectorImageBuilder vectorImageBuilder;
	vectorImageBuilder.setInterpolationDistanceInProjectorRange((int32_t)(UINT32_MAX * 0.05));
	vectorImageBuilder.setLineFirstPointIgnoreDistanceSquaredInProjectorRange(std::pow(UINT32_MAX * 0.0013, 2));
	vectorImageBuilder.setLineFirstPointMergeDistanceSquaredInProjectorRange(std::pow(UINT32_MAX * 0.0005, 2));
	vectorImageBuilder.setRadialDistortionCoefficientSecondDegreeInvertedUV(0.08);
	vectorImageBuilder.setRadialDistortionCoefficientSecondDegree(-0.044);
	vectorImageBuilder.setRadialDistortionCoefficientFourthDegree(-0.007);
	vectorImageBuilder.setRadialDistortionCoefficientSixthDegree(-0.005);
	vectorImageBuilder.StartNewVectorImage();


	for (double scale = 0.0; scale <= 1.0; scale += 0.1)
		CreateLaserOutputPatternSquareScaledUsingVectorImageBuilder(vectorImageBuilder, scale);

	CreateLaserOutputPatternPlusFullRangeUsingVectorImageBuilder(vectorImageBuilder);
	CreateLaserOutputPatternCrossFullRangeUsingVectorImageBuilder(vectorImageBuilder);

	CreateLaserOutputPatternHorizontalDiamondOutsideDrawingAreaUsingVectorImageBuilder(vectorImageBuilder);
	CreateLaserOutputPatternVerticalDiamondOutsideDrawingAreaUsingVectorImageBuilder(vectorImageBuilder);
	CreateLaserOutputPatternCrossOutsideDrawingAreaUsingVectorImageBuilder(vectorImageBuilder);
	CreateLaserOutputPatternPlusOutsideDrawingAreaUsingVectorImageBuilder(vectorImageBuilder);

	vectorImageBuilder.FinishVectorImage();
	points = vectorImageBuilder.getVectorImagePoints();
}

void test_jmlaser_output(int number_of_projectors) {
	std::vector<JMVectorStruct> points;
	//create_laser_output_pattern(points);
	CreateLaserOutputPatternUsingVectorImageBuilder(points);

	for (int i = 0; i < number_of_projectors; ++i) {
		laser_projector_bridge::JMLaserProjector laser_projector;
		std::cout << ">>> |setupProjectorUsingIndex(i)" << std::endl;
		laser_projector.setupProjectorUsingIndex(i);
		std::cout << laser_projector << "\n\n" << std::endl;
		laser_projector.startOutput();
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
