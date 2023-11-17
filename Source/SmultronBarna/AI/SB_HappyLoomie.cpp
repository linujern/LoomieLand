// Copyright (c) 2020 Smultron Barna AB


#include "AI/SB_HappyLoomie.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationPath.h"

ASB_HappyLoomie::ASB_HappyLoomie()
	:m_MoodRandomTimer(0.0f)
	,m_CurrentMood(Mood::NONE)
	,m_MoodMinRange(40.0f)
	,m_MoodMaxRange(60.0f)
	,m_ComfyIdleTimer(30.0f)
	,m_ComfyLocation(nullptr)
	,m_MoodHasStarted(false)
	,m_StopMovement(false)
	,m_ChasingTime(7.0f)
	,m_ChattyTarget(nullptr)
	,m_ID(0)
	,m_DistanceTillChat(500.0f)
	,m_Widget(nullptr)
	,m_Waiting(false)
	,m_DancePositionIndex(-1)
	,m_CheckIfMoodFinished(false)
	,m_Gift(nullptr)
	,m_GiftTospawn(nullptr)
	,m_GiftThrown(false)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
	,m_FriendlyTarget(nullptr)
	,m_ItemSpeed(70.0f)
	,m_ThrowDistance(400.0f)
	,m_MoodPreSet(false)
	,m_WaterTarget(0.0f)
	,m_CanInteract(true)
	,m_Interacting(false)
	,m_Talking(false)
	,m_StartedSleeping(false)
{
	
	
}

void ASB_HappyLoomie::Idle(float DeltaTime)
{
	//Don't Know What Idle is Supposed to Have
}

void ASB_HappyLoomie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority())
		return;

	if (m_MovementSpeed > 0)
	{



		if ((m_MoodRandomTimer -= DeltaTime) <= 0 && !m_Waiting)
		{
			ChooseNewMood();
			m_ChasingTime = m_InitialChasingTime;
			m_Widget->SetVisibility(false);
		}

		if (m_StopMovement)
		{
			StateMachine(DeltaTime);
			UpdateRotation(DeltaTime);
		}

		if (!m_Interacting)
		{
			MoodStateMachine(DeltaTime);
			UpdateRotation(DeltaTime);
		}
		else
		{
			Interact(DeltaTime);
		}

	}
	else
	{
		if(m_Interacting)
		{
			Interact(DeltaTime);
		}
	}
	
	

}

void ASB_HappyLoomie::MoodStateMachine(float DeltaTime)
{
	switch (m_CurrentMood)
	{
	case Mood::COMFY:
		Comfy(DeltaTime);
		break;
	case Mood::ROWDY:
		Rowdy(DeltaTime);
		break;
	case Mood::SLEEPY:
		Sleepy(DeltaTime);
		break;
	case Mood::CHATTY:
		Chatty(DeltaTime);
		break;
	case Mood::GROOVY:
		Groovy(DeltaTime);
		break;
	case Mood::FRIENDLY:
		Friendly(DeltaTime);
		break;
	case Mood::FLOATY:
		Floaty(DeltaTime);
		break;
	case Mood::SCARED:
		Scared();
		break;
	default:
		break;
	}
}

void ASB_HappyLoomie::Comfy(float DeltaTime)
{
	if(m_MoodHasStarted || m_StopMovement == true)
	{
		if(!m_ComfyLocation || !IsLocationReachable(m_ComfyLocation->GetActorLocation()))
		{
			m_StopMovement = true;
			return;
		}
		m_MoodHasStarted = false;
		m_StopMovement = false;
		
		m_MoveRequest.UpdateGoalLocation(m_ComfyLocation->GetActorLocation());
	}

	m_MoveRequestResult  =	m_Aicontroller->MoveTo(m_MoveRequest);

	UpdateRotation(DeltaTime);
	if (m_MoveRequestResult == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		if ((m_ComfyIdleTimer -= DeltaTime) <= 0)
		{
			ChooseNewMood();
			return;
		}
	}
}

void ASB_HappyLoomie::Rowdy(float DeltaTime)
{
	if(m_MoodHasStarted || m_StopMovement)
	{
		if(!m_Players.IsEmpty() && m_ChasingTime > 0)
		{
			ASBCharacter* Player = PlayerInVisibleRange();
			if(!Player)
			{
				m_StopMovement = true;
				return;
			}
			else
			{
				UpdateGoalLocation(Player->GetActorLocation());
				m_StopMovement = false;
			}
		}
		else
		{
			m_StopMovement = true;
			return;
		}
	}

	if((m_ChasingTime -= DeltaTime) > 0 && !m_StopMovement)
	{
		if(m_Aicontroller)
		m_Aicontroller->MoveTo(m_MoveRequest);

		if(!IsLocationReachable(m_MoveRequest.GetDestination()))
		{
			m_MoveRequest.UpdateGoalLocation(GetActorLocation());
		}
	}
	else
	{
		m_MoveRequest.UpdateGoalLocation(GetActorLocation());
		m_StopMovement = true;
		return;
	}
	UpdateRotation(DeltaTime);

}

