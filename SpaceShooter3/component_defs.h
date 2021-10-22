#pragma once

struct Component {
	constexpr bool is_tag()
	{
		return false;
	}
};

struct Tag {
	constexpr bool is_tag()
	{
		return true;
	}
};


struct Position : Component
{
	float x = 0;
	float y = 0;
	
	struct equals
	{
		bool operator()(const Position& lhs, const Position& rhs) const
		{
			return lhs.x == rhs.x && lhs.y == rhs.y;
		};
	};
	
};

struct Velocity : Component
{
	float x = 1;
	float y = 1;	
		
};

struct EnemyMovementPeriod : Component 
{
	float period = 0;
};

struct AttackTimer : Component
{
	float timer = 0;
};

struct PlayerBullet : Tag {};

struct EnemyBullet : Tag {};

struct EnemyTag : Tag{};

struct PlayerTag : Tag {};

struct BoundsDespawned : Tag {};
