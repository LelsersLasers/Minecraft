# Minecraft ig

## BUGS:

- Neighboring chunks not marked as dirty

## TODO:

- Is Y or Z up?????????????????
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
		- TODO: BUG
			- Werid looking edges between chunks despite the fact that there are no faces/triangles
			- Possible fix: sort chunks by distance to camera (farthest first)
	- World gen
		- Perlin noise
		- Trees
			- Wooden logs
				- Directional blocks
	- Infinite world
		- World::chunks is an unordered map?
	- Glass blocks
		- Sort blocks within a chunk?
			- Or does it work already?
	- Textures
		- Texture atlas
		- Replaces colors
- Everything 2
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