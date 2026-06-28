// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassCharacter/CharacterCaine.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"


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

	SwitcherCharacterCameraModeAndSpeed();
}

// Called every frame
void ACharacterCaine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//SwitcherCharacterMode();

	UpdateRotateDirection(DeltaTime);

	CurrentSpeedXY = GetVelocity().Size2D();

	

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
		
		//RBM
		EnhancedInputComponent->BindAction(IA_RBM, ETriggerEvent::Started, this, &ACharacterCaine::BlockStart);
		EnhancedInputComponent->BindAction(IA_RBM, ETriggerEvent::Completed, this, &ACharacterCaine::BlockStop);
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
	
	SwitcherCharacterCameraModeAndSpeed();
}

void ACharacterCaine::SprintStop()
{
	bIsSprinting = false;
	

	SwitcherCharacterCameraModeAndSpeed();
}


void ACharacterCaine::Roll()
{
	if (UAnimInstance* AnimInst = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr)
	{
		UAnimMontage* CurrentMontage = AnimInst->GetCurrentActiveMontage();
		if (CurrentMontage && (CurrentMontage == Montage_RollForward || CurrentMontage == Montage_RollBack || CurrentMontage == Montage_RollLeft || CurrentMontage == Montage_RollRight))
		{
			return;
		}
	}
	
	if (WeaponInHand)
	{
		FVector InputVector = GetLastMovementInputVector(); 

		if (InputVector.IsNearlyZero())
		{
			if (Montage_RollBack) PlayAnimMontage(Montage_RollBack);
			return;
		}

		
		FVector ForwardVector = FVector::ForwardVector;
		FVector RightVector = FVector::RightVector;

		if (AController* SelectedController = GetController())
		{
			// Берем вращение контроллера (куда смотрит камера)
			FRotator ControlRot = SelectedController->GetControlRotation();
			// Обнуляем наклон вверх/вниз, оставляем только поворот влево/вправо
			FRotator YawRotation(0.0f, ControlRot.Yaw, 0.0f);

			// Получаем чистые векторы Вперед и Вправо относительно КЛЮЧЕВОГО направления боя
			ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		}
		
		float ForwardDot = FVector::DotProduct(InputVector.GetSafeNormal(), ForwardVector);
		float RightDot = FVector::DotProduct(InputVector.GetSafeNormal(), RightVector);

		//ПО ДИАГОНАЛИ Комбинация клавиш WA WD
		if (ForwardDot > 0.3f && FMath::Abs(RightDot) > 0.3f)
		{
			if (RightDot > 0.f)
			{
				if (Montage_RollRight) PlayAnimMontage(Montage_RollRight);
			}
			else
			{
				if (Montage_RollLeft) PlayAnimMontage(Montage_RollLeft);
			}
		}
		//ПО ДИАГОНАЛИ Комбинация клавиш SA SD
		else if (ForwardDot < -0.3f && FMath::Abs(RightDot) > 0.3f)
		{
			if (RightDot > 0.f)
			{
				if (Montage_RollRight) PlayAnimMontage(Montage_RollRight);
			}
			else
			{
				if (Montage_RollLeft) PlayAnimMontage(Montage_RollLeft);
			}
		}
		//ПРЯМЫЕ НАПРАВЛЕНИЯ W A S D
		else
		{
			if (FMath::Abs(ForwardDot) >= FMath::Abs(RightDot))
			{
				if (ForwardDot > 0.f)
				{
					if (Montage_RollForward) PlayAnimMontage(Montage_RollForward);
				}
				else
				{
					if (Montage_RollBack) PlayAnimMontage(Montage_RollBack);
				}
			}
			else
			{
				if (RightDot > 0.f)
				{
					if (Montage_RollRight) PlayAnimMontage(Montage_RollRight);
				}
				else
				{
					if (Montage_RollLeft) PlayAnimMontage(Montage_RollLeft);
				}
			}
		}
	}
}


void ACharacterCaine::LBM()
{
	if (WeaponInHand && !bIsSprinting)
	{
		IInterface_Character_Weapon::Execute_Attack(WeaponInHand);
	}

	if (WeaponInHand && bIsSprinting)
	{
		FVector InputVector = GetLastMovementInputVector();
		FVector ForwardVector = FVector::ForwardVector;
		FVector RightVector = FVector::RightVector;

		if (AController* SelectedController = GetController())
		{
			// Берем вращение контроллера (куда смотрит камера)
			FRotator ControlRot = SelectedController->GetControlRotation();
			// Обнуляем наклон вверх/вниз, оставляем только поворот влево/вправо
			FRotator YawRotation(0.0f, ControlRot.Yaw, 0.0f);

			// Получаем чистые векторы Вперед и Вправо относительно КЛЮЧЕВОГО направления боя
			ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		}

		float ForwardDot = FVector::DotProduct(InputVector.GetSafeNormal(), ForwardVector);
		float RightDot = FVector::DotProduct(InputVector.GetSafeNormal(), RightVector);

		if (ForwardDot > 0.8)
		{
			IInterface_Character_Weapon::Execute_SprintAttack(WeaponInHand);
						
		}
	}
			

	else if (!WeaponInHand && EquippedWeaponInHips)
	{
		Sheathe_Unsheathe();
	}
}

