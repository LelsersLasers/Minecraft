# Minecraft ig

## TODO:

- C++ stuff
	- When to return/take a reference?
	- Better usage of pointers
		- Avoid creating object and then reassinging it
		- Can create some objects as `Type* var = new Type(args)`
	- Enhanced for loops?
- Replace Vector3/2Util with raymath
- Everything
	- Water block
		- semi transparent
		- If submerged: blue tint over everything
		- If top is air block, water block is less tall
	- World gen
		- Perlin noise
		- Trees
	- Infinite world
		- World::chunks is an unordered map?
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
	- Flowing water
	- Falling sand blocks