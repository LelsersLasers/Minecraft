# Minecraft ig

## BUGS:

- Don't place leaves inside of other blocks

## TODO:

- C++ stuff
	- When to return/take a reference?
	- Better usage of pointers
		- Avoid creating object and then reassinging it
		- Can create some objects as `Type* var = new Type(args)`
	- Pointers vs references vs reference_wrapper
	- Improve iterating
		- Enhanced for loops?
		- RN: Loop through chunkOrder multiple times
	- Add `f` to floats
- Block has array[6] of Colors (indexed by Dir) instead of getColorFn()/getColor()
- Everything
	- World gen
		- 2d Perlin noise
		- Octaves
		- Basic biomes
	- Trees
		- With leaves
			- Semi transparent
	- Textures
		- Texture atlas
		- Replaces colors
- Everything 2
    - When under water, show the "top" of the water
	- Glass blocks
		- Sort blocks within a chunk?
			- Or does it work already?
	- Directional blocks
		- Wooden logs
	- Working player
		- Collision physics
		- Player::IsSubmerged()
	- Inventory
		- Hotbar
		- Break blocks -> add to inventory
		- Place blocks -> remove from inventory
	- Day/night cycle
		- Sun/moon
		- Skybox
		- Lighting
	- Infinite world
		- Hold only some chunks in memory
			- Live read/write to file
	- World gen
		- More biomes
        - 3d perlin noise
			- "Instead of sampling the Perlin noise in two dimensions try sampling it in three and placing a cube wherever it’s greater than some threshold. It’s not as simple as a basic mesh but it lets you create complex features like caves and overhangs."
        - Made from 5 diiferent noise setups
            - +spline interpolation (or something like that)
        - Caves
	- Fog
		- For far away chunks
	- Flowing water
	- Falling sand blocks
	- Preformance
		- Unload models from GPU when not in view
		- Store chunk block data as a list of indexes to a vector of blocks
			- Avoids repeating/duplicating block info
