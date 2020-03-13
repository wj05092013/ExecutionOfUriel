#include "ExecutionOfUriel/Public/EUMath.h"

FVector2D FEUMath::RandPointInUnitSectorAroundX(float MaxDegreeFromX)
{
	float Radian = MaxDegreeFromX * PI / 180.0f;

	float Cos = FMath::Cos(Radian);
	float VecLen;
	FVector2D RandVec;

	do
	{
		RandVec.X = FMath::FRandRange(-1.0f, 1.0f);
		RandVec.Y = FMath::FRandRange(-1.0f, 1.0f);
		VecLen = RandVec.Size();

		// The angle between (X,Y) and the X axis has to be lesser than 'Radian'.
		// And, the vector size is in the range of (0, 1].
	} while ((RandVec.X < VecLen * Cos) || (VecLen <= 0.0f || 1.0f < VecLen));

	return RandVec;
}

FVector2D FEUMath::RandPointInSectorAroundX(float Radius, float MaxDegreeFromX)
{
	FVector2D RandVec = RandPointInUnitSectorAroundX(MaxDegreeFromX);

	return Radius * RandVec;
}

FVector2D FEUMath::RandPointInUnitArcAroundX(float MaxDegreeFromX)
{
	FVector2D RandVec = RandPointInUnitSectorAroundX(MaxDegreeFromX);
	RandVec.Normalize();

	return RandVec;
}

FVector2D FEUMath::RandPointInArcAroundX(float Radius, float MaxDegreeFromX)
{
	FVector2D RandVec = RandPointInUnitArcAroundX(MaxDegreeFromX);

	return Radius * RandVec;
}