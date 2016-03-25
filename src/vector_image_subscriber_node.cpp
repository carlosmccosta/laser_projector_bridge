/**\file vector_image_subscriber_node.cpp
 * \brief Description...
 *
 * @version 1.0
 * @author Carlos Miguel Correia da Costa
 */

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <includes>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#include <iostream>
#include <laser_projector_bridge/vector_image_subscriber.h>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </includes>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// ###################################################################################   <main>   ##############################################################################
int main(int argc, char** argv) {
	laser_projector_bridge::VectorImageSubscriber subscriber;
	if (subscriber.setupServerFromCommandLineConfiguration(argc, argv)) {
		subscriber.startServer();
	} else {
		std::cerr << "Failed to load configuration!\nUsage: " << subscriber.getCommandLineConfigurationLayout() << std::endl;
	}
	return 0;
}
