// Copyright (c) 2020 Smultron Barna AB

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SB_VoicelineSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SMULTRONBARNA_API USB_VoicelineSystem : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USB_VoicelineSystem();

	UFUNCTION(BlueprintCallable)
	void PlayVoiceLine();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
	TArray<USoundBase*> m_VoiceLines;
	int  m_CurrentVoiceLine;

	UPROPERTY(EditAnywhere)
	bool m_PlayInOrder;

};
