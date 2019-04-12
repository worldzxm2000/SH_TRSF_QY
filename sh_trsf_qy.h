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
	//长度(2字节)
	int len;
	//源地址(2字节)
	int SrcAddr;
	//目的地址(2字节)
	int DesAddr;
	//帧命令
	int Command;
	//数据域
	QString data;

} Frame;


//定义结构和联合
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


//采集时钟发送命令
void Transform2Time(QString data, QJsonObject &json);
//错误数据
void Transform2ErrorData(QString data, QJsonObject &json);
//GPRS小时平均体积含水量和频率数据
void Transform2GPRSPerVolume(QString data, QJsonObject &json);
//心跳包
void Transform2Heartbeat(QString data, QJsonObject &json);
//通信结束
void Transform2CloeseConnection(QString data, QJsonObject &json);
//时间格式
QString Convert2Time(QString strTime);
//解析数据
EXTERN_C SH_TRSF_QY_EXPORT LRESULT Char2Json(QString &buff, QJsonObject &json);
//命令发送窗口事件响应
EXTERN_C SH_TRSF_QY_EXPORT void  GetControlWidget(QString StationID, uint Socket, QWidget* parent = NULL);
//矫正时钟
EXTERN_C SH_TRSF_QY_EXPORT void SetTime(QString StationID, uint Socket);
//获取业务号
EXTERN_C SH_TRSF_QY_EXPORT int GetServiceTypeID();
//获取业务名称
EXTERN_C SH_TRSF_QY_EXPORT QString GetServiceTypeName();
//获取版本号
EXTERN_C SH_TRSF_QY_EXPORT QString GetVersionNo();
//获取端口号
EXTERN_C SH_TRSF_QY_EXPORT int GetPort();
//显示返回值
EXTERN_C SH_TRSF_QY_EXPORT void  SetValueToControlWidget(QStringList list);
//发送命令
EXTERN_C SH_TRSF_QY_EXPORT void SetCommand(uint Socket, int CommandType, QString Params1, QString Params2,QString StationID);//<>
SetParamDlg *w;//终端窗体
bool isActive;//判断终端窗体是否开启 //<>
#endif // SH_TRSF_QY_H
