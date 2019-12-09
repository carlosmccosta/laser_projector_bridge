#pragma once

/**\file projection_model_properties.h
 * \brief Class with the projection model properties for building vector images.
 *
 * @version 1.0
 * @author Carlos Miguel Correia da Costa
 */


namespace laser_projector_bridge {

// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||   ProjectionModelProperties   |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
/**
 * \brief Class with the projection model properties for building vector images.
 */
class ProjectionModelProperties {
	// ==========================================================================   <public-section>   ============================================================================
	public:
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <constructors-destructor>    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		ProjectionModelProperties() :
			image_width_in_pixels_(2000.0),
			image_height_in_pixels_(2000.0),
			focal_length_x_in_pixels_(2830.0),
			focal_length_y_in_pixels_(2800.0),
			principal_point_x_in_pixels_(1000.0),
			principal_point_y_in_pixels_(1000.0),
			distance_between_mirrors_(0.001),
			distance_to_image_plane_for_correcting_distortion_(2830.0),
			distance_to_image_plane_for_converting_x_galvo_angle_to_drawing_area_(2830),
			distance_to_image_plane_for_converting_y_galvo_angle_to_drawing_area_(2800),
			compute_distances_to_image_planes_(false),
			scale_image_plane_points_using_intrinsics_(false),
			change_to_principal_point_origin_when_correcting_galvanometer_distortion_(true),
			use_ray_to_plane_intersection_for_converting_galvos_angles_to_drawing_area_(false),
			radial_distortion_correction_first_coefficient_(0),
			radial_distortion_correction_second_coefficient_(0),
			radial_distortion_correction_third_coefficient_(0),
			tangential_distortion_correction_first_coefficient_(0),
			tangential_distortion_correction_second_coefficient_(0) {}

