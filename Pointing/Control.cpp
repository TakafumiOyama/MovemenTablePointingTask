
#include "StdAfx.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <cstdio>
//#include <ctime>
//#include <MMSystem.h>
//#include "boost/cstdint.hpp"
#include "Control.h"

using namespace serial;
using namespace roomba;

serial::CSerial serial_1;
serial::CSerial serial_2;
serial::CSerial serial_3;
serial::CSerial serial_4;

/**
* @brief : 命令用のコード
*/ 
//オペコードが与えるそれぞれの数値を変数に
struct Opcode {					
	enum type : unsigned short{
		START = 128,
		CONTROL = 130,
		SAFE_MODE = 131,
		FULL_MODE = 132,
		DRIVE = 137,
		CLEAN = 135,
		MAX = 136,
		SPOT = 134,
		SEEK_DOCK = 143,
		SCHEDULE = 167,
		SENSORS = 142,
		MOTORS = 138,
		DRIVE_DIRECT = 145,
		POWER = 133,
		SONG = 140,
		PLAY = 141
	};
};

/**
* @brief	: センサ値のデータ配列
*/ 
//センサーの値がどのように取得されているか
struct SensorByte {
	enum type : unsigned short{
		BUMPS_WHEELDROPS = 0,
		WALL = 1,
		CLIFF_LEFT = 2,
		CLIFF_FRONT_LEFT = 3,
		CLIFF_FRONT_RIGHT= 4,
		CLIFF_RIGHT = 5,
		VIRTUAL_WALL = 6,
		MOTOR_OVERCURRENTS = 7,
		DIRT_DETECTOR_LEFT = 8,
		DIRT_DETECTOR_RIGHT = 9,
		ROMOTE_OPCODE = 10,
		BUTTONS = 11,
		DISTANCE = 12,
		ANGLE = 14,
		CHARGE_STATE = 16,
		VOLTAGE = 17,
		CURRENT = 19,
		TEMPERATURE = 21,
		CHARGE = 22,
		CAPACITY = 24
	};
};


/**
* @brief	: コンストラクタ、デストラクタ
*/ 
CControl::CControl(void)
	: serial_()
	, init_flag_(false)
	, sensor_data_()
	, time(0)
	, StartTime(0)
{
}


CControl::~CControl(void)
{
	//finalize();
}
/**
* @brief	: 初期化関数
* @param[in]: 使用するポート番号
* @return	: 成功判定
*/
bool CControl::initialize( const std::string& com ) {
	// 指定ポートオープン
	if( !serial_.CommOpen()) {
		TRACE("Open error\n");
		return false;
	}

	if( !wake() ) {
		TRACE("roomba change mode failed\n");
		return false;
	}

	init_flag_ = true;
	return true;
}

/**
* @brief	: 終了処理
* @return	: 成功判定
*/
bool CControl::finalize() {
	drive( 0, 0 );
	sleep();
	return true;
}

/**
* @brief	: 初期化確認関数
* @return	: 成功判定
*/
bool CControl::isInitialize() const {
	return init_flag_;
}

/**
* @brief	: RoombaにFULL_MODEで接続
* @return	: 成功判定
*/
bool CControl::wake() {
	// passiveモードに遷移
//	serial_.CommMode(Opcode::START);
	std::string command;
	command = Opcode::START;
	serial_.CommMode(command);
	if( !serial_.CommMode( command/*Opcode::START*/ ) ) { 
		TRACE("can't send opcode : START\n");
		return false;
	}

	// SAFEモードに遷移
//	serial_.CommMode(Opcode::SAFE_MODE);
	command = Opcode::SAFE_MODE;
	serial_.CommMode(command);
	if( !serial_.CommMode(command/* Opcode::SAFE_MODE*/ ) ) {
		TRACE("can't send opcode : SAFE_MODE\n");	
		return false;
	}

	// FULLモードに遷移
//	serial_.CommMode(Opcode::FULL_MODE);
	command = Opcode::FULL_MODE;
	serial_.CommMode(command);
	if( !serial_.CommMode(command/* Opcode::FULL_MODE*/ ) ) {
		TRACE("can't send opcode : FULL_MODE\n");	
		return false;
	}

	TRACE("roomba connect success\n");
	return true;
}

/**
* @brief	: Roombaをスリープ状態にする
* @return	: 成功判定
*/
bool CControl::sleep() {
	std::string command;
	command = Opcode::POWER;
	serial_.CommMode(command);
	if( !serial_.CommMode( command/*Opcode::POWER*/ ) ) {
		std::cerr << "sleep error" << std::endl;
		return false;
	}
	return false;
}

	
int Num_max(int x,int y)
{
	if(x<y || x==y) return y;
	return x;
}
int Num_min(int x,int y)
{
	if(x>y || x==y) return y;
	return x;
}


