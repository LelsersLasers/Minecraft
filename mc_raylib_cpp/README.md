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
	- Placing/destroying blocks
		- Raycast
		- `chunk.dirty = true` (rebuild chunk model)
		- Rebuild neighboring chunks
	- Water block
		- semi transparent
	- World gen
		- Perlin noise
		- Trees
	- Infinite world
		- World::chunks is an unordered map?
- Everything 2
	- Day/night cycle
		- Sun/moon
		- Skybox
		- Lighting
	- Infinite world
		- Hold only some chunks in memory
			- Live read/write to file
	- World gen
		- Biomes