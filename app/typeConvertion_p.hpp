#pragma once

#include "ActorList.hpp"
#include "EnemyList.hpp"
#include "PlayerList.hpp"
#include "BonusList.hpp"

#include "engine/types.hpp"
#include "engine/Enemy.hpp"
#include "engine/Bonus.hpp"

#include <QPoint>
#include <QSize>
#include <QRect>

inline ActorList::Direction fromEngine(const battlecity::engine::direction_type& d) {
    switch (d.type()) {
    case battlecity::engine::direction_type::Up:
        return ActorList::Up;
    case battlecity::engine::direction_type::Down:
        return ActorList::Down;
    case battlecity::engine::direction_type::Left:
        return ActorList::Left;
    case battlecity::engine::direction_type::Right:
        return ActorList::Right;
    }
    Q_UNREACHABLE();
}

inline EnemyList::Type fromEngine(battlecity::engine::Enemy::Type type) {
    switch(type) {
    case battlecity::engine::Enemy::Type::Armor:
        return EnemyList::Armor;
    case battlecity::engine::Enemy::Type::Basic:
        return EnemyList::Basic;
    case battlecity::engine::Enemy::Type::Fast:
        return EnemyList::Fast;
    case battlecity::engine::Enemy::Type::Power:
        return EnemyList::Power;
    }
    Q_UNREACHABLE();
}

inline battlecity::engine::direction_type toEngine(ActorList::Direction arg) {
    switch (arg) {
    case ActorList::Up:
        return battlecity::math::Direction::Up;
    case ActorList::Left:
        return battlecity::math::Direction::Left;
    case ActorList::Down:
        return battlecity::math::Direction::Down;
    case ActorList::Right:
        return battlecity::math::Direction::Right;
    }
    Q_UNREACHABLE();
}

inline BonusList::Type fromEngine(const battlecity::engine::Bonus::Type& type)
{
    switch (type) {
    case battlecity::engine::Bonus::Type::Granade:
        return BonusList::Granade;
    case battlecity::engine::Bonus::Type::Helmet:
        return BonusList::Helmet;
    case battlecity::engine::Bonus::Type::Shovel:
        return BonusList::Shovel;
    case battlecity::engine::Bonus::Type::Star:
        return BonusList::Star;
    case battlecity::engine::Bonus::Type::Tank:
        return BonusList::Tank;
    case battlecity::engine::Bonus::Type::Timer:
        return BonusList::Timer;
    default: break;
    }

    Q_UNREACHABLE();
}

inline QPoint toPoint(const battlecity::engine::coordinate_vector_type& v) {
    return {v.x, v.y};
}

inline QSize toSize(const battlecity::engine::coordinate_vector_type& v) {
    return {v.x, v.y};
}

inline QRect fromEngine(const battlecity::engine::coordinate_rect_type r) {
    return QRect(toPoint(r.topLeft()), toSize(r.size()));
}


