#pragma once

#include <cstdlib>
#include <string>
#include "Serial.h"

using namespace serial;
namespace roomba{

	//�[�d���
	struct ChangingState{
		enum type : int{
			NOT_CHANGING = 0,
			CHARGING_RECOVERY,
			CHARGING,
			TRICKLE_CHARGING,
			WAITING,
			CHARGING_ERROR
		};
	};

	/// @brief : ���y�p�̃R�[�h
	struct Note {
		enum type : int {
			C=0, CS, D, DS, E, F, FS, G, GS, A, AS, B, REST
		};
	};

	/// @brief : ���K
	class Tone {
	public:
		Tone() 
			: note(Note::C), octave(1) {}
		Tone( unsigned int note_, unsigned int oct_ ) 
			: note( note_ ), octave( oct_ )	{}
		unsigned int note;
		unsigned int octave;
	};

	/// @brief : �ꉹ
	class Sound {
	public:
		Sound( const Tone& tone, const int& duration ) 
			: tone( tone ), duration( duration ) {}
		Sound()
			: tone(), duration(0) {}
		bool isValid() const {
			int n = 24 + (tone.octave * 12) + tone.note;
			return ( n >= 31 && n<=107 ) || (tone.note == Note::REST);
		}

		Tone tone;
		unsigned char duration;  // duration/64[s]
	};

	/// @brief	�F �����o�̃Z���T�f�[�^��ۑ����邽�߂̍\���̌Q
	struct Cliff{
		Cliff() 
			:left(false), right(false), front_left(false), front_right(false)
		{}

		bool left;
		bool right;
		bool front_right;
		bool front_left;
	};

	struct Button {
		Button() 
			: power(false), spot(false), clean(false)
		{max = false;}

		bool power;
		bool spot;
		bool clean;
		bool max;
	};

	struct Bump {
		Bump() 
			: right(false), left(false)
		{}
		bool right;
		bool left;
	};

	struct WheelDrop {
		WheelDrop() 
			: caster(false), left(false), right(false)
		{}
		bool caster;
		bool left;
		bool right;
	};

	struct DirtDetector {
		DirtDetector() 
			: right(false), left(false)
		{}
		unsigned char right;
		unsigned char left;
	};

	struct Battery {
		Battery()
			: charging_state(0), voltage(0), current(0)
			, charge(0), capacity(0), temperature(0)
		{}
		unsigned int charging_state;
		unsigned int voltage;
		unsigned int charge;
		unsigned int capacity;
		int current;
		int temperature;
	};

	struct Pose {
		Pose() 
			: distance(0), angle(0)
		{}

		double angle;
		int distance;
	};

	struct MotorOvercurrent {
		MotorOvercurrent() 
			: drive_left(false), drive_right(false)
			, main_brush(false), vacuum(false), side_brush(false)
		{}
		bool drive_left;
		bool drive_right;
		bool main_brush;
		bool vacuum;
		bool side_brush;
	};

	struct SensorData {
		SensorData() 
			: wall(false), virtual_wall(false)
			, cliff(), button(), wheeldrop(), bump(), dirt()
			, battery(), pose(), motor_overcurrent()
		{}

		bool wall;
		bool virtual_wall;

		Pose pose;
		MotorOvercurrent motor_overcurrent;
		Battery battery;
		Cliff cliff;
		Button button;
		WheelDrop wheeldrop;
		Bump bump;
		DirtDetector dirt;
	};



	/**
	* @class name : Controller
	* @brief : �����o���R���g���[�����邽�߂̊֐�
	*		 : �p�b�V�u���[�h���g�p����
	*/
	class CControl
	{

	// �R���X�g���N�^�A�f�X�g���N�^------------------------
	public:
		CControl(void);
		virtual ~CControl(void);

	// �A�N�Z�X�p�֐�--------------------------------------
	public:
		/// @brief : ����������
		/// @param name="com" : �g�p����V���A���|�[�g
		/// @return : ��������
		bool initialize( const std::string& com );
		
		/// @brief : �I������
		/// @return : ��������
		bool finalize();

		/// @brief : ���a�Ƒ��x���w�肵���ړ�
		/// @param name="v" : �ړ����x [mm/s] (-500-500)
		/// @param name="l" : ��]���a [mm] (-2000-2000)
		void drive( short v, short r );
		
		/// @brief : ���E�ԗւɑ��x�w������ē�����
		/// @param name="l" : ���ԗւ̑��x [mm/s] (-500 - 500)
		/// @param name="r" : �E�ԗւ̑��x [mm/s] (-500 - 500)
		void driveDirect( short l, short r );
	
		/// @breif : �|������J�n
		void clean();
		
		/// @brief : ���̏��|��
		void spot();

		/// @brief : �h�b�N�̌���
		void seekDock();

