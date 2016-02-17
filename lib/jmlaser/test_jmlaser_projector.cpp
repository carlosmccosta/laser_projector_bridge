#include <iostream>
#include <vector>
#include <climits>
#include <laser_projector_bridge/JMLaserProjector.h>

using namespace std;
using namespace laser_projector_bridge;


void test_jmlaser_projector_without_enumeration() {
	std::string projector_name = JMLaserProjector::jmLaserBridgeGetDeviceListEntry(0);
	projector_name = JMLaserProjector::jmLaserBridgeGetDeviceListEntry(0);
	cout << "Projector name: " << projector_name << endl;
}


void test_jmlaser_projector(int number_of_projectors) {
	for (int i = 0; i < number_of_projectors; ++i) {
		JMLaserProjector laser_projector;

		cout << ">>> |setupProjector()" << endl;
		laser_projector.setupProjector();
		cout << laser_projector << "\n\n" << endl;

		std::string friendly_name = laser_projector.getProjectorFriendlyName();
		laser_projector.setProjectorFriendlyName(friendly_name + " TestFriendlyName");

		cout << ">>> |setupProjectorUsingIndex(i)" << endl;
		laser_projector.setupProjectorUsingIndex(i);
		cout << laser_projector << "\n\n" << endl;

		laser_projector.setProjectorFriendlyName(friendly_name);

		cout << ">>> |setupProjectorUsingName(std::string(\"Netlase 1552 #0\"))" << endl;
		laser_projector.setupProjectorUsingName(std::string("Netlase 1552 #0"));
		cout << laser_projector << "\n\n" << endl;

		cout << ">>> |setupProjectorUsingFriendlyName(std::string(\"ILP 622 LAN\"))" << endl;
		laser_projector.setupProjectorUsingFriendlyName(std::string("ILP 622 LAN"));
		cout << laser_projector << "\n\n" << endl;
	}
}


void create_laser_output_pattern(std::vector<JMVectorStruct>& points) {
//	points.push_back(JMVectorStruct(0, 0, USHRT_MAX));
//	points.push_back(JMVectorStruct(0.1*INT_MAX, 0, USHRT_MAX));
//	points.push_back(JMVectorStruct(0, 0, USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX));
//	points.push_back(JMVectorStruct(0.1*INT_MAX, 0, USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX));

//	points.push_back(JMVectorStruct(0, 0, UCHAR_MAX));
//	points.push_back(JMVectorStruct(0.1*INT_MAX, 0, UCHAR_MAX));
//	points.push_back(JMVectorStruct(0, 0, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX));
//	points.push_back(JMVectorStruct(0.1*INT_MAX, 0, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX));

//	points.push_back(JMVectorStruct(0, 0, 100, 100, 100, 100, 100, 100, 100, 100));
//	points.push_back(JMVectorStruct(0.1*INT_MAX, 0, 100, 100, 100, 100, 100, 100, 100, 100));

	points.push_back(JMVectorStruct(100, 100, 100, 100, 100, 100, 100, 100, 100, 100));
	points.push_back(JMVectorStruct(500, 500, 0, 100, 100, 100, 100, 100, 100, 100));
}


void test_jmlaser_output(int number_of_projectors) {
	std::vector<JMVectorStruct> points;
	create_laser_output_pattern(points);

	for (int i = 0; i < number_of_projectors; ++i) {
		JMLaserProjector laser_projector;

		cout << ">>> |setupProjectorUsingIndex(i)" << endl;
		laser_projector.setupProjectorUsingIndex(i);
		cout << laser_projector << "\n\n" << endl;

		cout << ">>> Sending pattern to projector" << endl;
		laser_projector.startOutput();
		if (laser_projector.sendVectorImageToProjector(points, 60000, 1000)) {
			cout << ">>> Pattern was sent successfully" << endl;
		}
		laser_projector.stopOutput();
	}
}



int main(int argc, char **argv) {
	cout << "===============================================================================" << endl;
	cout << ">>>> Testing laser_projector_bridge::JMLaserProjector" << endl;
	cout << "===============================================================================\n" << endl;

//	test_jmlaser_projector_without_enumeration();

	int number_of_projectors = JMLaserProjector::jmLaserBridgeEnumerateDevices();
	if (number_of_projectors <= 0) {
		cout << ">>> No projectors were found!" << endl;
		return -1;
	} else {
//		test_jmlaser_projector(number_of_projectors);
		test_jmlaser_output(number_of_projectors);
	}

	return 0;
}
