/**\file pattern_builder.cpp
* \brief Namespace with functions for building patterns.
*
* @version 1.0
* @author Carlos Miguel Correia da Costa
*/

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <includes>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#include <laser_projector_bridge/pattern_builder.h>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   </includes>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


namespace laser_projector_bridge {
	namespace pattern_builder {

		void createGridInProjectorRange(laser_projector_bridge::VectorImageBuilder &vector_image_builder, size_t numberOfColumns, size_t numberOfRows, uint32_t columnSize, uint32_t rowSize, int32_t columnStartOffset, int32_t rowStartOffset) {
			int64_t columnStart = columnStartOffset;
			int64_t columnEnd = columnStart + columnSize * numberOfColumns;
			int64_t rowStart = rowStartOffset;
			int64_t rowEnd = rowStartOffset;
			int64_t switchValue;

			for (size_t rowLine = 0; rowLine < numberOfRows + 1; ++rowLine)
			{
				vector_image_builder.addNewLine((int32_t)columnStart, (int32_t)rowStart, (int32_t)columnEnd, (int32_t)rowEnd);
				rowStart += rowSize;
				rowEnd += rowSize;
				switchValue = rowStart;
				rowStart = rowEnd;
				rowEnd = switchValue;
			}

			columnStart = columnStartOffset;
			columnEnd = columnStartOffset;
			rowStart = rowStartOffset;
			rowEnd = rowStart + rowSize * numberOfRows;

			for (size_t column = 0; column < numberOfColumns + 1; ++column)
			{
				vector_image_builder.addNewLine((int32_t)columnStart, (int32_t)rowStart, (int32_t)columnEnd, (int32_t)rowEnd);
				columnStart += columnSize;
				columnEnd += columnSize;
				switchValue = columnStart;
				columnStart = columnEnd;
				columnEnd = switchValue;
			}
		}


