#include "renderer.h"
void CameraMove(Camera* camera, POINT* preMousePos, int mouseSensi) {
	if (GetKeyState('W') < 0) {
		Vector3 d = Multi(Normalize(Sub(camera->target, camera->position)), 0.1f);
		camera->position = Add(camera->position, d);
		camera->target = Add(camera->target, d);
	}
	if (GetKeyState('S') < 0) {
		Vector3 d = Multi(Normalize(Sub(camera->target, camera->position)), -0.1f);
		camera->position = Add(camera->position, d);
		camera->target = Add(camera->target, d);
	}
	if (GetKeyState('A') < 0) {
		Vector3 d = Cross(camera->up, Sub(camera->target, camera->position));
		d = Multi(Normalize(d), 0.1f);
		camera->position = Add(camera->position, d);
		camera->target = Add(camera->target, d);
	}
	if (GetKeyState('D') < 0) {
		Vector3 d = Cross(camera->up, Sub(camera->target, camera->position));
		d = Multi(Normalize(d), -0.1f);
		camera->position = Add(camera->position, d);
		camera->target = Add(camera->target, d);
	}
	if (GetKeyState('C') < 0) {
		if (camera->mode == FIRST_PERSON)
			camera->mode = THIRD_PERSON;
		else
			camera->mode = FIRST_PERSON;
	}
	if (GetKeyState(VK_SPACE) < 0) {
		Vector3 d = Multi(Normalize(camera->up), 0.1f);
		camera->position = Add(camera->position, d);
		camera->target = Add(camera->target, d);
	}
	if (GetKeyState(VK_LSHIFT) < 0) {
		Vector3 d = Multi(Normalize(camera->up), -0.1f);
		camera->position = Add(camera->position, d);
		camera->target = Add(camera->target, d);
	}
	if (camera->mode == FIRST_PERSON || (camera->mode == THIRD_PERSON && GetAsyncKeyState(MK_LBUTTON) < 0)) {
		if (preMousePos->x == 0 && preMousePos->y == 0) {
			GetCursorPos(preMousePos);
		}
		POINT currentMousePos;
		GetCursorPos(&currentMousePos);
		float offsetH = -(float)(currentMousePos.x - preMousePos->x) / mouseSensi * 2 * PI;//Horizontal
		float offsetV = -(float)(currentMousePos.y - preMousePos->y) / mouseSensi * 2 * PI;//Vertical
		if (camera->mode == THIRD_PERSON) {
			offsetH *= -1;
			offsetV *= -1;
		}
		Vector3 cameraZ = Normalize(Sub(camera->target, camera->position));
		Vector3 cameraY = Normalize(camera->up);
		Vector3 cameraX = Normalize(Cross(cameraY, cameraZ));
		Vector3 direction = Add(Multi(cameraZ, cosf(offsetV)), Multi(cameraY, sinf(offsetV)));
		camera->up = Normalize(Cross(direction, cameraX));
		direction = Add(direction, Add(Multi(cameraZ, cosf(offsetH)), Multi(cameraX, sinf(offsetH))));
		camera->target = Add(camera->position, direction);
		preMousePos->x = currentMousePos.x;
		preMousePos->y = currentMousePos.y;
	}
	else {
		preMousePos->x = 0;
		preMousePos->y = 0;
	}
}