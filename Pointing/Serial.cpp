#include "StdAfx.h"
#include <list>
#include <iostream>
#include <algorithm>
#include <queue>
//
//#include "boost/foreach.hpp"
//#include "boost/asio.hpp"
//#include "boost/bind.hpp"
//#include "boost/thread.hpp"
//#include "boost/thread/condition.hpp"
//#include "boost/smart_ptr.hpp"
#include "Serial.h"

using namespace serial;
//using namespace boost::asio;
using namespace std;

CSerial::CSerial(void)
	: /*impl(new serial_impl()), */
	  is_connect_(true)
{
}
//
//
CSerial::~CSerial(void)
{
}

//初期化関数
int CSerial::Init(void)
{
	this->CommOpen();
	return 0;

}

//シリアル通信のデバイスを開き，プロトコルの設定
bool CSerial::CommOpen(void)
{
	//該当するRoombaのポート番号をCOM~に入力
	hCom = CreateFileA("\\\\.\\COM3", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	DWORD er = GetLastError();

	if(hCom == INVALID_HANDLE_VALUE){
		m_comErrFlag = true;
		TRACE("Roomba1 接続失敗\n");
		return false;
	}

	DCB dcb;
	memset(&dcb, 0, sizeof(DCB));
    dcb.DCBlength = sizeof(DCB) ;
 //   dcb.BaudRate = CBR_115200;		//有線
    dcb.BaudRate = CBR_57600;		//無線
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    SetCommState(hCom, &dcb);
	TRACE("接続成功\n");
	is_connect_ = true;
	TRACE("is_connect_ = %d\n",is_connect_);
	return true;
}


//Roomba2のポートをオープン
bool CSerial::CommOpen2(void)
{
	//該当するRoombaのポート番号をCOM~に入力
	hCom2 = CreateFileA("\\\\.\\COM9", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	DWORD er = GetLastError();

	if(hCom2 == INVALID_HANDLE_VALUE){
		m_comErrFlag = true;
		TRACE("Roomba2 接続失敗\n");
		return false;
	}

	DCB dcb;
	memset(&dcb, 0, sizeof(DCB));
    dcb.DCBlength = sizeof(DCB) ;
    //dcb.BaudRate = CBR_115200;		//有線
    dcb.BaudRate = CBR_57600;		//無線
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
	SetCommState(hCom2, &dcb);
	TRACE("接続成功\n");
	is_connect_ = true;
	TRACE("is_connect_ = %d\n",is_connect_);
	return true;
}

//Roomba3のポートをオープン
bool CSerial::CommOpen3(void)
{
	//該当するRoombaのポート番号をCOM~に入力
	hCom3 = CreateFileA("\\\\.\\COM13", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);//_T("COM5")   //"\\\\.\\COM10"
	DWORD er = GetLastError();

	if(hCom3 == INVALID_HANDLE_VALUE){
		m_comErrFlag = true;
		TRACE("Roomba3 接続失敗\n");
		return false;
	}

	DCB dcb;
	memset(&dcb, 0, sizeof(DCB));
    dcb.DCBlength = sizeof(DCB) ;
 //   dcb.BaudRate = CBR_115200;		//有線
    dcb.BaudRate = CBR_57600;		//無線
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
	SetCommState(hCom3, &dcb);
	TRACE("接続成功\n");
	is_connect_ = true;
	TRACE("is_connect_ = %d\n",is_connect_);
	return true;
}

//Roomba4のポートをオープン
bool CSerial::CommOpen4(void)
{
	//該当するRoombaのポート番号をCOM~に入力
	hCom4 = CreateFileA("\\\\.\\COM19", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	DWORD er = GetLastError();

	if(hCom4 == INVALID_HANDLE_VALUE){
		m_comErrFlag = true;
		TRACE("Roomba4 接続失敗\n");
		return false;
	}

	DCB dcb;
	memset(&dcb, 0, sizeof(DCB));
    dcb.DCBlength = sizeof(DCB) ;
 //   dcb.BaudRate = CBR_115200;		//有線
    dcb.BaudRate = CBR_57600;		//無線
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
	SetCommState(hCom4, &dcb);
	TRACE("接続成功\n");
	is_connect_ = true;
	TRACE("is_connect_ = %d\n",is_connect_);
	return true;
}

//シリアル通信デバイスの開放
bool CSerial::CommClose(void)
{
	if( !is_connect_ ) 
	{
		TRACE("接続が解除できません\n");
		return false;
		
	}
	if(hCom != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hCom);
		hCom = INVALID_HANDLE_VALUE;
	}

	return true;
}

//シリアル通信デバイスの開放
bool CSerial::CommClose2(void)
{
	if( !is_connect_ ) 
	{
		TRACE("接続が解除できません\n");
		return false;
		
	}
	if(hCom2 != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hCom2);
		hCom2 = INVALID_HANDLE_VALUE;
	}
	return true;
}

//シリアル通信デバイスの開放
bool CSerial::CommClose3(void)
{
	if( !is_connect_ ) 
	{
		TRACE("接続が解除できません\n");
		return false;
		
	}
	if(hCom3 != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hCom3);
		hCom3 = INVALID_HANDLE_VALUE;
	}

	return true;
}

