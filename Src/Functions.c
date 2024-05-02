#include "Functions.h"

#include "math.h"
#include "ClearStack.h"

static float MinRadiusDiastanceUp(float position1, float position2)
{
	ADD_TO_CLEAR();
	float distance = 0;

	if (position1 >= position2)
		distance = 2 * (float) M_PI - position1 + position2;
	else
		distance = position2 - position1;

	return distance;
}

static float MinRadiusDiastanceDown(float position1, float position2)
{
	ADD_TO_CLEAR();
	float distance = 0;

	if (position1 >= position2)
		distance = position1 - position2;
	else
		distance = 2 * (float) M_PI - position2 + position1;

	return distance;
}

float MinRadiusDiastance(float position1, float position2)
{
	ADD_TO_CLEAR();
	float distance = 0;

	float distanceUp = MinRadiusDiastanceUp(position1, position2);
	CLEAR_STACK();
	float distanceDown = MinRadiusDiastanceDown(position1, position2);
	CLEAR_STACK();
	if (distanceUp < distanceDown)
		distance = distanceUp;
	else
		distance = -distanceDown;

	return distance;
}