//shortの上位バイトと下位バイトを入れ替える関数
short CControl::Exchange(short *val)
{
	union
	{
		short s;
		char c[2];
	}tmp;

	char x;
	tmp.s = *val;
	TRACE("val = %x %x\n",tmp.c[0], tmp.c[1]);
	x = tmp.c[0];
	tmp.c[0] = tmp.c[1];
	tmp.c[1] = x;
	TRACE("val = %x %x\n",tmp.c[0], tmp.c[1]);
	return tmp.s;
}

/**
* @brief	: 回転半径、移動速度を指定して動かす
* @param[in]: 移動速度
* @param[in]: 回転半径
*/
void CControl::drive( short v, short r ) {

	// 最大値、最小値の調整
	v = Num_max( (short)-500, Num_min( (short)500, v ) ); //移動速度
	r = Num_max( (short)-2000, Num_min( (short)2000, r ) ); //回転半径
	TRACE("v = %x, r = %x\n",v,r);
	union{
	short s;
	char c[2];
	}dat;

		// 送信
	char send_data[5] = {0};
	std::string command;
		command = Opcode::DRIVE;
		//ステラ
		//*((short*)(send_data + 1)) = v;
		//*((short*)(send_data + 3)) = r;

		//上位バイトと下位バイトの入れ替え
		char temp;
		dat.s = v;
		send_data[0] = dat.c[1];
		send_data[1] = dat.c[0];
		dat.s = r;
		send_data[2] = dat.c[1];
		send_data[3] = dat.c[0];
//		TRACE("drive Op = %x, v1 = %d, v2 = %d, r1 = %d, r2 = %d\n",command,send_data[0],send_data[1],send_data[2],send_data[3]);

		//temp = send_data[1];
		//send_data[1] = send_data[2];
		//send_data[2] = temp;
		//temp = send_data[3];
		//send_data[3] = send_data[4];
		//send_data[4] = temp;

		//convertInt16toChar( v, &send_data[1] );
		//convertInt16toChar( r, &send_data[3] );
		//TRACE("Op = %x, v1 = %x, v2 = %x, r1 = %x, r2 = %x\n",send_data[0],send_data[1],send_data[2],send_data[3],send_data[4]);
		//for(int i=0;i<5;i++){
		//	CString str;
		//	str = (CString)send_data[i];
		//	serial_.CommSend1(str);
		//	TRACE("str = %s\n", str);
		//}
		serial_.CommMode(command);
		serial_.CommSend(send_data,4);
		//for(int i=0;i<5;i++)
		//{
		//	//data = &send_data[i];
		//	serial_.CommSend(&send_data[i],1);
		//}

		//serial_.CommSend(send_data,5);
		//for(int i=0;i<5;i++){
		//	TRACE("data_send = %x\n",send_data[i]);
		//}

		//	serial_.CommSend( send_data,5);
	//	TRACE(" %d, %d へdrive\n",v,r);
	//	TRACE("time = %d",time);
	//time = 0;
}



/**
* @brief	: 普通に掃除する
*/
void CControl::clean() {
	std::string command;
	command = Opcode::CLEAN;
	serial_.CommMode(command);
	//serial_.CommMode( Opcode::CLEAN );
}

/**
* @brief		: int16bitをcharに変換する関数
* @param[in]	: 変換する変数
* @param[out]	: 変換結果
*/
void CControl::convertInt16toChar( short v, char* ret ) {
	unsigned char *tmp;
	tmp = reinterpret_cast<unsigned char*>(&v);
	ret[1] = *tmp;
	ret[0] = *(tmp+1);	
}


/// @brief	: 定置清掃
void CControl::spot() {
	std::string command;
	command = Opcode::SPOT;
	serial_.CommMode(command);
	//serial_.CommMode( Opcode::SPOT );
}

/// @brief	: 充電ドックに戻る
void CControl::seekDock() {
	std::string command;
	command = Opcode::SEEK_DOCK;
	serial_.CommMode(command);
	//serial_.CommMode( Opcode::SEEK_DOCK );
}

//void CControl::motors(bool main_blush, bool vacuum, bool side_brush) {
//	char tmp[2] = {0,0};
//	tmp[0] = Opcode::MOTORS;
//	tmp[1] |= (main_blush) ? 0x04 : 0x00;
//	tmp[1] |= (vacuum) ? 0x02 : 0x00;
//	tmp[1] |= (side_brush) ? 0x01 : 0x00;
//	serial_.send( tmp, 2 );
//}

/// @brief	: センサデータを更新
bool CControl::renewSensorData() {

	//// センサデータを全て取得
	////char data[2];
	////data[0] = Opcode::SENSORS;
	////data[1] = 0;
	////serial_.send( data, 2 );
	//std::string command;
	//command = Opcode::SENSORS;
	//serial_.CommMode(command);

	//// 受信プロセス
	//// TODO 26biyeたまるまで待機することになった。なんとか、しないとまずい？
	//std::string result;
	//for( ;result.size() < 26; ) {
	//	std::string str;
	//	if( !serial_.receive( str, 100 ) ) {
	//		std::cerr << "get sensors error" << std::endl;
	//		return false;
	//	}
	//	result += str;
	//}

	//// 受信データの分析開始
	//// 今回は、脱輪、壁、仮想壁、ボタン、距離、角度の取得を行う
	//unsigned char sensors[26];
	//std::copy( result.begin(), result.end(), sensors );

	//setWalls( sensors );
	//setWheelDrops( sensors );
	//setBumps( sensors );
	//setPosition( sensors );
	//setButtons( sensors );
	//setCliffs( sensors );
	//setBattery( sensors );
	//setMotorOverCurrent( sensors );

	return true;
}