void ASB_HappyLoomie::Sleepy(float DeltaTime)
{
	if(m_MoodHasStarted)
	{
		OnSleeping();
		m_MoodHasStarted = false;
		m_StartedSleeping = true;
	}

	m_CanInteract = false;
	m_MoveRequest.UpdateGoalLocation(GetActorLocation());
	m_MoveRequestResult = m_Aicontroller->MoveTo(m_MoveRequest);
}

void ASB_HappyLoomie::Chatty(float DeltaTime)
{

	if(m_MoodHasStarted)
	{
		if(m_LoomieManager)
		{
			m_LoomieManager->CheckForChatty(this);
		}
		
		m_MoodHasStarted = false;
	}

	if(m_ChattyTarget)
	{

		if (FVector::Dist(GetActorLocation(), m_ChattyTarget->GetActorLocation()) > m_DistanceTillChat && m_Aicontroller)
		{
			UpdateGoalLocation(m_ChattyTarget->GetActorLocation());

			m_MoveRequestResult = m_Aicontroller->MoveTo(m_MoveRequest);

			UpdateRotation(DeltaTime);
		}
		else
		{
			FRotator Rotator = UKismetMathLibrary::FindLookAtRotation( GetActorLocation(), m_ChattyTarget->GetActorLocation());
			Rotator = FMath::RInterpTo(GetActorRotation(), Rotator, DeltaTime, m_RotationSpeed);
			SetActorRotation(Rotator);
			m_MoveRequest.UpdateGoalLocation(GetActorLocation());
			m_MoveRequestResult = m_Aicontroller->MoveTo(m_MoveRequest);
			m_Talking = true;
			//m_Widget->SetVisibility(true);
		}
		if(m_MoodRandomTimer < 0 && !m_CheckIfMoodFinished)
		{
			if(m_LoomieManager)
			{
				m_LoomieManager->CheckifChattyFinished(this);
				m_CheckIfMoodFinished = true;
			}
		}
		
	}
}

void ASB_HappyLoomie::Groovy(float DeltaTime)
{
	if(m_MoodHasStarted)
	{
		if(m_LoomieManager)
		{
			m_LoomieManager->CheckForGroovy(this);
		}
		m_MoodHasStarted = false;
	}

	if(!m_DancePostions.IsEmpty() && !m_StopMovement)
	{
		if(!m_CheckIfMoodFinished && m_DancePositionIndex == -1)
		{
			m_DancePositionIndex = GetClosestDancePostion();
			UpdateGoalLocation(m_DancePostions[m_DancePositionIndex]);
			m_CheckIfMoodFinished = true;
		}
		

		m_MoveRequestResult = m_Aicontroller->MoveTo(m_MoveRequest);
		if(m_MoveRequestResult == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			if(m_DancePositionIndex  == m_DancePostions.Num() - 1)
			{
				m_DancePositionIndex = 0;
			}
			else
			{
				m_DancePositionIndex++;
			}
			UpdateGoalLocation(m_DancePostions[m_DancePositionIndex]);
		}

		UpdateRotation(DeltaTime);
		if(m_MoodRandomTimer < 0 && m_CheckIfMoodFinished && m_LoomieManager)
		{
			m_LoomieManager->CheckifDanceFinished();
			m_CheckIfMoodFinished = false;
			m_DancePositionIndex = -1;
		}
	}

}

void ASB_HappyLoomie::Friendly(float DeltaTime)
{
	if (!m_StopMovement || !m_FriendlyTarget)
	{


		if (!m_FriendlyTarget)
		{
			
			m_FriendlyTarget = GetFriendlyTarget();
			if (!m_FriendlyTarget)
			{
				m_StopMovement = true;
			}
			else
			{
				m_StopMovement = false;
			}
		}
		if (m_FriendlyTarget)
		{

			if (FVector::Dist(GetActorLocation(), m_FriendlyTarget->GetActorLocation()) > m_ThrowDistance && !m_GiftThrown)
			{
				UpdateGoalLocation(m_FriendlyTarget->GetActorLocation());

				m_MoveRequestResult = m_Aicontroller->MoveTo(m_MoveRequest);
			}
			else if (!m_GiftThrown && !m_Gift)
			{
				FTransform transform = GetActorTransform();
				transform.SetLocation(FVector(transform.GetLocation().X, transform.GetLocation().Y, transform.GetLocation().Z + 100.0f));
				m_Gift = GetWorld()->SpawnActor<ASB_Gift>(m_GiftTospawn, transform);
				m_GiftThrown = true;
				m_Gift->SetTarget(m_FriendlyTarget->GetActorLocation());
				m_StopMovement = true;
				m_MoveRequest.UpdateGoalLocation(GetActorLocation());
				m_MoveRequestResult = m_Aicontroller->MoveTo(m_MoveRequest);

			}
			UpdateRotation(DeltaTime);
		}
	}
}

