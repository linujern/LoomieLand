// Copyright (c) 2020 Smultron Barna AB

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "NavigationSystem.h"
#include "SB_LoomieManager.generated.h"

class ASB_HappyLoomie;
class ASB_GloomieLoomie;
class ASB_BaseLoomieAI;


UCLASS()
class SMULTRONBARNA_API ASB_LoomieManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASB_LoomieManager();

	TArray<FVector> CalculateDanceOrigin(TArray<ASB_HappyLoomie*> GroopyLoomies, bool& IsLocationReachable);
	void CheckForChatty(ASB_HappyLoomie* CurrentLoomie);
	void CheckForGroovy(ASB_HappyLoomie* CurrentLoomie);

	TArray<FVector> DancePositionsCalc(FVector CenterPos);
	
	void CheckifDanceFinished();
	TArray<ASB_HappyLoomie*> GetHappyLoomies() { return m_HappyLoomies; }

	void SpawnHappyLoomies();
	void SpawnGloomieLoomies();

	void DestroyHappies();
	void DestroyDoomies();

	void CheckifChattyFinished(ASB_HappyLoomie* CurrentLoomie);
	void AddLoomie(ASB_BaseLoomieAI* Loomie);

	UFUNCTION(BlueprintCallable)
		void OnMakeHappyLoomies(TArray<ASB_GloomieLoomie*> Gloomies, TArray<ASB_HappyLoomie*> HappyLoomies);

	int GetNewID() { return m_IDCounter++; }

	void SpawnLoomie(FTransform StartTransform);
	void DestroyLoomie(ASB_GloomieLoomie* Gloomie);

	TArray<AActor*> GetWaterActors() { return m_WaterActors; }

	FVector OffSetDanceLocation(FVector StartLocation );
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Loomies")
	TArray<ASB_HappyLoomie*> m_HappyLoomies;

	UPROPERTY(VisibleAnywhere, Category = "Loomies")
	TArray<ASB_GloomieLoomie*> m_GloomieLoomies;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	USphereComponent* m_SpawnSphere;

	UPROPERTY(EditAnywhere)
	UClass* m_HappyLoomieBP;

	UPROPERTY(EditAnywhere)
	UClass* m_GloomieLoomieBP;

	UPROPERTY(BlueprintReadWrite)
	int32 m_AmountTransformGloomies;

	UFUNCTION(BlueprintImplementableEvent)
		void SetParameterValue();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	int32 Quantity;

	UPROPERTY(EditAnywhere)
	float Radius;

	UPROPERTY(EditAnywhere)
	int m_AmountofHappyLoomies;

	UPROPERTY(EditAnywhere)
	int m_AmountofGloomieLoomies;

	UPROPERTY()
	UNavigationSystemV1* m_NavSys;

	UPROPERTY(VisibleAnywhere)
	int testtimer;

	int m_IDCounter;

	UPROPERTY(EditAnywhere, Category = "Water")
	TArray<AActor*> m_WaterActors;

	
};
