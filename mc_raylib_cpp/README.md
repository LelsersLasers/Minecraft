# Minecraft ig

## BUGS:

- Leaves and water??

## TODO 1 (aka things I might actually do)

- C++ stuff
    - Constructor initalization list
- World gen
	- Trilinear interpolation??
	- Lower resolution Z noise?
- Performance
	- Profile??
		- `gprof build/mainWithProfile  > gprofOutput.txt`

## TODO 2 (aka everything I don't actually want to do)

- When under water, show the "top" of the water
- Glass blocks
	- Sort blocks within a chunk?
		- Or does it work already?
	- New mesh?
- Directional blocks
	- Wooden logs
- Working player
	- Collision physics
	- Player::IsSubmerged()
- Inventory
	- Hotbar
	- Break blocks -> add to inventory
		- Or maybe "float" above ground like in the actual game
	- Place blocks -> remove from inventory
- Day/night cycle
	- Sun/moon
	- Skybox
	- Lighting
- Infinite world
	- Hold only some chunks in memory
		- Live read/write to file
- World gen
	- Biomes based on noise maps
		- Ex: humidity and temperature
	- 3d perlin noise
	- Height made from multiple noise maps with different setups (octaves, etc)
		- +spline interpolation (or something like that)
	- Caves
- Fog
	- For far away chunks
- Flowing water
- Falling sand blocks
- "Ticks"
	- Dirt -> grass
	- Etc
- Preformance
	- Unload models from GPU when not in view
	- Store chunk block data as a list of indexes to a vector of blocks
		- Avoids repeating/duplicating block info
	- Block has array[6] of Vector2 texcoords (indexed by Dir) instead of getTexcoordsFn()/getTexcoords() ??
	- Enhanced (range based) for loops??
	- Global texture instead of per model?