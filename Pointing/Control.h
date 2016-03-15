#pragma once

#include <cstdlib>
#include <string>
#include "Serial.h"

using namespace serial;
namespace roomba{

	//充電状態
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

	/// @brief : 音楽用のコード
	struct Note {
		enum type : int {
			C=0, CS, D, DS, E, F, FS, G, GS, A, AS, B, REST
		};
	};

	/// @brief : 音階
	class Tone {
	public:
		Tone() 
			: note(Note::C), octave(1) {}
		Tone( unsigned int note_, unsigned int oct_ ) 
			: note( note_ ), octave( oct_ )	{}
		unsigned int note;
		unsigned int octave;
	};

	/// @brief : 一音
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

	/// @brief	： ルンバのセンサデータを保存するための構造体群
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
	* @brief : ルンバをコントロールするための関数
	*		 : パッシブモードを使用する
	*/
	class CControl
	{

	// コンストラクタ、デストラクタ------------------------
	public:
		CControl(void);
		virtual ~CControl(void);

	// アクセス用関数--------------------------------------
	public:
		/// @brief : 初期化処理
		/// @param name="com" : 使用するシリアルポート
		/// @return : 成功判定
		bool initialize( const std::string& com );
		
		/// @brief : 終了処理
		/// @return : 成功判定
		bool finalize();

		/// @brief : 半径と速度を指定した移動
		/// @param name="v" : 移動速度 [mm/s] (-500-500)
		/// @param name="l" : 回転半径 [mm] (-2000-2000)
		void drive( short v, short r );
		
		/// @brief : 左右車輪に速度指定をして動かす
		/// @param name="l" : 左車輪の速度 [mm/s] (-500 - 500)
		/// @param name="r" : 右車輪の速度 [mm/s] (-500 - 500)
		void driveDirect( short l, short r );
	
		/// @breif : 掃除動作開始
		void clean();
		
		/// @brief : その場を掃除
		void spot();

		/// @brief : ドックの検索
		void seekDock();

		/// @brief : モーター動作（ブラシ）
		/// @param name="main_blush" : メインの掃除用モーター
		/// @param name="vacuum" : 吸引モーター
		/// @param name="side_blush" : サイドの掃除用モーター
//		void motors( bool main_blush, bool vacuum, bool side_brush );

		/// @brief : 初期化確認関数
		/// @return : 成功判定
		bool isInitialize() const;

		/// @brief : センサデータを更新する
		/// @return : 成功判定
		bool renewSensorData();

		/// @brief : センサデータ取得
		/// @return : センサデータ構造体
		SensorData getSensorData() const;

		/// @brief : 音楽セット（単音）
		/// @param name="song_no" : セットする音楽番号 （0-4)
		/// @param name="sound" : 音
		/// @return : 成功判定
		bool song( unsigned int song_no, const Sound& sound );

		/// @brief : 音楽セット（連音）
		/// @param name="song_no" : セットする音楽番号 （0-4)
		/// @param name="sound" : 連音
		/// @return : 成功判定
		bool song( unsigned int song_no, const std::vector<Sound>& sounds );
		
		/// @brief : 音楽再生
		/// @param name="song_no" : 再生する音楽 (0-4)
		bool play( const unsigned int& song_no );


	// 内部で使用する関数------------------------------------
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
		//ボタン1回押すとどのくらい移動するかの移動距離
		static const int distance = 500;
		//ボタン1回押すとどのくらい開店するかの回転角度
		static const int rotate = 1000;
		//ボタン1回押したときの動作時間
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

		//移動のための関数寄せ集め
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
