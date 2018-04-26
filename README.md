# Procedural City and Bridges
Generates procedural cities and bridges for the game [Sail and Sacrifice](http://store.steampowered.com/app/792940/Sail_And_Sacrifice) using [Eternal Temple](https://www.unrealengine.com/marketplace/eternal-temple) assets. Made using Unreal Engine’s C++ API.  

For source files, see Source/ModularTown/ModularBuilding, ModularBridge, GrammarShape, CitySpawner, and SimplexNoise.  

## City Generation
As shown in the video, the city generation queries the landscape with a designer-specified range and cell size, placing buildings where the landscape allows. The buildings are oriented toward the center of the city and scaled according to height (smaller when higher up). All transformations are set relative to a grid size (600 for this pack). The meshes can easily be replaced with any modular mesh components for walls, entrances, etc to create a variety of styles. Integrating the city generation with my existing game project automatically adds height variation, dynamic terrain textures, and procedural foliage. 

![citygen](Images/CityGen.gif)

## Building Grammar
The building grammar involves the placement of different types of floors, walls, roofs, and columns. Buildings also include procedurally placed torches to light up the interiors as well as beds and chairs that NPCs can sit and sleep in depending on the time of day during gameplay. Each building has randomly placed entrances with one guaranteed entrance in the front of the building. There are five possible types of buildings with 1-5 rooms.   

![citywalk](Images/CityWalk.gif)

## Bridge Generation
Generates a multi-level bridge from a given start position to a given target position (including desired height). The bridge tries to have a reasonable number of turns and staircases while reaching the target position. The geometry placed includes different types of base blocks, railings, balconies, staircases, columns, and arches. Having a start and end location allows for designer freedom in placement of bridges that conform to gameplay needs.  

![bridgewalk1](Images/BridgeWalk1.gif)
![bridgewalk2](Images/BridgeWalk2.gif)
