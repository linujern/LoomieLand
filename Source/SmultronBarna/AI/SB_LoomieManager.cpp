// Copyright (c) 2020 Smultron Barna AB


#include "AI/SB_LoomieManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "SB_HappyLoomie.h"
#include "SB_GloomieLoomie.h"

// Sets default values
ASB_LoomieManager::ASB_LoomieManager()
	:Radius(200)
	,Quantity(10)
	,m_AmountTransformGloomies(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_SpawnSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SpawnSphere"));
	SetRootComponent(m_SpawnSphere);
	testtimer = 50.0f;
}



TArray<FVector> ASB_LoomieManager::CalculateDanceOrigin(TArray<ASB_HappyLoomie*> GroopyLoomies, bool& IsLocationReachable)
{
	
	FVector CenterPos = FVector::Zero();
	for(ASB_HappyLoomie* Loomie: GroopyLoomies)
	{
		CenterPos += Loomie->GetActorLocation();
	}
	CenterPos /= GroopyLoomies.Num();

	TArray<FVector> DancePositions = DancePositionsCalc(OffSetDanceLocation(CenterPos));
	for (ASB_HappyLoomie* Loomie : GroopyLoomies)
	{
		
		if(!Loomie->IsLocationReachable(DancePositions[0]))
		{
			IsLocationReachable = false;
		}
		
	}
	
	return DancePositions;
}

void ASB_LoomieManager::CheckForChatty(ASB_HappyLoomie* CurrentLoomie)
{
	if (!HasAuthority())
		return;

	ASB_HappyLoomie* ClosestLoomie = nullptr;

	float ClosestDistance = 1000000;
	for (ASB_HappyLoomie* OtherLoomies : m_HappyLoomies)
	{

		if (!CurrentLoomie->GetChattyTarget() && !OtherLoomies->GetChattyTarget())
		{
			if (CurrentLoomie->GetCurrentMood() == OtherLoomies->GetCurrentMood() && CurrentLoomie->GetID() != OtherLoomies->GetID() && OtherLoomies->GetMovementSpeed() != 0)
			{
				float Distance = FVector::Dist(CurrentLoomie->GetActorLocation(), OtherLoomies->GetActorLocation());
				if (Distance < ClosestDistance && CurrentLoomie->IsLocationReachable(OtherLoomies->GetActorLocation()))
				{
					ClosestLoomie = OtherLoomies;
					ClosestDistance = Distance;
				}
			}


			
		}
	}
	if (ClosestLoomie)
	{
		CurrentLoomie->SetChattyTarget(ClosestLoomie);
		ClosestLoomie->SetChattyTarget(CurrentLoomie);
		CurrentLoomie->GetStopMovement() = false;
		ClosestLoomie->GetStopMovement() = false;
		CurrentLoomie->GetWaiting() = true;
		ClosestLoomie->GetWaiting() = true;

	}
	else
	{
		CurrentLoomie->GetStopMovement() = true;
	}
}

void ASB_LoomieManager::CheckForGroovy(ASB_HappyLoomie* CurrentLoomie)
{
	if (!HasAuthority())
		return;

	TArray<ASB_HappyLoomie*> GroovyLoomies;
	GroovyLoomies.Add(CurrentLoomie);
	for (ASB_HappyLoomie* OtherLoomies : m_HappyLoomies)
	{
		if (CurrentLoomie->GetCurrentMood() == OtherLoomies->GetCurrentMood() && CurrentLoomie->GetID() != OtherLoomies->GetID() && OtherLoomies->GetMovementSpeed() != 0)
		{
			GroovyLoomies.Add(OtherLoomies);
		}
	}
	if (GroovyLoomies.Num() >= 3)
	{
		bool IsDanceCenterReachable = true;
		TArray<FVector> DancePositions = CalculateDanceOrigin(GroovyLoomies, IsDanceCenterReachable);
		if (IsDanceCenterReachable)
		{
			for (ASB_HappyLoomie* GroovyLoomie : GroovyLoomies)
			{
				if (GroovyLoomie->GetDancePositions().IsEmpty())
				{
					GroovyLoomie->GetDancePositions() = DancePositions;
					GroovyLoomie->GetStopMovement() = false;
					GroovyLoomie->GetWaiting() = true;

				}
			}
		}
		else
		{
			CurrentLoomie->GetStopMovement() = true;
		}
	}
	else
	{
		CurrentLoomie->GetStopMovement() = true;
	}
}

