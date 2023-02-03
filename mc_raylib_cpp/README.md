# Minecraft ig

## BUGS:

## TODO:

- C++ stuff
	- When to return/take a reference?
	- Better usage of pointers
		- Avoid creating object and then reassinging it
		- Can create some objects as `Type* var = new Type(args)`
	- Improve iterating
		- Mostly iterating over World::chunks
		- Enhanced for loops?
- Replace Vector3/2Util with raymath
- Block has array[6] of Colors (indexed by Dir) instead of getColorFn()/getColor()
- Everything
	- World gen
		- Perlin noise
		- "Instead of sampling the Perlin noise in two dimensions try sampling it in three and placing a cube wherever it’s greater than some threshold. It’s not as simple as a basic mesh but it lets you create complex features like caves and overhangs."
		- Octaves
		- More than just grass???
	- Trees
		- Wooden logs
			- Directional blocks
		- Leaves
			- Semi transparent
	- Infinite world
		- World::chunks is an unordered map?
	- Textures
		- Texture atlas
		- Replaces colors
	- Preformance
		- View distance
		- Skip "blank" chunks
		- Only raycast neighboring chunks
- Everything 2
	- Glass blocks
		- Sort blocks within a chunk?
			- Or does it work already?
	- Working player
		- Collision physics
		- Player::IsSubmerged()
	- Day/night cycle
		- Sun/moon
		- Skybox
		- Lighting
	- Infinite world
		- Hold only some chunks in memory
			- Live read/write to file
	- World gen
		- Biomes
	- Fog
		- For far away chunks
	- Flowing water
	- Falling sand blocks