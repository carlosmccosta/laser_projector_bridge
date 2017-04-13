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
		DrawingAreaWidth(2000.0),
		DrawingAreaHeight(2000.0),
		DrawingAreaXOffset(0.0),
		DrawingAreaYOffset(0.0),
		_drawingAreaToProjectorRangeXScale((double)std::numeric_limits<uint32_t>::max() / 2000.0),
		_drawingAreaToProjectorRangeYScale((double)std::numeric_limits<uint32_t>::max() / 2000.0),
		RadialDistortionCoefficientSecondDegreeInvertedUV(0.08),
		RadialDistortionCoefficientSecondDegree(-0.044),
		RadialDistortionCoefficientFourthDegree(-0.007),
		RadialDistortionCoefficientSixthDegree(-0.005),
		LineFirstPointMergeDistanceSquaredInProjectorRange(std::pow((double)std::numeric_limits<uint32_t>::max() * 0.0005, 2.0)),
		LineFirstPointIgnoreDistanceSquaredInProjectorRange(std::pow((double)std::numeric_limits<uint32_t>::max() * 0.0001, 2.0)),
		InterpolationDistanceInProjectorRange((int32_t)((double)std::numeric_limits<uint32_t>::max() * 0.0002))
		{}

VectorImageBuilder::~VectorImageBuilder() {}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </constructors-destructor>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <functions>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
void VectorImageBuilder::StartNewVectorImage() {
	VectorImagePoints.clear();
	_drawingAreaToProjectorRangeXScale = ((double)std::numeric_limits<uint32_t>::max()) / DrawingAreaWidth;
	_drawingAreaToProjectorRangeYScale = ((double)std::numeric_limits<uint32_t>::max()) / DrawingAreaHeight;
}

void VectorImageBuilder::FinishVectorImage() {
	if (!VectorImagePoints.empty()) {
		JMVectorStruct& lastPoint = VectorImagePoints.back();
		if (lastPoint.i != 0) {
			lastPoint.i = 0;
			VectorImagePoints.push_back(lastPoint);
		}
		CorrectRadialDistortionOnVectorImage();
		// todo: laser path optimization and blanking
	}
}

bool VectorImageBuilder::ConvertProjectorOriginToDrawingAreaOrigin(double x, double y, double& newX, double& newY, AxisPosition drawingAreaOriginAxisPosition) {
	switch (drawingAreaOriginAxisPosition) {
		case AxisPosition::TopLeft: {
			newX = x + DrawingAreaWidth * 0.5;
			newY = (DrawingAreaHeight - y) + DrawingAreaHeight * 0.5;
			break;
		}

		case AxisPosition::BottomLeft: {
			newX = x + DrawingAreaWidth * 0.5;
			newY = y + DrawingAreaHeight * 0.5;
			break;
		}

		case AxisPosition::Middle: {
			newX = x;
			newY = y;
			break;
		}

		default: {
			newX = 0.0;
			newY = 0.0;
			return false;
		}
	}

	return true;
}

bool VectorImageBuilder::ConvertPointFromDrawingAreaToProjectorOrigin(double x, double y, double& xPointInDrawingAreaAndProjectorOrigin, double& yPointInDrawingAreaAndProjectorOrigin, AxisPosition originAxisPosition) {
	switch (originAxisPosition) {
		case AxisPosition::TopLeft: {
			xPointInDrawingAreaAndProjectorOrigin = x - DrawingAreaWidth * 0.5;
			yPointInDrawingAreaAndProjectorOrigin = (DrawingAreaHeight - y) - DrawingAreaHeight * 0.5;
			break;
		}

		case AxisPosition::BottomLeft: {
			xPointInDrawingAreaAndProjectorOrigin = x - DrawingAreaWidth * 0.5;
			yPointInDrawingAreaAndProjectorOrigin = y - DrawingAreaHeight * 0.5;
			break;
		}

		case AxisPosition::Middle: {
			xPointInDrawingAreaAndProjectorOrigin = x;
			yPointInDrawingAreaAndProjectorOrigin = y;
			break;
		}

		default: {
			xPointInDrawingAreaAndProjectorOrigin = 0.0;
			yPointInDrawingAreaAndProjectorOrigin = 0.0;
			return false;
		}
	}

	return true;
}

