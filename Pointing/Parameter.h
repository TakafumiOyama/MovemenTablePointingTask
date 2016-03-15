#pragma once
#include "Vector.h"
#include <MMSystem.h>
#include <vector>

using namespace std;
class CParameter
{
public:
	CParameter(void);
	~CParameter(void);

	int m_WindowPos_x;
	int m_WindowPos_y;
	int m_WindowSize_x;
	int m_WindowSize_y;
	int m_objNum;

	
	//���������E�B���h�E�̏����ʒu
	static const int WindowInitPosX = 0;
	static const int WindowInitPosY = 0;
	static const int VirtualWindowInitPosX = 0;
	static const int VirtualWindowInitPosY = 0;
	//���������E�B���h�E�̏����T�C�Y
	static int WindowSizeX;
	static int WindowSizeY;
	static const int VirtualWindowSizeX = 3840;
	static const int VirtualWindowSizeY = 2160;
	static const int DefWindowSizeX = 1920;
	static const int DefWindowSizeY = 1080;

	//��Ֆڏ�Ƀf�t�H���g�̈ʒu��ݒ�F1���̃f�B�X�v���C�ɂ�3x3��9�_�z�u�A�f�B�X�v���C��3��
	static CPoint DefObjectPos[4][3][3];
	static CPoint DefTargetPos[4][3][3];
	static CPoint ObjectPos[100];
	static CPoint ObjectPos2[100];
	static CPoint TargetPos[100];
	static CPoint tmpObjectPos[100];
	static CPoint GhostObjectPos[100];
	//���ꂼ��̒l���擾���邽�߂̊֐�
	int CreateObjectPos();
	int CreateTargetPos();
	int CreateDefPos3();
	int CreateDefPos4();
	CPoint GetObjectPos(int i);
	CPoint GetTouchPos(CPoint);
	void GetAllParameter(int);
	//�l��o�^���邽�߂̊֐�
	CPoint RegistTouchPos(CPoint);
	
	//�E�B���h�E�n���h��
	HWND hWnd;



	//-------------------------�^�b�`�Ɋւ���ϐ��E�֐�-----------------------------------
	//�I�u�W�F�N�g�z�u�̃p�^�[��
	static int m_objectPattern;
	//���݂̃^�b�`��
	static int m_TouchCount;
	//�f�B�X�v���C����w�𗣂�����
	static int m_RemoveCount;
	//�������^�b�`������(��{�I�ɂ͎��s�̔ԍ��Ƃ��̒l����v)
	static int m_correct;
	//�ԈႦ����
	static int m_wrong;
	//�J�E���g������֐�
	int AddCorrect();
	int AddWrong();

	//�^�b�`�̃J�E���g���擾����֐�
	//����
	int GetCorrectCount();
	//�듚
	int GetWrongCount();

	//�ŏ��̃^�b�`�̔���
	static bool IsFirstTouch;
	//���s�����ǂ����̔���
	static bool IsTasking;
	//�������^�b�`�������ǂ����̔���
	static bool IsCorrectTouch;
	// �^�X�N���I��������ǂ����̔���
	static bool IsTaskFinish;
	// �X�^�[�g�{�^���������ꂽ���ǂ����̔���
	static bool IsStandby;
	// �S�[��������Ă������ǂ���
	static bool m_CreateGoalFlag;
	// 

	//���݂̃I�u�W�F�N�g�̈ʒu
	CPoint currentObjPos;
	//���݃^�b�`�����ʒu
	static CPoint currentTouchPos;
	static CPoint GhostTouchPos;
	static CPoint RemovePos;




	//-------------------------------�t�@�C���o�͊֌W------------------------------------------
	//�����̃��x���t��
	int RecordInit();
	//�X�V���邽�тɃp�����[�^���t�@�C���ɏ�������
	int RecordInformation();
	//�f�[�^�𒙂߂�D����s�v2015/12/16
	int PushBackData();
	//�������J�n���鎞�Ԃ�ۑ����Ă����D���������2015/12/16
	static DWORD m_ProgramStart;

	//�I�u�W�F�N�g���o����������
	static DWORD m_TaskStartTime;
	//�^�b�`���J�n��������
	static DWORD m_TouchTime;
	//�h���b�O���I����������
	static DWORD m_TaskFinishTime;
	//----------------------------------------------------------------------------------------------------


	//--------------------------�e�[�u���̌`��Ǘ�--------------------------------------------
	//�ό`��̌`����w�肷��֐�
	int TableState(int);
	//���ɕό`����`�̎w��
	static int m_TableState;
	//�ό`�O�̌`����w�肷��֐�
	int PreTableState(int);
	//���܂ł̌`��
	static int m_PreTableState;
	//�^�X�N�̎��
	static int m_ShapePattern;

	//�ړ��Ɋւ���w��
	static bool LetsGo;
	// �ړ����J�n����
	static bool m_MoveStart;
	// �ړ����ł���
	static bool m_Moving;
	//�ό`������^�X�N���ǂ���
	static bool m_ShapeShift;
	//�����`������`�ɕ���ł��邩
	static bool IsLined;
	//L���^�ɕ���ł��邩�ǂ���
	static bool IsLshape;


	//--------------------------�I�u�W�F�N�g�̃v���Z�b�g----------------------------------------
	//�E�B���h�E�̖���
	static int m_windowNum;
	//�v���Z�b�g�̔ԍ�
	static int m_presetNum;
	struct ObjectSet{
		int targetCount;
		CPoint Object;
		CPoint Target;
	}OBJ;
	//�v���Z�b�g�̃p�����[�^���쐬���邽�߂̊֐�
	void CreatePreset(int);
	void SetPreset3(int);
	void SetPreset4(int);


private:
	//�O�̃^�b�`���s��ꂽ���ԁC���̃^�b�`�܂ł̎��Ԃ��擾����̂ɗ��p
	CString preTime;
	
protected:
	typedef struct _OUTPUTDATA{
		COleDateTimeSpan time;
		int touchCount;
		int correctCount;
		int wrongCount;
		int touchPos_x;
		int touchPox_y;
	}OUTPUTDATA;	
	std::vector<OUTPUTDATA> m_outputDataBuffer;


};
