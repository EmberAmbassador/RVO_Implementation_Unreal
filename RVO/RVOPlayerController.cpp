#include "RVOPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "CustomCharacter.h"
#include "EngineUtils.h"
#include "Engine/World.h"

ARVOPlayerController::ARVOPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ARVOPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ARVOPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("NormalMove", IE_Pressed, this, &ARVOPlayerController::OnNormal);
	InputComponent->BindAction("RVOMove", IE_Pressed, this, &ARVOPlayerController::OnRVO);
	InputComponent->BindAction("CustomRVOMove", IE_Pressed, this, &ARVOPlayerController::OnCustomRVO);
	InputComponent->BindAction("Reset", IE_Pressed, this, &ARVOPlayerController::OnReset);
}

/*void ARVOPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (ARVOCharacter* MyPawn = Cast<ARVOCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void ARVOPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void ARVOPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void ARVOPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void ARVOPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}*/

void ARVOPlayerController::OnNormal() {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Movement without RVO"));
	}

	// Old value -590.00f
	FVector Location;

	for (TActorIterator<ACustomCharacter> CharacterItr(GetWorld()); CharacterItr; ++CharacterItr) {
		FVector aLoc = CharacterItr->GetActorLocation();
		float yLoc = aLoc[1];
		if (yLoc < 0) {
			Location = FVector(-900.0f, 690.0f, 258.0f);
		}
		else {
			Location = FVector(-900.0f, -800.0f, 258.0f);
		}
		//UAIBlueprintHelperLibrary::SimpleMoveToLocation(CharacterItr->GetController(), Location);
		CharacterItr->target = Location;
	}
}

void ARVOPlayerController::OnRVO() {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Built-In RVO"));
	}

	FVector Location;

	for (TActorIterator<ACustomCharacter> CharacterItr(GetWorld()); CharacterItr; ++CharacterItr) {
		FVector aLoc = CharacterItr->GetActorLocation();
		float yLoc = aLoc[1];
		if (yLoc < 0) {
			Location = FVector(-900.0f, 690.0f, 258.0f);
		}
		else {
			Location = FVector(-900.0f, -800.0f, 258.0f);
		}
		CharacterItr->GetCharacterMovement()->SetAvoidanceEnabled(true);
		//UAIBlueprintHelperLibrary::SimpleMoveToLocation(CharacterItr->GetController(), Location);
		CharacterItr->target = Location;
	}
}

void ARVOPlayerController::OnReset() {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Reset"));
	}

	FVector Location = FVector(-590.0f, -800.0f, 258.0f);

	for (TActorIterator<ACustomCharacter> CharacterItr(GetWorld()); CharacterItr; ++CharacterItr) {
		CharacterItr->GetCharacterMovement()->SetAvoidanceEnabled(false); // Makes sure that RVO is disabled
		CharacterItr->SetActorLocation(Location); //Resets the location of the charaacters
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(CharacterItr->GetController(), Location); // Stops movement of the characters
		Location = FVector(-590.0f, 690.0f, 258.0f);
		CharacterItr->setRVO(false);
		CharacterItr->target = CharacterItr->GetActorLocation();
	}
}

void ARVOPlayerController::OnCustomRVO() {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Custom RVO"));
	}

	FVector Location;

	for (TActorIterator<ACustomCharacter> CharacterItr(GetWorld()); CharacterItr; ++CharacterItr) {
		FVector aLoc = CharacterItr->GetActorLocation();
		float yLoc = aLoc[1];
		if (yLoc < 0) {
			Location = FVector(-900.0f, 690.0f, 258.0f);
		}
		else {
			Location = FVector(-900.0f, -800.0f, 258.0f);
		}
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(CharacterItr->GetController(), Location);
		CharacterItr->target = Location;
		Location = FVector(-900.0f, -800.0f, 258.0f);
		CharacterItr->setRVO(true);
	}
}


