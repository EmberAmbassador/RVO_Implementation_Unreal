#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RVOPlayerController.generated.h"

UCLASS()
class ARVOPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARVOPlayerController();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	void OnNormal();
	void OnRVO();
	void OnCustomRVO();
	void OnReset();
};


