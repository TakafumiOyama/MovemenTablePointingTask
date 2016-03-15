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

//�������֐�
int CSerial::Init(void)
{
	this->CommOpen();
	return 0;

}

//�V���A���ʐM�̃f�o�C�X���J���C�v���g�R���̐ݒ�
bool CSerial::CommOpen(void)
{
	//�Y������Roomba�̃|�[�g�ԍ���COM~�ɓ���
	hCom = CreateFileA("\\\\.\\COM3", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	DWORD er = GetLastError();

	if(hCom == INVALID_HANDLE_VALUE){
		m_comErrFlag = true;
		TRACE("Roomba1 �ڑ����s\n");
		return false;
	}

	DCB dcb;
	memset(&dcb, 0, sizeof(DCB));
    dcb.DCBlength = sizeof(DCB) ;
 //   dcb.BaudRate = CBR_115200;		//�L��
    dcb.BaudRate = CBR_57600;		//����
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    SetCommState(hCom, &dcb);
	TRACE("�ڑ�����\n");
	is_connect_ = true;
	TRACE("is_connect_ = %d\n",is_connect_);
	return true;
}


//Roomba2�̃|�[�g���I�[�v��
bool CSerial::CommOpen2(void)
{
	//�Y������Roomba�̃|�[�g�ԍ���COM~�ɓ���
	hCom2 = CreateFileA("\\\\.\\COM9", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	DWORD er = GetLastError();

	if(hCom2 == INVALID_HANDLE_VALUE){
		m_comErrFlag = true;
		TRACE("Roomba2 �ڑ����s\n");
		return false;
	}

	DCB dcb;
	memset(&dcb, 0, sizeof(DCB));
    dcb.DCBlength = sizeof(DCB) ;
    //dcb.BaudRate = CBR_115200;		//�L��
    dcb.BaudRate = CBR_57600;		//����
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
	SetCommState(hCom2, &dcb);
	TRACE("�ڑ�����\n");
	is_connect_ = true;
	TRACE("is_connect_ = %d\n",is_connect_);
	return true;
}

//Roomba3�̃|�[�g���I�[�v��
bool CSerial::CommOpen3(void)
{
	//�Y������Roomba�̃|�[�g�ԍ���COM~�ɓ���
	hCom3 = CreateFileA("\\\\.\\COM13", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);//_T("COM5")   //"\\\\.\\COM10"
	DWORD er = GetLastError();

	if(hCom3 == INVALID_HANDLE_VALUE){
		m_comErrFlag = true;
		TRACE("Roomba3 �ڑ����s\n");
		return false;
	}

	DCB dcb;
	memset(&dcb, 0, sizeof(DCB));
    dcb.DCBlength = sizeof(DCB) ;
 //   dcb.BaudRate = CBR_115200;		//�L��
    dcb.BaudRate = CBR_57600;		//����
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
	SetCommState(hCom3, &dcb);
	TRACE("�ڑ�����\n");
	is_connect_ = true;
	TRACE("is_connect_ = %d\n",is_connect_);
	return true;
}

//Roomba4�̃|�[�g���I�[�v��
bool CSerial::CommOpen4(void)
{
	//�Y������Roomba�̃|�[�g�ԍ���COM~�ɓ���
	hCom4 = CreateFileA("\\\\.\\COM19", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	DWORD er = GetLastError();

	if(hCom4 == INVALID_HANDLE_VALUE){
		m_comErrFlag = true;
		TRACE("Roomba4 �ڑ����s\n");
		return false;
	}

	DCB dcb;
	memset(&dcb, 0, sizeof(DCB));
    dcb.DCBlength = sizeof(DCB) ;
 //   dcb.BaudRate = CBR_115200;		//�L��
    dcb.BaudRate = CBR_57600;		//����
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
	SetCommState(hCom4, &dcb);
	TRACE("�ڑ�����\n");
	is_connect_ = true;
	TRACE("is_connect_ = %d\n",is_connect_);
	return true;
}

//�V���A���ʐM�f�o�C�X�̊J��
bool CSerial::CommClose(void)
{
	if( !is_connect_ ) 
	{
		TRACE("�ڑ��������ł��܂���\n");
		return false;
		
	}
	if(hCom != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hCom);
		hCom = INVALID_HANDLE_VALUE;
	}

	return true;
}

//�V���A���ʐM�f�o�C�X�̊J��
bool CSerial::CommClose2(void)
{
	if( !is_connect_ ) 
	{
		TRACE("�ڑ��������ł��܂���\n");
		return false;
		
	}
	if(hCom2 != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hCom2);
		hCom2 = INVALID_HANDLE_VALUE;
	}
	return true;
}

//�V���A���ʐM�f�o�C�X�̊J��
bool CSerial::CommClose3(void)
{
	if( !is_connect_ ) 
	{
		TRACE("�ڑ��������ł��܂���\n");
		return false;
		
	}
	if(hCom3 != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hCom3);
		hCom3 = INVALID_HANDLE_VALUE;
	}

	return true;
}

