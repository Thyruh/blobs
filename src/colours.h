#pragma once
#ifndef COLOURS_H
#define COLOURS_H

#define EGGPLANT     CLITERAL(Color){ 0x0e, 0x07, 0x0d, 0xFF }

#define RANDOM_U8 GetRandomValue(0, 255)

#define RANDOM_COLOUR()\
	(Color){ RANDOM_U8, RANDOM_U8, RANDOM_U8, 0xFF }

#endif // COLOURS_H
