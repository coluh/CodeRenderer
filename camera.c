#include "renderer.h"
void CameraMove(Camera* camera, POINT* preMousePos, int mouseSensi) {
	if (GetKeyState('W') < 0) {
		Vector3 d = Multi(camera->target, 0.1f);
		camera->position = Add(camera->position, d);
	}
	if (GetKeyState('S') < 0) {
		Vector3 d = Multi(camera->target, -0.1f);
		camera->position = Add(camera->position, d);
	}
	if (GetKeyState('A') < 0) {
		Vector3 d = Cross(camera->up, camera->target);
		d = Multi(Normalize(d), 0.1f);
		camera->position = Add(camera->position, d);
	}
	if (GetKeyState('D') < 0) {
		Vector3 d = Cross(camera->up, camera->target);
		d = Multi(Normalize(d), -0.1f);
		camera->position = Add(camera->position, d);
	}
	if (GetKeyState('C') < 0) {
		if (camera->mode == FIRST_PERSON)
			camera->mode = THIRD_PERSON;
		else
			camera->mode = FIRST_PERSON;
	}
	if (GetKeyState(VK_SPACE) < 0)
		camera->position.z += 0.1f;
	if (GetKeyState(VK_LSHIFT) < 0)
		camera->position.z -= 0.1f;
	if (camera->mode == FIRST_PERSON || (camera->mode == THIRD_PERSON && GetAsyncKeyState(MK_LBUTTON) < 0)) {
		if (preMousePos->x == 0 && preMousePos->y == 0) {
			GetCursorPos(preMousePos);
		}
		POINT currentMousePos;
		GetCursorPos(&currentMousePos);
		float offsetH = (float)(currentMousePos.x - preMousePos->x) / mouseSensi * 2 * PI;//Horizontal
		float offsetV = (float)(currentMousePos.y - preMousePos->y) / mouseSensi * 2 * PI;//Vertical
		if (camera->mode == THIRD_PERSON) {
			offsetH *= -1;
			offsetV *= -1;
		}
		Vector3 cameraZ = Normalize(camera->target);
		Vector3 cameraY = Normalize(camera->up);
		Vector3 cameraX = Normalize(Cross(cameraY, cameraZ));
		//固定y, 移动x与z轴
		cameraZ = Normalize(Sub(Multi(cameraZ, cosf(offsetH)), Multi(cameraX, sinf(offsetH))));
		cameraX = Normalize(Cross(cameraY, cameraZ));
		//固定x, 移动z与y轴
		cameraZ = Normalize(Sub(Multi(cameraZ, cosf(offsetV)), Multi(cameraY, sinf(offsetV))));
		cameraY = Normalize(Cross(cameraZ, cameraX));
		//apply to target and up
		camera->target = cameraZ;
		camera->up = cameraY;
		preMousePos->x = currentMousePos.x;
		preMousePos->y = currentMousePos.y;
	}
	else {
		preMousePos->x = 0;
		preMousePos->y = 0;
	}
}