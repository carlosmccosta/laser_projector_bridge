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
#include <laser_projector_bridge/projection_model_properties.h>
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
		void addReverseImage();
		bool convertPointFromDrawingAreaOriginToProjectorOrigin(double x, double y, double &x_point_in_drawing_area_and_projector_origin, double &y_point_in_drawing_area_and_projector_origin, AxisPosition projector_origin_axis_position = BottomLeft);
		bool convertPointFromProjectorOriginToDrawingAreaOrigin(double x, double y, double &x_point_in_drawing_area_origin, double &y_point_in_drawing_area_origin, AxisPosition drawing_area_origin_axis_position = BottomLeft);
		bool convertPointFromDrawingAreaInProjectorOriginToProjectorRange(double x_point_in_drawing_area_and_projector_origin, double y_point_in_drawing_area_and_projector_origin, int32_t &x_point_in_projector_range, int32_t &y_point_in_projector_range);
		bool convertPointFromProjectorRangeToDrawingArea(int32_t x_point_in_projector_range, int32_t y_point_in_projector_range, double &x_point_in_drawing_area_range, double &y_point_in_drawing_area_range, AxisPosition origin_axis_position = BottomLeft);
		bool trimLineInDrawingAreaAndProjectorOrigin(double &start_x_in_projector_origin, double &start_y_in_projector_origin, double &end_x_in_projector_origin, double &end_y_in_projector_origin);
		bool isPointInProjectorOriginWithinDrawingArea(double x, double y);
		bool addNewLine(double start_x, double start_y, double end_x, double end_y,
		                uint16_t red = std::numeric_limits<uint16_t>::max(), uint16_t green = std::numeric_limits<uint16_t>::max(), uint16_t blue = std::numeric_limits<uint16_t>::max(),
		                uint16_t intensity = std::numeric_limits<uint16_t>::max(), AxisPosition origin_axis_position = BottomLeft);
		bool addNewLine(int32_t start_x, int32_t start_y, int32_t end_x, int32_t end_y,
		                uint16_t red = std::numeric_limits<uint16_t>::max(), uint16_t green = std::numeric_limits<uint16_t>::max(), uint16_t blue = std::numeric_limits<uint16_t>::max(), uint16_t intensity = std::numeric_limits<uint16_t>::max());
		bool addNewLine(JMVectorStruct &start_point, JMVectorStruct &end_point);
		bool addNewPoint(double x, double y,
		                 uint16_t red = std::numeric_limits<uint16_t>::max(), uint16_t green = std::numeric_limits<uint16_t>::max(), uint16_t blue = std::numeric_limits<uint16_t>::max(),
		                 uint16_t intensity = std::numeric_limits<uint16_t>::max(), AxisPosition origin_axis_position = BottomLeft);
		bool addNewPoint(int32_t x, int32_t y,
		                 uint16_t red = std::numeric_limits<uint16_t>::max(), uint16_t green = std::numeric_limits<uint16_t>::max(), uint16_t blue = std::numeric_limits<uint16_t>::max(), uint16_t intensity = std::numeric_limits<uint16_t>::max());
		bool addNewPoint(JMVectorStruct &point);
		bool addNewPointWithLinearInterpolationFromLastPoint(JMVectorStruct &point);
		void addLastPointTurnedOff();
		void addLastPointBlankingPoints();
		void replaceLastPoint(JMVectorStruct &point);
		void removeLastPoint();
		void addFirstPointBlankingPoints(JMVectorStruct &start_point);
		void correctDistortionOnVectorImage();
		void computeScalingFactorsFromImagePlaneToDrawingArea(double& x_scale, double& y_scale);
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </functions>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <static functions>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		static void correctGalvanometerMirrorsDistortion(double& x, double& y, double distance_between_mirrors, double distance_to_image_plane_for_correcting_distortion, double distance_to_image_plane_for_converting_x_galvo_angle_to_drawing_area, double distance_to_image_plane_for_converting_y_galvo_angle_to_drawing_area, bool use_ray_to_plane_intersection_for_converting_galvos_angles_to_drawing_area);
		static void drawingAreaToGalvoAngles(double x, double y, double distance_between_mirrors, double distance_to_image_plane, double& galvo_x_angle, double& galvo_y_angle);
		static void galvoAnglesToDrawingArea(double galvo_x_angle, double galvo_y_angle, double distance_between_mirrors, double distance_to_image_plane, double& x, double& y);
		static void drawingAreaToPinHoleAngles(double x, double y, double distance_to_image_plane_for_x, double distance_to_image_plane_for_y, double& xAngle, double& yAngle, bool use_ray_to_plane_intersection_for_converting_galvos_angles_to_drawing_area);
		static void pinHoleAnglesToDrawingArea(double galvo_x_angle, double galvo_y_angle, double distance_to_image_plane_for_updating_new_x, double distance_to_image_plane_for_updating_new_y, double& x, double& y, bool use_ray_to_plane_intersection_for_converting_galvos_angles_to_drawing_area);
		static double computeDistanceToImagePlane(double focal_length_in_pixels, double image_size_in_pixels);
		static double computeFieldOfView(double focal_length_in_pixels, double image_size_in_pixels);
		static double changeFromDrawingAreaOriginToPrincipalPointOrigin(double drawing_area_value, double image_size, double principal_point);
		static double changeFromPrincipalPointOriginToDrawingAreaOrigin(double drawing_area_value, double image_size, double principal_point);
		static void correctLensDistortion(double& x, double& y, const ProjectionModelProperties& projection_model_properties);
		static bool lineIntersection(double p0_x, double p0_y, double p1_x, double p1_y,
		                             double p2_x, double p2_y, double p3_x, double p3_y,
		                             double &i_x, double &i_y, double comparison_epsilon = 1e-8);
		static double linearInterpolation(double a, double b, double t);
		static double jmVectorStructDistanceSquared(const JMVectorStruct& first, const JMVectorStruct& second);
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <static functions/>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <gets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		inline std::vector<JMVectorStruct>& getVectorImagePoints() { return vector_image_points_; }
		inline ProjectionModelProperties& getProjectionModelProperties() { return projection_model_properties_; }
		inline double getDrawingAreaXOffset() const { return drawing_area_x_offset_; }
		inline double getDrawingAreaYOffset() const { return drawing_area_y_offset_; }
		inline int32_t getMinimumProjectorRangeValueForClipping() { return minimum_projector_range_value_for_clipping_; }
		inline int32_t getMaximumProjectorRangeValueForClipping() { return maximum_projector_range_value_for_clipping_; }
		inline bool getTrimPointsOutsideDrawingArea() { return trim_points_outside_drawing_area_; }
		inline double getLineFirstPointMergeDistanceSquaredInProjectorRange() const { return line_first_point_merge_distance_squared_in_projector_range_; }
		inline double getLineFirstPointIgnoreDistanceSquaredInProjectorRange() const { return line_first_point_ignore_distance_squared_in_projector_range_; }
		inline int64_t getInterpolationDistanceInProjectorRange() const { return interpolation_distance_in_projector_range_; }
		inline size_t getNumberOfBlankingPointsForLineStartAndEnd() { return number_of_blanking_points_for_line_start_and_end_; }
		inline size_t getMaximumNumberOfPoints() { return maximum_number_of_points_; }
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </gets>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <sets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		inline void setDrawingAreaXOffset(double drawing_area_x_offset) { drawing_area_x_offset_ = drawing_area_x_offset; }
		inline void setDrawingAreaYOffset(double drawing_area_y_offset) { drawing_area_y_offset_ = drawing_area_y_offset; }
		inline void setMinimumProjectorRangeValueForClipping(int32_t minimum_projector_range_value_for_clipping) { minimum_projector_range_value_for_clipping_ = minimum_projector_range_value_for_clipping; }
		inline void setMaximumProjectorRangeValueForClipping(int32_t maximum_projector_range_value_for_clipping) { maximum_projector_range_value_for_clipping_ = maximum_projector_range_value_for_clipping; }
		inline void setTrimPointsOutsideDrawingArea(bool trim_points_outside_drawing_area) { trim_points_outside_drawing_area_ = trim_points_outside_drawing_area; }
		inline void setLineFirstPointMergeDistanceSquaredInProjectorRange(double line_first_point_merge_distance_squared_in_projector_range) { line_first_point_merge_distance_squared_in_projector_range_ = line_first_point_merge_distance_squared_in_projector_range; }
		inline void setLineFirstPointIgnoreDistanceSquaredInProjectorRange(double line_first_point_ignore_distance_squared_in_projector_range) { line_first_point_ignore_distance_squared_in_projector_range_ = line_first_point_ignore_distance_squared_in_projector_range; }
		inline void setInterpolationDistanceInProjectorRange(int64_t interpolation_distance_in_projector_range) { interpolation_distance_in_projector_range_ = interpolation_distance_in_projector_range; }
		inline void setNumberOfBlankingPointsForLineStartAndEnd(size_t number_of_blanking_points_for_line_start) { number_of_blanking_points_for_line_start_and_end_ = number_of_blanking_points_for_line_start; }
		inline void setMaximumNumberOfPoints(size_t maximum_number_of_points) { maximum_number_of_points_ = maximum_number_of_points; }
	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </sets>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// ============================================================================   </public-section>   =========================================================================


	// ============================================================================   <protected-section>   =======================================================================
	protected:
		std::vector<JMVectorStruct> vector_image_points_;
		ProjectionModelProperties projection_model_properties_;
		double drawing_area_x_offset_;
		double drawing_area_y_offset_;
		int32_t minimum_projector_range_value_for_clipping_;
		int32_t maximum_projector_range_value_for_clipping_;
		bool trim_points_outside_drawing_area_;
		double line_first_point_merge_distance_squared_in_projector_range_;
		double line_first_point_ignore_distance_squared_in_projector_range_;
		int64_t interpolation_distance_in_projector_range_;
		size_t number_of_blanking_points_for_line_start_and_end_;
		size_t maximum_number_of_points_;
	
	private:
		double drawing_area_to_projector_range_x_scale_;
		double drawing_area_to_projector_range_y_scale_;
	// ============================================================================   <protected-section>   =======================================================================
};

}
