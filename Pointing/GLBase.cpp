#include "StdAfx.h"
#include "GLBase.h"
#include "Vector.h"
#include "imgctl.h"



#define TEX_HEIGHT 512 
#define TEX_WIDTH 512 
static GLubyte image[TEX_HEIGHT][TEX_WIDTH][4];

const int TEXTURES = 3;
GLuint textureArray[TEXTURES];


CGLBase::CGLBase(void)
	: m_ChooseWall(0)
	, ObjectNum(0)
{
	m_pParameter = new CParameter();
}


CGLBase::~CGLBase(void)
{
}



int CGLBase::Init(void)
{

	////3Dシーンの背景を黒にする。
	//::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//::glClearDepth(1.0f);
	//////デプスバッファを有効化
	//::glEnable(GL_DEPTH_TEST);


	 glClearColor(0.0, 0.0, 0.0, 0.0);
	 glDepthFunc(GL_LEQUAL); 
	 glEnable(GL_DEPTH_TEST); 
//	 initTexture();
	 SetImage(); 
	 glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEX_WIDTH, TEX_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	 //int w = m_pParameter->WindowSizeX;
	 //int h = m_pParameter->WindowSizeY;
	 int w = WindowSizeX;
	 int h = WindowSizeY;
	 glViewport(0, 0, w, h);
	 glLoadIdentity();
	 glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);
	 
	 //glutInitDisplayMode (GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	return (0) ;

}


void CGLBase::TestDraw(void)
{

	//シーンをクリアする。色情報、デプスバッファをクリア

	
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);  

	
	glPushMatrix(); 
/*	glTranslatef(0.0, 0.0,-20.0); 
	glRotatef(0, 0.0, 1.0, 0.0);*/ 
	//glColor3f(1.0, 1.0, 1.0);
	displayTexPolygon(); 

	glPopMatrix(); 

   return;


}


int CGLBase::ViewSetting(int w, int h)
{

	//glMatrixMode(GL_PROJECTION) ;
	//glLoadIdentity() ;
	//// gluPerspective( 視野角度[deg],  アスペクト比(ウィンドウの幅/高さ), 
	//		//描画する距離範囲（視点から最も近い点までの距離）, 視点から最も遠い点までの距離);
	//gluPerspective(60, (GLfloat)w/(GLfloat)h, 0.5, 100.0) ;
	// //glViewport( 左下のピクセル座標X, 左下のピクセル座標Y, ウィンドウ幅，高さ );
	//glViewport (0, 0, w , h) ;
	//glMatrixMode(GL_MODELVIEW) ;
	//glLoadIdentity ();


	if(0 >= w || 0 >= h) return 0;
	
	//::glViewport(0, 0, w, h);
	//::glMatrixMode(GL_PROJECTION);
	//::glLoadIdentity();
	//::glOrtho(0, w, h, 0, -1.0f, 1.0f);
	//::glMatrixMode(GL_MODELVIEW);
	//::glLoadIdentity();


		glViewport(0, 0,  w,  h); 
		glMatrixMode(GL_PROJECTION); 
		glLoadIdentity(); 

		//gluPerspective(90.0, (double)w / (double)h, 1.0, 100.0);
		//glTranslated(0.0, 0.0, -1.0);
	 
	//::glOrtho(0, w, h, 0, -1.0f, 1.0f);
	 // glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
	glMatrixMode(GL_MODELVIEW);
	glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);
	glLoadIdentity();

	

	return 0;
}

void CGLBase::initTexture(void)
{

}


