#pragma once

/**\file vector_image_builder.h
 * \brief Class for building vector images with linear interpolation and radial distortion correction.
 *
 * @version 1.0
 * @author Carlos Miguel Correia da Costa
 */

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// std includes
#include <cmath>
#include <cstdint>
#include <limits>
#include <vector>


// external libs includes
#include <laser_projector_bridge/jmlaser.h>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



namespace laser_projector_bridge {

// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||   VectorImageBuilder   |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
/**
 * \brief Class for building vector images with linear interpolation and radial distortion correction.
 */
class VectorImageBuilder {
	// ==========================================================================   <public-section>   ============================================================================
	public:
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <constructors-destructor>    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		VectorImageBuilder();
		virtual ~VectorImageBuilder();
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </constructors-destructor>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <enums>    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		enum AxisPosition {
			TopLeft,        // x -> right | y -> down
			BottomLeft,     // x -> right | y -> up
			Middle          // x -> right | y -> up
		};
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </enums>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <functions>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		void startNewVectorImage();
		void finishVectorImage();
		bool convertProjectorOriginToDrawingAreaOrigin(double x, double y, double &new_x, double &new_y, AxisPosition drawing_area_origin_axis_position = BottomLeft);
		bool convertPointFromDrawingAreaToProjectorOrigin(double x, double y, double &x_point_in_drawing_area_and_projector_origin, double &y_point_in_drawing_area_and_projector_origin, AxisPosition origin_axis_position = BottomLeft);
		bool convertPointFromDrawingAreaInProjectorOriginToProjectorRange(double x_point_in_drawing_area_and_projector_origin, double y_point_in_drawing_area_and_projector_origin, int32_t &x_point_in_projector_range, int32_t &y_point_in_projector_range);
		bool convertPointFromProjectorRangeToDrawingArea(int32_t x_point_in_projector_range, int32_t y_point_in_projector_range, double &x_point_in_drawing_area_range, double &y_point_in_drawing_area_range, AxisPosition origin_axis_position = BottomLeft);
		bool trimLineInDrawingAreaAndProjectorOrigin(double &start_x_in_projector_origin, double &start_y_in_projector_origin, double &end_x_in_projector_origin, double &end_y_in_projector_origin);
		bool isPointInProjectorOriginWithinDrawingArea(double x, double y);
		bool addNewLine(double start_x, double start_y, double end_x, double end_y,
		                uint16_t red = std::numeric_limits<uint16_t>::max(), uint16_t green = std::numeric_limits<uint16_t>::max(), uint16_t blue = std::numeric_limits<uint16_t>::max(),
		                uint16_t intensity = std::numeric_limits<uint16_t>::max(), AxisPosition origin_axis_position = BottomLeft);
		void addNewLine(int32_t start_x, int32_t start_y, int32_t end_x, int32_t end_y,
		                uint16_t red = std::numeric_limits<uint16_t>::max(), uint16_t green = std::numeric_limits<uint16_t>::max(), uint16_t blue = std::numeric_limits<uint16_t>::max(), uint16_t intensity = std::numeric_limits<uint16_t>::max());
		void addNewLine(JMVectorStruct &start_point, JMVectorStruct &end_point);
		bool addNewPoint(double x, double y,
		                 uint16_t red = std::numeric_limits<uint16_t>::max(), uint16_t green = std::numeric_limits<uint16_t>::max(), uint16_t blue = std::numeric_limits<uint16_t>::max(),
		                 uint16_t intensity = std::numeric_limits<uint16_t>::max(), AxisPosition origin_axis_position = BottomLeft);
		void addNewPoint(int32_t x, int32_t y,
		                 uint16_t red = std::numeric_limits<uint16_t>::max(), uint16_t green = std::numeric_limits<uint16_t>::max(), uint16_t blue = std::numeric_limits<uint16_t>::max(), uint16_t intensity = std::numeric_limits<uint16_t>::max());
		void addNewPoint(JMVectorStruct &point);
		void addNewPointWithLinearInterpolationFromLastPoint(JMVectorStruct &point);
		void correctRadialDistortion(JMVectorStruct &point);
		void correctRadialDistortionOnVectorImage();
		void replaceLastPoint(JMVectorStruct &point);
		void removeLastPoint();
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </functions>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <static functions>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		static double jmVectorStructDistanceSquared(JMVectorStruct first, JMVectorStruct second);
		static bool lineIntersection(double p0_x, double p0_y, double p1_x, double p1_y,
		                             double p2_x, double p2_y, double p3_x, double p3_y,
		                             double &i_x, double &i_y);
		static double linearInterpolation(double a, double b, double t);
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <static functions/>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <gets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		inline double getDrawingAreaWidth() const { return drawing_area_width_; }
		inline double getDrawingAreaHeight() const { return drawing_area_height_; }
		inline double getDrawingAreaXOffset() const { return drawing_area_x_offset_; }
		inline double getDrawingAreaYOffset() const { return drawing_area_y_offset_; }
		inline double getRadialDistortionCoefficientSecondDegreeInvertedUV() const { return radial_distortion_coefficient_second_degree_inverted_uv_; }
		inline double getRadialDistortionCoefficientSecondDegree() const { return radial_distortion_coefficient_second_degree_; }
		inline double getRadialDistortionCoefficientFourthDegree() const { return radial_distortion_coefficient_fourth_degree_; }
		inline double getRadialDistortionCoefficientSixthDegree() const { return radial_distortion_coefficient_sixth_degree_; }
		inline double getLineFirstPointMergeDistanceSquaredInProjectorRange() const { return line_first_point_merge_distance_squared_in_projector_range_; }
		inline double getLineFirstPointIgnoreDistanceSquaredInProjectorRange() const { return line_first_point_ignore_distance_squared_in_projector_range_; }
		inline int32_t getInterpolationDistanceInProjectorRange() const { return interpolation_distance_in_projector_range_; }
		inline std::vector<JMVectorStruct>& getVectorImagePoints() { return vector_image_points_; }
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </gets>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <sets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		inline void setDrawingAreaWidth(double drawing_area_width) { VectorImageBuilder::drawing_area_width_ = drawing_area_width; }
		inline void setDrawingAreaHeight(double drawing_area_height) { VectorImageBuilder::drawing_area_height_ = drawing_area_height; }
		inline void setDrawingAreaXOffset(double drawing_area_x_offset) { VectorImageBuilder::drawing_area_x_offset_ = drawing_area_x_offset; }
		inline void setDrawingAreaYOffset(double drawing_area_y_offset) { VectorImageBuilder::drawing_area_y_offset_ = drawing_area_y_offset; }
		inline void setRadialDistortionCoefficientSecondDegreeInvertedUV(double radial_distortion_coefficient_second_degree_inverted_uv) { VectorImageBuilder::radial_distortion_coefficient_second_degree_inverted_uv_ = radial_distortion_coefficient_second_degree_inverted_uv; }
		inline void setRadialDistortionCoefficientSecondDegree(double radial_distortion_coefficient_second_degree) { VectorImageBuilder::radial_distortion_coefficient_second_degree_ = radial_distortion_coefficient_second_degree; }
		inline void setRadialDistortionCoefficientFourthDegree(double radial_distortion_coefficient_fourth_degree) { VectorImageBuilder::radial_distortion_coefficient_fourth_degree_ = radial_distortion_coefficient_fourth_degree; }
		inline void setRadialDistortionCoefficientSixthDegree(double radial_distortion_coefficient_sixth_degree) { VectorImageBuilder::radial_distortion_coefficient_sixth_degree_ = radial_distortion_coefficient_sixth_degree; }
		inline void setLineFirstPointMergeDistanceSquaredInProjectorRange(double line_first_point_merge_distance_squared_in_projector_range) { VectorImageBuilder::line_first_point_merge_distance_squared_in_projector_range_ = line_first_point_merge_distance_squared_in_projector_range; }
		inline void setLineFirstPointIgnoreDistanceSquaredInProjectorRange(double line_first_point_ignore_distance_squared_in_projector_range) { VectorImageBuilder::line_first_point_ignore_distance_squared_in_projector_range_ = line_first_point_ignore_distance_squared_in_projector_range; }
		inline void setInterpolationDistanceInProjectorRange(int32_t interpolation_distance_in_projector_range) { VectorImageBuilder::interpolation_distance_in_projector_range_ = interpolation_distance_in_projector_range; }
	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </sets>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// ============================================================================   </public-section>   =========================================================================


	// ============================================================================   <protected-section>   =======================================================================
	protected:
		double drawing_area_width_;
		double drawing_area_height_;
		double drawing_area_x_offset_;
		double drawing_area_y_offset_;
		double drawing_area_to_projector_range_x_scale_;
		double drawing_area_to_projector_range_y_scale_;
		double radial_distortion_coefficient_second_degree_inverted_uv_;
		double radial_distortion_coefficient_second_degree_;
		double radial_distortion_coefficient_fourth_degree_;
		double radial_distortion_coefficient_sixth_degree_;
		double line_first_point_merge_distance_squared_in_projector_range_;
		double line_first_point_ignore_distance_squared_in_projector_range_;
		int32_t interpolation_distance_in_projector_range_;
		std::vector<JMVectorStruct> vector_image_points_;
	// ============================================================================   <protected-section>   =======================================================================
};

}