short CControl::convertChartoInt16( const unsigned char *data ) {
	short tmp = ((*data << 8) & 0xFF00) | *(data+1);
	return tmp;
}

unsigned short CControl::convertChartoUInt16(const unsigned char *data) {
	unsigned short tmp = ((*data << 8) & 0xFF00) | *(data+1);
	return tmp;
}




/// @brief : 音楽を鳴らす
bool CControl::play(const unsigned int& song_no) {

	return true;
}

bool CControl::song( unsigned int song_no, const roomba::Sound &sound) {

	return true;
}

bool CControl::song( unsigned int song_no, const std::vector<Sound>& music ) {
	return true;
}

char CControl::convertTonetoChar(const roomba::Tone &tone) {
	char ret = 0;
	if( tone.note != Note::REST ) ret = static_cast<char>(24 + (tone.octave * 12) + tone.note);
	return ret;
}

void CControl::RoombaConnect1()
{
	serial_1.CommOpen();
	std::string command;
	command = Opcode::START;
	serial_1.CommMode(command);
	command = Opcode::CONTROL;
	serial_1.CommMode(command);
}

void CControl::RoombaConnect2()
{
	serial_2.CommOpen2();
	std::string command;
	command = Opcode::START;
	serial_2.CommMode2(command);
	command = Opcode::CONTROL;
	serial_2.CommMode2(command);
}

void CControl::RoombaConnect3()
{
	serial_3.CommOpen3();
	std::string command;
	command = Opcode::START;
	serial_3.CommMode3(command);
	command = Opcode::CONTROL;
	serial_3.CommMode3(command);
}

void CControl::RoombaConnect4()
{
	serial_4.CommOpen4();
	std::string command;
	command = Opcode::START;
	serial_4.CommMode4(command);
	command = Opcode::CONTROL;
	serial_4.CommMode4(command);
}

void CControl::RoombaDisConnect1()
{
	std::string command;
	command = Opcode::START;
	serial_1.CommMode(command);
	serial_1.CommClose();
}

void CControl::RoombaDisConnect2()
{
	std::string command;
	command = Opcode::START;
	serial_2.CommMode2(command);
	serial_2.CommClose2();
}

void CControl::RoombaDisConnect3()
{
	std::string command;
	command = Opcode::START;
	serial_3.CommMode3(command);
	serial_3.CommClose3();
}

void CControl::RoombaDisConnect4()
{
	std::string command;
	command = Opcode::START;
	serial_4.CommMode4(command);
	serial_4.CommClose4();
}

// ************* Controll **************** 

void CControl::RoombaGo1()
{
	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;
	send_data[0] = 0;
	send_data[1] = 250;
	send_data[2] = 0;//100;//128
	send_data[3] = 250;
	serial_1.CommMode(command);
	serial_1.CommSend(send_data,4);
}


void CControl::RoombaGoRight1()
{
	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 0;
	send_data[1] = 100;	//それぞれの車輪50(0,50)/100(0.100)で設定中
	//左の車輪
	send_data[2] = 0;
	send_data[3] = 200;

	serial_1.CommMode(command);
	serial_1.CommSend(send_data,4);
}

void CControl::RoombaGoLeft1()
{
	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 0;
//	send_data[1] = 248;	
	send_data[1] = 200;
	//左の車輪
	send_data[2] = 0;
	send_data[3] = 100;

	serial_1.CommMode(command);
	serial_1.CommSend(send_data,4);


}

void CControl::RoombaRight1()
{
	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	send_data[0] = 255;	//それぞれの車輪-50(255,206)/50(0.50)で設定中
	send_data[1] = 216;
	send_data[2] = 0;
	send_data[3] = 40;
	serial_1.CommMode(command);
	serial_1.CommSend(send_data,4);
}

void CControl::RoombaLeft1()
{
	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	send_data[0] = 0;
	send_data[1] = 40;	//それぞれの車輪50(0,50)/-50(255.206)で設定中
	send_data[2] = 255;
	send_data[3] = 216;
	serial_1.CommMode(command);
	serial_1.CommSend(send_data,4);

	//Sleep(1000);
	//
	//send_data[0] = 0;
	//send_data[1] = 0;	//それぞれの車輪50(0,50)/-50(255.206)で設定中
	//send_data[2] = 0;
	//send_data[3] = 0;
	serial_1.CommMode(command);
	serial_1.CommSend(send_data,4);
	
}

