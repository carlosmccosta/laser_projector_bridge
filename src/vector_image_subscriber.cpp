/**\file vector_image_subscriber.cpp
 * \brief Description...
 *
 * @version 1.0
 * @author Carlos Miguel Correia da Costa
 */

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <includes>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#include <laser_projector_bridge/vector_image_subscriber.h>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </includes>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

namespace laser_projector_bridge {

// =============================================================================  <public-section>  ============================================================================
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <constructors-destructor>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
VectorImageSubscriber::VectorImageSubscriber() :
		zmq_context_(1),
		zmq_subscriber_(zmq_context_, ZMQ_SUB),
		zmq_initialized_(false),
		laser_projector_initialized_(false),
		keep_processing_new_msgs_(true)
		{}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </constructors-destructor>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <VectorImageServer-functions>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
std::string VectorImageSubscriber::getCommandLineConfigurationLayout() {
	return std::string("<laser_projector_friendly_name> [zmq_server_port] [zmq_host] [zmq_protocol]");
}

bool VectorImageSubscriber::setupServerFromCommandLineConfiguration(int _argc, char** _argv) {
	if (_argc < 2) { return false; }

	if (setupLaserProjector(std::string(_argv[1]))) {
		int port = 1337;
		std::string host("localhost");
		std::string protocol("tcp");

		if (_argc > 2) { if (!(std::stringstream(std::string(_argv[2])) >> port)) { return false; } }
		if (_argc > 3) { host = std::string(_argv[3]); }
		if (_argc > 4) { protocol = std::string(_argv[4]); }
		return setupZMQ(port, host, protocol);
	}

	return false;
}

bool VectorImageSubscriber::setupZMQ(int _port, std::string _host, std::string _protocol) {
	if (_port > 0 && !_host.empty() && (_protocol == "tcp" || _protocol == "udp")) {
		std::stringstream ss_connection;
		ss_connection << _protocol << "://" << _host << ":" << _port;

		try {
			zmq_subscriber_.connect(ss_connection.str().c_str());
			zmq_subscriber_.setsockopt(ZMQ_SUBSCRIBE, "", 0);
		} catch (...) { return false; }

		zmq_initialized_ = true;
		return true;
	}

	return false;
}

bool VectorImageSubscriber::setupLaserProjector(std::string _projector_friendly_name) {
	if (!_projector_friendly_name.empty()) {
		laser_projector_initialized_ = true;
	}

	return false;
}

void VectorImageSubscriber::startServer() {
	if (zmq_initialized_ && laser_projector_initialized_) {
		while (keep_processing_new_msgs_) {
			zmq::message_t msg;
			if (zmq_subscriber_.recv(&msg)) {
				processMessage(msg);
			}
		}
	}
}

bool VectorImageSubscriber::processMessage(zmq::message_t& msg) {
	std::cout << "Received msg with size " << msg.size() << std::endl;
	return false;
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </VectorImageServer-functions>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// =============================================================================  </public-section>  ===========================================================================


} /* namespace laser_projector_bridge */
