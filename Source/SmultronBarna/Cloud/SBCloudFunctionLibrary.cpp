// Copyright (c) 2020 Smultron Barna AB


#include "SBCloudFunctionLibrary.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Camera/CameraComponent.h"

float USBCloudFunctionLibrary::GetFrustumWidth(UCameraComponent* Camera, float Distance)
{
	FMinimalViewInfo Info;
	Camera->GetCameraView(0, Info);
	const float FrustumAngle = Info.FOV;
	const float HozHalfAngleInRadians = FMath::DegreesToRadians(FrustumAngle * 0.5f);
	return Distance * FMath::Tan(HozHalfAngleInRadians) * 2.0f;
}

float USBCloudFunctionLibrary::GetFrustumHeight(UCameraComponent* Camera, float Distance, bool FixedAspectRatio)
{
	const float AspectRatio = GetAspectRatio(Camera, FixedAspectRatio);
	return GetFrustumWidth(Camera, Distance) / AspectRatio;	
}

float USBCloudFunctionLibrary::GetDistanceByFrustumWidth(UCameraComponent* Camera, float FrustumWidth)
{
	FMinimalViewInfo Info;
	Camera->GetCameraView(0, Info);
	const float FrustumAngle = Info.FOV;
	const float HozHalfAngleInRadians = FMath::DegreesToRadians(FrustumAngle * 0.5f);
	return FrustumWidth * 0.5f / FMath::Tan(HozHalfAngleInRadians);
}

float USBCloudFunctionLibrary::GetDistanceByFrustumHeight(UCameraComponent* Camera, float FrustumHeight,
	bool FixedAspectRatio)
{
	const float AspectRatio = GetAspectRatio(Camera, FixedAspectRatio);
	return GetDistanceByFrustumWidth( Camera, FrustumHeight*AspectRatio );
}

float USBCloudFunctionLibrary::GetFieldOfViewByFrustumWidth(UCameraComponent* Camera, float Distance,
	float FrustumWidth)
{
	Distance = FMath::Abs(Distance);
	const float Radians = 2.0f * FMath::Atan(FrustumWidth * 0.5f / Distance);
	return FMath::RadiansToDegrees(Radians);
}

float USBCloudFunctionLibrary::GetFieldOfViewByFrustumHeight(UCameraComponent* Camera, float Distance,
	float FrustumHeight, bool FixedAspectRatio)
{
	const float AspectRatio = GetAspectRatio(Camera, FixedAspectRatio);
	return GetFieldOfViewByFrustumWidth( Camera, Distance, FrustumHeight*AspectRatio );
}

float USBCloudFunctionLibrary::GetAspectRatio(UCameraComponent* Camera, bool FixedAspectRatio)
{
	float AspectRatio;
	if( FixedAspectRatio )
	{
		FMinimalViewInfo info;
		Camera->GetCameraView(0, info);
		AspectRatio = info.AspectRatio;
	}
	else
	{
		FVector2D Size = UWidgetLayoutLibrary::GetViewportSize(Camera->GetWorld());
		AspectRatio = Size.X/Size.Y;
	}

	return AspectRatio;
}
