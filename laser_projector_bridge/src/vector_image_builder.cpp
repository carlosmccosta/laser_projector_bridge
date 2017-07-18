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
		drawing_area_width_(2000.0),
		drawing_area_height_(2000.0),
		drawing_area_x_focal_length_in_pixels_(2750.0),
		drawing_area_y_focal_length_in_pixels_(2750.0),
		distance_between_mirrors_in_projector_range_percentage_(0.01),
		minimum_projector_range_value_for_clipping_((int32_t)((double)std::numeric_limits<int32_t>::min() * 0.98)),
		maximum_projector_range_value_for_clipping_((int32_t)((double)std::numeric_limits<int32_t>::max() * 0.98)),
		line_first_point_merge_distance_squared_in_projector_range_(std::pow((double)std::numeric_limits<uint32_t>::max() * 0.0005, 2.0)),
		line_first_point_ignore_distance_squared_in_projector_range_(std::pow((double)std::numeric_limits<uint32_t>::max() * 0.001, 2.0)),
		interpolation_distance_in_projector_range_((int64_t)((double)std::numeric_limits<uint32_t>::max() * 0.002)),
		number_of_blanking_points_for_line_start_and_end_(2),
		maximum_number_of_points_(16000),
		drawing_area_x_offset_(0.0),
		drawing_area_y_offset_(0.0),
		drawing_area_to_projector_range_x_scale_((double)std::numeric_limits<uint32_t>::max() / 2000.0),
		drawing_area_to_projector_range_y_scale_((double)std::numeric_limits<uint32_t>::max() / 2000.0)
		{}

VectorImageBuilder::~VectorImageBuilder() {}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </constructors-destructor>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <functions>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
void VectorImageBuilder::startNewVectorImage() {
	vector_image_points_.clear();
	drawing_area_to_projector_range_x_scale_ = ((double)std::numeric_limits<uint32_t>::max()) / drawing_area_width_;
	drawing_area_to_projector_range_y_scale_ = ((double)std::numeric_limits<uint32_t>::max()) / drawing_area_height_;
}

