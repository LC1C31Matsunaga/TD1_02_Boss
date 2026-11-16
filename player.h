#pragma once
#include "common.h"

struct Player
{
	Vector2 position;
	Vector2 speed;
	Vector2 acceleration;
	Vector2 size;
	bool isGrounded;
	int jumpCount;
	int hp;
};