void CControl::RoombaStop1()
{
	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE;

	serial_1.CommMode(command);
	serial_1.CommSend(send_data,4);

}

void CControl::RoombaBack1()
{


	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;
	send_data[0] = 255;//255.6
	send_data[1] = 6;
	send_data[2] = 255;
	send_data[3] = 6;

	serial_1.CommMode(command);
	serial_1.CommSend(send_data,4);


}

void CControl::RoombaBackRight1()
{

	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 255;
//	send_data[1] = 8;	//(=248)
//	send_data[1] = 6;	//(=250)
	//send_data[1] = 24;	//(=232)
	send_data[1] = 156;	//
	//左の車輪
	send_data[2] = 255;
	send_data[3] = 56;//(=-100)
	//send_data[3] = 163;//(=-93)


	serial_1.CommMode(command);
	serial_1.CommSend(send_data,4);

}

void CControl::RoombaBackLeft1()
{

	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 255;
	//send_data[1] = 156;
	send_data[1] = 56;	

	//左の車輪
	send_data[2] = 255;
	//send_data[3] = 8; 
	send_data[3] = 156; 
	serial_1.CommMode(command);
	serial_1.CommSend(send_data,4);
}


void CControl::RoombaGoLeftfor3rotation1()
{

	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 0;
	send_data[1] = 250;	//それぞれの車輪100(0,100)/50(0.50)で設定中
	//左の車輪
	send_data[2] = 0;
	send_data[3] = 170;

	serial_1.CommMode(command);
	serial_1.CommSend(send_data,4);


}

void CControl::RoombaBackLeftfor3rotation1()
{

		std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 255;
	send_data[1] = 6;	//それぞれの車輪-100(255,156)/-50(255.206)で設定中
	//左の車輪
	send_data[2] = 255;
	send_data[3] = 81;
	serial_1.CommMode(command);
	serial_1.CommSend(send_data,4);



}




void CControl::RoombaGo2()
{

	//0.250.0.230
	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;
	send_data[0] = 0;
	send_data[1] = 250;
	send_data[2] = 0;//100;//128
	send_data[3] = 230;
	serial_2.CommMode2(command);
	serial_2.CommSend2(send_data,4);



}


void CControl::RoombaGoRight2()
{
//	std::string command;
//	char send_data[4]={0};
//	command = Opcode::DRIVE_DIRECT;
//
//	send_data[0] = 0;
//	send_data[1] = 50;	//それぞれの車輪100(0,100)/255(0.255)で設定中
//	send_data[2] = 0;
//	send_data[3] = 131;
//	serial_2.CommMode2(command);
//	serial_2.CommSend2(send_data,4);


	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 0;
	send_data[1] = 100;	//それぞれの車輪100(0,100)/255(0.255)で設定中
	//左の車輪
	send_data[2] = 0;
	send_data[3] = 248;
	
	serial_2.CommMode2(command);
	serial_2.CommSend2(send_data,4);

}

void CControl::RoombaGoLeft2()
{
	//std::string command;
	//char send_data[4]={0};
	//command = Opcode::DRIVE_DIRECT;

	//send_data[0] = 0;
	//send_data[1] = 131;	//それぞれの車輪100(0,100)/-100(255.156)で設定中
	//send_data[2] = 0;
	//send_data[3] = 50;
	//serial_2.CommMode2(command);
	//serial_2.CommSend2(send_data,4);

	
	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 0;
	send_data[1] = 248;	//それぞれの車輪100(0,100)/-100(255.156)で設定中
	//左の車輪
	send_data[2] = 0;
	send_data[3] = 100;

	serial_2.CommMode2(command);
	serial_2.CommSend2(send_data,4);


}

void CControl::RoombaRight2()
{
	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	send_data[0] = 255;	//それぞれの車輪-100(255,156)/100(0.100)で設定中
	send_data[1] = 216;
	send_data[2] = 0;
	send_data[3] = 40;
	serial_2.CommMode2(command);
	serial_2.CommSend2(send_data,4);
}

void CControl::RoombaLeft2()
{
	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	send_data[0] = 0;
	send_data[1] = 40;	//それぞれの車輪100(0,100)/-100(255.156)で設定中
	send_data[2] = 255;
	send_data[3] = 216;
	serial_2.CommMode2(command);
	serial_2.CommSend2(send_data,4);
}

void CControl::RoombaStop2()
{
	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE;

	serial_2.CommMode2(command);
	serial_2.CommSend2(send_data,4);

}

void CControl::RoombaBack2()
{
	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;
	send_data[0] = 255;
	send_data[1] = 6;
	send_data[2] = 255;
	send_data[3] = 26;

	serial_2.CommMode2(command);
	serial_2.CommSend2(send_data,4);
}

