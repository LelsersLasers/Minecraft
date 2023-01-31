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
	- Water block
		- semi transparent
		- If top is air block, water block is less tall
		- Improve chunk sorting/ordering
			- When to sort
	- World gen
		- Perlin noise
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