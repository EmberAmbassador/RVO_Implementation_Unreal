#include "CustomCharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EngineUtils.h"
#include "Engine/World.h"

// Sets default values
ACustomCharacter::ACustomCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
}

// Called when the game starts or when spawned
void ACustomCharacter::BeginPlay()
{
	Super::BeginPlay();

	target = this->GetActorLocation();
	
}

// Called every frame
void ACustomCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float collDistance = 400;
	// The percentage with which we want to correct our velocity in each direction
	float strongCorrection = 7;
	float weakCorrection = 0.5;
	float xCorrection = weakCorrection;
	float yCorrection = weakCorrection;
	
	if (customRVO && this->GetVelocity() != FVector(0, 0, 0)) {
		for (TActorIterator<ACustomCharacter> CharacterItr(GetWorld()); CharacterItr; ++CharacterItr) {
			if (*CharacterItr != this) {
				// Only check the velocity object for close neighbours
				float dist = FVector::Dist(this->GetActorLocation(), CharacterItr->GetActorLocation());
				if (dist < 400) {
					if (ACustomCharacter::VelocityObstacle(this->GetVelocity(), CharacterItr->GetVelocity(), this->GetActorLocation(), CharacterItr->GetActorLocation())) {
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Collision detected"));

						FVector currentVel = this->GetVelocity();
						FVector currentVel2 = CharacterItr->GetVelocity();

						// Stop the actors movement. This is necessary to be able to change its velocity
						UAIBlueprintHelperLibrary::SimpleMoveToLocation(this->GetController(), this->GetActorLocation());
						UAIBlueprintHelperLibrary::SimpleMoveToLocation(CharacterItr->GetController(), CharacterItr->GetActorLocation());

						// Calculate the new velocity and check in the next tick whether or not this solved the collision
						
						float mX = currentVel[0];
						float mY = currentVel[1];
						float mZ = currentVel[2];

						
						float oX = currentVel2[0];
						float oY = currentVel2[1];
						float oZ = currentVel2[2];

						// Adjust the velocity of this character
						if(FMath::IsNearlyZero(mX)) {
							if (mY > 0) {
								mX = ((mX - (mX * -xCorrection)) * 2) - mX;
							}
							else {
								mX = ((mX + (mX * xCorrection)) * 2) - mX;
							}
						}
						else if (FMath::IsNearlyZero(mY)) {
							if (mX > 0) {
								mY = ((mY - (mY * -yCorrection)) * 2) - mY;
							}
							else {
								mY = ((mY + (mY * yCorrection)) * 2) - mY;
							}
						}
						else if (abs(mX) < abs(mY)) {
							xCorrection = strongCorrection;
							yCorrection = weakCorrection;
							//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Fuckup detected"));
							if (mY < 0) {
								mX = ((mX * (1 - xCorrection)) * 2) - mX;
							}
							else {
								mX = ((mX * (1 + xCorrection)) * 2) - mX;
							}
							if (mX < 0) {
								mY = (((mY * (1 - yCorrection)) * 2) - mY);
							}
							else {
								mY = (((mY * (1 + yCorrection)) * 2) - mY);
							}
						}
						else if (abs(mX) > abs(mY)) {
							yCorrection = strongCorrection;
							xCorrection = weakCorrection;
							if (mY < 0) {
								mX = ((mX * (1 + -xCorrection)) * 2) - mX;
							}
							else {
								mX = ((mX * (1 + xCorrection)) * 2) - mX;
							}
							if (mX < 0) {
								mY = (((mY * (1 + -yCorrection)) * 2) - mY);
							}
							else {
								mY = (((mY * (1 + yCorrection)) * 2) - mY);
							}
						}
						else{
							if (mY < 0) {
								mX = ((mX * (1 + -xCorrection)) * 2) - mX;
							}
							else {
								mX = ((mX * (1 + xCorrection)) * 2) - mX;
							}
							if (mX < 0) {
								mY = (((mY * (1 + -yCorrection)) * 2) - mY);
							}
							else {
								mY = (((mY * (1 + yCorrection)) * 2) - mY);
							}
						}

						// Adjust the velocity of the obstacle
						if (FMath::IsNearlyZero(oX)) {
							if (oY > 0) {
								oX = ((oX - (oX * -xCorrection)) * 2) - oX;
							}
							else {
								oX = ((oX + (oX * xCorrection)) * 2) - oX;
							}
						}
						else if (FMath::IsNearlyZero(oY)) {
							if (oX > 0) {
								oY = ((oY - (oY * -yCorrection)) * 2) - oY;
							}
							else {
								oY = ((oY + (oY * yCorrection)) * 2) - oY;
							}
						}
						else if (abs(oX) < abs(oY)) {
							xCorrection = strongCorrection;
							yCorrection = weakCorrection;
							if (oY < 0) {
								oX = ((oX * (1 + -xCorrection)) * 2) - oX;
							}
							else {
								oX = ((oX * (1 + xCorrection)) * 2) - oX;
							}
							if (oX < 0) {
								oY = (((oY * (1 + -yCorrection)) * 2) - oY);
							}
							else {
								oY = (((oY * (1 + yCorrection)) * 2) - oY);
							}
						}
						else if (abs(oX) > abs(oY)) {
							yCorrection = strongCorrection;
							xCorrection = weakCorrection;
							if (oY < 0) {
								oX = ((oX * (1 + -xCorrection)) * 2) - oX;
							}
							else {
								oX = ((oX * (1 + xCorrection)) * 2) - oX;
							}
							if (oX < 0) {
								oY = (((oY * (1 + -yCorrection)) * 2) - oY);
							}
							else {
								oY = (((oY * (1 + yCorrection)) * 2) - oY);
							}
						}
						else {
							if (oY < 0) {
								oX = ((oX * (1 + -xCorrection)) * 2) - oX;
							}
							else {
								oX = ((oX * (1 + xCorrection)) * 2) - oX;
							}
							if (oX < 0) {
								oY = (((oY * (1 + -yCorrection)) * 2) - oY);
							}
							else {
								oY = (((oY * (1 + yCorrection)) * 2) - oY);
							}
						}

						while (true)
						{
							this->GetMovementComponent()->Velocity = FVector(mX, mY, mZ);
							CharacterItr->GetMovementComponent()->Velocity = FVector(oX, oY, oZ);

							// Check if we can continue with our old velocity
							if (ACustomCharacter::VelocityObstacle(currentVel, currentVel2, this->GetActorLocation(), CharacterItr->GetActorLocation())) {
								break;
							}
						}

						/*// Debug message to check the correction we made
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::SanitizeFloat(mX));
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::SanitizeFloat(mY));*/
					}
				}
			}
		}
	} else {
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this->GetController(), target);
	}

	/*FVector veloc = this->GetVelocity();
	float yC = veloc[1];
	if(!FMath::IsNearlyZero(yC)) {GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::SanitizeFloat(yC)); }
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("My velocity is %s"), *this->GetVelocity()[2].ToString()));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::SanitizeFloat(yC));*/
}

