/*
	ModularBridge.h
	Extends Actor and creates a bridge between two locations
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ModularBridge.generated.h"

UCLASS()
class ARCHIPLEAGOALPHA_API AModularBridge : public AActor {
	GENERATED_BODY()
	

private:
	int d_height;
	int d_dir;
	int d_tick;
	int d_turns;
	FVector d_pos;
	FVector d_target;

	UStaticMesh* baseMesh;
	UStaticMesh* railMesh;
	UStaticMesh* stairMesh;
	UStaticMesh* stairRailMesh;
	UStaticMesh* balconyMesh;

	UStaticMesh* railTurnMesh;
	UStaticMesh* pillar1Mesh;
	UStaticMesh* pillar2Mesh;

	UStaticMesh* archMesh;
	UStaticMesh* archBaseMesh;
	UStaticMesh* archTallMesh;
	UStaticMesh* archTallBaseMesh;


	void rule_stairs(int diff);
	void rule_flat(int turn);
	void update_pos();

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
