#pragma once
//��7��ʵ���ػ�
class MapObj
{
public:
	//��7��ʵ���ػ�
	CPoint p1, p2;//ͼ�εĿ��Ƶ�
	int type;//ͼ�ε�����
	//��7��

	//��8��������ɫ
	COLORREF color;
	//��8��
public:
	MapObj();
	virtual ~MapObj();
};