void CGLBase::displayTexPolygon(void)
{
	glPopMatrix();
	//glEnable(GL_TEXTURE_2D); 
	glBegin(GL_QUADS); 
		//glTexCoord2f(0.0, 0.0); glVertex3f(-5.0,-5.0, 0.0); 
		//glTexCoord2f(0.0, 1.0); glVertex3f(-5.0, 5.0, 0.0);
		//glTexCoord2f(1.0, 1.0); glVertex3f( 5.0, 5.0, 0.0); 
		//glTexCoord2f(1.0, 0.0); glVertex3f( 5.0,-5.0, 0.0); 
	glColor3d(1.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex2d(-0.8,-0.8); 
	glTexCoord2f(0.0, 1.0); glVertex2d(-0.8, 0.8);
	glTexCoord2f(1.0, 1.0); glVertex2d( 0.0, 0.8); 
	glTexCoord2f(1.0, 0.0); glVertex2d( 0.0,-0.8);
	glEnd();
	glPushMatrix();

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glViewport(1920,0,1920,2160);
	glLoadIdentity();
	glOrtho(1.0, 0.0, -1.0, 1.0, -1.0, 1.0);

	//glBegin(GL_QUADS);
	//glColor3d(0.0, 1.0, 0.0);
	//glTexCoord2f(0.0, 0.0); glVertex2d( 0.0,-0.8); 
	//glTexCoord2f(0.0, 1.0); glVertex2d( 0.0, 0.8);
	//glTexCoord2f(1.0, 1.0); glVertex2d( 0.8, 0.8); 
	//glTexCoord2f(1.0, 0.0); glVertex2d( 0.8,-0.8); 
	//glEnd();
	glPushMatrix();

	//glColor3d(0.0,0.0,1.0);
	//CPoint p = CPoint(4800, 540);
	//this->DrawSquare(p);
	//glFlush();
	//
	 
	//glDisable(GL_TEXTURE_2D);

}

void CGLBase::DrawSubDisplay(int x, int y, int w, int h)
{
	static int r = 0;
	glViewport(x,y,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);
	glLoadIdentity();
}


void reshape(int w, int h) { 

		glViewport(0, 0, (GLsizei) w, (GLsizei) h); 
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glFrustum(-5.0, 5.0,-5.0, 5.0, 5.0, 500.0); 
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();

}


void CGLBase::Draw(double rot, int x,int y,int pattern)
{
	//Rotate = Rotate + rot;
	//Image.x = Image.x + x;
	//Image.y = Image.y + y;

	//int ObjNum;
	CPoint touch;

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 


	//L字型になった時の描画処理
	if(IsLshape)
	{
		//2枚目の下部
		if(2760 < currentTouchPos.x && currentTouchPos.x < 3840 && 1010 < currentTouchPos.y)
		{
			GhostTouchPos.x = currentTouchPos.y + 2760;
			GhostTouchPos.y = 3840 - currentTouchPos.x;
		//3枚目の左
		}else if(3830 <= currentTouchPos.x && currentTouchPos.x < 3910)
		{
			GhostTouchPos.x = 3840 - currentTouchPos.y;
			GhostTouchPos.y = currentTouchPos.x - 2760;
			if(1080 < GhostTouchPos.y)
			{
				Elaser.x = GhostTouchPos.x;
				Elaser.y = GhostTouchPos.y - 140;
				GhostTouchPos.y = 1079;
				//glColor3d(1.0,1.0,1.0);
				//DrawSquare(Elaser);
			}
		}else{
			GhostTouchPos.x = -100;
			GhostTouchPos.y = -100;
		}

	}
	if(!IsLshape)
		GhostTouchPos = CPoint(-100,-100);


	if(m_CreateGoalFlag)
		DrawGoal(TargetPos[m_correct]);


	if(IsStandby && m_Moving == false)
	{
		if(IsLshape)
		{
			glClearColor(1.0, 1.0, 1.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			glPushMatrix();
			glBegin(GL_QUADS);
			glColor3d(0.0,0.0,0.0);
			glVertex2d(-0.52,  0.1);
			glVertex2d(-0.52, -0.1);
			glVertex2d(-0.48, -0.1);
			glVertex2d(-0.48,  0.1);
			glEnd();
			glPopMatrix();
		}else{
			glClearColor(1.0, 1.0, 1.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			glPushMatrix();
			glBegin(GL_QUADS);
			glColor3d(0.0,0.0,0.0);
			glVertex2d(-0.02,  0.1);
			glVertex2d(-0.02, -0.1);
			glVertex2d( 0.02, -0.1);
			glVertex2d( 0.02,  0.1);
			glEnd();
			glPopMatrix();
		}
		//Sleep(3000);
		
		m_CreateGoalFlag = false;
		//IsStandby = false;
		IsFirstTouch = true;
		IsTasking = false;
		GhostObjectPos[m_correct] = ObjectPos[m_correct];

	}else if(m_Moving == true){
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	
	}else{

		if(IsTaskFinish == true){			//タッチしていない状態
			glColor3d(1.0,0.0,0.0);
			DrawSquare(ObjectPos[m_correct]);
			tmpObjectPos[m_correct] = ObjectPos[m_correct];
			IsTaskFinish = false;
			//TRACE("スタート\n");
		}else if(IsTasking == true && IsCorrectTouch == true)		//ドラッグしている状態
		{
			glColor3d(0.0,1.0,0.0);
			//if(1930 < currentTouchPos.x && currentTouchPos.y < 1080 && IsLined)
			//	glColor3d(1.0,1.0,1.0);
//			if(!IsLshape)
//				glColor3d(1.0,1.0,1.0);
			DrawSquare(GhostTouchPos);
		
			//glColor3d(0.0,1.0,0.0);
			DrawSquare(currentTouchPos);
		}else if(IsTasking == false && IsCorrectTouch == true){			//手を放した状態
			glColor3d(0.0,0.0,1.0);
			if(1930 < currentTouchPos.x && currentTouchPos.y < 1080 && IsLined)
				glColor3d(1.0,1.0,1.0);
//			if(!IsLshape)
//				glColor3d(1.0,1.0,1.0);
			DrawSquare(GhostTouchPos);
			glColor3d(0.0,0.0,1.0);
			DrawSquare(tmpObjectPos[m_correct]);
		}else{
			//TRACE("ハズレ\n");
			glColor3d(1.0,0.0,0.0);
//			if(!IsLshape)
//				glColor3d(1.0,1.0,1.0);
			DrawSquare(GhostTouchPos);
			glColor3d(1.0,0.0,0.0);
			DrawSquare(tmpObjectPos[m_correct]);
			//TRACE("(%d,%d)\n",currentTouchPos.x, currentTouchPos.y);
		}
	}	


	if(IsLshape)
	{	
		
		if(3830 <= currentTouchPos.x && currentTouchPos.x < 3910)
		{
			glColor3d(1.0,1.0,1.0);
			GhostTouchPos.y = 1080 + (currentTouchPos.x - 3980);
			DrawSquare(GhostTouchPos);
			Elaser.x = 3770;
			Elaser.y = currentTouchPos.y;
			DrawSquare(Elaser);
		}

		if(2760 < currentTouchPos.x && currentTouchPos.x < 3840 && 1010 < currentTouchPos.y)
		{
			Elaser.x = 3770;
			Elaser.y = 3840 - currentTouchPos.x;
			glColor3d(1.0,1.0,1.0);
			DrawSquare(Elaser);
		}
	}


}


void CGLBase::DrawRotate(int rot)
{
	//シーンをクリアする。色情報、デプスバッファをクリア

	Rotate = Rotate + rot;
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

	
	glPushMatrix(); 
	glTranslatef(0, 0,-20.0); 
	glRotatef(Rotate, 0.0, 1.0, 0.0); 
	glColor3f(1.0, 1.0, 1.0);
	displayTexPolygon(); 

	 glPopMatrix(); 
}


void CGLBase::DrawMove(int x,int y,int z)
{
	Image.x += x;
	Image.y += y;

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

	glPushMatrix(); 
	glTranslatef(Image.x,Image.y,-20.0);
	glColor3f(1.0, 1.0, 1.0);
	displayTexPolygon(); 
	
	glPopMatrix(); 
}

void CGLBase::SetImage(void)
{
	char buf1[100];
	char *files1[1];

	sprintf_s(buf1, "images/ball128.jpg");
	*files1 = buf1;
	BOOL genTexture1 = GenerateTextures(files1, &m_TextureId1,1);
	DWORD er1 = GetLastError();
	
	char buf2[100];
	char *files2[1];

	sprintf_s(buf2, "images/windows128.jpg");
	*files2 = buf2;
	BOOL genTexture2 = GenerateTextures(files2, &m_TextureId2,1);
	DWORD er2 = GetLastError();
	
	char buf3[100];
	char *files3[1];

	sprintf_s(buf3, "images/round128.jpg");
	*files3 = buf3;
	BOOL genTexture3 = GenerateTextures(files3, &m_TextureId3,1);
	DWORD er3 = GetLastError();
	
    glBindTexture(GL_TEXTURE_2D, NULL);

	 
}

BOOL CGLBase::GenerateTextures(char **filePaths, UINT *ids, int count /* = 1 */)
{
	HDIB hDIB;             // テクスチャのDIB
	BITMAPINFOHEADER bih;  // DIBのヘッダ
	LPCIMGDATA pImageData; // テクスチャのビットマップデータ

	char *filePath;  // パス
	UINT *id;        // テクスチャID

	// テクスチャ名の生成
	::glGenTextures(count, ids);

	for(int i = 0; i < count; ++i) {
		filePath = filePaths[i];
		id = &ids[i];

		// 画像タイプに応じてDIBを生成
		// このあたりはimgctlに依存
		// 何故かGIFを指定すると落ちるのでとりあえず回避
		if(GetImageType(filePath, NULL) == IMG_GIF || (hDIB = ToDIB(filePath)) == 0) {
			return FALSE;
		}

		// DIBのヘッダとデータ部を取得
		HeadDIB(hDIB, &bih);
		pImageData = DataDIB(hDIB);

		// ビットの並び方を調整
		unsigned char temp;
		for(unsigned int i = 0; i < bih.biSizeImage; i += 3) {
			temp = pImageData->pData[i];
			pImageData->pData[i] = pImageData->pData[i+2];
			pImageData->pData[i+2] = temp;
		}

		// テクスチャIDを選択
		::glBindTexture(GL_TEXTURE_2D, *id);

		// テクスチャの生成
		::gluBuild2DMipmaps(
			GL_TEXTURE_2D,    // テクスチャの次元
			3,                // 色の数(R，G，Bで3つ）
			bih.biWidth,      // ビットマップの幅
			bih.biHeight,     // ビットマップの高さ
			GL_RGB,           // ビットマップの色形式
			GL_UNSIGNED_BYTE, // ビットマップデータの形式
			pImageData->pData // ビットマップデータへのポインタ
			);

		// DIBハンドルの破棄
//		DeleteDIB(hDIB);
		
		// フィルタの設定
		::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		//::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	return TRUE;
}

void *CGLBase::loadTexture(int width, int height, int channels, const char* filePathDummy){
    // 画像を読み込む(ここでは仮として自動生成する)
    typedef unsigned char* iterator_t;
    int image_size = width * height * channels * sizeof(unsigned char);
    void *image = ::malloc( image_size ); // nullチェック略
    iterator_t itr_dest = iterator_t(image);
    iterator_t itr_dest_end = itr_dest + image_size;
    //  ストライプ作成
    int pixel = 0; int stride = 3;
    while (itr_dest != itr_dest_end) {
        *itr_dest++ = 255-50 * pixel ;
        *itr_dest++ = 255-50 * pixel ;
        *itr_dest++ = 255-10 * pixel ;
        if (channels == RGBA) {
            *itr_dest++ = 100;  //  半透明
        }
        pixel = ++pixel % stride;
    }
    // 上左端に目印をつける
    itr_dest = iterator_t(image);
    for(int i = 0 ; i < width / 3; i++){
        *itr_dest++ = 50;  // cyan
        *itr_dest++ = 255;
        *itr_dest++ = 255;
        if (channels == RGBA) {
            *itr_dest++ = 100;
        }
    }
    return image;
}

void CGLBase::textureFromImage(GLuint textureId, image_t* image){
    // IDによって現在のテクスチャを指定する
    glBindTexture(GL_TEXTURE_2D, textureId );
   // パラメータ設定(例)
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );	// texture environment
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    // 画像からテクスチャ作成
    GLenum format = GL_RGB;
    if(image->channels == RGB){
        format = GL_RGB;  //  4channelのときはGL_RGBAを引数にする
    }
    glTexImage2D(GL_TEXTURE_2D, 0, format, image->width, image->height, 0, format, GL_UNSIGNED_BYTE, image->buffer );
}

void CGLBase::ColorPattern(int i)
{
	switch(i)
	{
		case 0:
			glColor3d(1.0, 1.0, 1.0);
			break;
		case 1:
			glColor3d(1.0, 0.0, 0.0);
			break;
		case 2:
			glColor3d(0.0, 1.0, 0.0);
			break;
		case 3:
			glColor3d(0.0, 0.0, 1.0);
			break;
		case 4:
			glColor3d(1.0, 1.0, 0.0);
			break;
		case 5:
			glColor3d(1.0, 0.0, 1.0);
			break;
		case 6:
			glColor3d(0.0, 1.0, 1.0);
			break;
		default:
			break;
	}
}


void CGLBase::DrawSquare(CPoint touch)
{
	//glClearColor(1.0,1.0,1.0,1.0);
	//double cwidth = (double)m_pParameter->WindowSizeX;
	//double cheight = (double)m_pParameter->WindowSizeY;
	double cwidth = (double)WindowSizeX;
	double cheight = (double)WindowSizeY;
	double aspect = cwidth / cheight;

	//CVector obj = CVector((double)touch.x / cwidth, -((double)touch.y / cwidth));
	CVector obj = CVector((double)touch.x*2 / cwidth - 1.0, -((double)touch.y*2 / cheight) + 1.0);
	//CPoint obj = CPoint(0,0);
	
	//TRACE("(%f,%f)\n",obj.x,obj.y);

	double scalex = 50.0/1920.0;
	double scaley = scalex*aspect;

	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex2d(obj.x-scalex, obj.y-scaley);
	glVertex2d(obj.x-scalex, obj.y+scaley);
	glVertex2d(obj.x+scalex, obj.y+scaley);
	glVertex2d(obj.x+scalex, obj.y-scaley);
	glEnd();
	glPopMatrix();
}

void CGLBase::GetObjectPos()
{
	ObjNow = this->ObjectPos[m_pParameter->m_correct];
	TRACE("Get(%d, %d)", ObjNow.x, ObjNow.y);
}


int CGLBase::GetObjectNum(int i)
{
	ObjectNum = i;
	return ObjectNum;
}


void CGLBase::DrawGoal(CPoint point)
{
	double cwidth = (double)WindowSizeX;
	double cheight = (double)WindowSizeY;
	double aspect = cwidth / cheight;

	//CVector obj = CVector((double)touch.x / cwidth, -((double)touch.y / cwidth));
	CVector obj = CVector((double)point.x*2 / cwidth - 1.0, -((double)point.y*2 / cheight) + 1.0);
	//CPoint obj = CPoint(0,0);
	
	//TRACE("(%f,%f)\n",obj.x,obj.y);

	double scalex = 70.0/1920.0;
	double scaley = scalex*aspect;

	glPushMatrix();
	glLineWidth(8.0);
	glBegin(GL_LINE_LOOP);
	glColor3d(0.0,0.0,0.0);
	glVertex2d(obj.x-scalex, obj.y-scaley);
	glVertex2d(obj.x-scalex, obj.y+scaley);
	glVertex2d(obj.x+scalex, obj.y+scaley);
	glVertex2d(obj.x+scalex, obj.y-scaley);
	glEnd();
	glPopMatrix();

}