void ASB_HappyLoomie::Floaty(float DeltaTime)
{
	if (m_MoodHasStarted)
	{
		if (m_LoomieManager && !m_LoomieManager->GetWaterActors().IsEmpty())
		{


			m_StopMovement = false;
			TArray<AActor*> WaterActors = m_LoomieManager->GetWaterActors();
			float ClosestDistance = 100000000000000.f;
			AActor* ClosestWater = nullptr;
			for (AActor* Actor : WaterActors)
			{
				float CurrentDistance = FVector::Dist(GetActorLocation(), Actor->GetActorLocation());
				if (IsLocationReachable(Actor->GetActorLocation()) && CurrentDistance < ClosestDistance)
				{
					ClosestWater = Actor;
					ClosestDistance = CurrentDistance;
				}
			}
			if (ClosestWater)
			{
				m_WaterTarget = ClosestWater->GetActorLocation();
				if(m_NavSystem)
				{
					FNavLocation OutLocation;
					m_NavSystem->GetRandomReachablePointInRadius(m_WaterTarget, 500.0f, OutLocation);
					m_WaterTarget = OutLocation.Location;
				}
			}
			m_MoodHasStarted = false;
		}
	}
	if(m_WaterTarget != FVector(0.0f))
	{
		m_MoveRequest.UpdateGoalLocation(m_WaterTarget);
		m_Aicontroller->MoveTo(m_MoveRequest);
		UpdateRotation(DeltaTime);
	}
	else
	{
		m_StopMovement = true;
	}
}

void ASB_HappyLoomie::ChooseNewMood()
{
	if(m_StartedSleeping && m_CurrentMood == Mood::SLEEPY)
	{
		OnSleepFinished();
		m_StartedSleeping = false;
	}

	if (!m_MoodPreSet)
	{
		do
		{
			m_MoodRandomTimer = FMath::RandRange(m_MoodMinRange, m_MoodMaxRange);

			int RandomMoodValue = FMath::RandRange(0, (int)Mood::LAST - 1);

			m_CurrentMood = static_cast<Mood> (RandomMoodValue);
		} while (m_CurrentMood == Mood::SCARED || m_CurrentMood == Mood::NONE);
	}

	m_MoodHasStarted = true;
	m_CanInteract = true;
	m_StopMovement = false;
	m_CheckIfMoodFinished = false;
	m_FriendlyTarget = nullptr;
	m_GiftThrown = false;
	m_Gift = nullptr;
}

void ASB_HappyLoomie::Interact(float DeltaTime)
{
	ASBCharacter* ClosestPlayer = nullptr;
	float ClosestDistance = 100000;

	m_MoveRequest.UpdateGoalLocation(GetActorLocation());
	m_MoveRequestResult = m_Aicontroller->MoveTo(m_MoveRequest);

	for(ASBCharacter* Player : m_Players)
	{
		if (Player)
		{
			float CurrentDistance = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
			if (CurrentDistance < ClosestDistance)
			{
				ClosestPlayer = Player;
				ClosestDistance = CurrentDistance;
			}
		}
	}
	if (ClosestPlayer)
	{
		FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FVector(ClosestPlayer->GetActorLocation().X, ClosestPlayer->GetActorLocation().Y, GetActorLocation().Z));
		Rotator = FMath::RInterpTo(GetActorRotation(), Rotator, DeltaTime, m_RotationSpeed);
		SetActorRotation(Rotator);
	}
}

bool ASB_HappyLoomie::IsLocationReachable(FVector GoalActor)
{
	UNavigationPath* TestPath = m_NavSystem->FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), GoalActor);
	if (!TestPath->IsPartial())
	{
		return true;
	}
	else
	{
		return false;
	}
}

