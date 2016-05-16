#pragma once

/**\file ImageTracer.h
 * \brief Gazebo CameraPlugin that creates image outlines
 *
 * @version 1.0
 * @author Carlos Miguel Correia da Costa
 */


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// std includes
#include <iostream>
#include <string>

// external libs includes
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <flatbuffers/flatbuffers.h>

// project includes
#include <laser_projector_bridge/msgs/vector_image_generated.h>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </includes>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


namespace laser_projector_bridge {

// ##############################################################################   ImageTracer   ##############################################################################
/**
 * \brief Class that creates image outlines
 */
class ImageTracer {
	// ========================================================================   <public-section>   ===========================================================================
	public:
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <constructors-destructor>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		ImageTracer();
		virtual ~ImageTracer () {}
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </constructors-destructor>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <functions>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		virtual void processImage(const unsigned char *_image, unsigned int _width, unsigned int _height, unsigned int _depth, const std::string &_format/*, msgs::VectorImage& vector_image_out*/);
		void segmentImageHSV(cv::Mat& _image_hsv);
		void detectContours(cv::Mat& _binary_image/*, msgs::VectorImage& vector_image_out*/);
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </functions>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// ========================================================================   </public-section>  ===========================================================================


	// ========================================================================   <protected-section>   ========================================================================
	protected:
		int color_segmentation_lower_hue_;
		int color_segmentation_upper_hue_;
		int color_segmentation_lower_saturation_;
		int color_segmentation_upper_saturation_;
		int color_segmentation_lower_value_;
		int color_segmentation_upper_value_;
		bool show_debug_images_;
	// ========================================================================   </protected-section>  ========================================================================
};


}
