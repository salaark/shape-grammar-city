/*
	ModularBuilding.h
	Extends Actor and creates the base GrammarShape
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrammarShape.h"
#include "ModularBuilding.generated.h"

UCLASS()
class MODULARTOWN_API AModularBuilding : public AActor {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AModularBuilding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Root component is punlic
	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;

	UGrammarShape* Base;
};