void CControl::RoombaBackRight2()
{
	//std::string command;
	//char send_data[4]={0};
	//command = Opcode::DRIVE_DIRECT;

	//send_data[0] = 255;
	//send_data[1] = 206;	//それぞれの車輪-100(0,100)/-255(255.1)で設定中
	//send_data[2] = 255;
	//send_data[3] = 125;
	//serial_2.CommMode2(command);
	//serial_2.CommSend2(send_data,4);

		std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 255;
	send_data[1] = 8;	//それぞれの車輪-100(0,100)/-255(255.1)で設定中
	//左の車輪
	send_data[2] = 255;
	send_data[3] = 156;

	serial_2.CommMode2(command);
	serial_2.CommSend2(send_data,4);

}

void CControl::RoombaBackLeft2()
{
	//std::string command;
	//char send_data[4]={0};
	//command = Opcode::DRIVE_DIRECT;

	//send_data[0] = 255;
	//send_data[1] = 125;	//それぞれの車輪-255(255,1)/-100(255.156)で設定中
	//send_data[2] = 255;
	//send_data[3] = 206;
	//serial_2.CommMode2(command);
	//serial_2.CommSend2(send_data,4);

		std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 255;
	send_data[1] = 156;	//それぞれの車輪-255(255,1)/-100(255.156)で設定中
	//左の車輪
	send_data[2] = 255;
	send_data[3] = 8;

	serial_2.CommMode2(command);
	serial_2.CommSend2(send_data,4);

}

void CControl::RoombaGoLeftfor3rotation2()
{

	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 0;
	send_data[1] = 170;	//それぞれの車輪100(0,100)/50(0.50)で設定中
	//左の車輪
	send_data[2] = 0;
	send_data[3] = 250;

	serial_2.CommMode2(command);
	serial_2.CommSend2(send_data,4);


}

void CControl::RoombaBackLeftfor3rotation2()
{

		std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 255;
	send_data[1] = 81;	//それぞれの車輪-100(255,156)/-50(255.206)で設定中
	//左の車輪
	send_data[2] = 255;
	send_data[3] = 6;
	serial_2.CommMode2(command);
	serial_2.CommSend2(send_data,4);



}

void CControl::RoombaGoRightfor3rotation2()
{

	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 0;
	send_data[1] = 170;	//それぞれの車輪100(0,100)/50(0.50)で設定中
	//左の車輪
	send_data[2] = 0;
	send_data[3] = 255;

	serial_2.CommMode2(command);
	serial_2.CommSend2(send_data,4);


}

void CControl::RoombaBackRightfor3rotation2()
{

		std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 255;
	send_data[1] = 6;	//それぞれの車輪-100(255,156)/-50(255.206)で設定中
	//左の車輪
	send_data[2] = 255;
	send_data[3] = 81;
	serial_2.CommMode2(command);
	serial_2.CommSend2(send_data,4);



}




void CControl::GetSensor1()
{
	
	std::string command;
	char send[1] = {0};
	command = Opcode::SENSORS;
	send[0] = 25;
	serial_1.CommMode(command);
	serial_1.CommSend(send,1);
	serial_1.CommReceive(2);
	TRACE("Roomba 1 のバッテリー残量 %d %d\n",serial_1.s_data1[0],serial_1.s_data1[1]);


}

void CControl::GetSensor2()
{
	
	std::string command;
	char send[1] = {0};
	command = Opcode::SENSORS;
	send[0] = 25;

	
	serial_2.CommMode2(command);
	serial_2.CommSend2(send,1);
	serial_2.CommReceive2(2);
	TRACE("Roomba 2 のバッテリー残量 %d %d\n",serial_2.s_data2[0],serial_2.s_data2[1]);

}

bool CControl::GetClush1()
{
	std::string command;
	char send[1] = {0};
	command = Opcode::SENSORS;
	send[0] = 7;

	serial_1.CommMode(command);
	serial_1.CommSend(send,1);
	serial_1.CommReceive(1);

	if(serial_1.s_data1[0] == 1 || serial_1.s_data1[0] == 2 || serial_1.s_data1[0] == 3)
		return true;
	else
		return false;
}

bool CControl::GetClush2()
{
	std::string command;
	char send[1] = {0};
	command = Opcode::SENSORS;
	send[0] = 7;

	serial_2.CommMode2(command);
	serial_2.CommSend2(send,1);
	serial_2.CommReceive2(1);
	
	if(serial_2.s_data2[0] == 1 || serial_2.s_data2[0] == 2 || serial_2.s_data2[0] == 3)
		return true;
	else
		return false;
}

void CControl::RoombaGo3()
{
	//std::string command;
	//char send_data[4]={0};
	//command = Opcode::DRIVE;
	//send_data[0] = 0;
	//send_data[1] = 100;
	//send_data[2] = 128;
	//send_data[3] = 0;
	//serial_1.CommMode(command);
	//serial_1.CommSend(send_data,4);

	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;
	send_data[0] = 0;
	send_data[1] = 150;
	send_data[2] = 0;//100;//128
	send_data[3] = 150;
	serial_3.CommMode3(command);
	serial_3.CommSend3(send_data,4);



}