bool VectorImageBuilder::ConvertPointFromDrawingAreaInProjectorOriginToProjectorRange(double xPointInDrawingAreaAndProjectorOrigin, double yPointInDrawingAreaAndProjectorOrigin, int32_t& xPointInProjectorRange, int32_t& yPointInProjectorRange) {
	bool pointOverflow = false;

	double xWithOffset = xPointInDrawingAreaAndProjectorOrigin - DrawingAreaXOffset;
	if (xWithOffset < DrawingAreaWidth * -0.5) {
		xPointInProjectorRange = std::numeric_limits<int32_t >::min();
		pointOverflow = true;
	} else if (xWithOffset > DrawingAreaWidth * 0.5) {
		xPointInProjectorRange = std::numeric_limits<int32_t >::max();
		pointOverflow = true;
	} else {
		xPointInProjectorRange = (int32_t)((xPointInDrawingAreaAndProjectorOrigin - DrawingAreaXOffset) * _drawingAreaToProjectorRangeXScale);
	}

	double yWithOffset = yPointInDrawingAreaAndProjectorOrigin - DrawingAreaYOffset;
	if (yWithOffset < DrawingAreaHeight * -0.5) {
		yPointInProjectorRange = std::numeric_limits<int32_t >::min();
		pointOverflow = true;
	} else if (xWithOffset > DrawingAreaHeight * 0.5) {
		yPointInProjectorRange = std::numeric_limits<int32_t >::max();
		pointOverflow = true;
	} else {
		yPointInProjectorRange = (int32_t)((yPointInDrawingAreaAndProjectorOrigin - DrawingAreaYOffset) * _drawingAreaToProjectorRangeYScale);
	}

	return !pointOverflow;
}

bool VectorImageBuilder::ConvertPointFromProjectorRangeToDrawingArea(int32_t xPointInProjectorRange, int32_t yPointInProjectorRange, double& xPointInDrawingAreaRange, double& yPointInDrawingAreaRange, AxisPosition originAxisPosition) {
	double xPointInDrawingAreaAndProjectorOrigin = ((double)xPointInProjectorRange / _drawingAreaToProjectorRangeXScale) + DrawingAreaXOffset;
	double yPointInDrawingAreaAndProjectorOrigin = ((double)yPointInProjectorRange / _drawingAreaToProjectorRangeYScale) + DrawingAreaYOffset;
	return ConvertProjectorOriginToDrawingAreaOrigin(xPointInDrawingAreaAndProjectorOrigin, yPointInDrawingAreaAndProjectorOrigin, xPointInDrawingAreaRange, yPointInDrawingAreaRange, originAxisPosition);
}

