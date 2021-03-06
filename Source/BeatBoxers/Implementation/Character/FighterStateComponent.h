// copyright 2017 BYU Animation

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "BeatBoxers.h"
#include "Interfaces/IFighterState.h"
#include "Interfaces/IFighter.h"
#include "Interfaces/IFighterWorld.h"
#include "Interfaces/IMoveset.h"
#include "Interfaces/IInputParser.h"
#include "Interfaces/IFighterPlayerState.h"
#include "BBFunctionLibrary.h"
#include "FighterStateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEATBOXERS_API UFighterStateComponent : public UActorComponent, public IFighterState
{
	GENERATED_UCLASS_BODY()

protected:
	IFighterWorld *MyFighterWorld;
	IFighter *MyFighter;
	IMoveset *MyMoveset;
	IInputParser *MyInputParser;
	IFighterPlayerState *MyFighterPlayerState;

	uint32 bIsWindowActive : 1;
	uint32 bIsHitboxActive : 1;
	uint32 bHasMoveWindowHit : 1;
	uint32 bIsBeingMoved : 1;
	uint32 bIsCurrentStunBlock : 1;
	uint32 bIsFrozenForSolo : 1;
	uint32 bWantsToCharge: 1;

	EWindowEnd CurrentWindowEnd;
	FMovement CurrentMovement;
	FMoveWindow CurrentWindow;
	TArray<TWeakObjectPtr<AActor>> ActorsToIgnore;
	float MoveDirection;
	float CurrentWindowAccuracy;

	UPROPERTY(EditDefaultsOnly)
	FEffects DefaultHitEffects;

	UPROPERTY(EditDefaultsOnly)
	FEffects DefaultBlockEffects;

	FTimerHandle TimerHandle_Window;
	FTimerHandle TimerHandle_Stun;
	FTimerHandle TimerHandle_Movement;

	// Called when the game starts
	virtual void BeginPlay() override;

	// Sets the window timer to the window's windup time to call OnWindowWindupFinished.
	void StartCurrentWindowWindup();

	// Calls StartCurrentWindow.
	void OnCurrentWindowWindupFinished();

	// Sets the window timer to the window's duration to call OnWindowFinished, sets up hitbox.
	void StartCurrentWindow();

	// Calls StartCurrentWindowWinddown, disables hitbox.
	void OnCurrentWindowFinished();

	// Sets the window timer to the window's winddown duration to call OnWindowWinddownFinished.
	void StartCurrentWindowWinddown();

	// Tells Moveset the window has finished.
	void OnCurrentWindowWinddownFinished();

	// Checks to see if we're mid-window, cleans up if we are and informs Moveset.
	void EndWindow(EWindowEnd WindowEnd);

	// Requests the FighterWorld perform a scan.
	void PerformHitboxScan();

	// Tells input parser control has been returned.
	void OnStunFinished();

	// Plays effects relative to the attacker.
	void PlayerAttackerEffects(FEffects& Effects);

	// Attempts to disable tick.
	void TryDisableTick();

	// Called when the movement timer expires.
	void OnMovementTimer();

	bool MovementStep(float DeltaTime);

public:	
	/** IFighterState implementation */
	virtual void RegisterFighterWorld(TWeakObjectPtr<UObject> FighterWorld) override;
	virtual void RegisterFighter(TWeakObjectPtr<UObject> Fighter) override;
	virtual void RegisterMoveset(TWeakObjectPtr<UObject> Moveset) override;
	virtual void RegisterInputParser(TWeakObjectPtr<UObject> InputParser) override;
	virtual void RegisterFighterPlayerState(TWeakObjectPtr<UObject> FighterPlayerState) override;
	virtual bool IsInputBlocked() const override;
	virtual bool IsBlocking() const override;
	virtual bool IsCharging() const override;
	virtual bool IsStunned() const override;
	virtual bool IsMidMove() const override;
	virtual void StartMoveWindow(FMoveWindow& Window, float Accuracy) override;
	virtual void StartStun(float Duration, bool WasBlocked) override;
	virtual void SetMoveDirection(float Direction) override;
	virtual void SetWantsToCrouch(bool WantsToCrouch) override;
	virtual void SetWantsToCharge(bool WantsToCharge) override;
	virtual void ApplyMovement(FMovement Movement) override;
	virtual void Jump() override;
	virtual void OnLand() override;
	virtual float GetSpecial() const override;
	virtual void AddSpecial(float Amount) override;
	virtual bool UseSpecial(float Amount) override;
	virtual EStance GetStance() const override;
	virtual float GetCurrentHorizontalMovement() const override;
	virtual void EndSolo() override;
	/** End IFighterState implmementation */

	AController* GetOwnerController() const;

	UFUNCTION()
	void OnSoloStart(AActor *ActorSoloing);

	UFUNCTION()
	void OnSoloEnd();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
