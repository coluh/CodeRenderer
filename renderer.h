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
#define TIME_SLICE (1000/12)
void ControlFPS(clock_t*);
typedef struct VECTOR3 {
	float x;
	float y;
	float z;
} Vector3;
typedef Vector3 Vector;
typedef struct CAMERA {
    Vector3 position;       //���λ��
    Vector3 target;         //Ŀ��λ��
    Vector3 up;             //������Ϸ���
    float fovy;             //�ӳ���
    float aspect;           //����ӿڿ�߱�
    float zNear;            //������ü�ƽ��ľ���
} Camera;
typedef struct RAY {
    Vector3 origin;
    Vector3 direction;      //Normalized
} Ray;

float reluf(float a);
void pr(float l);

Vector3 Add(Vector3 a, Vector3 b);
Vector3 Sub(Vector3 a, Vector3 b);
Vector3 Mult(Vector3 vec, float s);
float Magnitude(Vector3 vec);
Vector3 Normalize(Vector3 vec);
Vector3 Cross(Vector3 a, Vector3 b);
float Dot(Vector3 a, Vector3 b);

Ray GenRay(const Camera* camera, float xRate, float yRate);
bool RayHitBall(Ray ray, Vector3 ballCenter, float radius, Vector3* outHitPosition, Vector3* outHitNormal);