#pragma once

#include < GL/gl.h >
#include  < GL/glu.h >
#include  < GL/glut.h >

#include "Vector.h"
#include "Parameter.h"

class CParameter;

class CGLBaseSub1 :
	public CParameter
{
public:
	CGLBaseSub1(void);
	~CGLBaseSub1(void);

protected:
	static const int RGB = 3;
	static const int RGBA = 4;
	struct image_t{
		int width;
		int height;
		int channels;
	    void*buffer;
	    ~image_t(){
	        if(buffer)::free(buffer);
		}
	};

public:
	int Init(void);//for initialization
	int ViewSetting(int w, int h);//for making gluPerspective
	void TestDraw(void); //for writing simple cube
	void initTexture(void);
	void displayTexPolygon(void);
	void reshape(int w, int h);

	void Draw(double rot, int x,int y,int pattern);
	void DrawRotate(int rot);
	void DrawMove(int x, int y,int z);
	void SetImage(void);

	void DrawSquare(CPoint);
	void DrawTriangle(int x, int y, int c);
	void DrawCircle(int x, int y, int c);
	void DrawGoal(CPoint);
	void ColorPattern(int i);
	static const int Scale = 1;

	

	void GetObjectPos();
	CPoint ObjNow;
	int ObjectNum;
	int GetObjectNum(int);

	CParameter * m_pParameter;

	int m_ChooseWall;

	BOOL GenerateTextures(char **filePaths, UINT *ids, int count);

	
	void * loadTexture(int width, int height, int channels, const char* filePathDummy);
	void textureFromImage(GLuint textureId, image_t*image);

protected:
	GLuint m_TextureId1;
	GLuint m_TextureId2;
	GLuint m_TextureId3;

	GLuint textureID1;
	GLuint textureID2;
	GLuint textureID3;

	CVector Image;
	CVector Wall1;
	CVector Wall2;
	CVector Wall3;

	float Rotate;
	float RotWall1;
	float RotWall2;
	float RotWall3;


};

