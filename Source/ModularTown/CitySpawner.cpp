// CitySpawner.cpp

#include "CitySpawner.h"
#include "ModularBuilding.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "SimplexNoise.h"

const int max_size = 25000;
const int grid_size = 600;

// Sets default values
ACitySpawner::ACitySpawner() {
 	// Set this actor to never call Tick().
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
}

// Called when the game starts or when spawned
void ACitySpawner::BeginPlay() {
	Super::BeginPlay();

	if(GetWorld() == nullptr) return;
	USimplexNoise::setNoiseSeed(15);
	for(int x = max_size/-2; x < max_size/2; x += grid_size*6) {
		for(int y = max_size/-2; y < max_size/2; y += grid_size*6) {
			float noise = USimplexNoise::SimplexNoise2D(x,y);
			if(noise > 0.2 && noise < 0.9) {
				FVector myLoc(x+this->GetActorLocation().X,y+this->GetActorLocation().Y,this->GetActorLocation().Z);
				FRotator myRot(0,rand()%360,0);
				FActorSpawnParameters SpawnInfo;

				/*
				bool bNoCollisionFail  = false;
				SpawnInfo.SpawnCollisionHandlingOverride = bNoCollisionFail ?
					ESpawnActorCollisionHandlingMethod::AlwaysSpawn : ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
				*/

				AModularBuilding* NewBuilding = GetWorld()->SpawnActor<AModularBuilding>(AModularBuilding::StaticClass(), myLoc, myRot, SpawnInfo);
				NewBuilding->SetActorRelativeScale3D(FVector(noise*3.3,noise*3.3,noise*3.3));
				NewBuilding->SetActorEnableCollision(true);
				NewBuilding->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			}
		}
	}
	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Spawned City!"));
}

// Called every frame
void ACitySpawner::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

