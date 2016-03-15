#pragma once

#include <vector>
#include <afxwin.h>
#include <string>
#include <queue>
#include <stdio.h>
//#include "boost/smart_ptr.hpp"

namespace boost{namespace system{class error_code;};}

namespace serial
{
	//�V���A���|�[�g�̃I�v�V������ݒ肷�邽�߂�Enum�N���X�Q
	
	//�|�[�g���[�g�ݒ�p
	class BaudRate{
	public:
		enum{
			B2400 = 2400,
			B4800 = 4800,
			B9600 = 9600,
			B19200 = 19200,
			B38400 = 38400,
			B115200 = 115200
		};
	};

	// �p���e�B�ݒ�p
	class Parity {
	public:
		enum {
			None=0,
			Odd,
			Even
		};
	};

	// �X�g�b�v�r�b�g�ݒ�p
	class StopBits {
	public:
		enum {
			One=0,
			OnePointFive,
			Two
		};
	};

	// �f�[�^�̃o�C�g�T�C�Y�ݒ�p
	class ByteSize{
	public:
		enum{
			B7 = 7,
			B8 = 8
		};
	};

	// �t���[�R���g���[���p�̐ݒ�
	class FlowControl{
	public:
		enum{
			None=0,
			Hardware,
			Software
		};
	};

	class CSerial;
	
	//obserber���g���ăR�[���o�b�N���������邽�߂̃C���^�[�t�F�[�X�N���X
	class CSerialObserver
	{
		friend CSerial;
	public:
		CSerialObserver(void){ id = (unsigned long) id; }
		virtual ~CSerialObserver(void){}

		unsigned long getId() const { return id; }

	protected:
		virtual void notify( const std::string& str )=0;
	
	private:
		unsigned long id;
	};

	class CSerial
	{
	public:
		//�R���X�g���N�^�A�f�X�g���N�^---------------
		CSerial(void);
		virtual ~CSerial(void);

		//����------------------------------------------
	public:
		bool open(
			const std::string& com = "COM1",
			int baudrate = BaudRate::B115200,
			int bytesize = ByteSize::B8,
			int parity = Parity::None,
			int stopbits = StopBits::One,
			int flowcontrol = FlowControl::None
		);
	

		//�|�[�g�̃N���[�Y, return��������
//		bool close();

		//observer��ǉ��E�폜����֐� return:��������
//		bool attach(CSerialObserver* ob);
//		bool detach(CSerialObserver* ob);

		//�f�[�^����M����֐�
//		bool receive(std::string&str, double timeout);
//		int receive(char* data, double timeout);

		//�ڑ��m�F return:�ڑ���
//		bool isConnect() const {return is_connect_;}

	
	private:
		//�X�V�֐�
//		virtual void notifyAll(const std::string& str);



	//����------------------------------------------------------
	private:

		class serial_impl;
//		boost::shared_ptr<serial_impl> impl;

		//��M�p�o�b�t�@
		char rBuffer[1024];
		int readSize;

//		void read_ok(const boost::system::error_code& e, size_t size);
		//�|�[�g���ڑ�����Ă��邩�ǂ����𔻒f����ϐ�
		bool is_connect_;

		//�ŐV�ł̎�M�f�[�^
//		std::string readData;


		//Asari
	public:
		int Init(void);
		bool CommOpen(void);
		bool CommOpen2(void);
		bool CommOpen3(void);
		bool CommOpen4(void);
		bool CommClose(void);
		bool CommClose2(void);
		bool CommClose3(void);
		bool CommClose4(void);
		//������𑗐M����֐�
		//bool send(const std::string& s);
		//bool send(char c);
		//bool send(const char *c, int size);
		//�f�[�^��������
		//bool write(const char* str, int n);
		//������𑗐M����֐�
		bool CommSend1(CString command);
		bool CommSend(const char* str, int size);
		bool CommMode(const std::string& command);
		bool CommSend2(const char* str, int size);
		bool CommMode2(const std::string& command);
		bool CommSend3(const char* str, int size);
		bool CommMode3(const std::string& command);
		bool CommSend4(const char* str, int size);
		bool CommMode4(const std::string& command);

		bool CommReceive(int);
		bool CommReceive2(int);
		bool RoombaClush();
		int TentoTwo(int);

		//�ϐ�
		unsigned char s_data1[10];	//�Z���T�[�f�[�^���i�[
		unsigned char s_data2[10];	//�Z���T�[�f�[�^���i�[

	private:
		bool send(const char* str, int n);
		bool send2(const char* str, int n);
		bool send3(const char* str, int n);
		bool send4(const char* str, int n);

	private:
		HANDLE hCom;
		HANDLE hCom2;
		HANDLE hCom3;
		HANDLE hCom4;
		HANDLE hRec;
		//�I�[�v�����̃G���[�m�F
		bool m_comErrFlag;
	};
}