//�V���A���ʐM�f�o�C�X�̊J��
bool CSerial::CommClose4(void)
{
	if( !is_connect_ ) 
	{
		TRACE("�ڑ��������ł��܂���\n");
		return false;
		
	}
	if(hCom4 != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hCom4);
		hCom4 = INVALID_HANDLE_VALUE;
	}
	return true;
}



//Roomba1 �̐��l�f�[�^���M�֐�
bool CSerial::CommSend(const char* command , int n) {

	if( !is_connect_ ) 
		return false;

	return send(command,n);
}
//Roomba2 �̐��l�f�[�^���M�֐�
bool CSerial::CommSend2(const char* command , int n) {

	if( !is_connect_ ) 
		return false;

	return send2(command,n);
}

//Roomba3 �̐��l�f�[�^���M�֐�
bool CSerial::CommSend3(const char* command , int n) {

	if( !is_connect_ ) 
		return false;

	return send3(command,n);
}
//Roomba4 �̐��l�f�[�^���M�֐�
bool CSerial::CommSend4(const char* command , int n) {

	if( !is_connect_ ) 
		return false;

	return send4(command,n);
}


//Roomba1 �̃R�}���h���M�֐�
bool CSerial::CommMode(const std::string& command)
{
	if( !is_connect_ ) 
	{
		TRACE("is_connect_ = %d",is_connect_);
		TRACE("�ڑ�����Ă��܂���\n");
		return false;
	}

	return send(command.c_str(),command.size());
}

//Roomba2 �̃R�}���h���M�֐�
bool CSerial::CommMode2(const std::string& command)
{
	if( !is_connect_ ) 
	{
		TRACE("is_connect_ = %d",is_connect_);
		TRACE("�ڑ�����Ă��܂���\n");
		return false;
	}

	return send2(command.c_str(),command.size());
}

//Roomba3 �̃R�}���h���M�֐�
bool CSerial::CommMode3(const std::string& command)
{
	if( !is_connect_ ) 
	{
		TRACE("is_connect_ = %d",is_connect_);
		TRACE("�ڑ�����Ă��܂���\n");
		return false;
	}

	return send3(command.c_str(),command.size());
}