bool VectorImageBuilder::TrimLineInDrawingAreaAndProjectorOrigin(double& startXInProjectorOrigin, double& startYInProjectorOrigin, double& endXInProjectorOrigin, double& endYInProjectorOrigin) {
	bool startPointValid = IsPointInProjectorOriginWithinDrawingArea(startXInProjectorOrigin, startYInProjectorOrigin);
	bool endPointValid = IsPointInProjectorOriginWithinDrawingArea(endXInProjectorOrigin, endYInProjectorOrigin);
	if (startPointValid && endPointValid) return true;
	if (!startPointValid && !endPointValid) return false;

	double validPointX, validPointY, invalidPointX, invalidPointY;
	if (!startPointValid) {
		invalidPointX = startXInProjectorOrigin;
		invalidPointY = startYInProjectorOrigin;
		validPointX = endXInProjectorOrigin;
		validPointY = endYInProjectorOrigin;
	} else {
		invalidPointX = endXInProjectorOrigin;
		invalidPointY = endYInProjectorOrigin;
		validPointX = startXInProjectorOrigin;
		validPointY = startYInProjectorOrigin;
	}

	double halfWidth = DrawingAreaWidth * 0.5;
	double halfHeight = DrawingAreaHeight * 0.5;
	double xIntersection = 0.0, yIntersection = 0.0;

	// left
	if (invalidPointX < -halfWidth && invalidPointY >= -halfHeight && invalidPointY <= halfHeight) {
		if (LineIntersection(-halfWidth, -halfHeight, -halfWidth, halfHeight, startXInProjectorOrigin, startYInProjectorOrigin, endXInProjectorOrigin, endYInProjectorOrigin, xIntersection, yIntersection))
			xIntersection = -halfWidth;
		else
			return false;
	}

	// right
	if (invalidPointX > halfWidth && invalidPointY >= -halfHeight && invalidPointY <= halfHeight) {
		if (LineIntersection(halfWidth, -halfHeight, halfWidth, halfHeight, startXInProjectorOrigin, startYInProjectorOrigin, endXInProjectorOrigin, endYInProjectorOrigin, xIntersection, yIntersection))
			xIntersection = halfWidth;
		else
			return false;
	}

	// top
	if (invalidPointY > halfHeight && invalidPointX >= -halfWidth && invalidPointX <= halfWidth) {
		if (LineIntersection(-halfWidth, halfHeight, halfWidth, halfHeight, startXInProjectorOrigin, startYInProjectorOrigin, endXInProjectorOrigin, endYInProjectorOrigin, xIntersection, yIntersection))
			yIntersection = halfHeight;
		else
			return false;
	}

	// bottom
	if (invalidPointY < -halfHeight && invalidPointX >= -halfWidth && invalidPointX <= halfWidth) {
		if (LineIntersection(-halfWidth, -halfHeight, halfWidth, -halfHeight, startXInProjectorOrigin, startYInProjectorOrigin, endXInProjectorOrigin, endYInProjectorOrigin, xIntersection, yIntersection))
			yIntersection = -halfHeight;
		else
			return false;
	}

	// bottom left
	if (invalidPointX < -halfWidth && invalidPointY < -halfHeight) {
		double xIntersectionBottom = 0.0, yIntersectionBottom = 0.0;
		double xIntersectionLeft = 0.0, yIntersectionLeft = 0.0;
		if (LineIntersection(-halfWidth, -halfHeight, halfWidth, -halfHeight, startXInProjectorOrigin, startYInProjectorOrigin, endXInProjectorOrigin, endYInProjectorOrigin, xIntersectionBottom, yIntersectionBottom) &&
			LineIntersection(-halfWidth, -halfHeight, -halfWidth, halfHeight, startXInProjectorOrigin, startYInProjectorOrigin, endXInProjectorOrigin, endYInProjectorOrigin, xIntersectionLeft, yIntersectionLeft)) {
			double distanceToBottomIntersection = std::pow(xIntersectionBottom - validPointX, 2.0) + std::pow(yIntersectionBottom - validPointY, 2.0);
			double distanceToLeftIntersection = std::pow(xIntersectionLeft - validPointX, 2.0) + std::pow(yIntersectionLeft - validPointY, 2.0);

			if (distanceToBottomIntersection < distanceToLeftIntersection) {
				xIntersection = xIntersectionBottom;
				yIntersection = -halfHeight;
			} else {
				xIntersection = -halfWidth;
				yIntersection = yIntersectionLeft;
			}
		} else {
			return false;
		}
	}

	// top left
	if (invalidPointX < -halfWidth && invalidPointY > halfHeight) {
		double xIntersectionTop = 0.0, yIntersectionTop = 0.0;
		double xIntersectionLeft = 0.0, yIntersectionLeft = 0.0;
		if (LineIntersection(-halfWidth, halfHeight, halfWidth, halfHeight, startXInProjectorOrigin, startYInProjectorOrigin, endXInProjectorOrigin, endYInProjectorOrigin, xIntersectionTop, yIntersectionTop) &&
			LineIntersection(-halfWidth, -halfHeight, -halfWidth, halfHeight, startXInProjectorOrigin, startYInProjectorOrigin, endXInProjectorOrigin, endYInProjectorOrigin, xIntersectionLeft, yIntersectionLeft)) {
			double distanceToTopIntersection = std::pow(xIntersectionTop - validPointX, 2.0) + std::pow(yIntersectionTop - validPointY, 2.0);
			double distanceToLeftIntersection = std::pow(xIntersectionLeft - validPointX, 2.0) + std::pow(yIntersectionLeft - validPointY, 2.0);

			if (distanceToTopIntersection < distanceToLeftIntersection) {
				xIntersection = xIntersectionTop;
				yIntersection = halfHeight;
			} else {
				xIntersection = -halfWidth;
				yIntersection = yIntersectionLeft;
			}
		} else {
			return false;
		}
	}

	// bottom right
	if (invalidPointX > halfWidth && invalidPointY < -halfHeight) {
		double xIntersectionBottom = 0.0, yIntersectionBottom = 0.0;
		double xIntersectionRight = 0.0, yIntersectionRight = 0.0;
		if (LineIntersection(-halfWidth, -halfHeight, halfWidth, -halfHeight, startXInProjectorOrigin, startYInProjectorOrigin, endXInProjectorOrigin, endYInProjectorOrigin, xIntersectionBottom, yIntersectionBottom) &&
			LineIntersection(halfWidth, -halfHeight, halfWidth, halfHeight, startXInProjectorOrigin, startYInProjectorOrigin, endXInProjectorOrigin, endYInProjectorOrigin, xIntersectionRight, yIntersectionRight)) {
			double distanceToBottomIntersection = std::pow(xIntersectionBottom - validPointX, 2.0) + std::pow(yIntersectionBottom - validPointY, 2.0);
			double distanceToRightIntersection = std::pow(xIntersectionRight - validPointX, 2.0) + std::pow(yIntersectionRight - validPointY, 2.0);

			if (distanceToBottomIntersection < distanceToRightIntersection) {
				xIntersection = xIntersectionBottom;
				yIntersection = -halfHeight;
			} else {
				xIntersection = halfWidth;
				yIntersection = yIntersectionRight;
			}
		} else {
			return false;
		}
	}

	// top right
	if (invalidPointX > halfWidth && invalidPointY > halfHeight) {
		double xIntersectionTop = 0.0, yIntersectionTop = 0.0;
		double xIntersectionRight = 0.0, yIntersectionRight = 0.0;
		if (LineIntersection(-halfWidth, halfHeight, halfWidth, halfHeight, startXInProjectorOrigin, startYInProjectorOrigin, endXInProjectorOrigin, endYInProjectorOrigin, xIntersectionTop, yIntersectionTop) &&
			LineIntersection(halfWidth, -halfHeight, halfWidth, halfHeight, startXInProjectorOrigin, startYInProjectorOrigin, endXInProjectorOrigin, endYInProjectorOrigin, xIntersectionRight, yIntersectionRight)) {
			double distanceToTopIntersection = std::pow(xIntersectionTop - validPointX, 2.0) + std::pow(yIntersectionTop - validPointY, 2.0);
			double distanceToRightIntersection = std::pow(xIntersectionRight - validPointX, 2.0) + std::pow(yIntersectionRight - validPointY, 2.0);

			if (distanceToTopIntersection < distanceToRightIntersection) {
				xIntersection = xIntersectionTop;
				yIntersection = halfHeight;
			} else {
				xIntersection = halfWidth;
				yIntersection = yIntersectionRight;
			}
		} else {
			return false;
		}
	}

	if (!startPointValid) {
		startXInProjectorOrigin = xIntersection;
		startYInProjectorOrigin = yIntersection;
	} else {
		endXInProjectorOrigin = xIntersection;
		endYInProjectorOrigin = yIntersection;
	}

	return true;
}