void ACharacterCaine::BlockStart()
{
	if (WeaponInHand)
	{
		bCharIsBlocking = true;

		SwitcherCharacterCameraModeAndSpeed();
	}
}

void ACharacterCaine::BlockStop()
{
	bCharIsBlocking = false;

	SwitcherCharacterCameraModeAndSpeed();
}



void ACharacterCaine::Interact()
{
	
	if (DetectedActor && !WeaponInHand)
	{
		WeaponInHand = DetectedActor;

		IInterface_Character_Weapon::Execute_FlyToHand(WeaponInHand, GetMesh());
		
		SwitcherCharacterCameraModeAndSpeed();
	}

	else if (!DetectedActor && WeaponInHand)
	{
		
		IInterface_Character_Weapon::Execute_DropWeapon(WeaponInHand, GetMesh());
		
		
	}
}
		


void ACharacterCaine::CLearWeaponInHand_Drop_Implementation()
{
	WeaponInHand = nullptr;

	SwitcherCharacterCameraModeAndSpeed();

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

	SwitcherCharacterCameraModeAndSpeed();
}



void ACharacterCaine::SwitcherCharacterCameraModeAndSpeed()
{
	
	
	if (bCharIsAttacing) return;

	if (WeaponInHand)
	{
		GetCharacterMovement()->MaxWalkSpeed = CombatWalkingSpeed;
	}

	if (WeaponInHand && bIsSprinting && !bCharIsBlocking)
	{
		GetCharacterMovement()->MaxWalkSpeed = CombatSprintSpeed;
	}
	
	if (WeaponInHand)
	{
		// Если оружие в руках: всегда привязаны к повороту камеры (контроллера)
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	
	else
	{
		// Если оружия нет: персонаж свободно крутится в сторону бега
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;

		// Возвращаем дефолтные скорости для режима без оружия
		GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;

		if (bIsSprinting)
		{
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		}
	}
	

}


	
void ACharacterCaine::SetMovementMode_Implementation(bool bSwitMovementhMode)
{
	if (bSwitMovementhMode)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	}

	if (!bSwitMovementhMode)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}


void ACharacterCaine::SetbHasWeapon_Implementation(bool SetbHasWeapon)
{
	bHasWeapon = SetbHasWeapon;
}

void ACharacterCaine::SetCharIsAttacing_Implementation(bool SwitchSetCharIsAttacing)
{
	bCharIsAttacing = SwitchSetCharIsAttacing;

	if (bCharIsAttacing)
	{
		// Выключаем вращение за камерой на время удара
		bUseControllerRotationYaw = false;
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}
	}

	else
	{
		SwitcherCharacterCameraModeAndSpeed();
	}
}


void ACharacterCaine::UpdateRotateDirection(float DeltaTime)
{
	if (bCharIsAttacing && Controller)
	{
		float ForwardInput = 0.0f;
		float RightInput = 0.0f;

		// Напрямую опрашиваем контроллер на нажатие клавиш WASD
		if (APlayerController* PC = Cast<APlayerController>(Controller))
		{
			if (PC->IsInputKeyDown(EKeys::W)) ForwardInput += 1.0f;
			if (PC->IsInputKeyDown(EKeys::S)) ForwardInput -= 1.0f;
			if (PC->IsInputKeyDown(EKeys::D)) RightInput += 1.0f;
			if (PC->IsInputKeyDown(EKeys::A)) RightInput -= 1.0f;
		}

		// Если игрок зажал хотя бы одну клавишу WASD во время удара
		if (ForwardInput != 0.0f || RightInput != 0.0f)
		{
			// Рассчитываем вектор направления относительно вашей камеры (как в ваших функциях)
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// Собираем финальный вектор желаемого направления
			FVector DesiredDirection = (ForwardDirection * ForwardInput) + (RightDirection * RightInput);
			DesiredDirection.Normalize();

			// Превращаем вектор в угол поворота (сбрасываем Pitch и Roll, чтобы персонаж не наклонялся)
			FRotator TargetRotation = DesiredDirection.Rotation();
			TargetRotation.Pitch = 0.0f;
			TargetRotation.Roll = 0.0f;

			// Плавно разворачиваем капсулу персонажа (15.0f — скорость поворота)
			FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 15.0f);
			SetActorRotation(NewRotation);
		}

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

		