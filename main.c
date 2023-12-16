#include "renderer.h"

int main(int argc, char* argv[]) {
	printf("\033[?25l");
	clock_t previousTime;
	previousTime = clock();
	POINT previousMousePos = { 0 };
	int height = 20;
	int width = 50*2;
	Camera camera = {
		{7, 0, 0},
		{0, 0, 0},
		{0, 0, 1},
		PI / 2.0f,
		(float)width / (float)height,
		0.5f,
		FIRST_PERSON
	};
	Vector3 sunPos = { 10, 0, 5 };
	float theta = 0;
	Vector3 ballCenter = { 0, 0, 0 };
	float ballRadius = 3.0f;
	while (true) {
		printf("\x1B[1;1H\x1B[1;32;40m");
		sunPos.x = 10 * cosf(theta);
		sunPos.y = 10 * sinf(theta);
		for (int row = 0; row < height; ++row) {
			for (int col = 0; col < width; ++col) {
				float xRate = col / (float)width;
				float yRate = row / (float)height;
				Ray ray = GenRay(&camera, xRate, yRate);
				Vector3 interPosition;
				Vector3 interNormal;
				bool hitted = RayHitBall(ray, ballCenter, ballRadius, &interPosition, &interNormal);
				if (hitted) {
					float diffuse = reluf(Dot(Normalize(Sub(sunPos, interPosition)), interNormal));
					pr(diffuse);
				}
				else
				{
					printf(" ");
				}
			}
			printf("\n");
		}
		theta += 0.1f;
		CameraMove(&camera, &previousMousePos, 400);
		ControlFPS(&previousTime);
	}
	return 0;
}