ASBCharacter* ASB_HappyLoomie::PlayerInVisibleRange()
{
	ASBCharacter* ClosestPlayer = nullptr;
	float		  ClosestDistance = 1000000;

	for (ASBCharacter* Player : m_Players)
	{
		FVector PlayerPosition = FVector(Player->GetActorLocation().X, Player->GetActorLocation().Y, GetActorLocation().Z);
		FVector EnemyToPlayer = PlayerPosition - GetActorLocation();

		float angle = FMath::Acos(EnemyToPlayer.Dot(GetActorForwardVector()) / (EnemyToPlayer.Length() * GetActorForwardVector().Length()));
		angle *= 180 / 3.1415;

		float VisionConeAngle = 90.0f;
		float VisionConeLength = 1000.0f;

		if ((angle < VisionConeAngle && EnemyToPlayer.Length() < VisionConeLength))
		{
			ClosestPlayer = Player;
			ClosestDistance = EnemyToPlayer.Length();
		}
	}
	return ClosestPlayer;
}

int ASB_HappyLoomie::GetClosestDancePostion()
{
	int Index = 0;
	float ClosestDistance = FVector::Dist(GetActorLocation(), m_DancePostions[0]);
	for(int i = 1; i < m_DancePostions.Num(); i++)
	{
		float CurrentDistance = FVector::Dist(GetActorLocation(), m_DancePostions[i]);
		if( CurrentDistance < ClosestDistance)
		{
			Index = i;
			ClosestDistance = CurrentDistance;
		}
	}

	return Index;
}

AActor* ASB_HappyLoomie::GetFriendlyTarget()
{
	AActor* Target = nullptr;
	float ClosestDistance = 1000000;
	for (ASBCharacter* Player : m_Players)
	{
		FVector PlayerVector = Player->GetActorLocation();
		PlayerVector.Z = GetActorLocation().Z;
		
		if (IsLocationReachable(Player->GetActorLocation()))
		{
			if (!Target)
			{
				Target = Player;
				ClosestDistance = FVector::Dist(Player->GetActorLocation(), GetActorLocation());
			}
			else if (FVector::Dist(Player->GetActorLocation(), GetActorLocation()) < ClosestDistance)
			{
				Target = Player;
				ClosestDistance = FVector::Dist(Player->GetActorLocation(), GetActorLocation());
			}
		}
	}
	if(!Target)
	{
		
		if (m_LoomieManager)
		{
			
			TArray<ASB_HappyLoomie*> HappyLoomies = m_LoomieManager->GetHappyLoomies();

			for (ASB_HappyLoomie* Loomie : HappyLoomies)
			{
				if (Loomie->GetCurrentMood() != Mood::GROOVY && IsLocationReachable(Loomie->GetActorLocation()) && Loomie->m_ID != m_ID)
				{
					if (FVector::Dist(Loomie->GetActorLocation(), GetActorLocation()) < ClosestDistance)
					{
						Target = Loomie;
						ClosestDistance = FVector::Dist(Loomie->GetActorLocation(), GetActorLocation());
					}
					else if (!Target)
					{
						Target = Loomie;
						ClosestDistance = FVector::Dist(Loomie->GetActorLocation(), GetActorLocation());

					}

				}
			}
		}
	}
	else
	{
		return Target;
	}


	return Target;
}

void ASB_HappyLoomie::Scared()
{
	m_MoveRequest.UpdateGoalLocation(GetActorLocation());
	m_MoveRequestResult = m_Aicontroller->MoveTo(m_MoveRequest);
}

void ASB_HappyLoomie::UpdateGoalLocation(FVector Location)
{
	m_MoveRequest.UpdateGoalLocation(Location);
	if (!IsLocationReachable(m_MoveRequest.GetGoalLocation()))
	{
		m_StopMovement = true;
		m_MoveRequest.UpdateGoalLocation(GetActorLocation());
	}
	else
	{
		m_StopMovement = false;
	}
}


void ASB_HappyLoomie::BeginPlay()
{
	ASB_BaseLoomieAI::BeginPlay();

	m_InitialChasingTime = m_ChasingTime;
	
	UActorComponent* WidgetComponent = GetComponentByClass(UWidgetComponent::StaticClass());

	UWidgetComponent* Widget = Cast<UWidgetComponent>(WidgetComponent);
	if(Widget)
	{
		m_Widget = Widget;
		m_Widget->SetVisibility(false);
	}

	m_VoiceLineSystem = GetComponentByClass<USB_VoicelineSystem>();

	int HappyLoomieCounter = 0;

	if(m_CurrentMood == Mood::NONE && m_MovementSpeed > 0)
	{
		ChooseNewMood();
		m_MoodPreSet = false;
	}
	else
	{
		m_MoodPreSet = true;
	}
	

	if (m_LoomieManager)
		m_ID = m_LoomieManager->GetNewID();
}
