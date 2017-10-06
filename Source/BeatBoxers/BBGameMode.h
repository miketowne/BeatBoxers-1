// copyright 2017 BYU Animation

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BeatBoxersStructs.h"
#include "IFighterWorld.h"
#include "BBGameMode.generated.h"

class AFighterCharacter;
class ABBPlayerController;
class ABBGameState;
class IFighter;

/**
 *
 */
UCLASS()
class BEATBOXERS_API ABBGameMode : public AGameMode, public IFighterWorld
{
	GENERATED_UCLASS_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector InitialCameraLocation;
	FVector InitialCameraLookAtLocation;

public:
	UPROPERTY(BlueprintAssignable)
	FSoloStartEvent SoloStartEvent;

	UPROPERTY(BlueprintAssignable)
	FSoloEndEvent SoloEndEvent;

	/** This actor must implement the IMusicBox interface. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> DefaultMusicBoxClass;

	/** IFighterWorld implementation */
	virtual EFighterDamageType GetDamageType(EStance Stance, EFighterDamageType DesiredOverride) const override;
	virtual struct FHitResult TraceHitbox(FMoveHitbox Hitbox, TArray< TWeakObjectPtr<AActor> >& IgnoreActors) override;
	virtual EHitResponse HitActor(TWeakObjectPtr<AActor> Actor, EFighterDamageType DamageType, FImpactData& Hit, FImpactData& Block, TWeakObjectPtr<AActor> Source, TWeakObjectPtr<AController> SourceController) override;
	virtual void ApplyMovementToActor(TWeakObjectPtr<AActor> Target, TWeakObjectPtr<AActor> Source, TWeakObjectPtr<AController> SourceController, FMovement& Movement);
	virtual void StartSolo(TWeakObjectPtr<AActor> OneSoloing) override;
	virtual FSoloStartEvent& GetOnSoloStartEvent() override;
	virtual FSoloEndEvent& GetOnSoloEndEvent() override;
	/** End IFighterWorld implementation */

	virtual bool DoesBlock(IFighter *Fighter, EFighterDamageType DamageType) const;

	virtual void StartMatch() override;

	/** Returns gamestate cast to BBGameState, can return null if cast fails.*/
	ABBGameState* GetBBGameState();
};
