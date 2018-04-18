// ModularBridge.cpp

#include "ModularBridge.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"

const int grid_size = 600;

// Sets default values
AModularBridge::AModularBridge() {
 	// Set this actor to call Tick()
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	d_dir = 0;
	d_pos = FVector(0,0,0);
	d_height = 0;
	d_tick = 100;
	d_turns = 0;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseObj(TEXT("/Game/EternalTemple/Environment/Bases/SM_Base_01"));
	baseMesh = BaseObj.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RailObj(TEXT("/Game/EternalTemple/Environment/RailingsAndDecors/SM_Railing_01"));
	railMesh = RailObj.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StairObj(TEXT("/Game/EternalTemple/Environment/Staircases/SM_Staircase_01"));
	stairMesh = StairObj.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SRailObj(TEXT("/Game/EternalTemple/Environment/RailingsAndDecors/SM_Railing_Staircase_01"));
	stairRailMesh = SRailObj.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RailTurnObj(TEXT("/Game/EternalTemple/Environment/RailingsAndDecors/SM_Railing_Corner_02"));
	railTurnMesh = RailTurnObj.Object;
	/*static ConstructorHelpers::FObjectFinder<UStaticMesh> Pillar1Obj(TEXT("/Game/EternalTemple/Environment/RailingsAndDecors/SM_Railing_Pillar_01"));
	pillar1Mesh = Pillar1Obj.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Pillar2Obj(TEXT("/Game/EternalTemple/Environment/RailingsAndDecors/SM_Railing_Pillar_02"));
	pillar2Mesh = Pillar2Obj.Object;*/
}

// Spawns the bridge between startLoc and endLoc
void AModularBridge::MakeBridge(FVector startLoc, FVector endLoc) {
	d_pos = startLoc;
	rule_stairs();
	update_pos(0.5);
	d_tick = 0;
}

// Called every frame
void AModularBridge::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );

	if(d_tick < 6) {
	    if(d_turns < 2 && rand()%3 == 0) {
			rule_turn(rand()%2 == 0);
			d_turns++;
		} else {
			rule_straight();
		}
		update_pos(1);
		d_tick++;
	}
}

void AModularBridge::update_pos(float scale) {
	switch(d_dir) {
		case 0: d_pos[0] += grid_size*scale; break;
		case 1: d_pos[1] += grid_size*scale; break;
		case 2: d_pos[0] -= grid_size*scale; break;
		case 3: d_pos[1] -= grid_size*scale; break;
	}
}

void AModularBridge::rule_stairs() {
	d_height++;
	switch(d_height) {
		case 1: {
			UStaticMeshComponent* base = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *(FString::Printf(TEXT("Stairs_%i"), d_tick)));
	 		base->SetStaticMesh(stairMesh);
	 		base->SetCollisionObjectType(ECC_WorldStatic);
	 		base->SetRelativeLocation(d_pos);
	 		base->SetRelativeRotation(FRotator(0,90*d_dir,0));
	 		base->SetupAttachment(RootComponent);
	 		base->RegisterComponent();

	 		UStaticMeshComponent* rail1 = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *(FString::Printf(TEXT("SRail1_%i"), d_tick)));
	 		rail1->SetStaticMesh(stairRailMesh);
	 		rail1->SetCollisionObjectType(ECC_WorldStatic);
	 		rail1->SetRelativeLocation(FVector(0,300,400));
	 		rail1->SetRelativeRotation(FRotator(0,90,0));
	 		rail1->SetupAttachment(base);
	 		rail1->RegisterComponent();

	 		UStaticMeshComponent* rail2 = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *(FString::Printf(TEXT("SRail2_%i"), d_tick)));
	 		rail2->SetStaticMesh(stairRailMesh);
	 		rail2->SetCollisionObjectType(ECC_WorldStatic);
	 		rail2->SetRelativeLocation(FVector(0,-300,400));
	 		rail2->SetRelativeRotation(FRotator(0,90,0));
	 		rail2->SetupAttachment(base);
	 		rail2->RegisterComponent();
			break; }
	}
}

void AModularBridge::rule_straight() {
	switch(d_height) {
		default: {
			UStaticMeshComponent* base = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *(FString::Printf(TEXT("Straight_%i"), d_tick)));
	 		base->SetStaticMesh(baseMesh);
	 		base->SetCollisionObjectType(ECC_WorldStatic);
	 		base->SetRelativeLocation(d_pos);
	 		base->SetRelativeRotation(FRotator(0,90*d_dir,0));
	 		base->SetupAttachment(RootComponent);
	 		base->RegisterComponent();

	 		UStaticMeshComponent* rail1 = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *(FString::Printf(TEXT("Rail1_%i"), d_tick)));
	 		rail1->SetStaticMesh(railMesh);
	 		rail1->SetCollisionObjectType(ECC_WorldStatic);
	 		rail1->SetRelativeLocation(FVector(0,300,400));
	 		rail1->SetRelativeRotation(FRotator(0,90,0));
	 		rail1->SetupAttachment(base);
	 		rail1->RegisterComponent();

	 		UStaticMeshComponent* rail2 = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *(FString::Printf(TEXT("Rail2_%i"), d_tick)));
	 		rail2->SetStaticMesh(railMesh);
	 		rail2->SetCollisionObjectType(ECC_WorldStatic);
	 		rail2->SetRelativeLocation(FVector(0,-300,400));
	 		rail2->SetRelativeRotation(FRotator(0,90,0));
	 		rail2->SetupAttachment(base);
	 		rail2->RegisterComponent();
			break; }
	}
}

void AModularBridge::rule_turn(bool turnRight) {
	switch(d_height) {
		default: {
			UStaticMeshComponent* base = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *(FString::Printf(TEXT("Turn_%i"), d_tick)));
	 		base->SetStaticMesh(baseMesh);
	 		base->SetCollisionObjectType(ECC_WorldStatic);
	 		base->SetRelativeLocation(d_pos);
	 		base->SetRelativeRotation(FRotator(0,90*d_dir,0));
	 		base->SetupAttachment(RootComponent);
	 		base->RegisterComponent();

	 		UStaticMeshComponent* rail = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *(FString::Printf(TEXT("RailT_%i"), d_tick)));
	 		rail->SetStaticMesh(railTurnMesh);
	 		rail->SetCollisionObjectType(ECC_WorldStatic);
	 		if(turnRight) {
	 			rail->SetRelativeLocation(FVector(300,-300,400));
	 			rail->SetRelativeRotation(FRotator(0,180,0));
	 		} else {
	 			rail->SetRelativeLocation(FVector(300,300,400));
	 			rail->SetRelativeRotation(FRotator(0,-90,0));
	 		}
	 		rail->SetupAttachment(base);
	 		rail->RegisterComponent();
			break; }
	}
	d_dir = turnRight ? (d_dir+1)%4 : (d_dir+3)%4;
}
