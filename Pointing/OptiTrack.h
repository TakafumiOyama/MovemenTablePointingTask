#pragma once
#include <vector>
#include "NPTrackingTools.h"
#include "Trackable.h"
#include "Sensor.h"
#include "Vector3D.h"
#include "Parameter.h"


class COptiTrack:
	public CSensor
{
public:
	COptiTrack(void);
	~COptiTrack(void);

	//�I�t�Z�b�g
	static const int PEOPLE_NUM = 6;
	
	// �������֐�
	int Init(void);
	// �v���J�n�{�^���Ȍ�̏���
	int StartLogging(void);
	// �A�b�v�f�[�g
	int Update(void);
	
	//Update�v�Z�p
	int CalcTrackableMetrics(void);			// �ʒu�E�����Ȃ�
	int CalcDistanceBetweenPeople(void);	// �ΐl����
	int CalcHeadRotation(void);				// ������]��
	int CalcPersonOrientation(void);		// �l�̌���
	int CalcPersonIsActive();			//�l���A�N�e�B�u���ǂ���
	int CalcSpeed();					//�l�̑��x
	int CalcFformation();              //F-formation
	int CalcTable();                   //�e�[�u���̈ʒu��p�x�Ȃǂ��v�Z
	int CalcWallDisplay();             //�ǃf�B�X�v���C�ɑ΂���ʒu��p�x�Ȃǂ��v�Z
	int CalcPeopleCenterofGravity();

	//�f�[�^�o��
	int PushBackData(void);
	int CSVOutInit(void);
	int CSVOut(void);

	//�l���擾����֐�
	int GetTrackableNum(void){return m_trackableNum;};
	int GetHeadRotateNum(int trackableId){return m_HeadRotationNum[trackableId];};
	int GetNoddingNum(int trackableId){return m_NoddingNum[trackableId];};
	int CheckSensor(void){return m_SensorCheckFlag;};
	int GetSpeed(int personId){return m_Speed[personId];};
	BOOL IsActive(int personId){return m_IsActive[personId];};
	double DistanceBetweenTwoPeople(int personId1, int personId2){return m_DistanceBetweenPeople[personId1][personId2];};
	bool GetFformationOk(int personId1, int personId2){return m_FformationFlag[personId1][personId2];};
	bool GetPersonToTableFlag(int personId1){return m_PersonSeeTableFlag[personId1];};
	bool IsPeopleActive(void){return m_IsPeopleActive;};
	bool GetIsPeopleUseTable(){return m_IsPeopleUseTable;};
	int GetActiveNum(){return m_ActiveNum;};
	int GetUseNum(){return m_UseNum;};
	int GetApproachNum(){return m_ApproachNum;};
	bool GetPersonApproachTable(int personId){ return m_IsPersonApproach[personId];};
	bool GetPersonGazeTableFlag(int personId){ return m_PersonGazeTableFlag[personId];};
	int GetTableToPersonAngle(int personId){ return m_TableToPersonAngle[personId];}; 
	double GetPersonSeeWallValueX(int personId){ return m_PersonSeeWallValue_x[personId];};
	double GetPersonSeeWallValueY(int personId){ return m_PersonSeeWallValue_y[personId];};	
	double GetDistancePersonToWall(int personId){ return m_DistancePersonToWall[personId];};
	int GetPersonLocationToTable(int personId){ return m_PersonLocationToTable[personId];};
	//CVector GetCenterVec(void){ return m_CenterVec;};

	// ���̑��֐�
	double GetDistanceBetweenTwoPeople(CTrackable a, CTrackable b);	// �Q�l�̑ΐl�������v�Z����֐�
	void SetAnswering(int personId, BOOL isAnswering){m_IsAnswering[personId] = isAnswering;}

	// �g���b�L���O�G���A�̑傫��(mm)
	static const int TRACKING_AREA_X = 4170;
	static const int TRACKING_AREA_Y = 2830;

	static const int OPTITRACK_REFRESH_INTERVAL = 100;	// �l���擾�������

		//== CheckResult function will display errors and ---
    //== exit application after a key is pressed =====---	
	void CheckResult(NPRESULT result);

	
	// �f�[�^������Ă���
	CTrackable GetTrackableInfo(int i)
	{
		//trackable�͈̔͊O�ɃA�N�Z�X���`�F�b�N
//		ASSERT(0 <= i && (unsigned int)i < m_trackable.size() - 1);

		return m_trackables[i];
	};

protected:

	// �͂��o���f�[�^�^
	typedef struct _OUTPUTDATA {
		COleDateTimeSpan time;		// �o�ߎ���
		int personId;				// �l��ID
		CTrackable metricData;		// �ʒu������Ȃǂ̊􉽊w���
		int headRotateNum;			// ������]��
		int noddingNum;
		double distanceBetweenPeople[PEOPLE_NUM];	// �ΐl����
		int bodyOrientation[PEOPLE_NUM];
		BOOL isAnswering;// �A���P�[�g�񓚒����ǂ���
		int TaskNumber;
		int ObjectNumber;
	} OUTPUTDATA;

	int m_frameCounter;		// �t���[���J�E���^�[
	BOOL m_flag_SimulationMode;		// �l�̓������V�~�����[�V�����Ŏ����������邩�ǂ���
	std::vector<OUTPUTDATA> m_outputDataBuffer;	// �͂��o���f�[�^�̃o�b�t�@
	int m_trackableNum;							// trackable�̐� //��{�ϊ���
	int m_InactiveNum[PEOPLE_NUM];		// �l�������Ă��Ȃ���
	BOOL m_IsActive[PEOPLE_NUM];			// �l���A�N�e�B�u���ǂ���
	bool m_IsPeopleActive;                      //�S�����A�N�e�B�u���ǂ���
	int m_ActiveNum;                            //�����Ă���l��
	

	// Update�֐����ōX�V�����p�����[�^
	std::vector<CTrackable> m_trackables;			// trackable��vector
	std::vector<CTrackable> m_previousTrackables;	// 1�O��trackable
	double m_PreDistanceBetweenPeople[PEOPLE_NUM][PEOPLE_NUM];   // �O�̑ΐl����
	double m_DistanceBetweenPeople[PEOPLE_NUM][PEOPLE_NUM];	// �ΐl����
	int m_PersonAngle[PEOPLE_NUM][PEOPLE_NUM];        //���ꂼ��̐l�̓��̌����̊p�x
	//������]�֘A
	int m_HeadRotationNum[PEOPLE_NUM];							// ������]��
	double m_RotateDegree[PEOPLE_NUM];
	double m_PreviousRotateDegree[PEOPLE_NUM];				// �O��P�ʎ��Ԃ̓�����]��
	int m_PersonOrientation[PEOPLE_NUM];
	//���Ȃ����֘A
	int m_NoddingNum[PEOPLE_NUM];						// ���Ȃ�����
	double m_PreviousPitchDegree[PEOPLE_NUM];
	double m_PitchDegree[PEOPLE_NUM];
	unsigned int m_NoddingStartTime[PEOPLE_NUM];
	double m_NoddingStartYaw[PEOPLE_NUM];
	//�������x
	double m_Speed[PEOPLE_NUM];
	//F-formation�̃t���O
	bool m_FformationFlag[PEOPLE_NUM][PEOPLE_NUM];
	//�e�[�u���֘A
	bool m_PersonSeeTableFlag[PEOPLE_NUM];    //�e�[�u�������Ă��邩�ǂ����̃t���O
	int m_PersonGazeNum[PEOPLE_NUM];         //�e�[�u�������Ă���b��
	bool m_PersonGazeTableFlag[PEOPLE_NUM];          //�e�[�u�������߂Ă��邩�ǂ����̃t���O
	int m_TableToPersonAngle[PEOPLE_NUM];    //�e�[�u���̐l�ɑ΂���p�x
	bool m_IsTableActive;                           //�e�[�u���𓮂������ǂ���
	bool m_IsPersonUseTable[PEOPLE_NUM];     //���ꂼ��̃��[�U���e�[�u�����g���Ă��邩�ǂ���
	bool m_IsPeopleUseTable;                        //���[�U�̂������ꂩ���e�[�u�����g���Ă��邩�ǂ���
	int m_UseNum;                               //�e�[�u�����g���Ă���l��
	bool m_IsPersonApproach[PEOPLE_NUM]; //�e�[�u���ɋ߂Â��Ă��邩�ǂ���
	int m_ApproachNum;                          //�e�[�u���ɋ߂Â��Ă���l��
	int m_PersonLocationToTable[PEOPLE_NUM]; //���[�U�̃e�[�u���ɑ΂���ꏊ
	//�ǃf�B�X�v���C�֘A
	double m_PersonSeeWallValue_x[PEOPLE_NUM]; //�ǃf�B�X�v���C�ɑ΂��郆�[�U�̌���
	double m_PersonSeeWallValue_y[PEOPLE_NUM];
	double m_DistancePersonToWall[PEOPLE_NUM]; //�l�̕ǃf�B�X�v���C�ɑ΂��鋗��

	BOOL m_CalcFirstTime;// yaw,pitch�̏������v�Z�p
	BOOL m_SensorCheckFlag;	//�Z���T���N���������ǂ���
	BOOL m_IsLogging;		// ���O����邩�ǂ���
	BOOL m_IsAnswering[PEOPLE_NUM];		// �A���P�[�g�񓚒����ǂ���

	CParameter * m_pParameter;
};