TArray<FVector> ASB_LoomieManager::DancePositionsCalc(FVector CenterPos)
{
	TArray<FVector> DancePostions;
	for (int i = 0; i < Quantity; i++)
	{
		int32 test = (360 / Quantity) * i;

		FRotator Rot = UKismetMathLibrary::MakeRotator(0, 0, (float)test);

		FVector PointLocation = (UKismetMathLibrary::GetForwardVector(Rot) * Radius) + CenterPos;

		DancePostions.Add(OffSetDanceLocation(PointLocation));
	}
	return DancePostions;
}

void ASB_LoomieManager::CheckifDanceFinished()
{
	if (!HasAuthority())
		return;

	TArray<ASB_HappyLoomie*> GroovyLoomies;
	for(ASB_HappyLoomie* HappyLoomie: m_HappyLoomies)
	{
		if(HappyLoomie->GetCurrentMood() == Mood::GROOVY)
		{
			if(HappyLoomie->GetMoodTimer() > 0)
			{
				return;
			}
			else
			{
				GroovyLoomies.Add(HappyLoomie);
			}
		}
	}
	for(ASB_HappyLoomie* Groovy: GroovyLoomies)
	{
		Groovy->GetWaiting() = false;
		Groovy->GetDancePositions().Empty();
	}
	
}

void ASB_LoomieManager::SpawnHappyLoomies()
{
	if (!HasAuthority())
		return;

	for(int NrOfHappy = 0; NrOfHappy < m_AmountofHappyLoomies; NrOfHappy++)
	{
		FVector2D Pos2D(0);
		FNavLocation Pos(FVector(0));
		FVector3d Temp(0);
		bool Test = false;
		do
		{	
			Test = m_NavSys->GetRandomPointInNavigableRadius(GetActorLocation(), m_SpawnSphere->GetUnscaledSphereRadius(), Pos);
			

		} while (!Test);

		FTransform NewTransform(Pos.Location);

		GetWorld()->SpawnActor<ASB_HappyLoomie>(m_HappyLoomieBP, NewTransform);
	}
}

void ASB_LoomieManager::SpawnGloomieLoomies()
{
	if (!HasAuthority())
		return;

	for (int NrOfGloomie = 0; NrOfGloomie < m_AmountofGloomieLoomies; NrOfGloomie++)
	{
		FVector2D Pos2D(0);
		FNavLocation Pos(FVector(0));
		FVector3d Temp(0);
		bool Test = false;
		do
		{
			Test = m_NavSys->GetRandomPointInNavigableRadius(GetActorLocation(), m_SpawnSphere->GetUnscaledSphereRadius(), Pos);

		} while (!Test);

		FTransform NewTransform(Pos.Location);

		GetWorld()->SpawnActor<ASB_GloomieLoomie>(m_GloomieLoomieBP, NewTransform);
		
	}
}

void ASB_LoomieManager::DestroyHappies()
{
	if (!HasAuthority())
		return;

	for(int NrOfLoomies = 0; NrOfLoomies < m_HappyLoomies.Num(); NrOfLoomies++)
	{
		if(m_HappyLoomies.Num() > 0)
		{
			ASB_HappyLoomie* CurrentLoomie = m_HappyLoomies[NrOfLoomies];

			CurrentLoomie->Destroy();

			m_HappyLoomies.RemoveAt(NrOfLoomies);
		}
	

		
	}
}

void ASB_LoomieManager::DestroyDoomies()
{
	if (!HasAuthority())
		return;

	for (int NrOfLoomies = 0; NrOfLoomies < m_GloomieLoomies.Num(); NrOfLoomies++)
	{
		if (m_GloomieLoomies.Num() > 0)
		{
			ASB_GloomieLoomie* CurrentLoomie = m_GloomieLoomies[NrOfLoomies];

			CurrentLoomie->Destroy();

			m_GloomieLoomies.RemoveAt(NrOfLoomies);
		}



	}


}

