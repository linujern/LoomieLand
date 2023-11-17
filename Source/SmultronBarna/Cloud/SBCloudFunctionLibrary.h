// Copyright (c) 2020 Smultron Barna AB

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SBCloudFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SMULTRONBARNA_API USBCloudFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Camera Frustum")
	static float GetFrustumWidth( UCameraComponent* Camera, float Distance );

	UFUNCTION(BlueprintCallable, Category="Camera Frustum")
	static float GetFrustumHeight( UCameraComponent* Camera, float Distance, bool FixedAspectRatio = false );

	UFUNCTION(BlueprintCallable, Category="Camera Frustum")
	static float GetDistanceByFrustumWidth( UCameraComponent* Camera, float FrustumWidth );

	UFUNCTION(BlueprintCallable, Category="Camera Frustum")
	static float GetDistanceByFrustumHeight( UCameraComponent* Camera, float FrustumHeight, bool FixedAspectRatio = false );

	UFUNCTION(BlueprintCallable, Category="Camera Frustum")
	static float GetFieldOfViewByFrustumWidth( UCameraComponent* Camera, float Distance, float FrustumWidth );

	UFUNCTION(BlueprintCallable, Category="Camera Frustum")
	static float GetFieldOfViewByFrustumHeight( UCameraComponent* Camera, float Distance, float FrustumHeight, bool FixedAspectRatio = false );

	UFUNCTION(BlueprintCallable, Category="Camera Frustum")
	static float GetAspectRatio( UCameraComponent* Camera, bool FixedAspectRatio = false);
};
	