//Roomba4 �̃R�}���h���M�֐�
bool CSerial::CommMode4(const std::string& command)
{
	if( !is_connect_ ) 
	{
		TRACE("is_connect_ = %d",is_connect_);
		TRACE("�ڑ�����Ă��܂���\n");
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
	//	TRACE("��M��\n");
	//	DWORD dwError;
	//	COMSTAT comStat;
	//	ClearCommError(hCom, &dwError, &comStat);
	//	DWORD dwCount;
	//	dwCount = comStat.cbInQue;
	//	TRACE("dwCount = %d\n",dwCount);
	//	if(ClearCommError(hCom, &dwError, &comStat))
	//	{
	//		TRACE("��M����\n");
	//		if(comStat.cbInQue != 0)
	//		{
	//			TRACE("��M���Ă��܂�\n");
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
	//	TRACE("��M��\n");
	//	DWORD dwError;
	//	COMSTAT comStat;
	//	ClearCommError(hCom2, &dwError, &comStat);
	//	DWORD dwCount;
	//	dwCount = comStat.cbInQue;
	//	TRACE("dwCount = %d\n",dwCount);
	//	if(ClearCommError(hCom2, &dwError, &comStat))
	//	{
	//		TRACE("��M����\n");
	//		if(comStat.cbInQue != 0)
	//		{
	//			TRACE("��M���Ă��܂�\n");
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
	//	TRACE("��M��\n");
	//	DWORD dwError;
	//	COMSTAT comStat;
	//	ClearCommError(hCom2, &dwError, &comStat);
	//	DWORD dwCount;
	//	dwCount = comStat.cbInQue;
	//	TRACE("dwCount = %d\n",dwCount);
	//	if(ClearCommError(hCom2, &dwError, &comStat))
	//	{
	//		TRACE("��M����\n");
	//		if(comStat.cbInQue != 0)
	//		{
	//			TRACE("��M���Ă��܂�\n");
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
	//	TRACE("��M��\n");
	//	DWORD dwError;
	//	COMSTAT comStat;
	//	ClearCommError(hCom2, &dwError, &comStat);
	//	DWORD dwCount;
	//	dwCount = comStat.cbInQue;
	//	TRACE("dwCount = %d\n",dwCount);
	//	if(ClearCommError(hCom2, &dwError, &comStat))
	//	{
	//		TRACE("��M����\n");
	//		if(comStat.cbInQue != 0)
	//		{
	//			TRACE("��M���Ă��܂�\n");
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
 //�f�[�^�擾�̊֐��D����Roomba1�̂ݑΉ�
 bool CSerial::CommReceive(int x)
 {
	//hRec = CreateFile(_T("COM7"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    int baudRate = 57600;
    unsigned long nn;
    COMMTIMEOUTS cto;

    /* ----------------------------------------------
        �t�@�C���̃N���G�C�g�^�I�[�v��
    ---------------------------------------------- */
    // �N���G�C�g�����t�@�C���̃t�@�C���n���h����Ԃ�
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
        �V���A���|�[�g�̏�ԑ���
    ---------------------------------------------- */
    //GetCommState( hRec, &dcb ); // �V���A���|�[�g�̏�Ԃ��擾
    //dcb.BaudRate = baudRate;
    //SetCommState( hRec, &dcb ); // �V���A���|�[�g�̏�Ԃ�ݒ�

    /* ----------------------------------------------
        �V���A���|�[�g�̃^�C���A�E�g��ԑ���
    ---------------------------------------------- */
    GetCommTimeouts( hCom, &cto ); // �^�C���A�E�g�̐ݒ��Ԃ��擾
    cto.ReadIntervalTimeout = 1000;
    cto.ReadTotalTimeoutMultiplier = 0;
    cto.ReadTotalTimeoutConstant = 1000;
    cto.WriteTotalTimeoutMultiplier = 0;
    cto.WriteTotalTimeoutConstant = 0;
    SetCommTimeouts( hCom, &cto ); // �^�C���A�E�g�̏�Ԃ�ݒ�

    /* ----------------------------------------------
        ��M�f�[�^�̓ǂݍ��݁i�P�s���̕�����j�@�󗘕ύX
    ---------------------------------------------- */

    ReadFile( hCom, s_data1, x/*�擾����o�C�g���C�ڐG�Ȃ�10*/, &nn, 0 ); // �V���A���|�[�g�ɑ΂���ǂݍ���
	//TRACE("ReadFile�͎��s����Ă��܂�\n");
	//TRACE("s_data = %x\n",s_data[0]);
        
//    }
	return true;
 }
 
 //�f�[�^�擾�̊֐�
 bool CSerial::CommReceive2(int x)
 {
	//hRec = CreateFile(_T("COM4"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    int baudRate = 57600;
    unsigned long nn;
    COMMTIMEOUTS cto2;

    /* ----------------------------------------------
        �t�@�C���̃N���G�C�g�^�I�[�v��
    ---------------------------------------------- */
    // �N���G�C�g�����t�@�C���̃t�@�C���n���h����Ԃ�
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
        �V���A���|�[�g�̏�ԑ���
    ---------------------------------------------- */
    //GetCommState( hRec, &dcb ); // �V���A���|�[�g�̏�Ԃ��擾
    //dcb.BaudRate = baudRate;
    //SetCommState( hRec, &dcb ); // �V���A���|�[�g�̏�Ԃ�ݒ�

    /* ----------------------------------------------
        �V���A���|�[�g�̃^�C���A�E�g��ԑ���
    ---------------------------------------------- */
    GetCommTimeouts( hCom2, &cto2 ); // �^�C���A�E�g�̐ݒ��Ԃ��擾
    cto2.ReadIntervalTimeout = 1000;
    cto2.ReadTotalTimeoutMultiplier = 0;
    cto2.ReadTotalTimeoutConstant = 1000;
    cto2.WriteTotalTimeoutMultiplier = 0;
    cto2.WriteTotalTimeoutConstant = 0;
    SetCommTimeouts( hCom2, &cto2 ); // �^�C���A�E�g�̏�Ԃ�ݒ�

    /* ----------------------------------------------
        ��M�f�[�^�̓ǂݍ��݁i�P�s���̕�����j�@�󗘕ύX
    ---------------------------------------------- */

    ReadFile( hCom2, s_data2, x/*�擾����o�C�g���C�ڐG�Ȃ�10*/, &nn, 0 ); // �V���A���|�[�g�ɑ΂���ǂݍ���
	//TRACE("ReadFile�͎��s����Ă��܂�\n");
	//TRACE("s_data = %x\n",s_data[0]);
        
//    }
	return true;
 }
int CSerial::TentoTwo(int x)
{
	//�ϐ��錾
    int nisin[16];
//    int jyu;


    //�ϊ�
    for(int i=0; i<16; i++){
        nisin[i] = x % 2;
        x = x / 2;
    }

    //�o��
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