#include "renderer.h"
struct LIGHT* root = NULL;
Block* baseBlock = NULL;
float Reluf(float a) {
	if (a > 0)
		return a;
	else
		return 0;
}
INT32 Gauss(float a) {
	INT32 b = (INT32)a;
	if (b > a)b--;
	return b;
}
void pr(float l) {
	int level = (int)(l * 20);
	printf("%c", " `.:;-=+~>?*0CXB#$W@"[level]);
}
void ControlFPS(clock_t* previousTime) {
	fflush(stdout);
	clock_t drawTime = clock() - *previousTime;
	clock_t waitTime = TIME_SLICE - drawTime;
	//printf("\n\n\n\n\n\n\n\n\n\n%d", drawTime);
	if (waitTime <= 0) {
		printf("\n____________\n\x1B[1;37;41m  TOO BUSY  \x1B[0m");
	}
	else {
		printf("\n____________\n\x1B[1;37;42m  RUN WELL  \x1B[0m");
		while (waitTime >= 20) {
			Sleep(1);//but in fact it sleeps for 15 miliseconds and more!
			drawTime = clock() - *previousTime;
			waitTime = TIME_SLICE - drawTime;
		}
		while (clock() - *previousTime < TIME_SLICE);
	}
	*previousTime = clock();
}
//add two vectors
Vector3 Add(Vector3 a, Vector3 b)
{
	Vector3 re = { a.x + b.x, a.y + b.y, a.z + b.z };
	return re;
}
Vector3 Sub(Vector3 a, Vector3 b)
{
	Vector3 re = { a.x - b.x, a.y - b.y, a.z - b.z };
	return re;
}
Vector3 Multi(Vector3 vec, float s)
{
	Vector3 re = { vec.x * s, vec.y * s, vec.z * s };
	return re;
}
float Magnitude(Vector3 vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}
Vector3 Normalize(Vector3 vec)
{
	float mag = Magnitude(vec);
	Vector3 re = { vec.x / mag, vec.y / mag, vec.z / mag };
	return re;
}
Vector3 Cross(Vector3 a, Vector3 b)
{
	Vector3 re = {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
	return re;
}
Vector3 LinearTrans33(struct MATRIX33 trans, Vector3 before) {
	Vector3 after;
	after = Multi(trans.col_one, before.x);
	after = Add(after, Multi(trans.col_two, before.y));
	after = Add(after, Multi(trans.col_three, before.z));
	return after;
}
float Dot(Vector3 a, Vector3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
//Ray
Ray GenRay(const Camera* camera, float xRate, float yRate) {
	//相机坐标系
	Vector3 cameraZ = Normalize(camera->target);
	Vector3 cameraY = Normalize(camera->up);
	Vector3 cameraX = Normalize(Cross(cameraY, cameraZ));
	//通过视场角计算屏幕平面大小
	float screenHeight = camera->zNear * tanf(camera->fovy / 2.0f) * 2.0f;
	float screenWidth = screenHeight * camera->aspect;
	//计算射线经过点坐标
	Vector3 screenCenter = Add(Multi(cameraZ, camera->zNear), camera->position);
	Vector3 screenPoint = screenCenter;
	//屏幕的上方向与相机上方向一致
	screenPoint = Add(screenPoint, Multi(cameraX, (0.5f - xRate) * screenWidth / HEIGHTBYWIDTH));	//  delete the '/2' when
	screenPoint = Add(screenPoint, Multi(cameraY, (0.5f - yRate) * screenHeight));					//drawing into a picture
	Ray ray = {
		camera->position,
		Normalize(Sub(screenPoint, camera->position))
	};
	return ray;
}
//ball
bool RayHitBall(Ray ray, Vector3 ballCenter, float radius, Vector3* outHitPosition, Vector3* outHitNormal) {
	//解方程  |S + tR - O|     = radius
	//即 	  (S + tR  - O)^2  = radius^2
	//即(S + tR -O)(S + tR -O) = radius^2
	//即 R^2*t^2 + 2R*OS*t + OS^2-r^2 = 0
	Vector3 center2RayOrigin = Sub(ray.origin, ballCenter);
	float a = Dot(ray.direction, ray.direction);
	float b = 2 * Dot(ray.direction, center2RayOrigin);
	float c = Dot(center2RayOrigin, center2RayOrigin) - radius * radius;
	float delta = b * b - 4 * a * c;
	if (delta > 0)
	{
		float t = (-b - sqrtf(delta)) / (2 * a);
		if (t < 0.001f)
		{
			t = (-b + sqrtf(delta)) / (2 * a);
		}

		if (t < 0.001f)
		{
			return false;
		}
		else
		{
			*outHitPosition = Add(ray.origin, Multi(ray.direction, t));
			*outHitNormal = Normalize(Sub(*outHitPosition, ballCenter));
			return true;
		}
	}

	return false;
}
/*
 *	检测射线是否射到Block, 已知block西北下角的坐标
 *	对于其中一个面, 算出面中点坐标和法向量
 *		计算射线与平面交点位置, 与面中点比较
 *			若x坐标或y坐标差大于0.5, continue
 *			否则, 若此次射线延伸的比例更短, 更新位置和法向量
 */
bool RayHitBlock(Ray ray, Vector3* outHitPos, Vector3* outHitNormal) {
	const Block* now = baseBlock;
	bool isFound = false;
	float t_last = RENDERING_DISTANCE * 100.0f;
	for (; now != NULL; now = now->next) {
		Vector3 origin = { (float)now->x,(float)now->y,(float)now->z };
		Ray normal[6] = { {
			Add(origin, (Vector3) { 0, 0.5, 0.5 }), {-1, 0, 0}
		}, {
			Add(origin, (Vector3) { 0.5, 0, 0.5 }), {0, -1, 0}
		}, {
			Add(origin, (Vector3) { 0.5, 0.5, 0 }), {0, 0, -1}
		}, {
			Add(origin, (Vector3) { 1, 0.5, 0.5 }), {1, 0, 0}
		}, {
			Add(origin, (Vector3) { 0.5, 1, 0.5 }), {0, 1, 0}
		}, {
			Add(origin, (Vector3) { 0.5, 0.5, 1 }), {0, 0, 1}
		} };
		for (int i = 0; i < 6; i++) {
			//normal[i].origin 坐标为负数时渲染错误
			float t = Dot(Sub(normal[i].origin, ray.origin), normal[i].direction);
			t /= Dot(ray.direction, normal[i].direction);
			if (t <= 0)
				continue;
			Vector3 hitPos = Add(ray.origin, Multi(ray.direction, t));
			if (normal[i].direction.x == 0 && fabsf(hitPos.x - normal[i].origin.x) >= 0.5)
				continue;
			if (normal[i].direction.y == 0 && fabsf(hitPos.y - normal[i].origin.y) >= 0.5)
				continue;
			if (normal[i].direction.z == 0 && fabsf(hitPos.z - normal[i].origin.z) >= 0.5)
				continue;
			if (isFound == false || (isFound == true && t < t_last)) {
				*outHitPos = hitPos;
				*outHitNormal = normal[i].direction;
				isFound = true;
				t_last = t;
			}
		}
	}
	if (isFound == true)
		return true;
	return false;
}
void AddLightV(Vector3 coo, enum LIGHT_TYPE type) {
	struct LIGHT* p = (struct LIGHT*)malloc(sizeof(struct LIGHT));
	if (p == NULL) {
		printf("\nGreat problem");
		return;
	}
	p->coordinate = coo;
	p->type = type;
	p->next = NULL;
	if (root == NULL) {
		root = p;
	}
	else {
		struct LIGHT* now = root;
		while (now->next != NULL)now = now->next;
		now->next = p;
	}
}
void AddLight(float x, float y, float z, enum LIGHT_TYPE type) {
	Vector3 v = {
		x, y, z
	};
	AddLightV(v, type);
}
float CalculateBrightness(Vector3 interPosition, Vector3 interNormal) {

	/*float diff = 0.5f * Reluf(Dot(interNormal, Normalize(Sub(sunPos, interPosition))));
	diff += 0.5f * Reluf(Dot(interNormal, Normalize(moonDire)));*/
	if (root == NULL)
		return 0;
	struct LIGHT* now = root;
	float diff = 0, n = 0;
	for (; now != NULL; now = now->next) {
		if (now->type == LIGHT_POINT)
			diff += Reluf(Dot(interNormal, Normalize(Sub(now->coordinate, interPosition))));
		else
			diff += Reluf(Dot(interNormal, Normalize(now->coordinate)));
		n++;
	}
	return diff / n;
}
void AddBlockV(Vector3 coo, UINT8 infor) {
	AddBlock((INT8)coo.x, (INT8)coo.y, (INT8)coo.z, infor);
}
void AddBlock(INT8 x, INT8 y, INT8 z, UINT8 infor) {
	Block* new = (Block*)malloc(sizeof(Block));
	if (new == NULL) {
		printf("\nMemory used up!");
		return;
	}
	new->x = x;
	new->y = y;
	new->z = z;
	new->information = infor & 0b01111111u;
	new->next = NULL;
	if (baseBlock == NULL) {
		baseBlock = new;
	}
	else {
		Block* this = baseBlock;	//Head Node
		for (; this->next != NULL; this = this->next);
		this->next = new;
	}
}

bool readBlock(){
	FILE* fp = fopen("data.blocks", "r");
	if (fp == NULL)
		return false;
	char buffer[128];
	while (fgets(buffer, 128, fp) != NULL) {
		char keyword[12];
		int i = 0;
		while (buffer[i] != ' ') {
			keyword[i] = buffer[i];
			i++;
		}
		keyword[i] = '\0';
		if (strcmp(keyword, "block") == 0) {
			int x, y, z;
			unsigned int infor;
			if (sscanf(buffer, "%*s %d %d %d %u", &x, &y, &z, &infor) < 0)
				return false;
			AddBlock((INT8)x, (INT8)y, (INT8)z, (UINT8)infor);
		}
	}
	fclose(fp);
	return true;
}
