#pragma once

#ifndef __CFD_H__
#define __CFD_H__

#include "FrameBuffer.h"

inline float Noise2D(float *p);
float PerlinNoise2D(float x, float y, float alpha, float beta, int n);

enum SourceShape
{
	Ball,
	Cuboid
};

class vec3i
{
public:
	int x, y, z;
	vec3i(int _x = 0, int _y = 0, int _z = 0) : x(_x), y(_y), z(_z) {}
};

class vec3f
{
public:
	float x, y, z;
	vec3f(float _x = 0.f, float _y = 0.f, float _z = 0.f) :x(_x), y(_y), z(_z) {}
	vec3f(const vec3f &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
};


class C_SmokeSource
{
public:
	int id;
	vec3f color;
	vec3f pos;
	float r;
	float initialD, dD;
	bool fixedDensity;
	float initialVC, initialTC;
	SourceShape shape;
	C_SmokeSource();
	C_SmokeSource::C_SmokeSource(int _id, vec3f _color, vec3f _pos, float _r, float _initialD, float _dD, bool _fixedDensity, float _initialVC, float _initialTC);
	virtual ~C_SmokeSource();

	virtual void draw_source(vec3f canvasL, vec3f canvasH);

};

class C_RecSmokeSource : public C_SmokeSource
{
public:
	float w, h, d;
	C_RecSmokeSource();
	C_RecSmokeSource(int _id, vec3f _color, vec3f _pos, float _r, float _initialD, float _dD, bool _fixedDensity, float _initialVC, float _initialTC, float _w, float _h, float _d);
	~C_RecSmokeSource();

	void draw_source(vec3f canvasL, vec3f canvasH);
};


class C_CFD
{
	int p0x, p0y, p0z, p1x, p1y, p1z;
	int size;
	float fDens, fVsc;
	float *fU0, *fV0, *fW0, *fU1, *fV1, *fW1, *fDiv, *fP;
	float *CU, *CV, *CW, *D0, *D1, *T0, *T1, *BU, *N2D_B;
	int *M;
	int *inSourceNum;
	float **DC0, **DC1;
	FrameBuffer buf;

public:
	int NX, NY, NZ;
	int sourceNum;
	C_SmokeSource **source;

	C_CFD();
	~C_CFD();
	int addSource(vec3f _color, vec3f _pos, SourceShape shape = Ball, float _r = 2.5f, float _initialD = 1.0f, float _dD = 0.05f, bool _fixedDensity = false, float _initialVC = 5.0f, float _initialTC = 300.0f, float _w = 2.0f, float _h = 2.0f, float _d = 2.0f);

	void addForce(float *u0, float *v0, float *w0, float *su,
		float *sv, float *sw);
	void diffuse(float vsc, float *a0, float *a1);
	void advect(float *a0, float *a1, float *u, float *v,
float *w);
	void project(float *u0, float *u1, float *v0, float *v1,
		float *w0, float *w1,
		float *div, float *p, float matDens);
	void setBnd(float *f);
	void setInflow(float *fu0, float *fv0, float *fw0);
	vec3f getVorticity(int i, int j, int k);
	void updateCurls(float *cu, float *cv, float *cw);
	void updateDensity(int frame);
	void updateVelocity();
	void updateTemperature();
	void addExtForces();
	void update(int frame);
	void render(int op, int frameN, vec3f canvasL, vec3f canvasH,
		GLuint* NoForAll, int textureCnt);
	// op = 0, real time; op = 1, compute each frame;
	// op = 2, draw each frame
	void show_all_source(vec3f canvasL, vec3f canvasH);
	void setObstacleBnd();
	void genN2DMap(float *n2D, int nW, int nH, float offset);
	void initBuf(int a_frameN);
	void freeBuf();
	void saveBuf(string filePath);
	bool loadBuf(string filePath);
	void clearField();
	int bufFrameNum();
};

#endif __CFD_H__