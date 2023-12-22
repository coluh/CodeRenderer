#include "renderer.h"

int main(int argc, char* argv[]) {
	printf("\033[?25l");	//hide curser
	clock_t previousTime;
	previousTime = clock();
	POINT previousMousePos = { 0 };
	int height = 20;
	int width = 20 * 2 * HEIGHTBYWIDTH;
	PrepareScreen(&width, &height);
	Camera camera = {
		{3, 1, 1},
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
	//FILE* fp = fopen("data.blocks", "w");
	Block stone4 = { -1, -1, 0, 0b10100000u, NULL };
	Block stone3 = { 0, 0, 0, 0b10100000u, &stone4 };
	Block stone2 = { 0, 0, 1, 0b10100000u, &stone3 };
	Block stone1 = { 0, 1, 0, 0b10100000u,&stone2 };
	Block stone = { 1, -1, 1, 0b10100000u, &stone1 };
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
				float xRate = col / (float)width;
				float yRate = row / (float)height;
				Ray ray = GenRay(&camera, xRate, yRate);
				Vector3 interPosition;	//要分清楚坐标和用坐标表示的方向啊!!
				Vector3 interNormal;	//噫嘘唏!!	这个是方向, 不是坐标也!!
				if (RayHitBlock(ray, &stone, &interPosition, &interNormal)) {
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
		ControlFPS(&previousTime);
	}
	//fclose(fp);
	return 0;
}