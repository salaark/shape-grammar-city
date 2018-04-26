// ModularBridge.cpp

#include "ModularBridge.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include <cmath>

#define MAX_ITERS 100
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
	d_tick = MAX_ITERS;
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
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Pillar1Obj(TEXT("/Game/EternalTemple/Environment/RailingsAndDecors/SM_Railing_Pillar_01"));
	pillar1Mesh = Pillar1Obj.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Pillar2Obj(TEXT("/Game/EternalTemple/Environment/RailingsAndDecors/SM_Railing_Pillar_02"));
	pillar2Mesh = Pillar2Obj.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BalconyObj(TEXT("/Game/EternalTemple/Environment/Bases/SM_Base_Balcony_02"));
	balconyMesh = BalconyObj.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArchObj(TEXT("/Game/EternalTemple/MeshParts/SM_Arch_02"));
	archMesh = ArchObj.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArchBaseObj(TEXT("/Game/EternalTemple/MeshParts/SM_Arch_02_Base"));
	archBaseMesh = ArchBaseObj.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArchTallBaseObj(TEXT("/Game/EternalTemple/MeshParts/SM_Arch_Tall_01_Base"));
	archTallBaseMesh = ArchTallBaseObj.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArchTallObj(TEXT("/Game/EternalTemple/MeshParts/SM_Arch_Tall_01"));
	archTallMesh = ArchTallObj.Object;
}

// Spawns the bridge between startLoc and endLoc
void AModularBridge::MakeBridge(FVector startLoc, FVector endLoc) {
	d_pos = startLoc;
	d_target = endLoc;
	float xdiff = d_target[0] - d_pos[0];
	float ydiff = d_target[1] - d_pos[1];
	d_dir = abs(xdiff) > abs(ydiff) ? (xdiff > 0 ? 0 : 2) : (ydiff > 0 ? 1 : 3);
	d_tick = 0;
}

// Called every frame to generate the bridge iteratively
void AModularBridge::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );

	if(d_tick < MAX_ITERS) {
		// Calculate distances to target in different dimensions
		float xdiff = d_target[0] - d_pos[0];
		float ydiff = d_target[1] - d_pos[1];
		float zdiff = d_target[2] - (d_height*400);
		// Calculate approximate remaining steps based on distance to target
		float remain = (abs(xdiff)/600 + abs(ydiff)/600) - 1;
		if(remain < 2) d_tick = MAX_ITERS - 1;
		// Convert z target distance to integer height value
		int hdiff = abs(zdiff) < 200 ? 0 : zdiff >= 1100 ? 3 : zdiff >= 600 ? 2 : zdiff >= 200 ? 1 : zdiff <= -1100 ? -3 : zdiff <= -600 ? -2 : -1;

		// Make up/down stairs if reaching the end and below/above target height OR randomly with no more than 1 stair per 3 iterations
		if((hdiff < 0 && remain <= -hdiff) || (remain > 4 && d_height > 1 && d_tick%3 == 0 && rand()%2 == 0)) {
			rule_stairs(-1);
		} else if(d_height == 0 || (hdiff > 0 && remain <= hdiff) || (remain > 4 && d_height < 3 && d_tick%3 == 0 && rand()%2 == 0)) {
			rule_stairs(1);
		} else {
			// Other than stairs, go straight and turn towards best target direction every (rand() % 3 + 2) iterations
			int target_dir = abs(xdiff) > abs(ydiff) ? (xdiff > 0 ? 0 : 2) : (ydiff > 0 ? 1 : 3);
			if(target_dir == d_dir || remain < 2 || d_tick % (rand() % 3 + 2) != 0) {
				rule_flat(0);
			} else if(target_dir == (d_dir + 1) % 4) {
				rule_flat(1);
			} else if(target_dir == (d_dir - 1) % 4) {
				rule_flat(-1);
			} else {
				// If going in complete opposite direction, turn in a reasonable manner to head towards target
				if(d_dir == 0) rule_flat(ydiff > 0 ? 1 : -1);
				else if(d_dir == 1) rule_flat(xdiff > 0 ? -1 : 1);
				else if(d_dir == 2) rule_flat(ydiff > 0 ? -1 : 1);
				else rule_flat(xdiff > 0 ? 1 : -1);
			}
		}
		update_pos();
		d_tick++;
	} else {
		SetActorTickEnabled(false);
	}
}