void VectorImageBuilder::finishVectorImage() {
	if (!vector_image_points_.empty()) {
		if (vector_image_points_.back().i != 0) {
			JMVectorStruct last_point_off = vector_image_points_.back();
			last_point_off.i = 0;
			vector_image_points_.push_back(last_point_off);

			for (size_t i = 0; i < number_of_blanking_points_for_line_start_and_end_; ++i)
				addNewPoint(last_point_off);
		}
		correctRadialDistortionOnVectorImage();
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


bool VectorImageBuilder::convertPointFromDrawingAreaToProjectorOrigin(double x, double y, double &x_point_in_drawing_area_and_projector_origin, double &y_point_in_drawing_area_and_projector_origin, AxisPosition origin_axis_position) {
	switch (origin_axis_position) {
		case AxisPosition::TopLeft: {
			x_point_in_drawing_area_and_projector_origin = x - drawing_area_width_ * 0.5;
			y_point_in_drawing_area_and_projector_origin = (drawing_area_height_ - y) - drawing_area_height_ * 0.5;
			break;
		}

		case AxisPosition::BottomLeft: {
			x_point_in_drawing_area_and_projector_origin = x - drawing_area_width_ * 0.5;
			y_point_in_drawing_area_and_projector_origin = y - drawing_area_height_ * 0.5;
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


bool VectorImageBuilder::convertProjectorOriginToDrawingAreaOrigin(double x, double y, double &new_x, double &new_y, AxisPosition drawing_area_origin_axis_position) {
	switch (drawing_area_origin_axis_position) {
		case AxisPosition::TopLeft: {
			new_x = x + drawing_area_width_ * 0.5;
			new_y = (drawing_area_height_ - y) + drawing_area_height_ * 0.5;
			break;
		}

		case AxisPosition::BottomLeft: {
			new_x = x + drawing_area_width_ * 0.5;
			new_y = y + drawing_area_height_ * 0.5;
			break;
		}

		case AxisPosition::Middle: {
			new_x = x;
			new_y = y;
			break;
		}

		default: {
			new_x = 0.0;
			new_y = 0.0;
			return false;
		}
	}

	return true;
}


bool VectorImageBuilder::convertPointFromDrawingAreaInProjectorOriginToProjectorRange(double x_point_in_drawing_area_and_projector_origin, double y_point_in_drawing_area_and_projector_origin, int32_t &x_point_in_projector_range,
                                                                                      int32_t &y_point_in_projector_range) {
	bool point_overflow = false;

	double x_with_offset = x_point_in_drawing_area_and_projector_origin - drawing_area_x_offset_;
	if (x_with_offset < drawing_area_width_ * -0.5) {
		x_point_in_projector_range = minimum_projector_range_value_for_clipping_;
		point_overflow = true;
	} else if (x_with_offset > drawing_area_width_ * 0.5) {
		x_point_in_projector_range = maximum_projector_range_value_for_clipping_;
		point_overflow = true;
	} else {
		x_point_in_projector_range = (int32_t)((x_point_in_drawing_area_and_projector_origin - drawing_area_x_offset_) * drawing_area_to_projector_range_x_scale_);
	}

	double y_with_offset = y_point_in_drawing_area_and_projector_origin - drawing_area_y_offset_;
	if (y_with_offset < drawing_area_height_ * -0.5) {
		y_point_in_projector_range = minimum_projector_range_value_for_clipping_;
		point_overflow = true;
	} else if (x_with_offset > drawing_area_height_ * 0.5) {
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
	return convertProjectorOriginToDrawingAreaOrigin(x_point_in_drawing_area_and_projector_origin, y_point_in_drawing_area_and_projector_origin, x_point_in_drawing_area_range, y_point_in_drawing_area_range, origin_axis_position);
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

	double half_width = drawing_area_width_ * 0.5;
	double half_height = drawing_area_height_ * 0.5;
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
	double half_width = drawing_area_width_ * 0.5;
	double half_height = drawing_area_height_ * 0.5;
	return (x >= -half_width && x <= half_width && y >= -half_height && y <= half_height);
}

bool VectorImageBuilder::addNewLine(double start_x, double start_y, double end_x, double end_y,
                                    uint16_t red, uint16_t green, uint16_t blue,
                                    uint16_t intensity, AxisPosition origin_axis_position) {
	double start_x_in_projector_origin;
	double start_y_in_projector_origin;
	double end_x_in_projector_origin;
	double end_y_in_projector_origin;

	if (!convertPointFromDrawingAreaToProjectorOrigin(start_x, start_y, start_x_in_projector_origin, start_y_in_projector_origin, origin_axis_position)) { return false; }
	if (!convertPointFromDrawingAreaToProjectorOrigin(end_x, end_y, end_x_in_projector_origin, end_y_in_projector_origin, origin_axis_position)) { return false; }

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
		if (distance_to_last_point_squared >= line_first_point_ignore_distance_squared_in_projector_range_ ||
		    last_point.i != start_point.i || last_point.r != start_point.r || last_point.g != start_point.g ||
		    last_point.b != start_point.b ||
		    last_point.cyan != start_point.cyan || last_point.deepblue != start_point.deepblue ||
		    last_point.yellow != start_point.yellow || last_point.user4 != start_point.user4) {
			if (last_point.i != 0) {
				last_point.i = 0;
				addNewPoint(last_point);
			}

			JMVectorStruct last_point_off = last_point;
			last_point_off.i = 0;
			for (size_t i = 0; i < number_of_blanking_points_for_line_start_and_end_; ++i)
				addNewPoint(last_point_off);

			JMVectorStruct start_point_off = start_point;
			start_point_off.i = 0;
			addNewPoint(start_point_off);
			for (size_t i = 0; i < number_of_blanking_points_for_line_start_and_end_; ++i)
				addNewPoint(start_point_off);

			addNewPoint(start_point);
		} else {
			if (distance_to_last_point_squared > 0 && distance_to_last_point_squared < line_first_point_merge_distance_squared_in_projector_range_) {
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
		JMVectorStruct start_point_off = start_point;
		start_point_off.i = 0;
		addNewPoint(start_point_off);

		for (size_t i = 0; i < number_of_blanking_points_for_line_start_and_end_; ++i)
			addNewPoint(start_point_off);

		addNewPoint(start_point);
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
	if (convertPointFromDrawingAreaToProjectorOrigin(x, y, x_in_projector_origin, y_in_projector_origin, origin_axis_position) &&
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
		if (vector_image_points_.empty()) {
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
		JMVectorStruct last_point_off = vector_image_points_.back();
		last_point_off.i = 0;
		for (size_t i = 0; i < number_of_blanking_points_for_line_start_and_end_; ++i)
			addNewPoint(last_point_off);
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

void VectorImageBuilder::correctRadialDistortionOnVectorImage() {
	double distance_to_x_image_plane = computeDistanceToImagePlane(drawing_area_x_focal_length_in_pixels_, drawing_area_width_, (double)std::numeric_limits<uint32_t>::max());
	double distance_to_y_image_plane = computeDistanceToImagePlane(drawing_area_y_focal_length_in_pixels_, drawing_area_height_, (double)std::numeric_limits<uint32_t>::max());

	for (size_t i = 0; i < vector_image_points_.size(); ++i) {
		correctRadialDistortion(vector_image_points_[i], distance_to_x_image_plane, distance_to_y_image_plane, distance_between_mirrors_in_projector_range_percentage_ * std::numeric_limits<uint32_t>::max());
	}
}

void VectorImageBuilder::correctRadialDistortion(JMVectorStruct &point, double distance_to_x_image_plane, double distance_to_y_image_plane, double distance_between_mirrors) {
	if (point.x <= minimum_projector_range_value_for_clipping_)
		point.x = minimum_projector_range_value_for_clipping_;
	else if (point.x >= maximum_projector_range_value_for_clipping_)
		point.x = maximum_projector_range_value_for_clipping_;

	if (point.y <= minimum_projector_range_value_for_clipping_)
		point.y = minimum_projector_range_value_for_clipping_;
	else if (point.y >= maximum_projector_range_value_for_clipping_)
		point.y = maximum_projector_range_value_for_clipping_;

	double y_denominator = distance_between_mirrors - distance_to_y_image_plane;
	double galvo_y_angle = std::atan((double)point.y / y_denominator);
	double x_denominator = distance_between_mirrors * (((distance_to_x_image_plane - distance_between_mirrors) / (distance_between_mirrors * std::cos(galvo_y_angle))) + 1);
	double galvo_x_angle = std::atan((double)point.x / x_denominator);
	double new_y = -(std::tan(galvo_y_angle) * distance_to_y_image_plane);
	double new_x = (std::tan(galvo_x_angle) * distance_to_x_image_plane);

	bool underflowX = (point.x < 0 && new_x > 0);
	bool underflowY = (point.y < 0 && new_y > 0);
	bool overflowX = (point.x > 0 && new_x < 0);
	bool overflowY = (point.y > 0 && new_y < 0);

	if (!((underflowX || overflowX) && std::abs(point.x) > (double)std::numeric_limits<int32_t>::max() * 0.5))
		point.x = static_cast<int32_t>(new_x);

	if (!((underflowY || overflowY) && std::abs(point.y) > (double)std::numeric_limits<int32_t>::max() * 0.5))
		point.y = static_cast<int32_t>(new_y);
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </functions>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <static functions>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
double VectorImageBuilder::computeDistanceToImagePlane(double focalLengthInPixels, double imageSizeInPixels, double projectorRange) {
	double fov = std::atan(imageSizeInPixels / focalLengthInPixels / 2.0) * 2.0;
	return (projectorRange / 2.0 / std::tan(fov / 2));
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
