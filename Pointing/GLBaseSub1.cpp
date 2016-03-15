#include "StdAfx.h"
#include "GLBaseSub1.h"
#include "Vector.h"
#include "imgctl.h"



#define TEX_HEIGHT 512 
#define TEX_WIDTH 512 
static GLubyte image[TEX_HEIGHT][TEX_WIDTH][4];

const int TEXTURES = 3;
//GLuint textureArray[TEXTURES];


CGLBaseSub1::CGLBaseSub1(void)
	: m_ChooseWall(0)
	, ObjectNum(0)
{
	m_pParameter = new CParameter();
}


CGLBaseSub1::~CGLBaseSub1(void)
{
}



int CGLBaseSub1::Init(void)
{

	////3D�V�[���̔w�i�����ɂ���B
	//::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//::glClearDepth(1.0f);
	//////�f�v�X�o�b�t�@��L����
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
	 int w = DefWindowSizeX;
	 int h = DefWindowSizeY;
	 glViewport(0, 0, w, h);
	 glLoadIdentity();
	 glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);
	 
	 //glutInitDisplayMode (GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	return (0) ;

}


void CGLBaseSub1::TestDraw(void)
{

	//�V�[�����N���A����B�F���A�f�v�X�o�b�t�@���N���A

	
	 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

	
	 glPushMatrix(); 
		glTranslatef(0.0, 0.0,-20.0); 
		glRotatef(0, 0.0, 1.0, 0.0); 
			glColor3f(1.0, 1.0, 1.0);
			displayTexPolygon(); 

	 glPopMatrix(); 

   return;


}