void AModularBridge::update_pos() {
	switch(d_dir) {
		case 0: d_pos[0] += grid_size; break;
		case 1: d_pos[1] += grid_size; break;
		case 2: d_pos[0] -= grid_size; break;
		case 3: d_pos[1] -= grid_size; break;
	}
}

void AModularBridge::rule_stairs(int diff) {
	if(diff > 0) d_height += diff;

	// Spawn stair mesh at correct height
	UStaticMeshComponent* stair = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *(FString::Printf(TEXT("Stairs_%i"), d_tick)));
	stair->SetStaticMesh(stairMesh);
	stair->SetCollisionObjectType(ECC_WorldStatic);
	int dx = 1;
	if(diff > 0) {
		stair->SetRelativeRotation(FRotator(0,90*d_dir,0));
	} else {
		stair->SetRelativeRotation(FRotator(0,(90*d_dir)-180,0));
		dx = -1;
	}
	switch(d_dir) {
		case 0: stair->SetRelativeLocation(d_pos+FVector(dx*300,0,400*(d_height-1))); break;
		case 1: stair->SetRelativeLocation(d_pos+FVector(0,dx*300,400*(d_height-1))); break;
		case 2: stair->SetRelativeLocation(d_pos+FVector(-dx*300,0,400*(d_height-1))); break;
		case 3: stair->SetRelativeLocation(d_pos+FVector(0,-dx*300,400*(d_height-1))); break;
	}
	stair->SetupAttachment(RootComponent);
	stair->RegisterComponent();

	// Spawn base mesh underneath stairs to reach current height
	for(int i = 1; i < d_height; i++) {
		UStaticMeshComponent* base = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *(FString::Printf(TEXT("BStair%i_%i"), i, d_tick)));
 		base->SetStaticMesh(baseMesh);
 		base->SetCollisionObjectType(ECC_WorldStatic);
 		base->SetRelativeLocation(FVector(-300,0,-400*i));
 		base->SetupAttachment(stair);
 		base->RegisterComponent();
	}

	// Spawn pillars and railings
	UStaticMeshComponent* rail1 = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *(FString::Printf(TEXT("SRail1_%i"), d_tick)));
	rail1->SetStaticMesh(stairRailMesh);
	rail1->SetCollisionObjectType(ECC_WorldStatic);
	rail1->SetRelativeLocation(FVector(0,300,400));
	rail1->SetRelativeRotation(FRotator(0,90,0));
	rail1->SetupAttachment(stair);
	rail1->RegisterComponent();

	UStaticMeshComponent* pillarb1 = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *(FString::Printf(TEXT("PillarB1_%i"), d_tick)));
	pillarb1->SetStaticMesh(pillar2Mesh);
	pillarb1->SetCollisionObjectType(ECC_WorldStatic);
	pillarb1->SetupAttachment(rail1);
	pillarb1->RegisterComponent();

	UStaticMeshComponent* rail2 = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *(FString::Printf(TEXT("SRail2_%i"), d_tick)));
	rail2->SetStaticMesh(stairRailMesh);
	rail2->SetCollisionObjectType(ECC_WorldStatic);
	rail2->SetRelativeLocation(FVector(0,-300,400));
	rail2->SetRelativeRotation(FRotator(0,90,0));
	rail2->SetupAttachment(stair);
	rail2->RegisterComponent();

	UStaticMeshComponent* pillarb2 = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *(FString::Printf(TEXT("PillarB2_%i"), d_tick)));
	pillarb2->SetStaticMesh(pillar2Mesh);
	pillarb2->SetCollisionObjectType(ECC_WorldStatic);
	pillarb2->SetupAttachment(rail2);
	pillarb2->RegisterComponent();

	// Spawn stair base pillars
	if(d_height == 1) {
		UStaticMeshComponent* pillara1 = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *(FString::Printf(TEXT("PillarA1_%i"), d_tick)));
		pillara1->SetStaticMesh(pillar1Mesh);
		pillara1->SetCollisionObjectType(ECC_WorldStatic);
		pillara1->SetRelativeLocation(FVector(0,600,-400));
		pillara1->SetupAttachment(rail1);
		pillara1->RegisterComponent();

		UStaticMeshComponent* pillara2 = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *(FString::Printf(TEXT("PillarA2_%i"), d_tick)));
		pillara2->SetStaticMesh(pillar1Mesh);
		pillara2->SetCollisionObjectType(ECC_WorldStatic);
		pillara2->SetRelativeLocation(FVector(0,600,-400));
		pillara2->SetupAttachment(rail2);
		pillara2->RegisterComponent();
	}

	if(diff < 0) d_height += diff;
}

