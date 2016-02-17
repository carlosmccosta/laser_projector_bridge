#include <iostream>
#include <laser_projector_bridge/JMLaserProjector.h>

using namespace std;
using namespace laser_projector_bridge;


void test_jmlaser_projector_without_enumeration() {
	std::string projector_name = JMLaserProjector::jmLaserBridgeGetDeviceListEntry(0);
	projector_name = JMLaserProjector::jmLaserBridgeGetDeviceListEntry(0);
	cout << "Projector name: " << projector_name << endl;
}


void test_jmlaser_projector_static_functions(int number_of_projectors) {
	for (int i = 0; i < number_of_projectors; ++i) {

	}
}


void test_jmlaser_projector(int number_of_projectors) {
	for (int i = 0; i < number_of_projectors; ++i) {
		JMLaserProjector laser_projector;

		cout << "setupProjector()" << endl;
		laser_projector.setupProjector();
		cout << laser_projector << "\n\n" << endl;

		std::string friendly_name = laser_projector.getProjectorFriendlyName();
		laser_projector.setProjectorFriendlyName(friendly_name + " TestFriendlyName");

		cout << "setupProjectorUsingIndex(i)" << endl;
		laser_projector.setupProjectorUsingIndex(i);
		cout << laser_projector << "\n\n" << endl;

		laser_projector.setProjectorFriendlyName(friendly_name);

		cout << "setupProjectorUsingName(std::string(\"Netlase 1552 #0\"))" << endl;
		laser_projector.setupProjectorUsingName(std::string("Netlase 1552 #0"));
		cout << laser_projector << "\n\n" << endl;

		cout << "setupProjectorUsingFriendlyName(std::string(\"ILP 622 LAN\"))" << endl;
		laser_projector.setupProjectorUsingFriendlyName(std::string("ILP 622 LAN"));
		cout << laser_projector << "\n\n" << endl;
	}
}



int main(int argc, char **argv) {
	cout << "===============================================================================" << endl;
	cout << ">>>> Testing laser_projector_bridge::JMLaserProjector" << endl;
	cout << "===============================================================================\n" << endl;

//	test_jmlaser_projector_without_enumeration();

	int number_of_projectors = JMLaserProjector::jmLaserBridgeEnumerateDevices();
	if (number_of_projectors <= 0) {
		cout << "No projectors were found!" << endl;
		return -1;
	} else {
//		test_jmlaser_projector_static_functions(number_of_projectors);
		test_jmlaser_projector(number_of_projectors);
	}

	return 0;
}
