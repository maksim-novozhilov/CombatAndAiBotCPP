// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AttachDetachSwordHpsAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class COMBATANDAIBOTCPP_API UAttachDetachSwordHpsAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;
	
};
