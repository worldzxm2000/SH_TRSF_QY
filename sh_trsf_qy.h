#ifndef SH_TRSF_QY_H
#define SH_TRSF_QY_H

#define SH_TRSF_QY_EXPORT __declspec(dllexport)
#ifdef SH_TRSF_QY_EXPORT
#else
#define SH_TRSF_QY_EXPORT __declspec(dllimport)
#endif

#include<windows.h>
#include<qjsonobject.h>
#include "setparamdlg.h"

#define HEADER 0xAA
#define TAIL 0xDD
#define FE 0xFE
#define SH_TRSF_QY "8"
typedef struct DataFrame {
	//����(2�ֽ�)
	int len;
	//Դ��ַ(2�ֽ�)
	int SrcAddr;
	//Ŀ�ĵ�ַ(2�ֽ�)
	int DesAddr;
	//֡����
	int Command;
	//������
	QString data;

} Frame;


//����ṹ������
typedef union
{
	struct
	{
		byte low_byte;
		byte mlow_byte;
		byte mhigh_byte;
		byte high_byte;
	}float_byte;
	float  value;
}FLOAT_UNION;


//�ɼ�ʱ�ӷ�������
void Transform2Time(QString data, QJsonObject &json);
//��������
void Transform2ErrorData(QString data, QJsonObject &json);
//GPRSСʱƽ�������ˮ����Ƶ������
void Transform2GPRSPerVolume(QString data, QJsonObject &json);
//������
void Transform2Heartbeat(QString data, QJsonObject &json);
//ͨ�Ž���
void Transform2CloeseConnection(QString data, QJsonObject &json);
//ʱ���ʽ
QString Convert2Time(QString strTime);
//��������
EXTERN_C SH_TRSF_QY_EXPORT LRESULT Char2Json(QString &buff, QJsonObject &json);
//����ʹ����¼���Ӧ
EXTERN_C SH_TRSF_QY_EXPORT void  GetControlWidget(QString StationID, uint Socket, QWidget* parent = NULL);
//����ʱ��
EXTERN_C SH_TRSF_QY_EXPORT void SetTime(QString StationID, uint Socket);
//��ȡҵ���
EXTERN_C SH_TRSF_QY_EXPORT int GetServiceTypeID();
//��ȡҵ������
EXTERN_C SH_TRSF_QY_EXPORT QString GetServiceTypeName();
//��ȡ�汾��
EXTERN_C SH_TRSF_QY_EXPORT QString GetVersionNo();
//��ȡ�˿ں�
EXTERN_C SH_TRSF_QY_EXPORT int GetPort();
//��ʾ����ֵ
EXTERN_C SH_TRSF_QY_EXPORT void  SetValueToControlWidget(QStringList list);
//��������
EXTERN_C SH_TRSF_QY_EXPORT void SetCommand(uint Socket, int CommandType, QString Params1, QString Params2,QString StationID);//<>
SetParamDlg *w;//�ն˴���
bool isActive;//�ж��ն˴����Ƿ��� //<>
#endif // SH_TRSF_QY_H
