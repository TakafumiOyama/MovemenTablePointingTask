#pragma once
class CTrackable
{
public:
	CTrackable(void);
	~CTrackable(void);


	int m_id;
	// カメラパラメータ
	float m_pos_x;
	float m_pos_y;
	float m_pos_z;
	float m_dir_x;
	float m_dir_y;
	float m_dir_z;
	float m_dir_w;
	float m_yaw;
	float m_pitch;
	float m_roll;
	float m_offset_x;
	float m_offset_y;
};