void CControl::RoombaGoRight3()
{
	//std::string command;
	//char send_data[4]={0};
	//command = Opcode::DRIVE_DIRECT;

	//send_data[0] = 0;
	//send_data[1] = 50;	//それぞれの車輪50(0,50)/100(0.100)で設定中
	//send_data[2] = 0;
	//send_data[3] = 131;
	//serial_1.CommMode(command);
	//serial_1.CommSend(send_data,4);

		std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 0;
	send_data[1] = 100;	//それぞれの車輪50(0,50)/100(0.100)で設定中
	//左の車輪
	send_data[2] = 0;
	send_data[3] = 200;

	serial_3.CommMode3(command);
	serial_3.CommSend3(send_data,4);

}

void CControl::RoombaGoLeft3()
{
	//std::string command;
	//char send_data[4]={0};
	//command = Opcode::DRIVE_DIRECT;

	//send_data[0] = 0;
	//send_data[1] = 131;	//それぞれの車輪100(0,100)/50(0.50)で設定中
	//send_data[2] = 0;
	//send_data[3] = 50;
	//serial_1.CommMode(command);
	//serial_1.CommSend(send_data,4);

		std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 0;
	send_data[1] = 200;	//それぞれの車輪100(0,100)/50(0.50)で設定中
	//左の車輪
	send_data[2] = 0;
	send_data[3] = 100;

	serial_3.CommMode3(command);
	serial_3.CommSend3(send_data,4);


}

void CControl::RoombaRight3()
{
	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//send_data[0] = 255;	//それぞれの車輪-50(255,206)/50(0.50)で設定中
	//send_data[1] = 216;
	//send_data[2] = 0;
	//send_data[3] = 40;
	//serial_3.CommMode3(command);
	//serial_3.CommSend3(send_data,4);

	//send_data[0] = 255;	//それぞれの車輪-50(255,206)/50(0.50)で設定中
	//send_data[1] = 206;
	//send_data[2] = 0;
	//send_data[3] = 50;
	//serial_3.CommMode3(command);
	//serial_3.CommSend3(send_data,4);

	send_data[0] = 255;	//それぞれの車輪-50(255,206)/50(0.50)で設定中
	send_data[1] = 215;
	send_data[2] = 0;
	send_data[3] = 41;
	serial_3.CommMode3(command);
	serial_3.CommSend3(send_data,4);

	//Sleep(2000);
	//
	//send_data[0] = 0;	//それぞれの車輪-50(255,206)/50(0.50)で設定中
	//send_data[1] = 0;
	//send_data[2] = 0;
	//send_data[3] = 0;
	//serial_3.CommMode3(command);
	//serial_3.CommSend3(send_data,4);
}



void CControl::RoombaLeft3()
{
	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//send_data[0] = 0;
	//send_data[1] = 40;	//それぞれの車輪50(0,50)/-50(255.206)で設定中
	//send_data[2] = 255;
	//send_data[3] = 216;
	//serial_3.CommMode3(command);
	//serial_3.CommSend3(send_data,4);
	//Sleep(2000);
	//
	//send_data[0] = 0;	//それぞれの車輪-50(255,206)/50(0.50)で設定中
	//send_data[1] = 0;
	//send_data[2] = 0;
	//send_data[3] = 0;
	//serial_3.CommMode3(command);
	//serial_3.CommSend3(send_data,4);
	send_data[0] = 0;
	send_data[1] = 45;	//それぞれの車輪50(0,50)/-50(255.206)で設定中
	send_data[2] = 255;
	send_data[3] = 211;
	serial_3.CommMode3(command);
	serial_3.CommSend3(send_data,4);
	

}

void CControl::RoombaStop3()
{
	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE;

	serial_3.CommMode3(command);
	serial_3.CommSend3(send_data,4);

}

void CControl::RoombaBack3()
{
	//std::string command;
	//char send_data[4]={0};
	//command = Opcode::DRIVE;
	//send_data[0] = 255;
	//send_data[1] = 156;
	//send_data[2] = 128;
	//send_data[3] = 0;

	//serial_1.CommMode(command);
	//serial_1.CommSend(send_data,4);

		std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;
	send_data[0] = 255;
	send_data[1] = 106;
	send_data[2] = 255;
	send_data[3] = 106;

	serial_3.CommMode3(command);
	serial_3.CommSend3(send_data,4);


}

void CControl::RoombaBackRight3()
{
	//std::string command;
	//char send_data[4]={0};
	//command = Opcode::DRIVE_DIRECT;

	//send_data[0] = 255;
	//send_data[1] = 206;	//それぞれの車輪-50(255,206)/-100(255.156)で設定中
	//send_data[2] = 255;
	//send_data[3] = 125;
	//serial_1.CommMode(command);
	//serial_1.CommSend(send_data,4);


		std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 255;
	send_data[1] = 156;	//それぞれの車輪-50(255,206)/-100(255.156)で設定中
	//左の車輪
	send_data[2] = 255;
	send_data[3] = 56;

	serial_3.CommMode3(command);
	serial_3.CommSend3(send_data,4);

}