int CGLBaseSub1::ViewSetting(int w, int h)
{

	//glMatrixMode(GL_PROJECTION) ;
	//glLoadIdentity() ;
	//// gluPerspective( ����p�x[deg],  �A�X�y�N�g��(�E�B���h�E�̕�/����), 
	//		//�`�悷�鋗���͈́i���_����ł��߂��_�܂ł̋����j, ���_����ł������_�܂ł̋���);
	//gluPerspective(60, (GLfloat)w/(GLfloat)h, 0.5, 100.0) ;
	// //glViewport( �����̃s�N�Z�����WX, �����̃s�N�Z�����WY, �E�B���h�E���C���� );
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

void CGLBaseSub1::initTexture(void)
{

}


void CGLBaseSub1::displayTexPolygon(void)
{

	 glEnable(GL_TEXTURE_2D); 
	 glBegin(GL_QUADS); 
		glTexCoord2f(0.0, 0.0); glVertex3f(-5.0,-5.0, 0.0); 
		glTexCoord2f(0.0, 1.0); glVertex3f(-5.0, 5.0, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f( 5.0, 5.0, 0.0); 
		glTexCoord2f(1.0, 0.0); glVertex3f( 5.0,-5.0, 0.0); 
	 glEnd();
	 
	 glDisable(GL_TEXTURE_2D);

}

//
//void reshape(int w, int h) { 
//
//		glViewport(0, 0, (GLsizei) w, (GLsizei) h); 
//		glMatrixMode(GL_PROJECTION); glLoadIdentity();
//		glFrustum(-5.0, 5.0,-5.0, 5.0, 5.0, 500.0); 
//		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
//
//}


void CGLBaseSub1::Draw(double rot, int x,int y,int pattern)
{
	//Rotate = Rotate + rot;
	//Image.x = Image.x + x;
	//Image.y = Image.y + y;

	int ObjNum;
	CPoint touch;

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

	//CreateObjectPos();
	//CreateTargetPos();
	if(IsTasking == true)
	{
		DrawSquare(currentTouchPos);
	}else{
		DrawSquare(ObjectPos[m_correct]);
	}
	DrawGoal(TargetPos[m_correct]);

}


void CGLBaseSub1::DrawRotate(int rot)
{
	//�V�[�����N���A����B�F���A�f�v�X�o�b�t�@���N���A

	Rotate = Rotate + rot;
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

	
	glPushMatrix(); 
	glTranslatef(0, 0,-20.0); 
	glRotatef(Rotate, 0.0, 1.0, 0.0); 
	glColor3f(1.0, 1.0, 1.0);
	displayTexPolygon(); 

	 glPopMatrix(); 
}


void CGLBaseSub1::DrawMove(int x,int y,int z)
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

void CGLBaseSub1::SetImage(void)
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

BOOL CGLBaseSub1::GenerateTextures(char **filePaths, UINT *ids, int count /* = 1 */)
{
	HDIB hDIB;             // �e�N�X�`����DIB
	BITMAPINFOHEADER bih;  // DIB�̃w�b�_
	LPCIMGDATA pImageData; // �e�N�X�`���̃r�b�g�}�b�v�f�[�^

	char *filePath;  // �p�X
	UINT *id;        // �e�N�X�`��ID

	// �e�N�X�`�����̐���
	::glGenTextures(count, ids);

	for(int i = 0; i < count; ++i) {
		filePath = filePaths[i];
		id = &ids[i];

		// �摜�^�C�v�ɉ�����DIB�𐶐�
		// ���̂������imgctl�Ɉˑ�
		// ���̂�GIF���w�肷��Ɨ�����̂łƂ肠�������
		if(GetImageType(filePath, NULL) == IMG_GIF || (hDIB = ToDIB(filePath)) == 0) {
			return FALSE;
		}

		// DIB�̃w�b�_�ƃf�[�^�����擾
		HeadDIB(hDIB, &bih);
		pImageData = DataDIB(hDIB);

		// �r�b�g�̕��ѕ��𒲐�
		unsigned char temp;
		for(unsigned int i = 0; i < bih.biSizeImage; i += 3) {
			temp = pImageData->pData[i];
			pImageData->pData[i] = pImageData->pData[i+2];
			pImageData->pData[i+2] = temp;
		}

		// �e�N�X�`��ID��I��
		::glBindTexture(GL_TEXTURE_2D, *id);

		// �e�N�X�`���̐���
		::gluBuild2DMipmaps(
			GL_TEXTURE_2D,    // �e�N�X�`���̎���
			3,                // �F�̐�(R�CG�CB��3�j
			bih.biWidth,      // �r�b�g�}�b�v�̕�
			bih.biHeight,     // �r�b�g�}�b�v�̍���
			GL_RGB,           // �r�b�g�}�b�v�̐F�`��
			GL_UNSIGNED_BYTE, // �r�b�g�}�b�v�f�[�^�̌`��
			pImageData->pData // �r�b�g�}�b�v�f�[�^�ւ̃|�C���^
			);

		// DIB�n���h���̔j��
//		DeleteDIB(hDIB);
		
		// �t�B���^�̐ݒ�
		::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		//::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	return TRUE;
}

void *CGLBaseSub1::loadTexture(int width, int height, int channels, const char* filePathDummy){
    // �摜��ǂݍ���(�����ł͉��Ƃ��Ď�����������)
    typedef unsigned char* iterator_t;
    int image_size = width * height * channels * sizeof(unsigned char);
    void *image = ::malloc( image_size ); // null�`�F�b�N��
    iterator_t itr_dest = iterator_t(image);
    iterator_t itr_dest_end = itr_dest + image_size;
    //  �X�g���C�v�쐬
    int pixel = 0; int stride = 3;
    while (itr_dest != itr_dest_end) {
        *itr_dest++ = 255-50 * pixel ;
        *itr_dest++ = 255-50 * pixel ;
        *itr_dest++ = 255-10 * pixel ;
        if (channels == RGBA) {
            *itr_dest++ = 100;  //  ������
        }
        pixel = ++pixel % stride;
    }
    // �㍶�[�ɖڈ������
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

void CGLBaseSub1::textureFromImage(GLuint textureId, image_t* image){
    // ID�ɂ���Č��݂̃e�N�X�`�����w�肷��
    glBindTexture(GL_TEXTURE_2D, textureId );
   // �p�����[�^�ݒ�(��)
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );	// texture environment
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    // �摜����e�N�X�`���쐬
    GLenum format = GL_RGB;
    if(image->channels == RGB){
        format = GL_RGB;  //  4channel�̂Ƃ���GL_RGBA�������ɂ���
    }
    glTexImage2D(GL_TEXTURE_2D, 0, format, image->width, image->height, 0, format, GL_UNSIGNED_BYTE, image->buffer );
}

void CGLBaseSub1::ColorPattern(int i)
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


void CGLBaseSub1::DrawSquare(CPoint touch)
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

	double scalex = 0.05;
	double scaley = scalex*aspect;

	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3d(1.0,0.0,0.0);
	glVertex2d(obj.x-scalex, obj.y-scaley);
	glVertex2d(obj.x-scalex, obj.y+scaley);
	glVertex2d(obj.x+scalex, obj.y+scaley);
	glVertex2d(obj.x+scalex, obj.y-scaley);
	glEnd();
	glPopMatrix();
}

void CGLBaseSub1::GetObjectPos()
{
	ObjNow = this->ObjectPos[m_pParameter->m_correct];
	TRACE("Get(%d, %d)", ObjNow.x, ObjNow.y);
}


int CGLBaseSub1::GetObjectNum(int i)
{
	ObjectNum = i;
	return ObjectNum;
}


void CGLBaseSub1::DrawGoal(CPoint point)
{
	double cwidth = (double)WindowSizeX;
	double cheight = (double)WindowSizeY;
	double aspect = cwidth / cheight;

	//CVector obj = CVector((double)touch.x / cwidth, -((double)touch.y / cwidth));
	CVector obj = CVector((double)point.x*2 / cwidth - 1.0, -((double)point.y*2 / cheight) + 1.0);
	//CPoint obj = CPoint(0,0);
	
	//TRACE("(%f,%f)\n",obj.x,obj.y);

	double scalex = 0.1;
	double scaley = scalex*aspect;

	glPushMatrix();
	glBegin(GL_LINE_LOOP);
	glColor3d(1.0,0.0,0.0);
	glVertex2d(obj.x-scalex, obj.y-scaley);
	glVertex2d(obj.x-scalex, obj.y+scaley);
	glVertex2d(obj.x+scalex, obj.y+scaley);
	glVertex2d(obj.x+scalex, obj.y-scaley);
	glEnd();
	glPopMatrix();

}