bool VectorImageBuilder::IsPointInProjectorOriginWithinDrawingArea(double x, double y) {
	double halfWidth = DrawingAreaWidth * 0.5;
	double halfHeight = DrawingAreaHeight * 0.5;
	if (x >= -halfWidth && x <= halfWidth && y >= -halfHeight && y <= halfHeight)
		return true;
	else
		return false;
}

bool VectorImageBuilder::AddNewLine(double startX, double startY, double endX, double endY,
                uint16_t red, uint16_t green, uint16_t blue,
                uint16_t intensity, AxisPosition originAxisPosition) {
	double startXInProjectorOrigin;
	double startYInProjectorOrigin;
	double endXInProjectorOrigin;
	double endYInProjectorOrigin;

	if (!ConvertPointFromDrawingAreaToProjectorOrigin(startX, startY, startXInProjectorOrigin, startYInProjectorOrigin, originAxisPosition)) { return false; }
	if (!ConvertPointFromDrawingAreaToProjectorOrigin(endX, endY, endXInProjectorOrigin, endYInProjectorOrigin, originAxisPosition)) { return false; }

	if (!TrimLineInDrawingAreaAndProjectorOrigin(startXInProjectorOrigin, startYInProjectorOrigin, endXInProjectorOrigin, endYInProjectorOrigin)) { return false; }

	int32_t startXInProjectorRange;
	int32_t startYInProjectorRange;
	int32_t endXInProjectorRange;
	int32_t endYInProjectorRange;

	ConvertPointFromDrawingAreaInProjectorOriginToProjectorRange(startXInProjectorOrigin, startYInProjectorOrigin, startXInProjectorRange, startYInProjectorRange);
	ConvertPointFromDrawingAreaInProjectorOriginToProjectorRange(endXInProjectorOrigin, endYInProjectorOrigin, endXInProjectorRange, endYInProjectorRange);

	AddNewLine(startXInProjectorRange, startYInProjectorRange, endXInProjectorRange, endYInProjectorRange, red, green, blue, intensity);
	return true;
}

