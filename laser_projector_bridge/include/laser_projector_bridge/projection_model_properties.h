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
			focal_length_x_in_pixels_(3000.0),
			focal_length_y_in_pixels_(3000.0),
			principal_point_x_in_pixels_(1000.0),
			principal_point_y_in_pixels_(1000.0),
			distance_between_mirrors_(0.001),
			distance_to_image_plane_(3000.0),
			compute_distance_to_image_plane_(false),
			scale_image_plane_points_using_intrinsics_(false),
			change_to_principal_point_origin_when_correcting_galvanometer_distortion_(false),
			radial_distortion_correction_first_coefficient_(0),
			radial_distortion_correction_second_coefficient_(0),
			radial_distortion_correction_third_coefficient_(0),
			tangencial_distortion_correction_first_coefficient_(0),
			tangencial_distortion_correction_second_coefficient_(0) {}

		virtual ~ProjectionModelProperties() {}
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </constructors-destructor>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <gets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		inline double getImageWidthInPixels() const { return image_width_in_pixels_; }
		inline double getImageHeightInPixels() const { return image_height_in_pixels_; }
		inline double getFocalLengthXInPixels() const { return focal_length_x_in_pixels_; }
		inline double getFocalLengthYInPixels() const { return focal_length_y_in_pixels_; }
		inline double getPrincipalPointXInPixels() const { return principal_point_x_in_pixels_; }
		inline double getPrincipalPointYInPixels() const { return principal_point_y_in_pixels_; }
		inline double getDistanceBetweenMirrors() const { return distance_between_mirrors_; }
		inline double getDistanceToImagePlane() const { return distance_to_image_plane_; }
		inline bool getComputeDistanceToImagePlane() const { return compute_distance_to_image_plane_; }
		inline bool getScaleImagePlanePointsUsingIntrinsics() const { return scale_image_plane_points_using_intrinsics_; }
		inline bool getChangeToPrincipalPointOriginWhenCorrectingGalvanometerDistortion() const { return change_to_principal_point_origin_when_correcting_galvanometer_distortion_; }
		inline double getRadialDistortionCorrectionFirstCoefficient() const { return radial_distortion_correction_first_coefficient_; }
		inline double getRadialDistortionCorrectionSecondCoefficient() const { return radial_distortion_correction_second_coefficient_; }
		inline double getRadialDistortionCorrectionThirdCoefficient() const { return radial_distortion_correction_third_coefficient_; }
		inline double getTangencialDistortionCorrectionFirstCoefficient() const { return tangencial_distortion_correction_first_coefficient_; }
		inline double getTangencialDistortionCorrectionSecondCoefficient() const { return tangencial_distortion_correction_second_coefficient_; }
		inline bool hasLensDistortionCoefficients()
		{
			return (radial_distortion_correction_first_coefficient_ != 0 ||
					radial_distortion_correction_second_coefficient_ != 0 ||
					radial_distortion_correction_third_coefficient_ != 0 ||
					tangencial_distortion_correction_first_coefficient_ != 0 ||
					tangencial_distortion_correction_second_coefficient_ != 0);
		}
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </gets>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <sets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		inline void setImageWidthInPixels(double _image_width_in_pixels) { image_width_in_pixels_ = _image_width_in_pixels; }
		inline void setImageHeightInPixels(double _image_height_in_pixels) { image_height_in_pixels_ = _image_height_in_pixels; }
		inline void setFocalLengthXInPixels(double _focal_length_x_in_pixels) { focal_length_x_in_pixels_ = _focal_length_x_in_pixels; }
		inline void setFocalLengthYInPixels(double _focal_length_y_in_pixels) { focal_length_y_in_pixels_ = _focal_length_y_in_pixels; }
		inline void setPrincipalPointXInPixels(double _principal_point_x_in_pixels) { principal_point_x_in_pixels_ = _principal_point_x_in_pixels; }
		inline void setPrincipalPointYInPixels(double _principal_point_y_in_pixels) { principal_point_y_in_pixels_ = _principal_point_y_in_pixels; }
		inline void setDistanceBetweenMirrors(double _distance_between_mirrors_in_projector_range_percentage) { distance_between_mirrors_ = _distance_between_mirrors_in_projector_range_percentage; }
		inline void setDistanceToImagePlane(double _distance_to_image_plane) { distance_to_image_plane_ = _distance_to_image_plane; }
		inline void setComputeDistanceToImagePlane(const bool _compute_distance_to_image_plane) { compute_distance_to_image_plane_ = _compute_distance_to_image_plane; }
		inline void setScaleImagePlanePointsUsingIntrinsics(const bool _scale_image_plane_points_using_intrinsics) { scale_image_plane_points_using_intrinsics_ = _scale_image_plane_points_using_intrinsics; }
		inline void setChangeToPrincipalPointOriginWhenCorrectingGalvanometerDistortion(const bool _change_to_principal_point_origin_when_correcting_galvanometer_distortion) { change_to_principal_point_origin_when_correcting_galvanometer_distortion_ = _change_to_principal_point_origin_when_correcting_galvanometer_distortion; }
		inline void setRadialDistortionCorrectionFirstCoefficient(double _radial_distortion_correction_first_coefficient) { radial_distortion_correction_first_coefficient_ = _radial_distortion_correction_first_coefficient; }
		inline void setRadialDistortionCorrectionSecondCoefficient(double _radial_distortion_correction_second_coefficient) { radial_distortion_correction_second_coefficient_ = _radial_distortion_correction_second_coefficient; }
		inline void setRadialDistortionCorrectionThirdCoefficient(double _radial_distortion_correction_third_coefficient) { radial_distortion_correction_third_coefficient_ = _radial_distortion_correction_third_coefficient; }
		inline void setTangencialDistortionCorrectionFirstCoefficient(double _tangencial_distortion_correction_first_coefficient) { tangencial_distortion_correction_first_coefficient_ = _tangencial_distortion_correction_first_coefficient; }
		inline void setTangencialDistortionCorrectionSecondCoefficient(double _tangencial_distortion_correction_second_coefficient) { tangencial_distortion_correction_second_coefficient_ = _tangencial_distortion_correction_second_coefficient; }
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
		double distance_to_image_plane_;
		bool compute_distance_to_image_plane_;
		bool scale_image_plane_points_using_intrinsics_;
		bool change_to_principal_point_origin_when_correcting_galvanometer_distortion_;
		double radial_distortion_correction_first_coefficient_;
		double radial_distortion_correction_second_coefficient_;
		double radial_distortion_correction_third_coefficient_;
		double tangencial_distortion_correction_first_coefficient_;
		double tangencial_distortion_correction_second_coefficient_;
	// ============================================================================   <protected-section>   =======================================================================
};

}
