#pragma once

/**\file pattern_builder.h
* \brief Namespace with functions for building patterns.
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
#include <laser_projector_bridge/jmlaser_projector.h>
#include <laser_projector_bridge/vector_image_builder.h>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


namespace laser_projector_bridge {
	// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||   PatternBuilder   |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	/**
	* \brief Namespace with functions for building patterns.
	*/
	namespace pattern_builder {
		void createGridInProjectorRange(laser_projector_bridge::VectorImageBuilder &vector_image_builder, size_t numberOfColumns, size_t numberOfRows, uint32_t columnSize, uint32_t rowSize, int32_t columnStartOffset = 0, int32_t rowStartOffset = 0);
		void createSquareFullRange(std::vector<JMVectorStruct> &points);
		void createSquareScaled(std::vector<JMVectorStruct> &points, double position_scale = 0.9);
		void createSquareScaled(laser_projector_bridge::VectorImageBuilder &vector_image_builder, double scale = 1.0);
		void createPlusFullRange(std::vector<JMVectorStruct> &points);
		void createPlusFullRange(laser_projector_bridge::VectorImageBuilder &vector_image_builder);
		void createCrossFullRange(std::vector<JMVectorStruct> &points);
		void createCrossFullRange(laser_projector_bridge::VectorImageBuilder &vector_image_builder);
		void createCrossScaled(std::vector<JMVectorStruct> &points);
		void createHorizontalDiamondOutsideDrawingArea(laser_projector_bridge::VectorImageBuilder &vector_image_builder);
		void createVerticalDiamondOutsideDrawingArea(laser_projector_bridge::VectorImageBuilder &vector_image_builder);
		void createCrossOutsideDrawingArea(laser_projector_bridge::VectorImageBuilder &vector_image_builder);
		void createPlusOutsideDrawingArea(laser_projector_bridge::VectorImageBuilder &vector_image_builder);
	}
}
