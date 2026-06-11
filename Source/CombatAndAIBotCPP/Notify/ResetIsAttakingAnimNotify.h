// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ResetIsAttakingAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class COMBATANDAIBOTCPP_API UResetIsAttakingAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;
	
};
