// Copyright (c) 2020 Smultron Barna AB

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include <GameFramework/FloatingPawnMovement.h>
#include "SB_Gift.generated.h"

UCLASS()
class SMULTRONBARNA_API ASB_Gift : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASB_Gift();

	void SetTarget(FVector Value) { Target = Value; }
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_StaticMesh;

	UFloatingPawnMovement* m_FloatingPawnMovement;

	UPROPERTY(EditAnywhere)
		float MaxSpeed;
	UPROPERTY(EditAnywhere)
	UBoxComponent* m_Collider;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool m_Launch;
	FVector Target;
};