		void createSquareFullRange(std::vector<JMVectorStruct> &points) {
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MIN, 0));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MIN, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0, INT_MIN, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, INT_MIN, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, 0, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, INT_MAX, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0, INT_MAX, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MAX, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, 0, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MIN, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MIN, 0));
		}


		void createSquareScaled(std::vector<JMVectorStruct> &points, double position_scale) {
			unsigned short intensity = (unsigned short)(USHRT_MAX * 0.9);
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MIN * position_scale), 0));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MIN * position_scale), intensity));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MAX * position_scale), (int)(INT_MIN * position_scale), intensity));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MAX * position_scale), (int)(INT_MAX * position_scale), intensity));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MAX * position_scale), intensity));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MIN * position_scale), intensity));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MIN * position_scale), 0));
		}


		void createSquareScaled(laser_projector_bridge::VectorImageBuilder &vector_image_builder, double scale) {
			double x_offset = (vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() - (vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * scale)) * 0.5;
			double y_offset = (vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() - (vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * scale)) * 0.5;
			double x_min = x_offset;
			double x_max = vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() - x_offset;
			double y_min = y_offset;
			double y_max = vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() - y_offset;
			vector_image_builder.addNewLine(x_min, y_min, x_max, y_min);
			vector_image_builder.addNewLine(x_max, y_min, x_max, y_max);
			vector_image_builder.addNewLine(x_max, y_max, x_min, y_max);
			vector_image_builder.addNewLine(x_min, y_max, x_min, y_min);
		}


		void createPlusFullRange(std::vector<JMVectorStruct> &points) {
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0, INT_MIN, 0));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0, INT_MIN, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0, INT_MAX, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, 0, 0));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, 0, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, 0, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, 0, 0));
		}


		void createPlusFullRange(laser_projector_bridge::VectorImageBuilder &vector_image_builder) {
			double half_width = vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 0.5;
			double half_height = vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.5;
			vector_image_builder.addNewLine(0.0, half_height, vector_image_builder.getProjectionModelProperties().getImageWidthInPixels(), half_height);
			vector_image_builder.addNewLine(half_width, 0.0, half_width, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels());
		}


		void createCrossFullRange(std::vector<JMVectorStruct> &points) {
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MIN, 0));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MIN, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN / 2, INT_MIN / 2, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0, 0, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX / 2, INT_MAX / 2, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, INT_MAX, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, INT_MIN, 0));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX, INT_MIN, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MAX / 2, INT_MIN / 2, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0, 0, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN / 2, INT_MAX / 2, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MAX, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(INT_MIN, INT_MAX, 0));
		}


		void createCrossFullRange(laser_projector_bridge::VectorImageBuilder &vector_image_builder) {
			vector_image_builder.addNewLine(INT_MIN, INT_MAX, INT_MAX, INT_MIN);
			vector_image_builder.addNewLine(INT_MIN, INT_MIN, INT_MAX, INT_MAX);
		}


		void createCrossScaled(std::vector<JMVectorStruct> &points) {
			double position_scale = 0.75;
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MIN * position_scale), 0));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MIN * position_scale), USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0, 0, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MAX * position_scale), (int)(INT_MAX * position_scale), USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MAX * position_scale), (int)(INT_MAX * position_scale), 0));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MAX * position_scale), (int)(INT_MIN * position_scale), 0));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MAX * position_scale), (int)(INT_MIN * position_scale), USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint(0, 0, USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MAX * position_scale), USHRT_MAX));
			points.push_back(laser_projector_bridge::JMLaserProjector::createSingleColorLaserPoint((int)(INT_MIN * position_scale), (int)(INT_MAX * position_scale), 0));
		}


		void createHorizontalDiamondOutsideDrawingArea(laser_projector_bridge::VectorImageBuilder &vector_image_builder) {
			double half_width = vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 0.5;
			double half_hHeight = vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.5;
			vector_image_builder.addNewLine(vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * -0.25, half_hHeight, half_width, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.75);
			vector_image_builder.addNewLine(half_width, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.75, vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 1.25, half_hHeight);
			vector_image_builder.addNewLine(vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 1.25, half_hHeight, half_width, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.25);
			vector_image_builder.addNewLine(half_width, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.25, vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * -0.25, half_hHeight);
		}


		void createVerticalDiamondOutsideDrawingArea(laser_projector_bridge::VectorImageBuilder &vector_image_builder) {
			double half_width = vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 0.5;
			double half_height = vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.5;
			vector_image_builder.addNewLine(half_height, vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * -0.25, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.75, half_width);
			vector_image_builder.addNewLine(vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.75, half_width, half_height, vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 1.25);
			vector_image_builder.addNewLine(half_height, vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 1.25, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.25, half_width);
			vector_image_builder.addNewLine(vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.25, half_width, half_height, vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * -0.25);
		}


		void createCrossOutsideDrawingArea(laser_projector_bridge::VectorImageBuilder &vector_image_builder) {
			vector_image_builder.addNewLine(vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * -0.25, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * -0.25, vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 0.25, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.25);
			vector_image_builder.addNewLine(vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * -0.25, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * -0.25, vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 0.15, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.25);
			vector_image_builder.addNewLine(vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * -0.25, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * -0.25, vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 0.35, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.25);

			vector_image_builder.addNewLine(vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * -0.25, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 1.25, vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 0.25, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.75);
			vector_image_builder.addNewLine(vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * -0.25, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 1.25, vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 0.15, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.75);
			vector_image_builder.addNewLine(vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * -0.25, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 1.25, vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 0.35, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.75);

			vector_image_builder.addNewLine(vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 1.25, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * -0.25, vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 0.75, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.25);
			vector_image_builder.addNewLine(vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 1.25, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * -0.25, vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 0.65, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.25);
			vector_image_builder.addNewLine(vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 1.25, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * -0.25, vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 0.85, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.25);

			vector_image_builder.addNewLine(vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 1.25, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 1.25, vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 0.75, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.75);
			vector_image_builder.addNewLine(vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 1.25, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 1.25, vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 0.65, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.75);
			vector_image_builder.addNewLine(vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 1.25, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 1.25, vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 0.85, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.75);
		}


		void createPlusOutsideDrawingArea(laser_projector_bridge::VectorImageBuilder &vector_image_builder) {
			double half_width = vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 0.5;
			double half_height = vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.5;
			vector_image_builder.addNewLine(half_width * 0.5, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.25, half_width * 0.5, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 1.25);
			vector_image_builder.addNewLine(vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 0.25, half_height * 0.5, vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 1.25, half_height * 0.5);
			vector_image_builder.addNewLine(half_width * 1.5, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * -0.25, half_width * 1.5, vector_image_builder.getProjectionModelProperties().getImageHeightInPixels() * 0.75);
			vector_image_builder.addNewLine(vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * -0.25, half_height * 1.5, vector_image_builder.getProjectionModelProperties().getImageWidthInPixels() * 0.75, half_height * 1.5);
		}
	}
}
