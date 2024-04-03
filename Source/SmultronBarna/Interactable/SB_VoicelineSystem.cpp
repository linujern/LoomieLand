// Copyright (c) 2020 Smultron Barna AB


#include "Interactable/SB_VoicelineSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ShapeComponent.h"

// Sets default values for this component's properties
USB_VoicelineSystem::USB_VoicelineSystem()
	: m_CurrentVoiceLine(0)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void USB_VoicelineSystem::BeginPlay()
{
	Super::BeginPlay();
}

void USB_VoicelineSystem::PlayVoiceLine()
{
	if (!m_VoiceLines.IsEmpty())
	{
		if (m_PlayInOrder)
		{
			if (GetAttachParentActor())
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_VoiceLines[m_CurrentVoiceLine], GetAttachParentActor()->GetActorLocation());
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Blue, "Blast Music");
				if (m_CurrentVoiceLine == m_VoiceLines.Num() - 1)
				{
					m_CurrentVoiceLine = 0;
				}
				else
				{
					m_CurrentVoiceLine++;
				}
			}
		}
		else
		{
			if (m_VoiceLines.Num() != 1)
			{

				int VoiceLineIndex = 0;
				do
				{
					VoiceLineIndex = FMath::RandRange(0, m_VoiceLines.Num() - 1);
				} while (VoiceLineIndex == m_CurrentVoiceLine);

				m_CurrentVoiceLine = VoiceLineIndex;
			}
			else
			{
				m_CurrentVoiceLine = 0;
			}
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_VoiceLines[m_CurrentVoiceLine], GetAttachParentActor()->GetActorLocation());
		}

	}
}


// Called every frame
void USB_VoicelineSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

