// Copyright (c) 2020 Smultron Barna AB

#pragma once

#include "CoreMinimal.h"
#include "AI/SB_BaseLoomieAI.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "SB_Gift.h"
#include "..\Interactable\SB_VoicelineSystem.h"
#include "SB_HappyLoomie.generated.h"


UENUM(BlueprintType)
enum class Mood
{
	NONE,
	COMFY,
	ROWDY,
	SLEEPY,
	CHATTY,
	GROOVY,
	FRIENDLY,
	FLOATY,
	SCARED,
	LAST,
};

UCLASS()
class SMULTRONBARNA_API ASB_HappyLoomie : public ASB_BaseLoomieAI
{
	GENERATED_BODY()
	
public:
	
	ASB_HappyLoomie();

	void Idle(float DeltaTime) override;

	void Tick(float DeltaTime) override;

	void MoodStateMachine(float DeltaTime);

	void Comfy(float DeltaTime);

	void Rowdy(float DeltaTime);

	void Sleepy(float DeltaTime);

	void Chatty(float DeltaTime);

	void Groovy(float DeltaTime);

	void Friendly(float DeltaTime);

	void Floaty(float DeltaTime);

	void ChooseNewMood();

	void Interact(float DeltaTime);

	bool IsLocationReachable(FVector GoalActor);

	ASBCharacter* PlayerInVisibleRange();

	UFUNCTION()
	Mood GetCurrentMood() { return m_CurrentMood; }

	UFUNCTION(BlueprintCallable)
	ASB_HappyLoomie* GetChattyTarget() { return m_ChattyTarget; }

	void SetChattyTarget(ASB_HappyLoomie* Loomie) { m_ChattyTarget = Loomie; }

	int GetID() { return m_ID; }

	bool& GetStopMovement() { return m_StopMovement; }
	
	void SetID(int Value) { m_ID = Value; }

	float GetMoodTimer() { return m_MoodRandomTimer;}

	bool& GetWaiting() { return m_Waiting; }

	TArray<FVector>& GetDancePositions() { return m_DancePostions; }

	int GetClosestDancePostion();

	AActor* GetFriendlyTarget();

	void Scared();

	void UpdateGoalLocation(FVector Location);

	void NotScared() { m_MoodPreSet = false; }

	void NotTalking() { m_Talking = false; }

	UPROPERTY(BlueprintReadWrite)
	bool m_Talking;

	UFUNCTION(BlueprintImplementableEvent)
		void OnSleeping();

	UFUNCTION(BlueprintImplementableEvent)
		void OnSleepFinished();
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	bool m_Interacting;

	UPROPERTY(BlueprintReadOnly)
	bool m_CanInteract;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mood")
	TEnumAsByte<Mood> m_CurrentMood;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mood|Chatty")
		float m_DistanceTillChat;

private:
	UPROPERTY(VisibleAnywhere, Category = "Mood")
	float m_MoodRandomTimer;

	UPROPERTY(EditAnywhere, Category = "Mood")
	int m_MoodMinRange;

	UPROPERTY(EditAnywhere, Category = "Mood")
	int m_MoodMaxRange;

	UPROPERTY(EditAnywhere, Category = "Mood|Comfy")
	float m_ComfyIdleTimer;

	UPROPERTY(EditAnywhere, Category = "Mood|Comfy")
	AActor* m_ComfyLocation;

	bool m_MoodHasStarted;

	bool m_StopMovement;

	UPROPERTY(EditAnywhere, Category = "Mood|Rowdy")
	float m_ChasingTime;

	float m_InitialChasingTime;

	UPROPERTY(VisibleAnywhere, Category = "Mood|Chatty")
	ASB_HappyLoomie* m_ChattyTarget;

	UPROPERTY(VisibleAnywhere)
	int m_ID;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* m_Widget;

	bool m_Waiting;

	TArray<FVector> m_DancePostions;

	int m_DancePositionIndex;

	bool m_CheckIfMoodFinished;

	UPROPERTY()
	AActor* m_FriendlyTarget;

	UPROPERTY(EditAnywhere, Category = "Mood|Friendly")
	float m_ThrowDistance;

	UPROPERTY(EditAnywhere, Category = "Mood|Friendly")
	float m_ItemSpeed;

	UPROPERTY(EditAnywhere, Category = "Mood|Friendly")
	UClass* m_GiftTospawn;

	UPROPERTY()
	ASB_Gift* m_Gift;

	bool m_GiftThrown;

	
	UPROPERTY(EditAnywhere)
	USB_VoicelineSystem* m_VoiceLineSystem;

	bool m_MoodPreSet;

	FVector m_WaterTarget;

	bool m_StartedSleeping;
};
