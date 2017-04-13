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
#include <cstdint>
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
		void StartNewVectorImage();
		void FinishVectorImage();
		bool ConvertProjectorOriginToDrawingAreaOrigin(double x, double y, double& newX, double& newY, AxisPosition drawingAreaOriginAxisPosition = BottomLeft);
		bool ConvertPointFromDrawingAreaToProjectorOrigin(double x, double y, double& xPointInDrawingAreaAndProjectorOrigin, double& yPointInDrawingAreaAndProjectorOrigin, AxisPosition originAxisPosition = BottomLeft);
		bool ConvertPointFromDrawingAreaInProjectorOriginToProjectorRange(double xPointInDrawingAreaAndProjectorOrigin, double yPointInDrawingAreaAndProjectorOrigin, int32_t& xPointInProjectorRange, int32_t& yPointInProjectorRange);
		bool ConvertPointFromProjectorRangeToDrawingArea(int32_t xPointInProjectorRange, int32_t yPointInProjectorRange, double& xPointInDrawingAreaRange, double& yPointInDrawingAreaRange, AxisPosition originAxisPosition = BottomLeft);
		bool TrimLineInDrawingAreaAndProjectorOrigin(double& startXInProjectorOrigin, double& startYInProjectorOrigin, double& endXInProjectorOrigin, double& endYInProjectorOrigin);
		bool IsPointInProjectorOriginWithinDrawingArea(double x, double y);
		bool AddNewLine(double startX, double startY, double endX, double endY,
		                uint16_t red = std::numeric_limits<uint16_t>::max(), uint16_t green = std::numeric_limits<uint16_t>::max(), uint16_t blue = std::numeric_limits<uint16_t>::max(),
		                uint16_t intensity = std::numeric_limits<uint16_t>::max(), AxisPosition originAxisPosition = BottomLeft);
		void AddNewLine(int32_t startX, int32_t startY, int32_t endX, int32_t endY,
		                uint16_t red = std::numeric_limits<uint16_t>::max(), uint16_t green = std::numeric_limits<uint16_t>::max(), uint16_t blue = std::numeric_limits<uint16_t>::max(), uint16_t intensity = std::numeric_limits<uint16_t>::max());
		void AddNewLine(JMVectorStruct& startPoint, JMVectorStruct& endPoint);
		bool AddNewPoint(double x, double y,
	                    uint16_t red = std::numeric_limits<uint16_t>::max(), uint16_t green = std::numeric_limits<uint16_t>::max(), uint16_t blue = std::numeric_limits<uint16_t>::max(),
                        uint16_t intensity = std::numeric_limits<uint16_t>::max(), AxisPosition originAxisPosition = BottomLeft);
		void AddNewPoint(int32_t x, int32_t y,
		                 uint16_t red = std::numeric_limits<uint16_t>::max(), uint16_t green = std::numeric_limits<uint16_t>::max(), uint16_t blue = std::numeric_limits<uint16_t>::max(), uint16_t intensity = std::numeric_limits<uint16_t>::max());
		void AddNewPoint(JMVectorStruct& point);
		void AddNewPointWithLinearInterpolationFromLastPoint(JMVectorStruct& point);
		void CorrectRadialDistortion(JMVectorStruct& point);
		void CorrectRadialDistortionOnVectorImage();
		void ReplaceLastPoint(JMVectorStruct& point);
		void RemoveLastPoint();
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </functions>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <static functions>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		static double JMVectorStructDistanceSquared(JMVectorStruct first, JMVectorStruct second);
		static bool LineIntersection(double p0_x, double p0_y, double p1_x, double p1_y,
		                             double p2_x, double p2_y, double p3_x, double p3_y,
		                             double& i_x, double& i_y);
		static double LinearInterpolation(double a, double b, double t);
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <static functions/>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <gets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		inline double getDrawingAreaWidth() const { return DrawingAreaWidth; }
		inline double getDrawingAreaHeight() const { return DrawingAreaHeight; }
		inline double getDrawingAreaXOffset() const { return DrawingAreaXOffset; }
		inline double getDrawingAreaYOffset() const { return DrawingAreaYOffset; }
		inline double getRadialDistortionCoefficientSecondDegreeInvertedUV() const { return RadialDistortionCoefficientSecondDegreeInvertedUV; }
		inline double getRadialDistortionCoefficientSecondDegree() const { return RadialDistortionCoefficientSecondDegree; }
		inline double getRadialDistortionCoefficientFourthDegree() const { return RadialDistortionCoefficientFourthDegree; }
		inline double getRadialDistortionCoefficientSixthDegree() const { return RadialDistortionCoefficientSixthDegree; }
		inline double getLineFirstPointMergeDistanceSquaredInProjectorRange() const { return LineFirstPointMergeDistanceSquaredInProjectorRange; }
		inline double getLineFirstPointIgnoreDistanceSquaredInProjectorRange() const { return LineFirstPointIgnoreDistanceSquaredInProjectorRange; }
		inline int32_t getInterpolationDistanceInProjectorRange() const { return InterpolationDistanceInProjectorRange; }
		inline std::vector<JMVectorStruct>& getVectorImagePoints() { return VectorImagePoints; }
		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </gets>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


		// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <sets>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		inline void setDrawingAreaWidth(double DrawingAreaWidth) { VectorImageBuilder::DrawingAreaWidth = DrawingAreaWidth; }
		inline void setDrawingAreaHeight(double DrawingAreaHeight) { VectorImageBuilder::DrawingAreaHeight = DrawingAreaHeight; }
		inline void setDrawingAreaXOffset(double DrawingAreaXOffset) { VectorImageBuilder::DrawingAreaXOffset = DrawingAreaXOffset; }
		inline void setDrawingAreaYOffset(double DrawingAreaYOffset) { VectorImageBuilder::DrawingAreaYOffset = DrawingAreaYOffset; }
		inline void setRadialDistortionCoefficientSecondDegreeInvertedUV(double RadialDistortionCoefficientSecondDegreeInvertedUV) { VectorImageBuilder::RadialDistortionCoefficientSecondDegreeInvertedUV = RadialDistortionCoefficientSecondDegreeInvertedUV; }
		inline void setRadialDistortionCoefficientSecondDegree(double RadialDistortionCoefficientSecondDegree) { VectorImageBuilder::RadialDistortionCoefficientSecondDegree = RadialDistortionCoefficientSecondDegree; }
		inline void setRadialDistortionCoefficientFourthDegree(double RadialDistortionCoefficientFourthDegree) { VectorImageBuilder::RadialDistortionCoefficientFourthDegree = RadialDistortionCoefficientFourthDegree; }
		inline void setRadialDistortionCoefficientSixthDegree(double RadialDistortionCoefficientSixthDegree) { VectorImageBuilder::RadialDistortionCoefficientSixthDegree = RadialDistortionCoefficientSixthDegree; }
		inline void setLineFirstPointMergeDistanceSquaredInProjectorRange(double LineFirstPointMergeDistanceSquaredInProjectorRange) { VectorImageBuilder::LineFirstPointMergeDistanceSquaredInProjectorRange = LineFirstPointMergeDistanceSquaredInProjectorRange; }
		inline void setLineFirstPointIgnoreDistanceSquaredInProjectorRange(double LineFirstPointIgnoreDistanceSquaredInProjectorRange) { VectorImageBuilder::LineFirstPointIgnoreDistanceSquaredInProjectorRange = LineFirstPointIgnoreDistanceSquaredInProjectorRange; }
		inline void setInterpolationDistanceInProjectorRange(int32_t InterpolationDistanceInProjectorRange) { VectorImageBuilder::InterpolationDistanceInProjectorRange = InterpolationDistanceInProjectorRange; }
	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </sets>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// ============================================================================   </public-section>   =========================================================================


	// ============================================================================   <protected-section>   =======================================================================
	protected:
		double DrawingAreaWidth;
		double DrawingAreaHeight;
		double DrawingAreaXOffset;
		double DrawingAreaYOffset;
		double _drawingAreaToProjectorRangeXScale;
		double _drawingAreaToProjectorRangeYScale;
		double RadialDistortionCoefficientSecondDegreeInvertedUV;
		double RadialDistortionCoefficientSecondDegree;
		double RadialDistortionCoefficientFourthDegree;
		double RadialDistortionCoefficientSixthDegree;
		double LineFirstPointMergeDistanceSquaredInProjectorRange;
		double LineFirstPointIgnoreDistanceSquaredInProjectorRange;
		int32_t InterpolationDistanceInProjectorRange;
		std::vector<JMVectorStruct> VectorImagePoints;
	// ============================================================================   <protected-section>   =======================================================================
};

}
