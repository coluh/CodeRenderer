#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <Windows.h>
#ifndef PI
    #define PI 3.14159265358979f
#endif
#define RENDERING_DISTANCE 10
#define TIME_SLICE (1000/12)
typedef struct VECTOR3 {
	float x;
	float y;
	float z;
} Vector3;
typedef Vector3 Vector;
enum CAMERA_MODE {
    FIRST_PERSON, THIRD_PERSON
};
typedef struct CAMERA {
    Vector3 position;       //相机位置
    Vector3 target;         //目标位置
    Vector3 up;             //相机的上方向
    float fovy;             //视场角
    float aspect;           //相机视口宽高比
    float zNear;            //相机到裁剪平面的距离
    enum CAMERA_MODE mode;
} Camera;
typedef struct RAY {
    Vector3 origin;
    Vector3 direction;      //Normalized
} Ray;
typedef struct BLOCK {
    UINT8 x;
    UINT8 y;
    UINT8 z;
    UINT8 information;
} Block;
void ControlFPS(clock_t*);
//make negitive value to zero
float reluf(float a);
//get a float between 0 and 1 and print char
void pr(float l);
//Vector
Vector3 Add(Vector3 a, Vector3 b);
Vector3 Sub(Vector3 a, Vector3 b);
Vector3 Multi(Vector3 vec, float s);
float Magnitude(Vector3 vec);
Vector3 Normalize(Vector3 vec);
Vector3 Cross(Vector3 a, Vector3 b);
float Dot(Vector3 a, Vector3 b);
//Draw
Ray GenRay(const Camera*, float, float);
//Camera
void CameraMove(Camera*, POINT*, int);
bool RayHitBall(Ray ray, Vector3 ballCenter, float radius, Vector3* outHitPosition, Vector3* outHitNormal);
bool RayHitBlock(Ray ray, Block block, Vector3* outHitPos, Vector3* outHitNormal);