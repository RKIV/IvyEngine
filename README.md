# IvyEngine
This is meant to be an exploration in graphics accelerated 3D rendering and building game engines. The end goal is to rebuild [Mogu](https://mfroehlich.itch.io/mogu) 
(originally built in Unity by me and a couple friends) in this new engine. Since Mogu is predominantly 2D with no complex shadowing or lighting, the engine will most
likely be similarly simple.
## Architecture
At the moment the architecture is a hodge-podge of various other engines I've worked in, predominantly Unreal and Godot.
### Leafs
Everything in the world that has a position is a Leaf. Every Leaf in the engine is a child of the RootLeaf or some other Leaf
### Components
Components can be attached to Leafs to encapsulate reusable functionality. Components don't have a location in the world however they can control the location of the leaf they are attached to
