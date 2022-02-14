#pragma once

#include <FixedPoint.hpp>
#include <Vector.hpp>
#include <Direction.hpp>
#include <Rect.hpp>

#include <cstddef>

namespace battlecity {
namespace engine {

using field_coordinate_type = int_fast8_t;
using field_vector_type = math::Vector<field_coordinate_type>;
using field_rect_type = math::Rect<field_coordinate_type>;

using coordinate_type = int;
using coordinate_vector_type = math::Vector<coordinate_type>;
using coordinate_rect_type = math::Rect<coordinate_type>;
using velocity_type = math::FixedPoint<coordinate_type, 4>;

using time_type = int;
using direction_type = math::Direction;
using score_type = int;
using health_type = int_fast8_t;

}}
