#pragma once

/**\file jmlaser_projector.h
 * \brief Class for communicating with laser projectors.
 *
 * @version 1.0
 * @author Carlos Miguel Correia da Costa
 */

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// std includes
#include <string>
#include <vector>
#include <iostream>

// external libs includes
#include <laser_projector_bridge/jmlaser.h>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



namespace laser_projector_bridge {

// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||   JMLaserProjector   ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
/**
 * \brief Class for communicating with laser projectors.
 */
class JMLaserProjector {
	// ==========================================================================   <public-section>   ============================================================================
	public:
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <constructors-destructor>    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		JMLaserProjector();
		virtual ~JMLaserProjector();
		void resetProjector();
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </constructors-destructor>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <static functions>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		static int jmLaserBridgeOpenDll();
		static int jmLaserBridgeCloseDll();

		/**
		 * Should be called only once when all the projectors are connected.
		 * jmLaserEnumerateDevices is not able to update the list when a new projector is connected.
		 * After the first call, jmLaserEnumerateDevices will return 0 even if there are connected projectors.
		 * As such this function stores the number of devices as a static variable on the first call.
		 * After the first call this function will return the static variable instead of calling jmLaserEnumerateDevices
		 */
		static int jmLaserBridgeEnumerateDevices();
		static std::string jmLaserBridgeGetDeviceListEntry(unsigned int list_index);
		static std::string jmLaserBridgeGetDeviceName(int projector_handle);
		static std::string jmLaserBridgeGetDeviceFamilyName(const std::string& projector_name);
		static std::string jmLaserBridgeGetFriendlyName(const std::string& projector_name);
		static bool jmLaserBridgeSetFriendlyName(int projector_handle, const std::string& projector_friendly_name);
		static int jmLaserBridgeOpenDevice(const std::string& projector_name);
		static int jmLaserBridgeGetMaxFrameSize(int projector_handle);
		static int jmLaserBridgeGetDeviceListLength();
		static bool jmLaserBridgeGetIsNetworkDevice(const std::string& projector_name);
		static std::string jmLaserBridgeGetNetworkAddress(const std::string& projector_name);
		static int jmLaserBridgeGetMinSpeed(int projector_handle);
		static int jmLaserBridgeGetMaxSpeed(int projector_handle);
		static int jmLaserBridgeGetSpeedStep(int projector_handle);
		static JMVectorStruct createSingleColorLaserPoint(int x, int y, unsigned short intensity);
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <static functions/>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <functions>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		bool setupProjector();
		bool setupProjectorUsingName(const std::string& projector_name);
		bool setupProjectorUsingFriendlyName(const std::string& projector_friendly_name);
		bool setupProjectorUsingIndex(unsigned int projector_index);
		bool closeProjector();
		bool setProjectorFriendlyName(const std::string& projector_friendly_name);
		bool startOutput();
		bool sendVectorImageToProjector(std::vector<JMVectorStruct>& points, unsigned int speed, unsigned int repetitions, bool add_reverse_image = true);
		bool stopOutput();
		friend std::ostream& operator<<(std::ostream& os, const JMLaserProjector& dt);
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </functions>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <gets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		inline static int getNumberOfProjectors() { return s_number_of_projectors_; }
		inline unsigned int getProjectorListEntryIndex() const { return projector_list_entry_index_; }
		inline int getProjectorHandle() const { return projector_handle_; }
		inline int getProjectorMaximumNumberOfVectorsPerFrame() const { return projector_maximum_number_of_vectors_per_frame_; }
		inline int getProjectorMinimumSpeed() const { return projector_minimum_speed_; }
		inline int getProjectorMaximumSpeed() const { return projector_maximum_speed_; }
		inline int getProjectorSpeedStep() const { return projector_speed_step_; }
		inline const std::string& getProjectorNetworkAddress() const { return projector_network_address_; }
		inline const std::string& getProjectorName() const { return projector_name_; }
		inline const std::string& getProjectorNameFromHandle() const { return projector_name_from_handle_; }
		inline const std::string& getProjectorFriendlyName() const { return projector_friendly_name_; }
		inline const std::string& getProjectorFamilyName() const { return projector_family_name_; }
		inline bool isProjectorOutputStarted() const { return projector_output_started_; }
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </gets>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <sets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		inline void setProjector_minimum_speed_(int projector_minimum_speed) { if (projector_minimum_speed > projector_minimum_speed_) projector_minimum_speed_ = projector_minimum_speed; }
		inline void setProjector_maximum_speed_(int projector_maximum_speed) { if (projector_maximum_speed < projector_maximum_speed_) projector_maximum_speed_ = projector_maximum_speed; }
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </sets>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// ============================================================================   </public-section>   =========================================================================


	// ============================================================================   <protected-section>   =======================================================================
	protected:
		static int s_number_of_projectors_;
		unsigned int projector_list_entry_index_;
		int projector_handle_;
		int projector_maximum_number_of_vectors_per_frame_;
		int projector_minimum_speed_;
		int projector_maximum_speed_;
		int projector_speed_step_;
		std::string projector_network_address_;
		std::string projector_name_;
		std::string projector_name_from_handle_;
		std::string projector_friendly_name_;
		std::string projector_family_name_;
		bool projector_output_started_;
	// ============================================================================   <protected-section>   =======================================================================
};

}