// Called to bind functionality to input
void ACustomCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PlayerInputComponent->BindAction("NormalMove", IE_Pressed, this, &ACustomCharacter::OnNormal);
	//PlayerInputComponent->BindAction("RVOMove", IE_Pressed, this, &ACustomCharacter::OnRVO);

}

void ACustomCharacter::setRVO(bool input)
{
	customRVO = input;
}

bool ACustomCharacter::VelocityObstacle(FVector va, FVector vb, FVector pa, FVector pb) {
	/*FHitResult* HitResult = new FHitResult();
	FVector ForwardVector = va - vb;
	FVector EndTrace = ((ForwardVector * 5000.0f) + pa);
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

	if (character->GetWorld()->LineTraceSingleByChannel(*HitResult, pa, EndTrace, ECC_Visibility, *TraceParams) {

	}*/

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("A at %s with velocity %s"), *pa.ToString(), *va.ToString()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("B at %s with velocity %s"), *pb.ToString(), *vb.ToString()));

	FVector minkSum = FVector(pb[0] - pa[0], pb[1] - pa[1], pb[2] - pa[2]);

	float mVelocity0 = (va[0] - vb[0]);
	float mVelocity1 = (va[1] - vb[1]);
	float mVelocity2 = (va[2] - vb[2]);
	float mVelocity;
	int step;


	if (!FMath::IsNearlyZero(mVelocity0)) {
		mVelocity = mVelocity0;
		step = 0;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::SanitizeFloat(mVelocity));
	}
	else if (!FMath::IsNearlyZero(mVelocity1)) {
		mVelocity = mVelocity1;
		step = 1;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::SanitizeFloat(mVelocity));
	}
	else if (!FMath::IsNearlyZero(mVelocity2)) {
		mVelocity = mVelocity2;
		step = 2;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::SanitizeFloat(mVelocity));
	}
	else if (pa == minkSum) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Collision at MinkSum"));
		return true;
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Everything must be zero..."));
		return false;
	}

	// We solve to first line of the velocity obstacle for t and get:
	// x1 + t * (va1 - vb1) = pa1 - pb1
	// t = (pa1 - pb1 - x1)/(va1 - vb1)
	// With x1 = pa1 we get:
	float t = (-pb[step] / mVelocity);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::SanitizeFloat(t));

	// Now we check whether the t we found solves the remaining two lines of our system of linear equations
	if (
		(!FMath::IsNearlyZero(mVelocity1) && t == (-pb[1] / (va[1] - vb[1])) && !FMath::IsNearlyZero(mVelocity2) && t == (-pb[2] / (va[2] - vb[2])))
		|| (FMath::IsNearlyZero(mVelocity1) && !FMath::IsNearlyZero(mVelocity2) && t == (-pb[2] / (va[2] - vb[2])))
		|| (!FMath::IsNearlyZero(mVelocity1) && t == (-pb[1] / (va[1] - vb[1])) && FMath::IsNearlyZero(mVelocity2))
		|| (FMath::IsNearlyZero(mVelocity1) && FMath::IsNearlyZero(mVelocity2))
		) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Collision at calc"));
		return true;
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("No collision"));
		return false;
	}
}