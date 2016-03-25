/**\file vector_image_publisher.cpp
 * \brief Description...
 *
 * @version 1.0
 * @author Carlos Miguel Correia da Costa
 */

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <includes>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#include <laser_projector_bridge/vector_image_publisher.h>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </includes>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

namespace laser_projector_bridge {


// =============================================================================  <public-section>  ============================================================================
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <constructors-destructor>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
VectorImagePublisher::VectorImagePublisher() :
		zmq_context_(1),
		zmq_publisher_(zmq_context_, ZMQ_PUB),
		zmq_initialized_(false)
		{}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </constructors-destructor>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <VectorImagePublisher-functions>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
bool VectorImagePublisher::setupZMQ(int _port, std::string _host, std::string _protocol) {
	if (_port > 0 && !_host.empty() && (_protocol == "tcp" || _protocol == "udp")) {
		std::stringstream ss_connection;
		ss_connection << _protocol << "://" << _host << ":" << _port;

		try {
			zmq_publisher_.bind(ss_connection.str().c_str());
		} catch (...) { return false; }

		zmq_initialized_ = true;
		return true;
	}

	return false;
}

bool VectorImagePublisher::sendMessage(const std::string& msg) {
	zmq::message_t zmq_msg(msg.size());
	std::memcpy(zmq_msg.data(), msg.data(), msg.size());
	std::cout << "Sending msg [" << msg << "] with size " << zmq_msg.size() << std::endl;
	return sendMessage(zmq_msg);
}

bool VectorImagePublisher::sendMessage(zmq::message_t& msg) {
	if (zmq_initialized_) {
		return zmq_publisher_.send(msg);
	} else {
		return false;
	}
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </VectorImagePublisher-functions>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// =============================================================================  </public-section>  ===========================================================================

} /* namespace laser_projector_bridge */
