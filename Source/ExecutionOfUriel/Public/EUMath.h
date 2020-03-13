#pragma once

#include "Math/UnrealMath.h"

struct FEUMath
{
	/*
	* Return a random vector that is distributed in a sector or an arc. The center of it is on the X axis.
	* @param Radius Radius of the sector/arc.
	* @param MaxDegreeFromX Maximum angle from the X axis.
	*/

	static FVector2D RandPointInUnitSectorAroundX(float MaxDegreeFromX);
	static FVector2D RandPointInSectorAroundX(float Radius, float MaxDegreeFromX);
	static FVector2D RandPointInUnitArcAroundX(float MaxDegreeFromX);
	static FVector2D RandPointInArcAroundX(float Radius, float MaxDegreeFromX);
};