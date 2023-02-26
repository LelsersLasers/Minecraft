#include "include/PerlinNoise.h"

#include "include/PerlinNoiseUtil.h"
#include <cmath>

double PerlinNoise3DWithOctaves(
	PerlinNoise& pn,
	double x,
	double y,
	double z,
	int octaves
) {
	double result = 0.0;
	double amp = 1.0;
	double freq = 1.0;
	double max = 0.0;

	for (int i = 0; i < octaves; i++) {
		result += pn.noise(x * freq, y * freq, z * freq) * amp;
		max += amp;
		amp /= 2.0;
		freq *= 2.0;
	}

	return result / max;
}

// double PerlinNoise3DWithOctaves(
// 	PerlinNoise& pn,
// 	double x,
// 	double y,
// 	double z,
// 	int octaves
// ) {
// 	double result = 0.0;
// 	for (int i = 0; i < octaves; i++) {
// 		double divisor = pow(2.0, (double)i);

// 		double noise = pn.noise(
// 			x * divisor,
// 			y * divisor,
// 			z * divisor
// 		);
// 		// shift from (0, 1) to (-1, 1)
// 		double shiftedNoise = 2.0 * (noise - 0.5);
// 		double scaledNoise = shiftedNoise / divisor;
// 		result += scaledNoise;
// 	}
	
// 	// shift from (-1, 1) to (0, 1)
// 	result = 0.5 * (result + 1.0);

// 	return result;
// }