		/// @brief : ���[�^�[����i�u���V�j
		/// @param name="main_blush" : ���C���̑|���p���[�^�[
		/// @param name="vacuum" : �z�����[�^�[
		/// @param name="side_blush" : �T�C�h�̑|���p���[�^�[
//		void motors( bool main_blush, bool vacuum, bool side_brush );

		/// @brief : �������m�F�֐�
		/// @return : ��������
		bool isInitialize() const;

		/// @brief : �Z���T�f�[�^���X�V����
		/// @return : ��������
		bool renewSensorData();

		/// @brief : �Z���T�f�[�^�擾
		/// @return : �Z���T�f�[�^�\����
		SensorData getSensorData() const;

		/// @brief : ���y�Z�b�g�i�P���j
		/// @param name="song_no" : �Z�b�g���鉹�y�ԍ� �i0-4)
		/// @param name="sound" : ��
		/// @return : ��������
		bool song( unsigned int song_no, const Sound& sound );

		/// @brief : ���y�Z�b�g�i�A���j
		/// @param name="song_no" : �Z�b�g���鉹�y�ԍ� �i0-4)
		/// @param name="sound" : �A��
		/// @return : ��������
		bool song( unsigned int song_no, const std::vector<Sound>& sounds );
		
		/// @brief : ���y�Đ�
		/// @param name="song_no" : �Đ����鉹�y (0-4)
		bool play( const unsigned int& song_no );


	// �����Ŏg�p����֐�------------------------------------
	protected:
		bool wake();
		bool sleep();
		void convertInt16toChar( short v, char* ret );
		short convertChartoInt16( const unsigned char *dat );
		unsigned short convertChartoUInt16( const unsigned char *dat );
		char convertTonetoChar( const Tone& tone );
		void setWalls( const unsigned char* );
		void setWheelDrops( const unsigned char* );
		void setPosition( const unsigned char* );
		void setButtons( const unsigned char* );
		void setCliffs( const unsigned char* );
		void setBumps( const unsigned char* );
		void setDirtDetector( const unsigned char* );
		void setBattery( const unsigned char* );
		void setMotorOverCurrent( const unsigned char* );

		//asari
		short Exchange(short*);

	private:
		bool init_flag_;
		serial::CSerial serial_;
		SensorData sensor_data_;

	public:
		//�{�^��1�񉟂��Ƃǂ̂��炢�ړ����邩�̈ړ�����
		static const int distance = 500;
		//�{�^��1�񉟂��Ƃǂ̂��炢�J�X���邩�̉�]�p�x
		static const int rotate = 1000;
		//�{�^��1�񉟂����Ƃ��̓��쎞��
		int time;
		int StartTime;
		int interval;
		int speed;
		int rotation;

		void RoombaConnect1();
		void RoombaConnect2();
		void RoombaConnect3();
		void RoombaConnect4();
		void RoombaDisConnect1();
		void RoombaDisConnect2();
		void RoombaDisConnect3();
		void RoombaDisConnect4();

		void RoombaGo1();
		void RoombaGoRight1();
		void RoombaGoLeft1();
		void RoombaRight1();
		void RoombaLeft1();
		void RoombaStop1();
		void RoombaBack1();
		void RoombaBackRight1();
		void RoombaBackLeft1();

		void RoombaGoLeftfor3rotation1();
		void RoombaBackLeftfor3rotation1();
		void RoombaGoRightfor3rotation1();
		void RoombaBackRightfor3rotation1();
		
		void RoombaGo2();
		void RoombaGoRight2();
		void RoombaGoLeft2();
		void RoombaRight2();
		void RoombaLeft2();
		void RoombaStop2();
		void RoombaBack2();
		void RoombaBackRight2();
		void RoombaBackLeft2();
		
		
		void RoombaGoRightfor3rotation2();
		void RoombaBackRightfor3rotation2();
		void RoombaGoLeftfor3rotation2();
		void RoombaBackLeftfor3rotation2();

		void RoombaGo3();
		void RoombaGoRight3();
		void RoombaGoLeft3();
		void RoombaRight3();
		void RoombaLeft3();
		void RoombaStop3();
		void RoombaBack3();
		void RoombaBackRight3();
		void RoombaBackLeft3();

				
		void RoombaGoLeftfor3rotation3();
		void RoombaBackLeftfor3rotation3();
		void RoombaGoRightfor3rotation3();
		void RoombaBackRightfor3rotation3();


		void RoombaGo4();
		void RoombaGoRight4();
		void RoombaGoLeft4();
		void RoombaRight4();
		void RoombaLeft4();
		void RoombaStop4();
		void RoombaBack4();
		void RoombaBackRight4();
		void RoombaBackLeft4();

		//�ړ��̂��߂̊֐��񂹏W��
		void RecToLine(int);
		void LineToRec(int);
		void RecToSep(int);
		void SepToRec(int);

		void GetSensor1();
		void GetSensor2();
		bool GetClush1();
		bool GetClush2();
	};
};

//
//class CControl
//{
//public:
//	CControl(void);
//	~CControl(void);
//};
//
