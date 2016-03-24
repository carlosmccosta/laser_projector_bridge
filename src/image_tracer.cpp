/**\file image_tracer.cpp
 * \brief Gazebo CameraPlugin that creates image outlines
 *
 * @version 1.0
 * @author Carlos Miguel Correia da Costa
 */
 

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#include <laser_projector_bridge/image_tracer.h>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </includes>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


namespace laser_projector_bridge {
// =============================================================================  <public-section>  ============================================================================
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <functions>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
ImageTracer::ImageTracer() :
		color_segmentation_lower_hue_(0),
		color_segmentation_upper_hue_(179),
		color_segmentation_lower_saturation_(0),
		color_segmentation_upper_saturation_(255),
		color_segmentation_lower_value_(127),
		color_segmentation_upper_value_(255),
		show_debug_images_(true)
		{}

void ImageTracer::processImage(const unsigned char *_image, unsigned int _width, unsigned int _height, unsigned int _depth, const std::string &_format) {
	if (_format == "R8G8B8" && _depth == 3) {
		cv::Mat source_image(_height, _width, CV_8UC3, (void*)_image);
		cv::Mat image;
		cv::cvtColor(source_image, image, CV_RGB2HSV);
		segmentImageHSV(image);
		detectContours(image);
	}
}


void ImageTracer::segmentImageHSV(cv::Mat& _image_hsv) {
	if (color_segmentation_lower_hue_ < color_segmentation_upper_hue_) {
		cv::inRange(_image_hsv,
			cv::Scalar(color_segmentation_lower_hue_, color_segmentation_lower_saturation_, color_segmentation_lower_value_),
			cv::Scalar(color_segmentation_upper_hue_, color_segmentation_upper_saturation_, color_segmentation_upper_value_),
			_image_hsv);
	} else {
		// when colors wrap around from near 180 to 0+
		cv::Mat lower_range;
		cv::inRange(_image_hsv,
			cv::Scalar(0, color_segmentation_lower_saturation_, color_segmentation_lower_value_),
			cv::Scalar(color_segmentation_upper_hue_, color_segmentation_upper_saturation_, color_segmentation_upper_value_),
			lower_range);

		cv::Mat higher_range;
		cv::inRange(_image_hsv,
			cv::Scalar(color_segmentation_lower_hue_, color_segmentation_lower_saturation_, color_segmentation_lower_value_),
			cv::Scalar(180, color_segmentation_upper_saturation_, color_segmentation_upper_value_),
			higher_range);

		cv::bitwise_or(lower_range, higher_range, _image_hsv);
	}
}

void ImageTracer::detectContours(cv::Mat& _binary_image) {
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::findContours(_binary_image, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_TC89_L1, cv::Point(0,0));
	std::vector<std::vector<cv::Point> > contours_approximated(contours.size());

	if (show_debug_images_) {
		cv::Mat image_contours_lines = cv::Mat::zeros(_binary_image.size(), CV_8UC3);
		cv::Mat image_contours_points = cv::Mat::zeros(_binary_image.size(), CV_8UC3);
		cv::RNG rng;

		for(size_t i = 0; i < contours.size(); ++i) {
			cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
			cv::drawContours(image_contours_lines, contours, (int)i, color, 2, 8, hierarchy, 0, cv::Point());
//			cv::approxPolyDP(contours[i], contours_approximated[i], 1, true);
			for (size_t j = 0; j < contours[i].size(); ++j) {
				image_contours_points.at<cv::Vec3b>(contours[i][j]) = cv::Vec3b(color[0], color[1], color[2]);
			}
		}

		const char* image_window_name_lines = "Image contours lines";
		cv::namedWindow(image_window_name_lines, cv::WINDOW_AUTOSIZE);
		cv::imshow(image_window_name_lines, image_contours_lines);

		const char* image_window_name_points = "Image contours points";
		cv::namedWindow(image_window_name_points, cv::WINDOW_AUTOSIZE);
		cv::imshow(image_window_name_points, image_contours_points);

		cv::waitKey(100);
	}
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </functions>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// =============================================================================  </public-section>  ===========================================================================
}
