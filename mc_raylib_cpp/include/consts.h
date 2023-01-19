
#define PI 3.14159265358979323846f

#define WINDOW_WIDTH_START 	(1280)
#define WINDOW_HEIGHT_START (720)

#define MOUSE_SENSITIVITY (1.0 / 400.0)

#define CHUNK_SIZE (8) // blocks per chunk ^3

#define WORLD_SIZE (2) // chunks per world ^3

#define CUBE_VERTICES ({ \
	{0.0, 0.0, 0.0}, \
	{1.0, 0.0, 0.0}, \
	{1.0, 1.0, 0.0}, \
	{0.0, 1.0, 0.0}, \
	{0.0, 0.0, 1.0}, \
	{1.0, 0.0, 1.0}, \
	{1.0, 1.0, 1.0}, \
	{0.0, 1.0, 1.0}  \
})