void VectorImageBuilder::AddNewLine(int32_t startX, int32_t startY, int32_t endX, int32_t endY,
                uint16_t red, uint16_t green, uint16_t blue, uint16_t intensity) {
	JMVectorStruct startPoint;
	startPoint.x = startX;
	startPoint.y = startY;
	startPoint.r = red;
	startPoint.g = green;
	startPoint.b = blue;
	startPoint.i = intensity;
	startPoint.deepblue = std::numeric_limits<uint16_t>::max();
	startPoint.yellow = std::numeric_limits<uint16_t>::max();
	startPoint.cyan = std::numeric_limits<uint16_t>::max();
	startPoint.user4 = std::numeric_limits<uint16_t>::max();

	JMVectorStruct endPoint;
	endPoint.x = endX;
	endPoint.y = endY;
	endPoint.r = red;
	endPoint.g = green;
	endPoint.b = blue;
	endPoint.i = intensity;
	endPoint.deepblue = std::numeric_limits<uint16_t>::max();
	endPoint.yellow = std::numeric_limits<uint16_t>::max();
	endPoint.cyan = std::numeric_limits<uint16_t>::max();
	endPoint.user4 = std::numeric_limits<uint16_t>::max();

	AddNewLine(startPoint, endPoint);
}

void VectorImageBuilder::AddNewLine(JMVectorStruct& startPoint, JMVectorStruct& endPoint) {
	if (!VectorImagePoints.empty()) {
		JMVectorStruct lastPoint = VectorImagePoints.back();
		double distanceToLastPointSquared = JMVectorStructDistanceSquared(startPoint, lastPoint);
		if (distanceToLastPointSquared >= LineFirstPointIgnoreDistanceSquaredInProjectorRange ||
		    lastPoint.i != startPoint.i || lastPoint.r != startPoint.r || lastPoint.g != startPoint.g ||
		    lastPoint.b != startPoint.b ||
		    lastPoint.cyan != startPoint.cyan || lastPoint.deepblue != startPoint.deepblue ||
		    lastPoint.yellow != startPoint.yellow || lastPoint.user4 != startPoint.user4) {
			if (lastPoint.i != 0) {
				lastPoint.i = 0;
				AddNewPoint(lastPoint);
			}

			JMVectorStruct startPointOff = startPoint;
			startPointOff.i = 0;
			AddNewPoint(startPointOff);
			AddNewPoint(startPoint);
		} else {
			if (distanceToLastPointSquared > 0 && distanceToLastPointSquared < LineFirstPointMergeDistanceSquaredInProjectorRange) {
				lastPoint.x = (int32_t)((lastPoint.x + startPoint.x) / 2.0);
				lastPoint.y = (int32_t)((lastPoint.y + startPoint.y) / 2.0);
				ReplaceLastPoint(lastPoint);
			}

			if (VectorImagePoints.size() > 1) {
				JMVectorStruct penultimatePoint = VectorImagePoints[VectorImagePoints.size() - 2];
				double distanceToPenultimatePointSquared = JMVectorStructDistanceSquared(endPoint, penultimatePoint);
				if (distanceToPenultimatePointSquared < LineFirstPointIgnoreDistanceSquaredInProjectorRange) {
					RemoveLastPoint();
				}
			}
		}
	} else {
		AddNewPoint(startPoint);
	}

	AddNewPointWithLinearInterpolationFromLastPoint(endPoint);
}

