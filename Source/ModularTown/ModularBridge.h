/*
	ModularBridge.h
	Extends Actor and creates a bridge between two locations
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ModularBridge.generated.h"

UCLASS()
class MODULARTOWN_API AModularBridge : public AActor {
	GENERATED_BODY()
	

private:
	int d_height;
	int d_dir;
	FVector d_pos;
	int d_tick;
	int d_turns;

	UStaticMesh* baseMesh;
	UStaticMesh* railMesh;
	UStaticMesh* stairMesh;
	UStaticMesh* stairRailMesh;

	UStaticMesh* railTurnMesh;
	//UStaticMesh* pillar1Mesh;
	//UStaticMesh* pillar2Mesh;

	void rule_stairs();
	void rule_straight();
	void rule_turn(bool turnRight);
	void update_pos(float scale);

public:	
	// Sets default values for this actor's properties
	AModularBridge();

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Root component is punlic
	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;

	// Function that spawns the bridge
	UFUNCTION(BlueprintCallable)
	void MakeBridge(FVector startLoc, FVector endLoc);
};