//シリアル通信デバイスの開放
bool CSerial::CommClose4(void)
{
	if( !is_connect_ ) 
	{
		TRACE("接続が解除できません\n");
		return false;
		
	}
	if(hCom4 != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hCom4);
		hCom4 = INVALID_HANDLE_VALUE;
	}
	return true;
}



//Roomba1 の数値データ送信関数
bool CSerial::CommSend(const char* command , int n) {

	if( !is_connect_ ) 
		return false;

	return send(command,n);
}
//Roomba2 の数値データ送信関数
bool CSerial::CommSend2(const char* command , int n) {

	if( !is_connect_ ) 
		return false;

	return send2(command,n);
}

//Roomba3 の数値データ送信関数
bool CSerial::CommSend3(const char* command , int n) {

	if( !is_connect_ ) 
		return false;

	return send3(command,n);
}
//Roomba4 の数値データ送信関数
bool CSerial::CommSend4(const char* command , int n) {

	if( !is_connect_ ) 
		return false;

	return send4(command,n);
}


//Roomba1 のコマンド送信関数
bool CSerial::CommMode(const std::string& command)
{
	if( !is_connect_ ) 
	{
		TRACE("is_connect_ = %d",is_connect_);
		TRACE("接続されていません\n");
		return false;
	}

	return send(command.c_str(),command.size());
}

//Roomba2 のコマンド送信関数
bool CSerial::CommMode2(const std::string& command)
{
	if( !is_connect_ ) 
	{
		TRACE("is_connect_ = %d",is_connect_);
		TRACE("接続されていません\n");
		return false;
	}

	return send2(command.c_str(),command.size());
}

//Roomba3 のコマンド送信関数
bool CSerial::CommMode3(const std::string& command)
{
	if( !is_connect_ ) 
	{
		TRACE("is_connect_ = %d",is_connect_);
		TRACE("接続されていません\n");
		return false;
	}

	return send3(command.c_str(),command.size());
}

