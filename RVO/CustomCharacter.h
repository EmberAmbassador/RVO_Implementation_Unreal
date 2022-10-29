#pragma once

#include "CoreMinimal.h"

#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "GameFramework/Character.h"




#include "CustomCharacter.generated.h"

UCLASS()
class RVO_API ACustomCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACustomCharacter();

	void MoveForward(float Axis);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void setRVO(bool input);

	//void updateTarget(FVector destination);

	bool VelocityObstacle(FVector va, FVector vb, FVector pa, FVector pb);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool customRVO = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector target;

};