bool VectorImageBuilder::AddNewPoint(double x, double y,
                 uint16_t red, uint16_t green, uint16_t blue,
                 uint16_t intensity, AxisPosition originAxisPosition) {
	double xInProjectorOrigin;
	double yInProjectorOrigin;
	int32_t xInProjectorRange;
	int32_t yInProjectorRange;
	if (ConvertPointFromDrawingAreaToProjectorOrigin(x, y, xInProjectorOrigin, yInProjectorOrigin, originAxisPosition) &&
	ConvertPointFromDrawingAreaInProjectorOriginToProjectorRange(xInProjectorOrigin, yInProjectorOrigin, xInProjectorRange, yInProjectorRange)) {
		AddNewPoint(xInProjectorRange, yInProjectorRange, red, green, blue, intensity);
		return true;
	}
	return false;
}

void VectorImageBuilder::AddNewPoint(int32_t x, int32_t y,
                 uint16_t red, uint16_t green, uint16_t blue, uint16_t intensity) {
	JMVectorStruct newPoint;
	newPoint.x = x;
	newPoint.y = y;
	newPoint.r = red;
	newPoint.g = green;
	newPoint.b = blue;
	newPoint.i = intensity;
	newPoint.deepblue = std::numeric_limits<uint16_t>::max();
	newPoint.yellow = std::numeric_limits<uint16_t>::max();
	newPoint.cyan = std::numeric_limits<uint16_t>::max();
	newPoint.user4 = std::numeric_limits<uint16_t>::max();

	AddNewPoint(newPoint);
}

void VectorImageBuilder::AddNewPoint(JMVectorStruct& point) {
	if (VectorImagePoints.empty()) {
		JMVectorStruct pointStartOff = point;
		pointStartOff.i = 0;
		VectorImagePoints.push_back(pointStartOff);
	}
	VectorImagePoints.push_back(point);
}

void VectorImageBuilder::AddNewPointWithLinearInterpolationFromLastPoint(JMVectorStruct& point) {
	JMVectorStruct lastPoint = VectorImagePoints.back();
	double distanceToLastPoint = std::sqrt(JMVectorStructDistanceSquared(lastPoint, point));
	int numberOfInterpolationPoints = (int)(distanceToLastPoint / (double)InterpolationDistanceInProjectorRange) - 1;

	if (numberOfInterpolationPoints > 0) {
		double tIncrement = 1.0 / (double) numberOfInterpolationPoints;
		double t = tIncrement;
		for (int i = 0; i < numberOfInterpolationPoints; ++i) {
			JMVectorStruct newPoint = lastPoint;
			newPoint.x = (int32_t)LinearInterpolation(lastPoint.x, point.x, t);
			newPoint.y = (int32_t)LinearInterpolation(lastPoint.y, point.y, t);
			AddNewPoint(newPoint);
			t += tIncrement;
		}
	}

	AddNewPoint(point);
}

