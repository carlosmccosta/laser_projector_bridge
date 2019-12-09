/**\file vector_image_builder.cpp
 * \brief Class for building vector images with linear interpolation and radial distortion correction.
 *
 * @version 1.0
 * @author Carlos Miguel Correia da Costa
 */

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <includes>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#include <laser_projector_bridge/vector_image_builder.h>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </includes>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



namespace laser_projector_bridge {

// ===============================================================================  <public-section>   ============================================================================
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <constructors-destructor>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	VectorImageBuilder::VectorImageBuilder() :
		drawing_area_x_offset_(0.0),
		drawing_area_y_offset_(0.0),
		minimum_projector_range_value_for_clipping_((int32_t)((double)std::numeric_limits<int32_t>::min() * 0.99)),
		maximum_projector_range_value_for_clipping_((int32_t)((double)std::numeric_limits<int32_t>::max() * 0.99)),
		trim_points_outside_drawing_area_(true),
		line_first_point_merge_distance_squared_in_projector_range_(std::pow((double)std::numeric_limits<uint32_t>::max() * 0.0005, 2.0)),
		line_first_point_ignore_distance_squared_in_projector_range_(std::pow((double)std::numeric_limits<uint32_t>::max() * 0.001, 2.0)),
		interpolation_distance_in_projector_range_((int64_t)((double)std::numeric_limits<uint32_t>::max() * 0.001)),
		number_of_blanking_points_for_line_start_and_end_(15),
		maximum_number_of_points_(16000),
		drawing_area_to_projector_range_x_scale_((double)std::numeric_limits<uint32_t>::max() / 2000.0),
		drawing_area_to_projector_range_y_scale_((double)std::numeric_limits<uint32_t>::max() / 2000.0)
		{}

VectorImageBuilder::~VectorImageBuilder() {}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </constructors-destructor>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <functions>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
void VectorImageBuilder::startNewVectorImage() {
	vector_image_points_.clear();
	drawing_area_to_projector_range_x_scale_ = ((double)std::numeric_limits<uint32_t>::max()) / projection_model_properties_.getImageWidthInPixels();
	drawing_area_to_projector_range_y_scale_ = ((double)std::numeric_limits<uint32_t>::max()) / projection_model_properties_.getImageHeightInPixels();
}

void VectorImageBuilder::finishVectorImage() {
	if (!vector_image_points_.empty()) {
		addLastPointBlankingPoints();
		correctDistortionOnVectorImage();
		// todo: laser path optimization and blanking
	}
}


void VectorImageBuilder::addReverseImage() {
	if (vector_image_points_.size() * 2 <= maximum_number_of_points_)
	{
		for (int i = (int)vector_image_points_.size() - 1; i >= 0; --i) {
			vector_image_points_.push_back(vector_image_points_[i]);
		}
	}
}


bool VectorImageBuilder::convertPointFromDrawingAreaOriginToProjectorOrigin(double x, double y, double &x_point_in_drawing_area_and_projector_origin, double &y_point_in_drawing_area_and_projector_origin, AxisPosition projector_origin_axis_position) {
	switch (projector_origin_axis_position) {
		case AxisPosition::TopLeft: {
			x_point_in_drawing_area_and_projector_origin = x - projection_model_properties_.getImageWidthInPixels() * 0.5;
			y_point_in_drawing_area_and_projector_origin = (projection_model_properties_.getImageHeightInPixels() - y) - projection_model_properties_.getImageHeightInPixels() * 0.5;
			break;
		}

		case AxisPosition::BottomLeft: {
			x_point_in_drawing_area_and_projector_origin = x - projection_model_properties_.getImageWidthInPixels() * 0.5;
			y_point_in_drawing_area_and_projector_origin = y - projection_model_properties_.getImageHeightInPixels() * 0.5;
			break;
		}

		case AxisPosition::Middle: {
			x_point_in_drawing_area_and_projector_origin = x;
			y_point_in_drawing_area_and_projector_origin = y;
			break;
		}

		default: {
			x_point_in_drawing_area_and_projector_origin = 0.0;
			y_point_in_drawing_area_and_projector_origin = 0.0;
			return false;
		}
	}

	return true;
}


bool VectorImageBuilder::convertPointFromProjectorOriginToDrawingAreaOrigin(double x, double y, double &x_point_in_drawing_area_origin, double &y_point_in_drawing_area_origin, AxisPosition drawing_area_origin_axis_position) {
	switch (drawing_area_origin_axis_position) {
		case AxisPosition::TopLeft: {
			x_point_in_drawing_area_origin = x + projection_model_properties_.getImageWidthInPixels() * 0.5;
			y_point_in_drawing_area_origin = (projection_model_properties_.getImageHeightInPixels() - y) + projection_model_properties_.getImageHeightInPixels() * 0.5;
			break;
		}

		case AxisPosition::BottomLeft: {
			x_point_in_drawing_area_origin = x + projection_model_properties_.getImageWidthInPixels() * 0.5;
			y_point_in_drawing_area_origin = y + projection_model_properties_.getImageHeightInPixels() * 0.5;
			break;
		}

		case AxisPosition::Middle: {
			x_point_in_drawing_area_origin = x;
			y_point_in_drawing_area_origin = y;
			break;
		}

		default: {
			x_point_in_drawing_area_origin = 0.0;
			y_point_in_drawing_area_origin = 0.0;
			return false;
		}
	}

	return true;
}


bool VectorImageBuilder::convertPointFromDrawingAreaInProjectorOriginToProjectorRange(double x_point_in_drawing_area_and_projector_origin, double y_point_in_drawing_area_and_projector_origin, int32_t &x_point_in_projector_range,
                                                                                      int32_t &y_point_in_projector_range) {
	bool point_overflow = false;

	double x_with_offset = x_point_in_drawing_area_and_projector_origin - drawing_area_x_offset_;
	if (x_with_offset < projection_model_properties_.getImageWidthInPixels() * -0.5) {
		x_point_in_projector_range = minimum_projector_range_value_for_clipping_;
		point_overflow = true;
	} else if (x_with_offset > projection_model_properties_.getImageWidthInPixels() * 0.5) {
		x_point_in_projector_range = maximum_projector_range_value_for_clipping_;
		point_overflow = true;
	} else {
		x_point_in_projector_range = (int32_t)((x_point_in_drawing_area_and_projector_origin - drawing_area_x_offset_) * drawing_area_to_projector_range_x_scale_);
	}

	double y_with_offset = y_point_in_drawing_area_and_projector_origin - drawing_area_y_offset_;
	if (y_with_offset < projection_model_properties_.getImageHeightInPixels() * -0.5) {
		y_point_in_projector_range = minimum_projector_range_value_for_clipping_;
		point_overflow = true;
	} else if (x_with_offset > projection_model_properties_.getImageHeightInPixels() * 0.5) {
		y_point_in_projector_range = maximum_projector_range_value_for_clipping_;
		point_overflow = true;
	} else {
		y_point_in_projector_range = (int32_t)((y_point_in_drawing_area_and_projector_origin - drawing_area_y_offset_) * drawing_area_to_projector_range_y_scale_);
	}

	return !point_overflow;
}

bool VectorImageBuilder::convertPointFromProjectorRangeToDrawingArea(int32_t x_point_in_projector_range, int32_t y_point_in_projector_range, double &x_point_in_drawing_area_range, double &y_point_in_drawing_area_range, AxisPosition origin_axis_position) {
	double x_point_in_drawing_area_and_projector_origin = ((double)x_point_in_projector_range / drawing_area_to_projector_range_x_scale_) + drawing_area_x_offset_;
	double y_point_in_drawing_area_and_projector_origin = ((double)y_point_in_projector_range / drawing_area_to_projector_range_y_scale_) + drawing_area_y_offset_;
	return convertPointFromProjectorOriginToDrawingAreaOrigin(x_point_in_drawing_area_and_projector_origin, y_point_in_drawing_area_and_projector_origin, x_point_in_drawing_area_range, y_point_in_drawing_area_range, origin_axis_position);
}

bool VectorImageBuilder::trimLineInDrawingAreaAndProjectorOrigin(double &start_x_in_projector_origin, double &start_y_in_projector_origin, double &end_x_in_projector_origin, double &end_y_in_projector_origin) {
	bool start_point_valid = isPointInProjectorOriginWithinDrawingArea(start_x_in_projector_origin, start_y_in_projector_origin);
	bool end_point_valid = isPointInProjectorOriginWithinDrawingArea(end_x_in_projector_origin, end_y_in_projector_origin);
	if (start_point_valid && end_point_valid) return true;
	if (!start_point_valid && !end_point_valid) return false;

	double valid_point_x, valid_point_y, invalid_point_x, invalid_point_y;
	if (!start_point_valid) {
		invalid_point_x = start_x_in_projector_origin;
		invalid_point_y = start_y_in_projector_origin;
		valid_point_x = end_x_in_projector_origin;
		valid_point_y = end_y_in_projector_origin;
	} else {
		invalid_point_x = end_x_in_projector_origin;
		invalid_point_y = end_y_in_projector_origin;
		valid_point_x = start_x_in_projector_origin;
		valid_point_y = start_y_in_projector_origin;
	}

	double half_width = projection_model_properties_.getImageWidthInPixels() * 0.5;
	double half_height = projection_model_properties_.getImageHeightInPixels() * 0.5;
	double x_intersection = 0.0, y_intersection = 0.0;

	// left
	if (invalid_point_x < -half_width && invalid_point_y >= -half_height && invalid_point_y <= half_height) {
		if (lineIntersection(-half_width, -half_height, -half_width, half_height, start_x_in_projector_origin, start_y_in_projector_origin, end_x_in_projector_origin, end_y_in_projector_origin, x_intersection, y_intersection))
			x_intersection = -half_width;
		else
			return false;
	}

	// right
	if (invalid_point_x > half_width && invalid_point_y >= -half_height && invalid_point_y <= half_height) {
		if (lineIntersection(half_width, -half_height, half_width, half_height, start_x_in_projector_origin, start_y_in_projector_origin, end_x_in_projector_origin, end_y_in_projector_origin, x_intersection, y_intersection))
			x_intersection = half_width;
		else
			return false;
	}

	// top
	if (invalid_point_y > half_height && invalid_point_x >= -half_width && invalid_point_x <= half_width) {
		if (lineIntersection(-half_width, half_height, half_width, half_height, start_x_in_projector_origin, start_y_in_projector_origin, end_x_in_projector_origin, end_y_in_projector_origin, x_intersection, y_intersection))
			y_intersection = half_height;
		else
			return false;
	}

	// bottom
	if (invalid_point_y < -half_height && invalid_point_x >= -half_width && invalid_point_x <= half_width) {
		if (lineIntersection(-half_width, -half_height, half_width, -half_height, start_x_in_projector_origin, start_y_in_projector_origin, end_x_in_projector_origin, end_y_in_projector_origin, x_intersection, y_intersection))
			y_intersection = -half_height;
		else
			return false;
	}

	// bottom left
	if (invalid_point_x < -half_width && invalid_point_y < -half_height) {
		double x_intersection_bottom = 0.0, y_intersection_bottom = 0.0;
		double x_intersection_left = 0.0, y_intersection_left = 0.0;
		if (lineIntersection(-half_width, -half_height, half_width, -half_height, start_x_in_projector_origin, start_y_in_projector_origin, end_x_in_projector_origin, end_y_in_projector_origin, x_intersection_bottom, y_intersection_bottom) &&
				lineIntersection(-half_width, -half_height, -half_width, half_height, start_x_in_projector_origin, start_y_in_projector_origin, end_x_in_projector_origin, end_y_in_projector_origin, x_intersection_left, y_intersection_left)) {
			double distance_to_bottom_intersection = std::pow(x_intersection_bottom - valid_point_x, 2.0) + std::pow(y_intersection_bottom - valid_point_y, 2.0);
			double distance_to_left_intersection = std::pow(x_intersection_left - valid_point_x, 2.0) + std::pow(y_intersection_left - valid_point_y, 2.0);

			if (distance_to_bottom_intersection < distance_to_left_intersection) {
				x_intersection = x_intersection_bottom;
				y_intersection = -half_height;
			} else {
				x_intersection = -half_width;
				y_intersection = y_intersection_left;
			}
		} else {
			return false;
		}
	}

	// top left
	if (invalid_point_x < -half_width && invalid_point_y > half_height) {
		double x_intersection_top = 0.0, y_intersection_top = 0.0;
		double x_intersection_left = 0.0, y_intersection_left = 0.0;
		if (lineIntersection(-half_width, half_height, half_width, half_height, start_x_in_projector_origin, start_y_in_projector_origin, end_x_in_projector_origin, end_y_in_projector_origin, x_intersection_top, y_intersection_top) &&
				lineIntersection(-half_width, -half_height, -half_width, half_height, start_x_in_projector_origin, start_y_in_projector_origin, end_x_in_projector_origin, end_y_in_projector_origin, x_intersection_left, y_intersection_left)) {
			double distance_to_top_intersection = std::pow(x_intersection_top - valid_point_x, 2.0) + std::pow(y_intersection_top - valid_point_y, 2.0);
			double distance_to_left_intersection = std::pow(x_intersection_left - valid_point_x, 2.0) + std::pow(y_intersection_left - valid_point_y, 2.0);

			if (distance_to_top_intersection < distance_to_left_intersection) {
				x_intersection = x_intersection_top;
				y_intersection = half_height;
			} else {
				x_intersection = -half_width;
				y_intersection = y_intersection_left;
			}
		} else {
			return false;
		}
	}

	// bottom right
	if (invalid_point_x > half_width && invalid_point_y < -half_height) {
		double x_intersection_bottom = 0.0, y_intersection_bottom = 0.0;
		double x_intersection_right = 0.0, y_intersection_right = 0.0;
		if (lineIntersection(-half_width, -half_height, half_width, -half_height, start_x_in_projector_origin, start_y_in_projector_origin, end_x_in_projector_origin, end_y_in_projector_origin, x_intersection_bottom, y_intersection_bottom) &&
				lineIntersection(half_width, -half_height, half_width, half_height, start_x_in_projector_origin, start_y_in_projector_origin, end_x_in_projector_origin, end_y_in_projector_origin, x_intersection_right, y_intersection_right)) {
			double distance_to_bottom_intersection = std::pow(x_intersection_bottom - valid_point_x, 2.0) + std::pow(y_intersection_bottom - valid_point_y, 2.0);
			double distance_to_right_intersection = std::pow(x_intersection_right - valid_point_x, 2.0) + std::pow(y_intersection_right - valid_point_y, 2.0);

			if (distance_to_bottom_intersection < distance_to_right_intersection) {
				x_intersection = x_intersection_bottom;
				y_intersection = -half_height;
			} else {
				x_intersection = half_width;
				y_intersection = y_intersection_right;
			}
		} else {
			return false;
		}
	}

	// top right
	if (invalid_point_x > half_width && invalid_point_y > half_height) {
		double x_intersection_top = 0.0, y_intersection_top = 0.0;
		double x_intersection_right = 0.0, y_intersection_right = 0.0;
		if (lineIntersection(-half_width, half_height, half_width, half_height, start_x_in_projector_origin, start_y_in_projector_origin, end_x_in_projector_origin, end_y_in_projector_origin, x_intersection_top, y_intersection_top) &&
				lineIntersection(half_width, -half_height, half_width, half_height, start_x_in_projector_origin, start_y_in_projector_origin, end_x_in_projector_origin, end_y_in_projector_origin, x_intersection_right, y_intersection_right)) {
			double distance_to_top_intersection = std::pow(x_intersection_top - valid_point_x, 2.0) + std::pow(y_intersection_top - valid_point_y, 2.0);
			double distance_to_right_intersection = std::pow(x_intersection_right - valid_point_x, 2.0) + std::pow(y_intersection_right - valid_point_y, 2.0);

			if (distance_to_top_intersection < distance_to_right_intersection) {
				x_intersection = x_intersection_top;
				y_intersection = half_height;
			} else {
				x_intersection = half_width;
				y_intersection = y_intersection_right;
			}
		} else {
			return false;
		}
	}

	if (!start_point_valid) {
		start_x_in_projector_origin = x_intersection;
		start_y_in_projector_origin = y_intersection;
	} else {
		end_x_in_projector_origin = x_intersection;
		end_y_in_projector_origin = y_intersection;
	}

	return true;
}

bool VectorImageBuilder::isPointInProjectorOriginWithinDrawingArea(double x, double y) {
	double half_width = projection_model_properties_.getImageWidthInPixels() * 0.5;
	double half_height = projection_model_properties_.getImageHeightInPixels() * 0.5;
	return (x >= -half_width && x <= half_width && y >= -half_height && y <= half_height);
}

bool VectorImageBuilder::addNewLine(double start_x, double start_y, double end_x, double end_y,
                                    uint16_t red, uint16_t green, uint16_t blue,
                                    uint16_t intensity, AxisPosition origin_axis_position) {
	double start_x_in_projector_origin;
	double start_y_in_projector_origin;
	double end_x_in_projector_origin;
	double end_y_in_projector_origin;

	if (!convertPointFromDrawingAreaOriginToProjectorOrigin(start_x, start_y, start_x_in_projector_origin, start_y_in_projector_origin, origin_axis_position)) { return false; }
	if (!convertPointFromDrawingAreaOriginToProjectorOrigin(end_x, end_y, end_x_in_projector_origin, end_y_in_projector_origin, origin_axis_position)) { return false; }

	if (!trimLineInDrawingAreaAndProjectorOrigin(start_x_in_projector_origin, start_y_in_projector_origin, end_x_in_projector_origin, end_y_in_projector_origin)) { return false; }

	int32_t start_x_in_projector_range;
	int32_t start_y_in_projector_range;
	int32_t end_x_in_projector_range;
	int32_t end_y_in_projector_range;

	convertPointFromDrawingAreaInProjectorOriginToProjectorRange(start_x_in_projector_origin, start_y_in_projector_origin, start_x_in_projector_range, start_y_in_projector_range);
	convertPointFromDrawingAreaInProjectorOriginToProjectorRange(end_x_in_projector_origin, end_y_in_projector_origin, end_x_in_projector_range, end_y_in_projector_range);

	return addNewLine(start_x_in_projector_range, start_y_in_projector_range, end_x_in_projector_range, end_y_in_projector_range, red, green, blue, intensity);
}

bool VectorImageBuilder::addNewLine(int32_t start_x, int32_t start_y, int32_t end_x, int32_t end_y,
                                    uint16_t red, uint16_t green, uint16_t blue, uint16_t intensity) {
	JMVectorStruct start_point;
	start_point.x = start_x;
	start_point.y = start_y;
	start_point.r = red;
	start_point.g = green;
	start_point.b = blue;
	start_point.i = intensity;
	start_point.deepblue = std::numeric_limits<uint16_t>::max();
	start_point.yellow = std::numeric_limits<uint16_t>::max();
	start_point.cyan = std::numeric_limits<uint16_t>::max();
	start_point.user4 = std::numeric_limits<uint16_t>::max();

	JMVectorStruct end_point;
	end_point.x = end_x;
	end_point.y = end_y;
	end_point.r = red;
	end_point.g = green;
	end_point.b = blue;
	end_point.i = intensity;
	end_point.deepblue = std::numeric_limits<uint16_t>::max();
	end_point.yellow = std::numeric_limits<uint16_t>::max();
	end_point.cyan = std::numeric_limits<uint16_t>::max();
	end_point.user4 = std::numeric_limits<uint16_t>::max();

	return addNewLine(start_point, end_point);
}

bool VectorImageBuilder::addNewLine(JMVectorStruct &start_point, JMVectorStruct &end_point) {
	if (vector_image_points_.size() + 2 > maximum_number_of_points_)
		return false;

	if (!vector_image_points_.empty()) {
		JMVectorStruct last_point = vector_image_points_.back();
		double distance_to_last_point_squared = jmVectorStructDistanceSquared(start_point, last_point);

		bool start_point_different_than_last_point = false;

		// check if points are the same using the ignore distance as threshold
		if (line_first_point_ignore_distance_squared_in_projector_range_ > 0 && distance_to_last_point_squared >= line_first_point_ignore_distance_squared_in_projector_range_) {
			start_point_different_than_last_point = true;
		}
		else
		// check if points are the same using their coordinates (when ignore distance threshold is not active)
		if (line_first_point_ignore_distance_squared_in_projector_range_ <= 0 && (last_point.x != start_point.x || last_point.y != start_point.y)) {
			start_point_different_than_last_point = true;
		}
		else
		// check if colors are different when points are considered the same
		if (last_point.i != start_point.i || last_point.r != start_point.r || last_point.g != start_point.g || last_point.b != start_point.b ||
			last_point.cyan != start_point.cyan || last_point.deepblue != start_point.deepblue || last_point.yellow != start_point.yellow || last_point.user4 != start_point.user4) {
			start_point_different_than_last_point = true;
		}

		if (start_point_different_than_last_point) {
			addLastPointBlankingPoints();
			addFirstPointBlankingPoints(start_point);
		} else {
			if (distance_to_last_point_squared > 0 && line_first_point_merge_distance_squared_in_projector_range_ > 0 && distance_to_last_point_squared < line_first_point_merge_distance_squared_in_projector_range_) {
				last_point.x = (int32_t)((last_point.x + start_point.x) / 2.0);
				last_point.y = (int32_t)((last_point.y + start_point.y) / 2.0);
				replaceLastPoint(last_point);
			}

			if (vector_image_points_.size() > 1) {
				JMVectorStruct penultimate_point = vector_image_points_[vector_image_points_.size() - 2];
				double distance_to_penultimate_point_squared = jmVectorStructDistanceSquared(end_point, penultimate_point);
				if (distance_to_penultimate_point_squared < line_first_point_ignore_distance_squared_in_projector_range_) {
					removeLastPoint();
				}
			}
		}
	} else {
		addFirstPointBlankingPoints(start_point);
	}

	return addNewPointWithLinearInterpolationFromLastPoint(end_point);
}

bool VectorImageBuilder::addNewPoint(double x, double y,
                                     uint16_t red, uint16_t green, uint16_t blue,
                                     uint16_t intensity, AxisPosition origin_axis_position) {
	double x_in_projector_origin;
	double y_in_projector_origin;
	int32_t x_in_projector_range;
	int32_t y_in_projector_range;
	if (convertPointFromDrawingAreaOriginToProjectorOrigin(x, y, x_in_projector_origin, y_in_projector_origin, origin_axis_position) &&
			convertPointFromDrawingAreaInProjectorOriginToProjectorRange(x_in_projector_origin, y_in_projector_origin, x_in_projector_range, y_in_projector_range)) {
		addNewPoint(x_in_projector_range, y_in_projector_range, red, green, blue, intensity);
		return true;
	}
	return false;
}

bool VectorImageBuilder::addNewPoint(int32_t x, int32_t y,
                                     uint16_t red, uint16_t green, uint16_t blue, uint16_t intensity) {
	JMVectorStruct new_point;
	new_point.x = x;
	new_point.y = y;
	new_point.r = red;
	new_point.g = green;
	new_point.b = blue;
	new_point.i = intensity;
	new_point.deepblue = std::numeric_limits<uint16_t>::max();
	new_point.yellow = std::numeric_limits<uint16_t>::max();
	new_point.cyan = std::numeric_limits<uint16_t>::max();
	new_point.user4 = std::numeric_limits<uint16_t>::max();

	return addNewPoint(new_point);
}

bool VectorImageBuilder::addNewPoint(JMVectorStruct &point) {
	if (vector_image_points_.size() < maximum_number_of_points_)
	{
		if (vector_image_points_.empty() && point.i > 0) {
			JMVectorStruct point_start_off = point;
			point_start_off.i = 0;
			vector_image_points_.push_back(point_start_off);
		}
		vector_image_points_.push_back(point);
		return true;
	}

	return false;
}

bool VectorImageBuilder::addNewPointWithLinearInterpolationFromLastPoint(JMVectorStruct &point) {
	if (interpolation_distance_in_projector_range_ > 0) {
		JMVectorStruct last_point = vector_image_points_.back();
		double distance_to_last_point = std::sqrt(jmVectorStructDistanceSquared(last_point, point));
		int64_t number_of_interpolation_points = (int64_t)(distance_to_last_point / (double)interpolation_distance_in_projector_range_) - 1;

		if (vector_image_points_.size() + number_of_interpolation_points > maximum_number_of_points_)
		{
			number_of_interpolation_points = maximum_number_of_points_ - vector_image_points_.size() - 1;
		}

		if (number_of_interpolation_points > 0) {
			double t_increment = 1.0 / (double)number_of_interpolation_points;
			double t = t_increment;
			for (int64_t i = 0; i < number_of_interpolation_points; ++i) {
				JMVectorStruct new_point = last_point;
				new_point.x = (int32_t)linearInterpolation(last_point.x, point.x, t);
				new_point.y = (int32_t)linearInterpolation(last_point.y, point.y, t);
				addNewPoint(new_point);
				t += t_increment;
			}
		}
	}

	return addNewPoint(point);
}

void VectorImageBuilder::addLastPointTurnedOff() {
	if (!vector_image_points_.empty()) {
		JMVectorStruct last_point_off = vector_image_points_.back();
		last_point_off.i = 0;
		addNewPoint(last_point_off);
	}
}

void VectorImageBuilder::addLastPointBlankingPoints() {
	if (!vector_image_points_.empty()) {
		JMVectorStruct last_point = vector_image_points_.back();

		if (last_point.i != 0) {
			for (size_t i = 0; i < number_of_blanking_points_for_line_start_and_end_; ++i)
				addNewPoint(last_point);
		}

		last_point.i = 0;
		addNewPoint(last_point);
		for (size_t i = 0; i < number_of_blanking_points_for_line_start_and_end_; ++i)
			addNewPoint(last_point);
	}
}

void VectorImageBuilder::replaceLastPoint(JMVectorStruct &point) {
	if (vector_image_points_.empty())
		vector_image_points_.push_back(point);
	else
		vector_image_points_.back() = point;
}

void VectorImageBuilder::removeLastPoint() {
	if (!vector_image_points_.empty())
		vector_image_points_.pop_back();
}

void VectorImageBuilder::addFirstPointBlankingPoints(JMVectorStruct &start_point) {
	JMVectorStruct start_point_off = start_point;
	start_point_off.i = 0;
	addNewPoint(start_point_off);
	for (size_t i = 0; i < number_of_blanking_points_for_line_start_and_end_; ++i)
		addNewPoint(start_point_off);

	addNewPoint(start_point);
	for (size_t i = 0; i < number_of_blanking_points_for_line_start_and_end_; ++i)
		addNewPoint(start_point);
}

void VectorImageBuilder::correctDistortionOnVectorImage() {
	if (projection_model_properties_.getComputeDistancesToImagePlanes()) {
		projection_model_properties_.setDistanceToImagePlaneForConvertingXGalvoAngleToDrawingArea(computeDistanceToImagePlane(projection_model_properties_.getFocalLengthXInPixels(), projection_model_properties_.getImageWidthInPixels()));
		projection_model_properties_.setDistanceToImagePlaneForConvertingYGalvoAngleToDrawingArea(computeDistanceToImagePlane(projection_model_properties_.getFocalLengthYInPixels(), projection_model_properties_.getImageHeightInPixels()));
		projection_model_properties_.setDistanceToImagePlaneForCorrectingDistortion(projection_model_properties_.getDistanceToImagePlaneForConvertingXGalvoAngleToDrawingArea());
	}

	double image_plane_to_drawing_area_x_scale, image_plane_to_drawing_area_y_scale;
	computeScalingFactorsFromImagePlaneToDrawingArea(
		projection_model_properties_.getFocalLengthXInPixels(), projection_model_properties_.getImageWidthInPixels(),
		projection_model_properties_.getFocalLengthYInPixels(), projection_model_properties_.getImageHeightInPixels(),
		projection_model_properties_.getDistanceBetweenMirrors(), projection_model_properties_.getDistanceToImagePlaneForCorrectingDistortion(),
		image_plane_to_drawing_area_x_scale, image_plane_to_drawing_area_y_scale);

	std::vector<JMVectorStruct> vector_image_points_trimmed;

	for (size_t i = 0; i < vector_image_points_.size(); ++i) {
		JMVectorStruct point = vector_image_points_[i];
		double original_x = (double)point.x / drawing_area_to_projector_range_x_scale_;
		double original_y = (double)point.y / drawing_area_to_projector_range_y_scale_;
		double new_x = original_x;
		double new_y = original_y;

		if (projection_model_properties_.getDistanceBetweenMirrors() != 0.0) {
			if (projection_model_properties_.getChangeToPrincipalPointOriginWhenCorrectingGalvanometerDistortion()) {
				new_x = changeFromDrawingAreaOriginToPrincipalPointOrigin(new_x, projection_model_properties_.getImageWidthInPixels(), projection_model_properties_.getPrincipalPointXInPixels());
				new_y = changeFromDrawingAreaOriginToPrincipalPointOrigin(new_y, projection_model_properties_.getImageHeightInPixels(), projection_model_properties_.getPrincipalPointYInPixels());
			}

			correctGalvanometerMirrorsDistortion(new_x, new_y, projection_model_properties_.getDistanceBetweenMirrors(),
				projection_model_properties_.getDistanceToImagePlaneForCorrectingDistortion(),
				projection_model_properties_.getDistanceToImagePlaneForConvertingXGalvoAngleToDrawingArea(),
				projection_model_properties_.getDistanceToImagePlaneForConvertingYGalvoAngleToDrawingArea(),
				projection_model_properties_.getUseRayToPlaneIntersectionForConvertingGalvosAnglesToDrawingArea());

			if (projection_model_properties_.getScaleImagePlanePointsUsingIntrinsics()) {
				new_x *= image_plane_to_drawing_area_x_scale;
				new_y *= image_plane_to_drawing_area_y_scale;
			}

			if (projection_model_properties_.getChangeToPrincipalPointOriginWhenCorrectingGalvanometerDistortion()) {
				new_x = changeFromPrincipalPointOriginToDrawingAreaOrigin(new_x, projection_model_properties_.getImageWidthInPixels(), projection_model_properties_.getPrincipalPointXInPixels());
				new_y = changeFromPrincipalPointOriginToDrawingAreaOrigin(new_y, projection_model_properties_.getImageHeightInPixels(), projection_model_properties_.getPrincipalPointYInPixels());
			}
		}

		if (projection_model_properties_.hasLensDistortionCoefficients()) {
			correctLensDistortion(new_x, new_y, projection_model_properties_);
		}

		bool point_outside_drawing_area = false;
		if (new_x < -projection_model_properties_.getImageWidthInPixels() / 2.0 || new_x > projection_model_properties_.getImageWidthInPixels() / 2.0 ||
			new_y < -projection_model_properties_.getImageHeightInPixels() / 2.0 || new_y > projection_model_properties_.getImageHeightInPixels() / 2.0)
		{
			point_outside_drawing_area = true;
			if (trim_points_outside_drawing_area_)
			{
				if (i == 0)
				{
					trimLineInDrawingAreaAndProjectorOrigin(original_x, original_y, new_x, new_y);
				}
				else
				{
					JMVectorStruct previousPoint = vector_image_points_[i - 1];
					double previousPointX = (double)previousPoint.x / drawing_area_to_projector_range_x_scale_;
					double previousPointY = (double)previousPoint.y / drawing_area_to_projector_range_y_scale_;
					trimLineInDrawingAreaAndProjectorOrigin(previousPointX, previousPointY, new_x, new_y);
				}
			}
		}

		if (!point_outside_drawing_area || (point_outside_drawing_area && trim_points_outside_drawing_area_))
		{
			new_x *= drawing_area_to_projector_range_x_scale_;
			new_y *= drawing_area_to_projector_range_y_scale_;

			point.x = (int32_t)new_x;
			point.y = (int32_t)new_y;

			if (new_x <= minimum_projector_range_value_for_clipping_)
				point.x = minimum_projector_range_value_for_clipping_;
			else if (new_x >= maximum_projector_range_value_for_clipping_)
				point.x = maximum_projector_range_value_for_clipping_;

			if (new_y <= minimum_projector_range_value_for_clipping_)
				point.y = minimum_projector_range_value_for_clipping_;
			else if (new_y >= maximum_projector_range_value_for_clipping_)
				point.y = maximum_projector_range_value_for_clipping_;

			if (trim_points_outside_drawing_area_)
				vector_image_points_[i] = point;
			else
				vector_image_points_trimmed.push_back(point);
		} else {
			if (point.i == 0 && !vector_image_points_trimmed.empty()) {
				JMVectorStruct previousPoint = vector_image_points_trimmed.back();
				previousPoint.i = 0;
				vector_image_points_trimmed.push_back(previousPoint);
			}
		}
	}

	if (!trim_points_outside_drawing_area_)
	{
		vector_image_points_.swap(vector_image_points_trimmed);
	}
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </functions>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <static functions>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
void VectorImageBuilder::correctGalvanometerMirrorsDistortion(double& x, double& y, double distance_between_mirrors, double distance_to_image_plane_for_correcting_distortion, double distance_to_image_plane_for_converting_x_galvo_angle_to_drawing_area, double distance_to_image_plane_for_converting_y_galvo_angle_to_drawing_area, bool use_ray_to_plane_intersection_for_converting_galvos_angles_to_drawing_area) {
	double galvo_x_angle;
	double galvo_y_angle;
	drawingAreaToGalvoAngles(x, y, distance_between_mirrors, distance_to_image_plane_for_correcting_distortion, galvo_x_angle, galvo_y_angle);
	pinHoleAnglesToDrawingArea(galvo_x_angle, galvo_y_angle, distance_to_image_plane_for_converting_x_galvo_angle_to_drawing_area, distance_to_image_plane_for_converting_y_galvo_angle_to_drawing_area, x, y, use_ray_to_plane_intersection_for_converting_galvos_angles_to_drawing_area);

	// Reverse of above
	//drawingAreaToPinHoleAngles(x, y, distance_to_image_plane_for_converting_x_galvo_angle_to_drawing_area, distance_to_image_plane_for_converting_y_galvo_angle_to_drawing_area, galvo_x_angle, galvo_y_angle);
	//galvoAnglesToDrawingArea(galvo_x_angle, galvo_y_angle, distance_between_mirrors, distance_to_image_plane_for_correcting_distortion, x, y, use_ray_to_plane_intersection_for_converting_galvos_angles_to_drawing_area);
}

void VectorImageBuilder::drawingAreaToGalvoAngles(double x, double y, double distance_between_mirrors, double distance_to_image_plane, double& galvo_x_angle, double& galvo_y_angle) {
	double yDenominator = distance_to_image_plane - distance_between_mirrors;
	galvo_y_angle = std::atan(y / yDenominator);
	double xDenominator = distance_between_mirrors * (((distance_to_image_plane - distance_between_mirrors) / (std::cos(galvo_y_angle) * distance_between_mirrors)) + 1);
	galvo_x_angle = std::atan(x / xDenominator);
}

void VectorImageBuilder::galvoAnglesToDrawingArea(double galvo_x_angle, double galvo_y_angle, double distance_between_mirrors, double distance_to_image_plane, double& x, double& y) {
	x = std::tan(galvo_x_angle) * distance_between_mirrors * (((distance_to_image_plane - distance_between_mirrors) / (std::cos(galvo_y_angle) * distance_between_mirrors)) + 1);
	y = std::tan(galvo_y_angle) * (distance_to_image_plane - distance_between_mirrors);
}

void VectorImageBuilder::drawingAreaToPinHoleAngles(double x, double y, double distance_to_image_plane_for_x, double distance_to_image_plane_for_y, double& xAngle, double& yAngle, bool use_ray_to_plane_intersection_for_converting_galvos_angles_to_drawing_area) {
	if (use_ray_to_plane_intersection_for_converting_galvos_angles_to_drawing_area) {
		xAngle = std::atan(x / distance_to_image_plane_for_x);
		yAngle = std::atan(y / distance_to_image_plane_for_y);
	} else {
		xAngle = std::asin(x / distance_to_image_plane_for_x);
		yAngle = std::asin(y / distance_to_image_plane_for_y);
	}
}

void VectorImageBuilder::pinHoleAnglesToDrawingArea(double galvo_x_angle, double galvo_y_angle, double distance_to_image_plane_for_updating_new_x, double distance_to_image_plane_for_updating_new_y, double& x, double& y, bool use_ray_to_plane_intersection_for_converting_galvos_angles_to_drawing_area) {
	// Pinhole angles to screen (TODO: improve conversion because galvos do not have a center of projection)
	if (use_ray_to_plane_intersection_for_converting_galvos_angles_to_drawing_area) {
		x = std::tan(galvo_x_angle) * distance_to_image_plane_for_updating_new_x;
		y = std::tan(galvo_y_angle) * distance_to_image_plane_for_updating_new_y;
	} else {
		x = std::sin(galvo_x_angle) * distance_to_image_plane_for_updating_new_x;
		y = std::sin(galvo_y_angle) * distance_to_image_plane_for_updating_new_y;
	}
	
}

double VectorImageBuilder::computeDistanceToImagePlane(double focal_length_in_pixels, double image_size_in_pixels) {
	double fov = computeFieldOfView(focal_length_in_pixels, image_size_in_pixels);
	return ((image_size_in_pixels / 2.0) / std::tan(fov / 2));
}

double VectorImageBuilder::computeFieldOfView(double focal_length_in_pixels, double image_size_in_pixels) {
	return std::atan(image_size_in_pixels / (focal_length_in_pixels * 2.0)) * 2.0;
}

void VectorImageBuilder::computeScalingFactorsFromImagePlaneToDrawingArea(double focal_length_in_pixels_x, double image_size_in_pixels_x, double focal_length_in_pixels_y, double image_size_in_pixels_y, double distance_between_mirrors, double distance_to_image_plane, double& x_scale, double& y_scale) {
	double fovX = computeFieldOfView(focal_length_in_pixels_x, image_size_in_pixels_x);
	double fovY = computeFieldOfView(focal_length_in_pixels_y, image_size_in_pixels_y);
	double x, y;
	galvoAnglesToDrawingArea(fovX / 2.0, fovY / 2.0, distance_between_mirrors, distance_to_image_plane, x, y);
	x_scale = image_size_in_pixels_x / x;
	y_scale = image_size_in_pixels_y / y;
}

double VectorImageBuilder::changeFromDrawingAreaOriginToPrincipalPointOrigin(double drawing_area_value, double image_size, double principal_point) {
	return (drawing_area_value + (image_size / 2.0)) - principal_point;
}

double VectorImageBuilder::changeFromPrincipalPointOriginToDrawingAreaOrigin(double drawing_area_value, double image_size, double principal_point) {
	return (drawing_area_value - (image_size / 2.0)) + principal_point;
}

void VectorImageBuilder::correctLensDistortion(double& x, double& y, const ProjectionModelProperties& projection_model_properties)
{
	double normalizedX = changeFromDrawingAreaOriginToPrincipalPointOrigin(x, projection_model_properties.getImageWidthInPixels(), projection_model_properties.getPrincipalPointXInPixels()) / projection_model_properties.getFocalLengthXInPixels();
	double normalizedY = changeFromDrawingAreaOriginToPrincipalPointOrigin(y, projection_model_properties.getImageHeightInPixels(), projection_model_properties.getPrincipalPointYInPixels()) / projection_model_properties.getFocalLengthYInPixels();
	double distanceSquared = normalizedX * normalizedX + normalizedY * normalizedY;

	double radialDistortionCorrectionScalingFactor = (1 +
		projection_model_properties.getRadialDistortionCorrectionFirstCoefficient() * distanceSquared +
		projection_model_properties.getRadialDistortionCorrectionSecondCoefficient() * distanceSquared * distanceSquared +
		projection_model_properties.getRadialDistortionCorrectionThirdCoefficient() * distanceSquared * distanceSquared * distanceSquared);

	double tangentialDistortionCorrectionOffsetFactorX = 2 * projection_model_properties.getTangentialDistortionCorrectionFirstCoefficient() * normalizedX * normalizedY +
		projection_model_properties.getTangentialDistortionCorrectionSecondCoefficient() * (distanceSquared + 2 * normalizedX * normalizedX);
	double tangentialDistortionCorrectionOffsetFactorY = projection_model_properties.getTangentialDistortionCorrectionFirstCoefficient() * (distanceSquared + 2 * normalizedY * normalizedY) +
		2 * projection_model_properties.getTangentialDistortionCorrectionSecondCoefficient() * normalizedX * normalizedY;

	double normalizedXUndistorted = normalizedX * radialDistortionCorrectionScalingFactor + tangentialDistortionCorrectionOffsetFactorX;
	double normalizedYUndistorted = normalizedY * radialDistortionCorrectionScalingFactor + tangentialDistortionCorrectionOffsetFactorY;

	x = changeFromPrincipalPointOriginToDrawingAreaOrigin(normalizedXUndistorted * projection_model_properties.getFocalLengthXInPixels(), projection_model_properties.getImageWidthInPixels(), projection_model_properties.getPrincipalPointXInPixels());
	y = changeFromPrincipalPointOriginToDrawingAreaOrigin(normalizedYUndistorted * projection_model_properties.getFocalLengthYInPixels(), projection_model_properties.getImageHeightInPixels(), projection_model_properties.getPrincipalPointYInPixels());
}

bool VectorImageBuilder::lineIntersection(double p0_x, double p0_y, double p1_x, double p1_y,
                                          double p2_x, double p2_y, double p3_x, double p3_y,
                                          double &i_x, double &i_y, double comparison_epsilon) {
	double s02_x, s02_y, s10_x, s10_y, s32_x, s32_y, t_numerator, denominator, t;
	s10_x = p1_x - p0_x;
	s10_y = p1_y - p0_y;
	s32_x = p3_x - p2_x;
	s32_y = p3_y - p2_y;

	denominator = s10_x * s32_y - s32_x * s10_y;
	if (std::abs(denominator) < comparison_epsilon)
		return false; // Collinear

	s02_x = p0_x - p2_x;
	s02_y = p0_y - p2_y;

	t_numerator = s32_x * s02_y - s32_y * s02_x;

	// Collision detected
	t = t_numerator / denominator;
	i_x = p0_x + (t * s10_x);
	i_y = p0_y + (t * s10_y);

	return true;
}

double VectorImageBuilder::linearInterpolation(double a, double b, double t) {
	return a * (1 - t) + b * t;
}

double VectorImageBuilder::jmVectorStructDistanceSquared(const JMVectorStruct& first, const JMVectorStruct& second) {
	return  std::pow((double)second.x - (double)first.x, 2.0) + std::pow((double)second.y - (double)first.y, 2.0);
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <static functions/>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ===============================================================================  </public-section>   ===========================================================================
}
