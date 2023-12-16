#include "renderer.h"

int main(int argc, char* argv[]) {
	printf("\033[?25l");
	clock_t previousTime;
	previousTime = clock();
	int height = 30;
	int width = 50;
	Camera camera = {
		{0, 1, -2},
		{0, 0, 0},
		{0, 1, 1},
		PI / 2.0f,
		(float)width / (float)height,
		0.5f
	};
	Vector3 lightPos = { 5, 5, -5 };
	Vector3 ballCenter = { 0, 0, 0 };
	float ballRadius = 1.2;
	while (true) {
		printf("\x1B[1;1H\x1B[1;32;40m");
        for (int row = 0; row < height; ++row){
            for (int col = 0; col < width; ++col){
                float xRate = col / (float)width;
                float yRate = 1 - row / (float)height;
                Ray ray = GenRay(&camera, xRate, yRate);
                Vector3 interPosition;
                Vector3 interNormal;
                bool hitted = RayHitBall(ray, ballCenter, ballRadius, &interPosition, &interNormal);
                if (hitted){
					float diffuse = fabsf(Dot(Normalize(Sub(lightPos, interPosition)), interNormal));
					pr(diffuse);
					pr(diffuse);
                }
                else
                {
					printf(" ");
					printf(" ");
                }
            }
			printf("\n");
        }
		ControlFPS(&previousTime);
	}
	return 0;
}