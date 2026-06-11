// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassCharacter/CharacterCaine.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ACharacterCaine::ACharacterCaine()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

}


// Called when the game starts or when spawned
void ACharacterCaine::BeginPlay()
{
	Super::BeginPlay();

	SetupInputMappingContext();
	
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;

	///////////////// Подключение Start and End OVERLAP
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACharacterCaine::OnWeaponOverlapStart);
		GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ACharacterCaine::OnWeaponOverlapEnd);
	}

	//Создаем 4 пустые ячейки в инвенторе
	WeaponInventory.Init(nullptr, 4);
}

// Called every frame
void ACharacterCaine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacterCaine::SetupInputMappingContext()
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (CaineMappingContext)
			{
				Subsystem->AddMappingContext(CaineMappingContext,0);
			}
		}
	}
}


void ACharacterCaine::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Движение wasd
		EnhancedInputComponent->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &ACharacterCaine::MoveForward);
		EnhancedInputComponent->BindAction(IA_MoveRight, ETriggerEvent::Triggered, this, &ACharacterCaine::MoveRight);

		//Мышь, камера
		EnhancedInputComponent->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &ACharacterCaine::Turn);
		EnhancedInputComponent->BindAction(IA_LookUp, ETriggerEvent::Triggered, this, &ACharacterCaine::LookUp);

		//Roll
		EnhancedInputComponent->BindAction(IA_Roll, ETriggerEvent::Started, this, &ACharacterCaine::Roll);

		//Sprint
		EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Started, this, &ACharacterCaine::SprintStart);
		EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &ACharacterCaine::SprintStop);

		//Interact
		EnhancedInputComponent->BindAction(IA_Interact, ETriggerEvent::Started, this, &ACharacterCaine::Interact);

		//Sheathe_Unsheathe
		EnhancedInputComponent->BindAction(IA_Sheathe_Unsheathe, ETriggerEvent::Started, this, &ACharacterCaine::Sheathe_Unsheathe);

		//LBM
		EnhancedInputComponent->BindAction(IA_LBM, ETriggerEvent::Started, this, &ACharacterCaine::LBM);
	}
}

void ACharacterCaine::MoveForward(const FInputActionValue& Value)
{
	float MovementValue = Value.Get<float>();

	

	if (Controller && (MovementValue != 0.0f))
	{
		
		const FRotator Rotation = Controller->GetControlRotation();
		
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		
		AddMovementInput(ForwardDirection, MovementValue);

		
	}
}

void ACharacterCaine::MoveRight(const FInputActionValue & Value)
{
	float MovementValue = Value.Get<float>();

	if (Controller && (MovementValue != 0.0f))
	{
		
		const FRotator Rotation = Controller->GetControlRotation();
		
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		
		AddMovementInput(RightDirection, MovementValue);
	}
}

void ACharacterCaine::Turn(const FInputActionValue & Value)
{
	
	float LookValue = Value.Get<float>();

	if (LookValue != 0.0f)
	{
		
		AddControllerYawInput(LookValue);
	}
}


void ACharacterCaine::LookUp(const FInputActionValue & Value)
{
	
	float LookValue = Value.Get<float>();

	if (LookValue != 0.0f)
	{
		
		AddControllerPitchInput(LookValue);
	}
}


void ACharacterCaine::SprintStart()
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ACharacterCaine::SprintStop()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
}

void ACharacterCaine::Roll()
{

}

void ACharacterCaine::LBM()
{
	if (WeaponInHand)
	{
		IInterface_Character_Weapon::Execute_Attack(WeaponInHand);
	}

	else if (!WeaponInHand && EquippedWeaponInHips)
	{
		Sheathe_Unsheathe();
	}
}

void ACharacterCaine::Interact()
{
	if (DetectedActor && !WeaponInHand)
	{
		WeaponInHand = DetectedActor;

		IInterface_Character_Weapon::Execute_FlyToHand(WeaponInHand, GetMesh());
		
		
	}

	else if (!DetectedActor && WeaponInHand)
	{
		
		IInterface_Character_Weapon::Execute_DropWeapon(WeaponInHand, GetMesh());
		

	}
}
		


void ACharacterCaine::CLearWeaponInHand_Drop_Implementation()
{
	WeaponInHand = nullptr;

}

	


void ACharacterCaine::Sheathe_Unsheathe()
{
	
	if (WeaponInHand)
	{
		
		IInterface_Character_Weapon::Execute_PlayMontagesSword_Sheathe(WeaponInHand, GetMesh());
	}
		

	else
	{
		IInterface_Character_Weapon::Execute_PlayMontagesSword_Unsheathe(EquippedWeaponInHips, GetMesh());
	}
		
}

		
		
void ACharacterCaine::SwapWeaponSlots_Implementation()
{
	
	if (WeaponInHand)
	{
		EquippedWeaponInHips = WeaponInHand;
		WeaponInHand = nullptr;
	}
	
	else if (EquippedWeaponInHips)
	{
		WeaponInHand = EquippedWeaponInHips;
		EquippedWeaponInHips = nullptr;
	}
}



//-------------------OVERLAPS------------------------------------------------------------------------------------------------------------------------
void ACharacterCaine::OnWeaponOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->Implements<UInterface_Character_Weapon>())
		{
		
			DetectedActor = OtherActor;

		}
		
	}

}
			

	

void ACharacterCaine::OnWeaponOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && DetectedActor == OtherActor)
	{
		DetectedActor = nullptr;
	}
}

	
//---------------------------------------------------------------------------------------------------------------------------------------------------

		