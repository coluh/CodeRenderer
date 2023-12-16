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
    enum CAMERA_MODE mode;
} Camera;
typedef struct RAY {
    Vector3 origin;
    Vector3 direction;      //Normalized
} Ray;
enum CAMERA_MODE {
    FIRST_PERSON, THIRD_PERSON
};
void ControlFPS(clock_t*);
//Print
float reluf(float a);
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
bool RayHitBall(Ray, Vector3, float, Vector3*, Vector3*);