//Roomba4 のコマンド送信関数
bool CSerial::CommMode4(const std::string& command)
{
	if( !is_connect_ ) 
	{
		TRACE("is_connect_ = %d",is_connect_);
		TRACE("接続されていません\n");
		return false;
	}

	return send4(command.c_str(),command.size());
}

 bool CSerial::send(const char* str, int n)
 {
	
	if(hCom != INVALID_HANDLE_VALUE)
	{
		CString strText;
		strText = str;
		DWORD dwBytes = 0;
        WriteFile(hCom, (LPCTSTR)str, (DWORD)n, &dwBytes, NULL);
		DWORD hr = GetLastError( );
	}

	//Sleep(3000);

	//if(hCom != INVALID_HANDLE_VALUE)
	//{
	//	TRACE("受信し\n");
	//	DWORD dwError;
	//	COMSTAT comStat;
	//	ClearCommError(hCom, &dwError, &comStat);
	//	DWORD dwCount;
	//	dwCount = comStat.cbInQue;
	//	TRACE("dwCount = %d\n",dwCount);
	//	if(ClearCommError(hCom, &dwError, &comStat))
	//	{
	//		TRACE("受信して\n");
	//		if(comStat.cbInQue != 0)
	//		{
	//			TRACE("受信しています\n");
	//			char* buff = new char [comStat.cbInQue];
	//			DWORD NumberOfBytesRead = 0;
	//			if(ReadFile(hCom, buff, comStat.cbInQue, &NumberOfBytesRead, NULL))
	//			{
	//				CString strText(buff, NumberOfBytesRead);
	//				TRACE("Received: %x\n", strText);
	//			}
	//			delete [] buff;
	//		}
	//	}
	//}

	return true;
 }
 
 bool CSerial::send2(const char* str, int n)
 {
	
	if(hCom2 != INVALID_HANDLE_VALUE)
	{
		CString strText;
		strText = str;
		DWORD dwBytes = 0;
        WriteFile(hCom2, (LPCTSTR)str, (DWORD)n, &dwBytes, NULL);
		DWORD hr = GetLastError( );
	}


	//if(hCom2 != INVALID_HANDLE_VALUE)
	//{
	//	TRACE("受信し\n");
	//	DWORD dwError;
	//	COMSTAT comStat;
	//	ClearCommError(hCom2, &dwError, &comStat);
	//	DWORD dwCount;
	//	dwCount = comStat.cbInQue;
	//	TRACE("dwCount = %d\n",dwCount);
	//	if(ClearCommError(hCom2, &dwError, &comStat))
	//	{
	//		TRACE("受信して\n");
	//		if(comStat.cbInQue != 0)
	//		{
	//			TRACE("受信しています\n");
	//			char* buff = new char [comStat.cbInQue];
	//			DWORD NumberOfBytesRead = 0;
	//			if(ReadFile(hCom2, buff, comStat.cbInQue, &NumberOfBytesRead, NULL))
	//			{
	//				CString strText(buff, NumberOfBytesRead);
	//				TRACE("Received: %x\n", strText);
	//			}
	//			delete [] buff;
	//		}
	//	}
	//}

	return true;
 }

 bool CSerial::send3(const char* str, int n)
 {
	
	if(hCom3 != INVALID_HANDLE_VALUE)
	{
		CString strText;
		strText = str;
		DWORD dwBytes = 0;
        WriteFile(hCom3, (LPCTSTR)str, (DWORD)n, &dwBytes, NULL);
		DWORD hr = GetLastError( );
	}


	//if(hCom2 != INVALID_HANDLE_VALUE)
	//{
	//	TRACE("受信し\n");
	//	DWORD dwError;
	//	COMSTAT comStat;
	//	ClearCommError(hCom2, &dwError, &comStat);
	//	DWORD dwCount;
	//	dwCount = comStat.cbInQue;
	//	TRACE("dwCount = %d\n",dwCount);
	//	if(ClearCommError(hCom2, &dwError, &comStat))
	//	{
	//		TRACE("受信して\n");
	//		if(comStat.cbInQue != 0)
	//		{
	//			TRACE("受信しています\n");
	//			char* buff = new char [comStat.cbInQue];
	//			DWORD NumberOfBytesRead = 0;
	//			if(ReadFile(hCom2, buff, comStat.cbInQue, &NumberOfBytesRead, NULL))
	//			{
	//				CString strText(buff, NumberOfBytesRead);
	//				TRACE("Received: %x\n", strText);
	//			}
	//			delete [] buff;
	//		}
	//	}
	//}

	return true;
 }
 
 bool CSerial::send4(const char* str, int n)
 {
	
	if(hCom4 != INVALID_HANDLE_VALUE)
	{
		CString strText;
		strText = str;
		DWORD dwBytes = 0;
        WriteFile(hCom4, (LPCTSTR)str, (DWORD)n, &dwBytes, NULL);
		DWORD hr = GetLastError( );
	}


	//if(hCom2 != INVALID_HANDLE_VALUE)
	//{
	//	TRACE("受信し\n");
	//	DWORD dwError;
	//	COMSTAT comStat;
	//	ClearCommError(hCom2, &dwError, &comStat);
	//	DWORD dwCount;
	//	dwCount = comStat.cbInQue;
	//	TRACE("dwCount = %d\n",dwCount);
	//	if(ClearCommError(hCom2, &dwError, &comStat))
	//	{
	//		TRACE("受信して\n");
	//		if(comStat.cbInQue != 0)
	//		{
	//			TRACE("受信しています\n");
	//			char* buff = new char [comStat.cbInQue];
	//			DWORD NumberOfBytesRead = 0;
	//			if(ReadFile(hCom2, buff, comStat.cbInQue, &NumberOfBytesRead, NULL))
	//			{
	//				CString strText(buff, NumberOfBytesRead);
	//				TRACE("Received: %x\n", strText);
	//			}
	//			delete [] buff;
	//		}
	//	}
	//}

	return true;
 }
 //データ取得の関数．現在Roomba1のみ対応
 bool CSerial::CommReceive(int x)
 {
	//hRec = CreateFile(_T("COM7"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    int baudRate = 57600;
    unsigned long nn;
    COMMTIMEOUTS cto;

    /* ----------------------------------------------
        ファイルのクリエイト／オープン
    ---------------------------------------------- */
    // クリエイトしたファイルのファイルハンドラを返す
    //hRec = CreateFile(_T("COM8"), 
    //                        GENERIC_READ | GENERIC_WRITE,
    //                        0,
    //                        0,
    //                        OPEN_EXISTING,
    //                        0,
    //                        0 ); 
    //if ( hRec == INVALID_HANDLE_VALUE ) {
    //    printf("Open Error!\n");
    //    exit(1);
    //}

    /* ----------------------------------------------
        シリアルポートの状態操作
    ---------------------------------------------- */
    //GetCommState( hRec, &dcb ); // シリアルポートの状態を取得
    //dcb.BaudRate = baudRate;
    //SetCommState( hRec, &dcb ); // シリアルポートの状態を設定

    /* ----------------------------------------------
        シリアルポートのタイムアウト状態操作
    ---------------------------------------------- */
    GetCommTimeouts( hCom, &cto ); // タイムアウトの設定状態を取得
    cto.ReadIntervalTimeout = 1000;
    cto.ReadTotalTimeoutMultiplier = 0;
    cto.ReadTotalTimeoutConstant = 1000;
    cto.WriteTotalTimeoutMultiplier = 0;
    cto.WriteTotalTimeoutConstant = 0;
    SetCommTimeouts( hCom, &cto ); // タイムアウトの状態を設定

    /* ----------------------------------------------
        受信データの読み込み（１行分の文字列）　浅利変更
    ---------------------------------------------- */

    ReadFile( hCom, s_data1, x/*取得するバイト数，接触なら10*/, &nn, 0 ); // シリアルポートに対する読み込み
	//TRACE("ReadFileは実行されています\n");
	//TRACE("s_data = %x\n",s_data[0]);
        
//    }
	return true;
 }
 
 //データ取得の関数
 bool CSerial::CommReceive2(int x)
 {
	//hRec = CreateFile(_T("COM4"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    int baudRate = 57600;
    unsigned long nn;
    COMMTIMEOUTS cto2;

    /* ----------------------------------------------
        ファイルのクリエイト／オープン
    ---------------------------------------------- */
    // クリエイトしたファイルのファイルハンドラを返す
    //hRec = CreateFile(_T("COM8"), 
    //                        GENERIC_READ | GENERIC_WRITE,
    //                        0,
    //                        0,
    //                        OPEN_EXISTING,
    //                        0,
    //                        0 ); 
    //if ( hRec == INVALID_HANDLE_VALUE ) {
    //    printf("Open Error!\n");
    //    exit(1);
    //}

    /* ----------------------------------------------
        シリアルポートの状態操作
    ---------------------------------------------- */
    //GetCommState( hRec, &dcb ); // シリアルポートの状態を取得
    //dcb.BaudRate = baudRate;
    //SetCommState( hRec, &dcb ); // シリアルポートの状態を設定

    /* ----------------------------------------------
        シリアルポートのタイムアウト状態操作
    ---------------------------------------------- */
    GetCommTimeouts( hCom2, &cto2 ); // タイムアウトの設定状態を取得
    cto2.ReadIntervalTimeout = 1000;
    cto2.ReadTotalTimeoutMultiplier = 0;
    cto2.ReadTotalTimeoutConstant = 1000;
    cto2.WriteTotalTimeoutMultiplier = 0;
    cto2.WriteTotalTimeoutConstant = 0;
    SetCommTimeouts( hCom2, &cto2 ); // タイムアウトの状態を設定

    /* ----------------------------------------------
        受信データの読み込み（１行分の文字列）　浅利変更
    ---------------------------------------------- */

    ReadFile( hCom2, s_data2, x/*取得するバイト数，接触なら10*/, &nn, 0 ); // シリアルポートに対する読み込み
	//TRACE("ReadFileは実行されています\n");
	//TRACE("s_data = %x\n",s_data[0]);
        
//    }
	return true;
 }
int CSerial::TentoTwo(int x)
{
	//変数宣言
    int nisin[16];
//    int jyu;


    //変換
    for(int i=0; i<16; i++){
        nisin[i] = x % 2;
        x = x / 2;
    }

    //出力
    for(int i=16-1; i>=0; i--){
        TRACE("%d ",nisin[i]);
    }
	TRACE("\n");
	TRACE("end\n");

	return x;
}

bool CSerial::RoombaClush()
{
	if(s_data1[0]==1 || s_data1[0]==2 || s_data1[0]==3
		)
		return true;
	return false;
}