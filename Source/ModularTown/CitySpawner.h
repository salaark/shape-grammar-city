/*
	CitySpawner.h
	Extends Actor and uses Simplex noise to place ModularBuildings
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CitySpawner.generated.h"

UCLASS()
class ARCHIPLEAGOALPHA_API ACitySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACitySpawner();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