void CControl::RoombaBackLeft3()
{
	//std::string command;
	//char send_data[4]={0};
	//command = Opcode::DRIVE_DIRECT;

	//send_data[0] = 255;
	//send_data[1] = 125;	//それぞれの車輪-100(255,156)/-50(255.206)で設定中
	//send_data[2] = 255;
	//send_data[3] = 206;
	//serial_1.CommMode(command);
	//serial_1.CommSend(send_data,4);

	
	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 255;
	send_data[1] = 56;	//それぞれの車輪-100(255,156)/-50(255.206)で設定中
	//左の車輪
	send_data[2] = 255;
	send_data[3] = 156;
	serial_3.CommMode3(command);
	serial_3.CommSend3(send_data,4);
}



void CControl::RoombaGoLeftfor3rotation3()
{

	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 0;
	send_data[1] = 250;	//それぞれの車輪100(0,100)/50(0.50)で設定中
	//左の車輪
	send_data[2] = 0;
	send_data[3] = 170;

	serial_3.CommMode3(command);
	serial_3.CommSend3(send_data,4);


}

void CControl::RoombaBackLeftfor3rotation3()
{

		std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 255;
	send_data[1] = 6;	//それぞれの車輪-100(255,156)/-50(255.206)で設定中
	//左の車輪
	send_data[2] = 255;
	send_data[3] = 81;
	serial_3.CommMode3(command);
	serial_3.CommSend3(send_data,4);



}


void CControl::RoombaGoRightfor3rotation3()
{

	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 0;
	send_data[1] = 170;	//それぞれの車輪100(0,100)/50(0.50)で設定中
	//左の車輪
	send_data[2] = 0;
	send_data[3] = 250;

	serial_3.CommMode3(command);
	serial_3.CommSend3(send_data,4);


}

void CControl::RoombaBackRightfor3rotation3()
{

		std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 255;
	send_data[1] = 81;	//それぞれの車輪-100(255,156)/-50(255.206)で設定中
	//左の車輪
	send_data[2] = 255;
	send_data[3] = 6;
	serial_3.CommMode3(command);
	serial_3.CommSend3(send_data,4);



}



void CControl::RoombaGo4()
{
//	std::string command;
//	char send_data[4]={0};
//	command = Opcode::DRIVE_DIRECT;
//
//	send_data[0] = 0;
//	send_data[1] = 50;	//それぞれの車輪100(0,100)/255(0.255)で設定中
//	send_data[2] = 0;
//	send_data[3] = 131;
//	serial_2.CommMode2(command);
//	serial_2.CommSend2(send_data,4);


	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 0;
	send_data[1] = 100;	//それぞれの車輪100(0,100)/255(0.255)で設定中
	//左の車輪
	send_data[2] = 0;
	send_data[3] = 248;
	
	serial_4.CommMode2(command);
	serial_4.CommSend2(send_data,4);

}


void CControl::RoombaGoRight4()
{
	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	send_data[0] = 0;
	send_data[1] = 50;	//それぞれの車輪100(0,100)/255(0.255)で設定中
	send_data[2] = 0;
	send_data[3] = 131;
	serial_4.CommMode2(command);
	serial_4.CommSend2(send_data,4);
}

void CControl::RoombaGoLeft4()
{
	//std::string command;
	//char send_data[4]={0};
	//command = Opcode::DRIVE_DIRECT;

	//send_data[0] = 0;
	//send_data[1] = 131;	//それぞれの車輪100(0,100)/-100(255.156)で設定中
	//send_data[2] = 0;
	//send_data[3] = 50;
	//serial_2.CommMode2(command);
	//serial_2.CommSend2(send_data,4);

	
	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 0;
	send_data[1] = 248;	//それぞれの車輪100(0,100)/-100(255.156)で設定中
	//左の車輪
	send_data[2] = 0;
	send_data[3] = 100;

	serial_4.CommMode2(command);
	serial_4.CommSend2(send_data,4);

}

void CControl::RoombaRight4()
{
	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	send_data[0] = 255;	//それぞれの車輪-100(255,156)/100(0.100)で設定中
	send_data[1] = 206;
	send_data[2] = 0;
	send_data[3] = 50;
	serial_4.CommMode2(command);
	serial_4.CommSend2(send_data,4);
}

void CControl::RoombaLeft4()
{
	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	send_data[0] = 0;
	send_data[1] = 50;	//それぞれの車輪100(0,100)/-100(255.156)で設定中
	send_data[2] = 255;
	send_data[3] = 206;
	serial_4.CommMode2(command);
	serial_4.CommSend2(send_data,4);
}

void CControl::RoombaStop4()
{
	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE;

	serial_4.CommMode2(command);
	serial_4.CommSend2(send_data,4);

}

void CControl::RoombaBack4()
{
	std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE;
	send_data[0] = 255;
	send_data[1] = 156;
	send_data[2] = 128;
	send_data[3] = 0;

	serial_4.CommMode2(command);
	serial_4.CommSend2(send_data,4);
}

