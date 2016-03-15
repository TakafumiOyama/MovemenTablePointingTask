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
	//シリアルポートのオプションを設定するためのEnumクラス群
	
	//ポートレート設定用
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

	// パリティ設定用
	class Parity {
	public:
		enum {
			None=0,
			Odd,
			Even
		};
	};

	// ストップビット設定用
	class StopBits {
	public:
		enum {
			One=0,
			OnePointFive,
			Two
		};
	};

	// データのバイトサイズ設定用
	class ByteSize{
	public:
		enum{
			B7 = 7,
			B8 = 8
		};
	};

	// フローコントロール用の設定
	class FlowControl{
	public:
		enum{
			None=0,
			Hardware,
			Software
		};
	};

	class CSerial;
	
	//obserberを使ってコールバックを実装するためのインターフェースクラス
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
		//コンストラクタ、デストラクタ---------------
		CSerial(void);
		virtual ~CSerial(void);

		//操作------------------------------------------
	public:
		bool open(
			const std::string& com = "COM1",
			int baudrate = BaudRate::B115200,
			int bytesize = ByteSize::B8,
			int parity = Parity::None,
			int stopbits = StopBits::One,
			int flowcontrol = FlowControl::None
		);
	

		//ポートのクローズ, return成功判定
//		bool close();

		//observerを追加・削除する関数 return:成功判定
//		bool attach(CSerialObserver* ob);
//		bool detach(CSerialObserver* ob);

		//データを受信する関数
//		bool receive(std::string&str, double timeout);
//		int receive(char* data, double timeout);

		//接続確認 return:接続状況
//		bool isConnect() const {return is_connect_;}

	
	private:
		//更新関数
//		virtual void notifyAll(const std::string& str);



	//属性------------------------------------------------------
	private:

		class serial_impl;
//		boost::shared_ptr<serial_impl> impl;

		//受信用バッファ
		char rBuffer[1024];
		int readSize;

//		void read_ok(const boost::system::error_code& e, size_t size);
		//ポートが接続されているかどうかを判断する変数
		bool is_connect_;

		//最新版の受信データ
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
		//文字列を送信する関数
		//bool send(const std::string& s);
		//bool send(char c);
		//bool send(const char *c, int size);
		//データ書き込み
		//bool write(const char* str, int n);
		//文字列を送信する関数
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

		//変数
		unsigned char s_data1[10];	//センサーデータを格納
		unsigned char s_data2[10];	//センサーデータを格納

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
		//オープン時のエラー確認
		bool m_comErrFlag;
	};
}
