#include "renderer.h"
float reluf(float a) {
	if (a > 0)
		return a;
	else
		return 0;
}
void pr(float l) {
	int level = (int)(l * 10);
	printf("%c", " .:-=*0#$@"[level]);
}
void ControlFPS(clock_t* previousTime) {
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
//Vector
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
bool RayHitBlock(Ray ray, Block blockPos, Vector3* outHitPos, Vector3* outHitNormal) {
	Vector3 origin = { (float)blockPos.x,(float)blockPos.y,(float)blockPos.z };
	Ray normal[6] = {{
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
	}};
	bool isFound = false;
	float t_last = RENDERING_DISTANCE * 100.0f;
	for (int i = 0; i < 6; i++) {
		//normal[i].direction
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
	if (isFound == true)
		return true;
	return false;
}
