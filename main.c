﻿#include "renderer.h"

int main(int argc, char* argv[]) {
	printf("\033[?25l");	//hide curser
	ShowCursor(false);
	clock_t previousTime;
	previousTime = clock();
	POINT previousMousePos = { 0 };
	short previousButtonState[2] = { -1,-1 };
	char* buffer = (char*)malloc(4096 * sizeof(char));
	setvbuf(stdout, buffer, _IOFBF, 4096);
	int height = 20;
	int width = 20 * 2 * HEIGHTBYWIDTH;
	PrepareScreen(&width, &height);
	Camera camera = {
		{2, 1, 2},
		{0, 0, 0},
		{0, 0, 1},
		PI / 2.0f,
		(float)width / (float)height,
		0.5f,
		FIRST_PERSON
	};
	camera.target = Normalize(Sub(camera.target, camera.position));
	AddLight(70, 40, 50, LIGHT_POINT);
	AddLight(1, -1, 1, LIGHT_PARALLEL);
	AddLight(-10, -10, -5, LIGHT_PARALLEL);
	if (!ReadBlock()) {
		BuildBaseBlock();
		camera.position = Add(camera.position, (Vector3) { 3, -1, 3 });
	}
	while (true) {
		printf("\x1B[1;1H");
		printf("Pos: ( %+.1f, %+.1f, %+.1f )", camera.position.x, camera.position.y, camera.position.z);
		printf("\x1B[1;35;47m\tFacing:(%+.2f,%+.2f,%+.2f)\tUp:(%+.2f,%+.2f,%+.2f)\tDegree:%+.2f", \
			camera.target.x, camera.target.y, camera.target.z, \
			camera.up.x, camera.up.y, camera.up.z, Dot(camera.target, camera.up));
		printf("\n\x1B[1;32;40m");
		// Block stone
		for (int row = 0; row < height; ++row) {
			for (int col = 0; col < width; ++col) {
				if (row == height / 2 && col == width / 2) {
					printf("\x1B[1;37;40m+\x1B[1;32;40m");
					continue;
				}
				float xRate = col / (float)width;
				float yRate = row / (float)height;
				Ray ray = GenRay(&camera, xRate, yRate);
				Vector3 interPosition;	//要分清楚坐标和用坐标表示的方向啊!!
				Vector3 interNormal;	//噫嘘唏!!	这个是方向, 不是坐标也!!
				if (RayHitBlock(ray, &interPosition, &interNormal)) {
					float diff = CalculateBrightness(interPosition, interNormal);
					pr(diff);
				}
				else {
					printf(" ");
				}
			}
			printf("\n");
		}
		CameraMove(&camera, &previousMousePos, 400);
		BlockChange(&camera, previousButtonState);
		ControlFPS(&previousTime);
	}
	//fclose(fp);
	return 0;
}