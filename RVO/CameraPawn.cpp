// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPawn.h"
#include "RVOCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"



// Sets default values
ACameraPawn::ACameraPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = mesh;
	
	springArm->AttachTo(RootComponent);
	springArm->TargetArmLength = 360.f;
	springArm->SetWorldRotation(FRotator(-60.0f, 0.f, 0.f));

	camera->AttachTo(springArm, USpringArmComponent::SocketName);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator newYaw = GetActorRotation();
	newYaw.Yaw += mouseInput.X;
	SetActorRotation(newYaw);

	FRotator newPitch = springArm->GetComponentRotation();
	
	newPitch.Pitch = FMath::Clamp(newPitch.Pitch + mouseInput.Y, -80.f, 0.f);
	springArm->SetWorldRotation(newPitch);
}

// Called to bind functionality to input
void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MouseYaw", this, &ACameraPawn::MouseYaw);
	InputComponent->BindAxis("MousePitch", this, &ACameraPawn::MousePitch);
}

void ACameraPawn::MouseYaw(float axis) {
	mouseInput.X = axis;
}

void ACameraPawn::MousePitch(float axis) {
	mouseInput.Y = axis;
}

