// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/CanNextSwordAttackNotifyState.h"
#include "ClassCharacter/CharacterCaine.h"
#include "Interfaces/Interface_Character_Weapon.h"

void UCanNextSwordAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (ACharacterCaine* Character = Cast<ACharacterCaine>(MeshComp->GetOwner()))
	{

		if (AActor* WeaponInHand = Character->GetCurrentWeaponInHand())

		{
			IInterface_Character_Weapon::Execute_CanNextAttack(WeaponInHand, true);
		}
	}

}

void UCanNextSwordAttackNotifyState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation,
	const FAnimNotifyEventReference & EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ACharacterCaine* Character = Cast<ACharacterCaine>(MeshComp->GetOwner()))
	{

		if (AActor* WeaponInHand = Character->GetCurrentWeaponInHand())

		{
			IInterface_Character_Weapon::Execute_CanNextAttack(WeaponInHand, false);
		}


	}

}

	