		virtual ~ProjectionModelProperties() {}
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </constructors-destructor>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <gets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		double getImageWidthInPixels() const { return image_width_in_pixels_; }
		double getImageHeightInPixels() const { return image_height_in_pixels_; }
		double getFocalLengthXInPixels() const { return focal_length_x_in_pixels_; }
		double getFocalLengthYInPixels() const { return focal_length_y_in_pixels_; }
		double getPrincipalPointXInPixels() const { return principal_point_x_in_pixels_; }
		double getPrincipalPointYInPixels() const { return principal_point_y_in_pixels_; }
		double getDistanceBetweenMirrors() const { return distance_between_mirrors_; }
		double getDistanceToImagePlaneForCorrectingDistortion() const { return distance_to_image_plane_for_correcting_distortion_; }
		double getDistanceToImagePlaneForConvertingXGalvoAngleToDrawingArea() const { return distance_to_image_plane_for_converting_x_galvo_angle_to_drawing_area_; }
		double getDistanceToImagePlaneForConvertingYGalvoAngleToDrawingArea() const { return distance_to_image_plane_for_converting_y_galvo_angle_to_drawing_area_; }
		bool getComputeDistancesToImagePlanes() const { return compute_distances_to_image_planes_; }
		bool getScaleImagePlanePointsUsingIntrinsics() const { return scale_image_plane_points_using_intrinsics_; }
		bool getChangeToPrincipalPointOriginWhenCorrectingGalvanometerDistortion() const { return change_to_principal_point_origin_when_correcting_galvanometer_distortion_; }
		bool getUseRayToPlaneIntersectionForConvertingGalvosAnglesToDrawingArea() const { return use_ray_to_plane_intersection_for_converting_galvos_angles_to_drawing_area_; }
		double getRadialDistortionCorrectionFirstCoefficient() const { return radial_distortion_correction_first_coefficient_; }
		double getRadialDistortionCorrectionSecondCoefficient() const { return radial_distortion_correction_second_coefficient_; }
		double getRadialDistortionCorrectionThirdCoefficient() const { return radial_distortion_correction_third_coefficient_; }
		double getTangentialDistortionCorrectionFirstCoefficient() const { return tangential_distortion_correction_first_coefficient_; }
		double getTangentialDistortionCorrectionSecondCoefficient() const { return tangential_distortion_correction_second_coefficient_; }
		bool hasLensDistortionCoefficients()
		{
			return (radial_distortion_correction_first_coefficient_ != 0 ||
					radial_distortion_correction_second_coefficient_ != 0 ||
					radial_distortion_correction_third_coefficient_ != 0 ||
					tangential_distortion_correction_first_coefficient_ != 0 ||
					tangential_distortion_correction_second_coefficient_ != 0);
		}
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </gets>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <sets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		void setImageWidthInPixels(double _image_width_in_pixels) { image_width_in_pixels_ = _image_width_in_pixels; }
		void setImageHeightInPixels(double _image_height_in_pixels) { image_height_in_pixels_ = _image_height_in_pixels; }
		void setFocalLengthXInPixels(double _focal_length_x_in_pixels) { focal_length_x_in_pixels_ = _focal_length_x_in_pixels; }
		void setFocalLengthYInPixels(double _focal_length_y_in_pixels) { focal_length_y_in_pixels_ = _focal_length_y_in_pixels; }
		void setPrincipalPointXInPixels(double _principal_point_x_in_pixels) { principal_point_x_in_pixels_ = _principal_point_x_in_pixels; }
		void setPrincipalPointYInPixels(double _principal_point_y_in_pixels) { principal_point_y_in_pixels_ = _principal_point_y_in_pixels; }
		void setDistanceBetweenMirrors(double _distance_between_mirrors_in_projector_range_percentage) { distance_between_mirrors_ = _distance_between_mirrors_in_projector_range_percentage; }
		void setDistanceToImagePlaneForCorrectingDistortion(double _distance_to_image_plane) { distance_to_image_plane_for_correcting_distortion_ = _distance_to_image_plane; }
		void setDistanceToImagePlaneForConvertingXGalvoAngleToDrawingArea(const double _distance_to_image_plane_for_converting_x_galvo_angle_to_drawing_area) { distance_to_image_plane_for_converting_x_galvo_angle_to_drawing_area_ = _distance_to_image_plane_for_converting_x_galvo_angle_to_drawing_area; }
		void setDistanceToImagePlaneForConvertingYGalvoAngleToDrawingArea(const double _distance_to_image_plane_for_converting_y_galvo_angle_to_drawing_area) { distance_to_image_plane_for_converting_y_galvo_angle_to_drawing_area_ = _distance_to_image_plane_for_converting_y_galvo_angle_to_drawing_area; }
		void setComputeDistancesToImagePlanes(const bool _compute_distance_to_image_plane) { compute_distances_to_image_planes_ = _compute_distance_to_image_plane; }
		void setScaleImagePlanePointsUsingIntrinsics(const bool _scale_image_plane_points_using_intrinsics) { scale_image_plane_points_using_intrinsics_ = _scale_image_plane_points_using_intrinsics; }
		void setChangeToPrincipalPointOriginWhenCorrectingGalvanometerDistortion(const bool _change_to_principal_point_origin_when_correcting_galvanometer_distortion) { change_to_principal_point_origin_when_correcting_galvanometer_distortion_ = _change_to_principal_point_origin_when_correcting_galvanometer_distortion; }
		void setUseRayToPlaneIntersectionForConvertingGalvosAnglesToDrawingArea(const bool _use_ray_to_plane_intersection_for_converting_galvos_angles_to_drawing_area) { use_ray_to_plane_intersection_for_converting_galvos_angles_to_drawing_area_ = _use_ray_to_plane_intersection_for_converting_galvos_angles_to_drawing_area; }
		void setRadialDistortionCorrectionFirstCoefficient(double _radial_distortion_correction_first_coefficient) { radial_distortion_correction_first_coefficient_ = _radial_distortion_correction_first_coefficient; }
		void setRadialDistortionCorrectionSecondCoefficient(double _radial_distortion_correction_second_coefficient) { radial_distortion_correction_second_coefficient_ = _radial_distortion_correction_second_coefficient; }
		void setRadialDistortionCorrectionThirdCoefficient(double _radial_distortion_correction_third_coefficient) { radial_distortion_correction_third_coefficient_ = _radial_distortion_correction_third_coefficient; }
		void setTangentialDistortionCorrectionFirstCoefficient(double _tangential_distortion_correction_first_coefficient) { tangential_distortion_correction_first_coefficient_ = _tangential_distortion_correction_first_coefficient; }
		void setTangentialDistortionCorrectionSecondCoefficient(double _tangential_distortion_correction_second_coefficient) { tangential_distortion_correction_second_coefficient_ = _tangential_distortion_correction_second_coefficient; }
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </sets>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// ============================================================================   </public-section>   =========================================================================


	// ============================================================================   <protected-section>   =======================================================================
	protected:
		double image_width_in_pixels_;
		double image_height_in_pixels_;
		double focal_length_x_in_pixels_;
		double focal_length_y_in_pixels_;
		double principal_point_x_in_pixels_;
		double principal_point_y_in_pixels_;
		double distance_between_mirrors_;
		double distance_to_image_plane_for_correcting_distortion_;
		double distance_to_image_plane_for_converting_x_galvo_angle_to_drawing_area_;
		double distance_to_image_plane_for_converting_y_galvo_angle_to_drawing_area_;
		bool compute_distances_to_image_planes_;
		bool scale_image_plane_points_using_intrinsics_;
		bool change_to_principal_point_origin_when_correcting_galvanometer_distortion_;
		bool use_ray_to_plane_intersection_for_converting_galvos_angles_to_drawing_area_;
		double radial_distortion_correction_first_coefficient_;
		double radial_distortion_correction_second_coefficient_;
		double radial_distortion_correction_third_coefficient_;
		double tangential_distortion_correction_first_coefficient_;
		double tangential_distortion_correction_second_coefficient_;
	// ============================================================================   <protected-section>   =======================================================================
};

}