void VectorImageBuilder::CorrectRadialDistortion(JMVectorStruct& point) {
	if (RadialDistortionCoefficientSecondDegree != 0 || RadialDistortionCoefficientFourthDegree != 0 || RadialDistortionCoefficientSixthDegree != 0) {
		double u = (double)point.x / (double)std::numeric_limits<int32_t >::max();
		double v = (double)point.y / (double)std::numeric_limits<int32_t >::max();
		double uInverted = ((double)std::numeric_limits<int32_t >::max() - std::abs((double)point.x)) / (double)std::numeric_limits<int32_t >::max();
		double vInverted = ((double)std::numeric_limits<int32_t >::max() - std::abs((double)point.y)) / (double)std::numeric_limits<int32_t >::max();
		double r = std::pow(u, 2.0) * std::pow(v, 2.0);
		double rWithInvertedUV = std::pow(uInverted, 2.0) * std::pow(vInverted, 2.0);
		double warp = RadialDistortionCoefficientSecondDegreeInvertedUV * rWithInvertedUV +
		              RadialDistortionCoefficientSecondDegree * r +
		              RadialDistortionCoefficientFourthDegree * r * r +
		              RadialDistortionCoefficientSixthDegree  * r * r * r;
		point.x = (int)((1.0 + warp) * (double)point.x);
	}
}

void VectorImageBuilder::CorrectRadialDistortionOnVectorImage() {
	for (int i = 0; i < VectorImagePoints.size(); ++i) {
		CorrectRadialDistortion(VectorImagePoints[i]);
	}
}

void VectorImageBuilder::ReplaceLastPoint(JMVectorStruct& point) {
	if (VectorImagePoints.empty())
		VectorImagePoints.push_back(point);
	else
		VectorImagePoints.back() = point;
}

void VectorImageBuilder::RemoveLastPoint() {
	VectorImagePoints.pop_back();
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </functions>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <static functions>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
double VectorImageBuilder::JMVectorStructDistanceSquared(JMVectorStruct first, JMVectorStruct second) {
	return  std::pow((double)second.x - (double)first.x, 2.0) + std::pow((double)second.y - (double)first.y, 2.0);
}

bool VectorImageBuilder::LineIntersection(double p0_x, double p0_y, double p1_x, double p1_y,
                                          double p2_x, double p2_y, double p3_x, double p3_y,
                                          double& i_x, double& i_y) {
	double s02_x, s02_y, s10_x, s10_y, s32_x, s32_y, s_numer, t_numer, denom, t;
	s10_x = p1_x - p0_x;
	s10_y = p1_y - p0_y;
	s32_x = p3_x - p2_x;
	s32_y = p3_y - p2_y;

	denom = s10_x * s32_y - s32_x * s10_y;
	if (denom == 0)
		return false; // Collinear

	bool denomPositive = denom > 0;

	s02_x = p0_x - p2_x;
	s02_y = p0_y - p2_y;
	s_numer = s10_x * s02_y - s10_y * s02_x;
	//if ((s_numer < 0) == denomPositive)
	//    return false; // No collision

	t_numer = s32_x * s02_y - s32_y * s02_x;
	//if ((t_numer < 0) == denomPositive)
	//    return false; // No collision

	//if (((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive))
	//    return false; // No collision

	// Collision detected
	t = t_numer / denom;
	i_x = p0_x + (t * s10_x);
	i_y = p0_y + (t * s10_y);

	return true;
}

double VectorImageBuilder::LinearInterpolation(double a, double b, double t) {
	return a * (1 - t) + b * t;
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <static functions/>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ===============================================================================  </public-section>   ===========================================================================
}
