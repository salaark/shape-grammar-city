// ModularBuilding.cpp

#include "ModularBuilding.h"

// Sets default values
AModularBuilding::AModularBuilding() {
 	// Set this actor to never call Tick()
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	Base = CreateDefaultSubobject<UGrammarShape>(TEXT("Base"));
	Base->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Base->SetRelativeLocation(FVector(0, 0, 40));
	Base->expand();
}

// Called when the game starts or when spawned
void AModularBuilding::BeginPlay() {
	Super::BeginPlay();
}
