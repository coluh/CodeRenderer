#include "renderer.h"

int main(int argc, char* argv[]) {
	printf("\033[?25l");
	clock_t previousTime;
	previousTime = clock();
	POINT previousMousePos = { 0 };
	int height = 20;
	int width = 50*2;
	Camera camera = {
		{7, 0, 1},
		{0, 0, 0},
		{0, 0, 1},
		PI / 2.0f,
		(float)width / (float)height,
		0.5f,
		FIRST_PERSON
	};
	Vector3 sunPos = { 7, 0, 5 };
	//FILE* fp = fopen("data.blocks", "w");
	Block stone = { 1,3,0,0b10100000u };
	while (true) {
		printf("\x1B[1;1H");
		printf("Pos: ( %.1f, %.1f, %.1f )\n\x1B[1;32;40m", camera.position.x, camera.position.y, camera.position.z);
		// Block stone
		for (int row = 0; row < height; ++row) {
			for (int col = 0; col < width; ++col) {
				float xRate = col / (float)width;
				float yRate = row / (float)height;
				Ray ray = GenRay(&camera, xRate, yRate);
				Vector3 interPosition;
				Vector3 interNormal;
				if (RayHitBlock(ray, stone, &interPosition, &interNormal)) {
					float diff = reluf(Dot(interNormal, Normalize(Sub(sunPos, interPosition))));
					pr(diff);
				}
				else {
					printf(" ");
				}
			}
			printf("\n");
		}
		CameraMove(&camera, &previousMousePos, 400);
		ControlFPS(&previousTime);
	}
	//fclose(fp);
	return 0;
}