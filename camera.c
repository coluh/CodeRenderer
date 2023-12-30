#include "renderer.h"
void PrepareScreen(int* w, int* h) {
	printf("   80 * 20\n  120 * 30\n  160 * 40\n  200 * 50\n\n\x1B[1;37;42m");
	char ch;
	int height = *h;
	do {
		printf("\x1B[0m\x1B[1;1H\x1B[2J");
		printf("\x1B[1;33;40mPlease zoom the font to a suitable size. j and k to choose. SPACE to confirm.\n");
		printf("\x1B[1;37;42m");//change to green
		switch (height) {
		case 20:
			for (int i = 0; i < 80; i++)printf("*");
			//\x1B[1;35;40m
			printf("\n*\x1B[1;35;46m\t   80 * 20  \x1B[1;37;42m");
			printf("\n*\x1B[1;35;40m\t  120 * 30  \x1B[1;37;42m");
			printf("\n*\x1B[1;35;40m\t  160 * 40  \x1B[1;37;42m");
			printf("\n*\x1B[1;35;40m\t  200 * 50  \x1B[1;37;42m");
			for (int i = 1; i < 16; i++)printf("\n*");
			break;
		case 30:
			for (int i = 0; i < 120; i++)printf("*");
			printf("\n*\x1B[1;35;40m\t   80 * 20  \x1B[1;37;42m");
			printf("\n*\x1B[1;35;46m\t  120 * 30  \x1B[1;37;42m");
			printf("\n*\x1B[1;35;40m\t  160 * 40  \x1B[1;37;42m");
			printf("\n*\x1B[1;35;40m\t  200 * 50  \x1B[1;37;42m");
			for (int i = 1; i < 26; i++)printf("\n*");
			break;
		case 40:
			for (int i = 0; i < 160; i++)printf("*");
			printf("\n*\x1B[1;35;40m\t   80 * 20  \x1B[1;37;42m");
			printf("\n*\x1B[1;35;40m\t  120 * 30  \x1B[1;37;42m");
			printf("\n*\x1B[1;35;46m\t  160 * 40  \x1B[1;37;42m");
			printf("\n*\x1B[1;35;40m\t  200 * 50  \x1B[1;37;42m");
			for (int i = 1; i < 36; i++)printf("\n*");
			break;
		case 50:
			for (int i = 0; i < 200; i++)printf("*");
			printf("\n*\x1B[1;35;40m\t   80 * 20  \x1B[1;37;42m");
			printf("\n*\x1B[1;35;40m\t  120 * 30  \x1B[1;37;42m");
			printf("\n*\x1B[1;35;40m\t  160 * 40  \x1B[1;37;42m");
			printf("\n*\x1B[1;35;46m\t  200 * 50  \x1B[1;37;42m");
			for (int i = 1; i < 46; i++)printf("\n*");
			break;
		}
		fflush(stdout);
		ch = _getch();
		switch (ch) {
		case 'j':
		case 'J':
			height += 10;
			break;
		case 'k':
		case 'K':
			height -= 10;
			break;
		default:
			break;
		}
		if (height == 60)height = 20;
		if (height == 10)height = 50;
	} while (ch != ' ');
	printf("\x1B[0m");
	*h = height;
	*w = height * 2 * HEIGHTBYWIDTH;
}
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
		//固定y, 移动x与z轴			* No! 这样是错的!
		//应当以(0,0,1)为轴转动		* 这样才对!!!
		struct MATRIX33 rotation = {
			{ cosf(offsetH), -sinf(offsetH), 0 },
			{ sinf(offsetH), cosf(offsetH), 0 },
			{ 0, 0, 1 }
		};
		cameraX = Normalize(LinearTrans33(rotation, cameraX));
		cameraY = Normalize(LinearTrans33(rotation, cameraY));
		cameraZ = Normalize(LinearTrans33(rotation, cameraZ));
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
// 0 for LeftButton, 1 for RightButton
void BlockChange(Camera* camera, short* preState) {
	Ray ray = GenRay(camera, 0.5, 0.5);
	Vector3 interPosition;
	Vector3 interNormal;
	if (!RayHitBlock(ray, &interPosition, &interNormal))return;
	if (GetKeyState(VK_LBUTTON) < 0 && preState[0] >= 0) {
		Vector3 blockPlace = InBlock(Add(interPosition, Multi(interNormal, -0.5)));
		INT8 x, y, z;
		x = (INT8)blockPlace.x;
		y = (INT8)blockPlace.y;
		z = (INT8)blockPlace.z;
		DeleteBlock(x, y, z);
	}
	if (GetKeyState(VK_RBUTTON) < 0 && preState[1] >= 0) {
		Vector3 blockPlace = InBlock(Add(interPosition, Multi(interNormal, 0.5)));
		INT8 x, y, z;
		x = (INT8)blockPlace.x;
		y = (INT8)blockPlace.y;
		z = (INT8)blockPlace.z;
		AddBlock(x, y, z, 0);
	}
	preState[0] = GetKeyState(VK_LBUTTON);
	preState[1] = GetKeyState(VK_RBUTTON);
}