void ASB_LoomieManager::CheckifChattyFinished(ASB_HappyLoomie* CurrentLoomie)
{
	if (!HasAuthority())
		return;

	if (CurrentLoomie->GetMoodTimer() < 0 && CurrentLoomie->GetChattyTarget()->GetMoodTimer() < 0)
	{
		CurrentLoomie->GetWaiting() = false;
		CurrentLoomie->GetChattyTarget()->GetWaiting() = false;
		CurrentLoomie->NotTalking();
		CurrentLoomie->GetChattyTarget()->NotTalking();
		CurrentLoomie->GetChattyTarget()->SetChattyTarget(nullptr);
		CurrentLoomie->SetChattyTarget(nullptr);
	}
}
void ASB_LoomieManager::AddLoomie(ASB_BaseLoomieAI* Loomie)
{
	ASB_GloomieLoomie* GLoomieCheck = Cast<ASB_GloomieLoomie>(Loomie);
	if(GLoomieCheck)
	{
		m_GloomieLoomies.Add(GLoomieCheck);
	}
	ASB_HappyLoomie* HappyCheck = Cast<ASB_HappyLoomie>(Loomie);
	if(HappyCheck)
	{
		m_HappyLoomies.Add(HappyCheck);
	}
}
void ASB_LoomieManager::OnMakeHappyLoomies(TArray<ASB_GloomieLoomie*> Gloomies, TArray<ASB_HappyLoomie*> HappyLoomies)
{
	if(!HasAuthority())
	{
		return;
	}
	
	
	
	for(ASB_GloomieLoomie* Loomie: m_GloomieLoomies)
	{

		
		for(ASB_GloomieLoomie* DestroyLoomie: Gloomies )
		{
			if(DestroyLoomie == Loomie && DestroyLoomie->IsValidLowLevel())
			{
				FTransform GloomieTransform = Loomie->GetTransform();
				Loomie->DestroyGloomie();
				Loomie->SetIsbeingDestroyed();
				
			}
		}
	}
	for(ASB_HappyLoomie* Loomie: m_HappyLoomies)
	{
		for(ASB_HappyLoomie* ScareLoomie: HappyLoomies)
		{
			if(ScareLoomie && ScareLoomie == Loomie && ScareLoomie->GetCurrentMood() == Mood::SCARED)
			{
				ScareLoomie->NotScared();
				ScareLoomie->ChooseNewMood();
			}
		}
	}
}
void ASB_LoomieManager::SpawnLoomie(FTransform StartTransform)
{
	GetWorld()->SpawnActor<ASB_HappyLoomie>(m_HappyLoomieBP, StartTransform);
}
void ASB_LoomieManager::DestroyLoomie(ASB_GloomieLoomie* Gloomie)
{
	if (!HasAuthority())
		return;

	for (int NrOfLoomies = 0; NrOfLoomies < m_GloomieLoomies.Num(); NrOfLoomies++)
	{
		if (m_GloomieLoomies[NrOfLoomies] == Gloomie)
		{
			ASB_GloomieLoomie* CurrentLoomie = m_GloomieLoomies[NrOfLoomies];

			CurrentLoomie->Destroy();

			m_GloomieLoomies.RemoveAt(NrOfLoomies);

			m_AmountTransformGloomies--;

			if(m_AmountTransformGloomies <= 0)
			{
				SetParameterValue();
				m_AmountTransformGloomies = 0;
			}
		}



	}
}
FVector ASB_LoomieManager::OffSetDanceLocation(FVector StartLocation)
{
	FHitResult Hit;
	FVector Offset(0, 0, 1000);
	FCollisionObjectQueryParams params;
	params.AddObjectTypesToQuery(ECC_WorldStatic);
	GetWorld()->LineTraceSingleByObjectType(Hit, StartLocation, StartLocation - Offset, params);
	Hit.Location.Z += 10.0f;
	return Hit.Location;
}
// Called when the game starts or when spawned
void ASB_LoomieManager::BeginPlay()
{
	Super::BeginPlay();

	m_NavSys = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());

	SpawnHappyLoomies();
	SpawnGloomieLoomies();
}

// Called every frame
void ASB_LoomieManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