void CControl::RoombaBackRight4()
{
	//std::string command;
	//char send_data[4]={0};
	//command = Opcode::DRIVE_DIRECT;

	//send_data[0] = 255;
	//send_data[1] = 206;	//それぞれの車輪-100(0,100)/-255(255.1)で設定中
	//send_data[2] = 255;
	//send_data[3] = 125;
	//serial_2.CommMode2(command);
	//serial_2.CommSend2(send_data,4);

		std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 255;
	send_data[1] = 8;	//それぞれの車輪-100(0,100)/-255(255.1)で設定中
	//左の車輪
	send_data[2] = 255;
	send_data[3] = 156;

	serial_4.CommMode2(command);
	serial_4.CommSend2(send_data,4);
}

void CControl::RoombaBackLeft4()
{
	//std::string command;
	//char send_data[4]={0};
	//command = Opcode::DRIVE_DIRECT;

	//send_data[0] = 255;
	//send_data[1] = 125;	//それぞれの車輪-255(255,1)/-100(255.156)で設定中
	//send_data[2] = 255;
	//send_data[3] = 206;
	//serial_2.CommMode2(command);
	//serial_2.CommSend2(send_data,4);

		std::string command;
	char send_data[4]={0};
	command = Opcode::DRIVE_DIRECT;

	//右の車輪
	send_data[0] = 255;
	send_data[1] = 156;	//それぞれの車輪-255(255,1)/-100(255.156)で設定中
	//左の車輪
	send_data[2] = 255;
	send_data[3] = 8;

	serial_4.CommMode2(command);
}


void CControl::RecToLine(int a)
{
	switch(a)
	{
	case 10:
		this->RoombaStop1();
		break;
	case 11:
		this->RoombaBack1();
		break;
	case 12:
		this->RoombaLeft1();
		break;
	case 13:
		this->RoombaBack1();
		break;
	case 14:
		this->RoombaRight1();
		break;
	case 15:
		this->RoombaGo1();
		break;
	case 16:
		this->RoombaStop1();
		break;
	case 20:
		this->RoombaStop2();
		break;
	case 21:
		this->RoombaGo2();
		break;
	case 22:
		this->RoombaRight2();
		break;
	case 23:
		this->RoombaGo2();
		break;
	case 24:
		this->RoombaLeft2();
		break;
	case 25:
		this->RoombaBack2();
		break;
	case 26:
		this->RoombaStop2();
		break;
	default:
		break;
	}
}
	
void CControl::LineToRec(int a)
{
	switch(a)
	{
	case 10:
		this->RoombaStop1();
		break;
	case 11:
		this->RoombaBack1();
		break;
	case 12:
		this->RoombaLeft1();
		break;
	case 13:
		this->RoombaGo1();
		break;
	case 14:
		this->RoombaRight1();
		break;
	case 15:
		this->RoombaGo1();
		break;
	case 16:
		this->RoombaStop1();
		break;
	case 20:
		this->RoombaStop2();
		break;
	case 21:
		this->RoombaGo2();
		break;
	case 22:
		this->RoombaRight2();
		break;
	case 23:
		this->RoombaBack2();
		break;
	case 24:
		this->RoombaLeft2();
		break;
	case 25:
		this->RoombaBack2();
		break;
	case 26:
		this->RoombaStop2();
		break;
	default:
		break;
	}
}

void CControl::RecToSep(int a)
{
	switch(a)
	{
	case 10:
		this->RoombaStop1();
		break;
	case 11:
		this->RoombaBack1();
		break;
	case 12:
		this->RoombaStop1();
		break;
	case 20:
		this->RoombaStop2();
		break;
	case 21:
		this->RoombaGo2();
		break;
	case 22:
		this->RoombaStop2();
		break;
	case 30:
		this->RoombaStop3();
		break;
	case 31:
		this->RoombaBack3();
		break;
	case 32:
		this->RoombaStop3();
		break;
	case 40:
		this->RoombaStop4();
		break;
	case 41:
		this->RoombaGo4();
		break;
	case 42:
		this->RoombaStop4();
		break;
	default:
		break;
	}
}

void CControl::SepToRec(int a)
{
	switch(a)
	{
	case 10:
		this->RoombaStop1();
		break;
	case 11:
		this->RoombaGo1();
		break;
	case 12:
		this->RoombaStop1();
		break;
	case 20:
		this->RoombaStop2();
		break;
	case 21:
		this->RoombaBack2();
		break;
	case 22:
		this->RoombaStop2();
		break;
	case 30:
		this->RoombaStop3();
		break;
	case 31:
		this->RoombaGo3();
		break;
	case 32:
		this->RoombaStop3();
		break;
	case 40:
		this->RoombaStop4();
		break;
	case 41:
		this->RoombaBack4();
		break;
	case 42:
		this->RoombaStop4();
		break;
	default:
		break;
	}
}