void AModularBridge::rule_flat(int turn) {
	// Spawn base mesh depending on height (flat/arches)
	UStaticMeshComponent* base = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *(FString::Printf(TEXT("Flat_%i"), d_tick)));
	switch(d_height) {
		case 1: {
	 		base->SetStaticMesh(baseMesh);
			break; }
		case 2: {
			base->SetStaticMesh(archBaseMesh);
			break; }
		case 3: {
			base->SetStaticMesh(archTallBaseMesh);
			break; }
	}
	base->SetCollisionObjectType(ECC_WorldStatic);
	base->SetRelativeLocation(d_pos);
	base->SetRelativeRotation(FRotator(0,90*d_dir-90,0));
	base->SetupAttachment(RootComponent);
	base->RegisterComponent();

	if(d_height == 2 || d_height == 3) {
		UStaticMeshComponent* arch = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *(FString::Printf(TEXT("Arch_%i"), d_tick)));
		arch->SetCollisionObjectType(ECC_WorldStatic);
		arch->SetStaticMesh(d_height == 2 ? archMesh : archTallMesh);
		arch->SetupAttachment(base);
		arch->RegisterComponent();
	}

	//Spawn special railing if turning, otherwise spawn railings or balcony (max 1 balcony per 3 iterations)
	float rheight = d_height == 1 ? 400 : d_height == 2 ? 800 : d_height == 3 ? 1200 : 0;
	if(turn != 0) {
		UStaticMeshComponent* rail = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *(FString::Printf(TEXT("RailT_%i"), d_tick)));
 		rail->SetStaticMesh(railTurnMesh);
 		rail->SetCollisionObjectType(ECC_WorldStatic);
 		//rail->SetRelativeLocation(FVector(300,0,400+height));
 		if(turn > 0) {
 			rail->SetRelativeLocation(FVector(300,300,rheight));
 			rail->SetRelativeRotation(FRotator(0,-90,0));
 			d_dir = (d_dir+1)%4;
 		} else {
 			rail->SetRelativeLocation(FVector(-300,300,rheight));
 			rail->SetRelativeRotation(FRotator(0,0,0));
 			d_dir = (d_dir+3)%4;
 		}
 		rail->SetupAttachment(base);
 		rail->RegisterComponent();
	} else if(d_tick%3 == 1 && d_height == 1 && rand()%2 == 0) {
		UStaticMeshComponent* balcony1 = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *(FString::Printf(TEXT("Balcony1_%i"), d_tick)));
 		balcony1->SetStaticMesh(balconyMesh);
 		balcony1->SetCollisionObjectType(ECC_WorldStatic);
 		balcony1->SetRelativeRotation(FRotator(0,180,0));
 		balcony1->SetRelativeLocation(FVector(300,0,rheight-400));
 		balcony1->SetupAttachment(base);
 		balcony1->RegisterComponent();

 		UStaticMeshComponent* balcony2 = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *(FString::Printf(TEXT("Balcony2_%i"), d_tick)));
 		balcony2->SetStaticMesh(balconyMesh);
 		balcony2->SetCollisionObjectType(ECC_WorldStatic);
 		balcony2->SetRelativeLocation(FVector(-300,0,rheight-400));
 		balcony2->SetupAttachment(base);
 		balcony2->RegisterComponent();
	} else {
 		UStaticMeshComponent* rail1 = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *(FString::Printf(TEXT("Rail1_%i"), d_tick)));
 		rail1->SetStaticMesh(railMesh);
 		rail1->SetCollisionObjectType(ECC_WorldStatic);
 		rail1->SetRelativeLocation(FVector(300,0,rheight));
 		rail1->SetupAttachment(base);
 		rail1->RegisterComponent();

 		UStaticMeshComponent* rail2 = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *(FString::Printf(TEXT("Rail2_%i"), d_tick)));
 		rail2->SetStaticMesh(railMesh);
 		rail2->SetCollisionObjectType(ECC_WorldStatic);
 		rail2->SetRelativeLocation(FVector(-300,0,rheight));
 		rail2->SetupAttachment(base);
 		rail2->RegisterComponent();
 	}
}
