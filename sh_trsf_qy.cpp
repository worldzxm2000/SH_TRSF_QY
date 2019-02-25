#include "sh_trsf_qy.h"
#include<qstring.h>
#include"qdatetime.h"
#include"qfile.h"
#include"qtextstream.h"
#include"qjsondocument.h"
#include "qdebug.h"
#include "qcoreapplication.h"
#include"qdir.h"
using namespace  std;

//获取业务号
int GetServiceTypeID()
{
	return 13;
}

//获取业务名称
QString GetServiceTypeName()
{
	QString name = QString::fromLocal8Bit("湿地土壤水分区域业务");
	return name;
}

//获取版本号
QString GetVersionNo()
{
	QString Version = QString::fromLocal8Bit("1.0");
	return Version;
}

//命令发送窗口事件响应
void  GetControlWidget(QString StationID, uint Socket, QWidget* parent) //<>
{
	if (isActive)
	{
		return;
	}
	w = new SetParamDlg(parent);
	w->StationID = StationID;
	isActive = true;
	w->Socket = Socket;
	w->isActive = &isActive;
	w->show();
}

//获取端口号
int GetPort()
{
	return 23003;
}

//矫正时钟
void SetTime(QString StationID, uint Socket)
{

	QDateTime nowtime = QDateTime::currentDateTime();
	QString datetime = nowtime.toString("yyyy-MM-dd hh:mm:ss");
	QString year, month, day, hour, min, sec;
	year = nowtime.toString("yy");
	month = nowtime.toString("MM");
	day = nowtime.toString("dd");
	hour = nowtime.toString("hh");
	min = nowtime.toString("mm");
	sec = nowtime.toString("ss");

	int chk = 0;
	int SrcAdrr = StationID.toInt();
	BYTE bytes[1024] = { 0 };
	bytes[0] = 0xaa;
	bytes[1] = 0x0d;//帧长度
	bytes[2] = 0x00;//帧长度
	bytes[3] = 0x84;//帧命令
	chk += bytes[3];
	bytes[4] = 0x01;//帧命令
	chk += bytes[4];
	bytes[5] = SrcAdrr & 0xff;//源地址
	chk += bytes[5];
	bytes[6] = (SrcAdrr >> 8) & 0xff;
	chk += bytes[6];
	bytes[7] = 0;//源地址
	chk += bytes[7];
	bytes[8] = 0;
	chk += bytes[8];
	bytes[9] = 0x14;//20
	chk += bytes[9];
	bytes[10] = year.toInt();
	chk += bytes[10];
	bytes[11] = month.toInt();
	chk += bytes[11];
	bytes[12] = day.toInt();
	chk += bytes[12];
	bytes[13] = hour.toInt();
	chk += bytes[13];
	bytes[14] = min.toInt();
	chk += bytes[14];
	bytes[15] = sec.toInt();
	chk += bytes[15];
	bytes[16] = chk & 0xff;//校验位 低八位
	bytes[17] = (chk >> 8) & 0xff;//高八位
	bytes[18] = 0xdd;
	::send(Socket, (char *)bytes, 19, 0);
}

//解析数据
LRESULT Char2Json(QString &buff, QJsonObject &json)
{
	int Count = 0;//数据个数
	int Current_P = buff.length();//当前数据指

								  //遍历查找数据
	for (int i = 0; i < buff.length(); i++)
	{
		//帧头  aa
		if (buff[i] == 0xaa)
		{
			Current_P = i;//指针指向帧头
			for (int j = i +1; j < buff.length(); j++)
			{
				if (buff[j] == 0xdd)
				{
					Current_P = j + 1;//指针移动到帧尾下一个字符
					
					QString strBuff = buff.mid(i, j - i +1);
					QJsonObject SubJson;
					SubJson.insert("ServiceTypeID", QString::number(SH_TRSF_QY));

					Frame frame = { 0,NULL,NULL };
					//获取帧长度
					frame.len = ((strBuff[1]).unicode() & 0xFF + strBuff[2].unicode() * 256) - 6;
					//判断帧长度 12为数据 0为心跳
					if (strBuff.length() - frame.len != 12|| strBuff.length() - frame.len==0)
						continue;
					//获取帧命令
					frame.Command = strBuff[3].unicode() + strBuff[4].unicode() * 256;
					//获取源地址
					frame.SrcAddr = strBuff[5].unicode() + strBuff[6].unicode() * 256;
					//获取目的地址
					frame.DesAddr = strBuff[7].unicode() + strBuff[8].unicode() * 256;
					//区站号
					SubJson.insert("StationID", "NULL");
					//设备号
					SubJson.insert("DeviceID", QString::number(frame.SrcAddr));
		
					frame.data = strBuff.mid(9, frame.len);
					//判断接收命令类型
					switch (frame.Command)
					{
						//读取采集时钟发送命令 0x0204
					case 516:
						Transform2Time(frame.data, SubJson);
						break;
						//GPRS平均体积含水量和频率数据 0x0404
					case 1028:
						Transform2GPRSPerVolume(frame.data, SubJson);
						break;
						//数据错误
					case 65028:
						Transform2ErrorData(frame.data, SubJson);
						break;
						//通讯结束 Oxff04
					case 65284:
						Transform2CloeseConnection(frame.data, SubJson);
						break;
						//心跳包数据 0x3A
					case 1082:
						Transform2Heartbeat(frame.data, SubJson);
						break;
					default:
						SubJson.insert("Command", frame.Command);
						break;
					}
					Count += 1;//数据个数
					//数据备份
					QDateTime current_date_time = QDateTime::currentDateTime();
					QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss");
					QString current_day = current_date_time.toString("yyyy-MM-dd");
					QString fileName = QCoreApplication::applicationDirPath() + "\\SH\\TRSF_QY\\" + QString::number(frame.SrcAddr) + "\\" + current_day;
					QDir dir(fileName);
					if (!dir.exists())
						dir.mkpath(fileName);//创建多级目录
					fileName += "\\data.txt";
					QFile file(fileName);

					if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
					{
					}
					QTextStream in(&file);

					QString ConvStr;
					for (int i = 0; i < strBuff.length(); i++)
					{
						ConvStr += (QString::number(strBuff[i].unicode(), 16)) + " ";
					}
					in << current_date << "\r\n" << ConvStr << "\r\n";
					file.close();
					json.insert(QString::number(Count), SubJson);
					i = j;//当前循环
					break;
				}
			}
		}
	}
	json.insert("DataLength", Count);//JSON数据个数
	if (Current_P >= buff.length())//判断当前指针位置
	{
		buff.clear();
	}//清除内存
	else
	{
		buff.remove(0, Current_P);
	}//将剩余字节存入缓存
	return 1;
}

QString Convert2Time(QString strTime)
{
	QString tmp;
	tmp = strTime.mid(0, 4) + "-" + strTime.mid(4, 2) + "-" + strTime.mid(6, 2) + " " + strTime.mid(8, 2) + ":" + strTime.mid(10, 2) + ":" + strTime.mid(12, 2);
	return tmp;
}

//采集时钟发送命令
void Transform2Time(QString data, QJsonObject &json)
{
	json.insert("DataType", 2);//数据类型 观测数据
	json.insert("ValueCount", 1);//返回值个数
								 //时间
	QString century = QString("%1").arg((int)data[0].unicode(), 2, 10, QChar('0'));
	QString year = QString("%1").arg((int)data[1].unicode(), 2, 10, QChar('0'));
	QString month = QString("%1").arg((int)data[2].unicode(), 2, 10, QChar('0'));
	QString day = QString("%1").arg((int)data[3].unicode(), 2, 10, QChar('0'));
	QString hour = QString("%1").arg((int)data[4].unicode(), 2, 10, QChar('0'));
	QString min = QString("%1").arg((int)data[5].unicode(), 2, 10, QChar('0'));
	QString sec = QString("%1").arg((int)data[6].unicode(), 2, 10, QChar('0'));
	QString time = Convert2Time(century + year + month + day + hour + min + sec);
	json.insert("RecvValue1", time);
}

//GPRS发送小时平均体积含水量和频率数据
void Transform2GPRSPerVolume(QString data, QJsonObject &json)
{
	//数据类型 观测数据
	json.insert("DataType", 1);

	//bytes转float
	FLOAT_UNION  f;
	//时间
	QString century = QString("%1").arg((int)data[0].unicode(), 2, 10, QChar('0'));
	QString year = QString("%1").arg((int)data[1].unicode(), 2, 10, QChar('0'));
	QString month = QString("%1").arg((int)data[2].unicode(), 2, 10, QChar('0'));
	QString day = QString("%1").arg((int)data[3].unicode(), 2, 10, QChar('0'));
	QString hour = QString("%1").arg((int)data[4].unicode(), 2, 10, QChar('0'));
	QString min = QString("%1").arg((int)data[5].unicode(), 2, 10, QChar('0'));
	QString sec = QString("%1").arg((int)data[6].unicode(), 2, 10, QChar('0'));
	QString time = Convert2Time(century + year + month + day + hour + min + sec);
	json.insert("ObserveTime", time);

	//获取电压值浮点型
	f.float_byte.low_byte = data[7].unicode();
	f.float_byte.mlow_byte = data[8].unicode();
	f.float_byte.mhigh_byte = data[9].unicode();
	f.float_byte.high_byte = data[10].unicode();
	json.insert("MainClctrVltgVal", QJsonValue(f.value));

	//原始中子数
	int I = data[11].unicode();
	I |= (data[12].unicode() << 8);
	I |= (data[13].unicode() << 16);
	I |= (data[14].unicode() << 24);
	json.insert("NeutronNum", I);

	//校正中子数
	I = data[15].unicode();
	I |= (data[16].unicode() << 8);
	I |= (data[17].unicode() << 16);
	I |= (data[18].unicode() << 24);
	json.insert("CorredNNum", I);

	//区域土壤水分
	f.float_byte.low_byte = data[19].unicode();
	f.float_byte.mlow_byte = data[20].unicode();
	f.float_byte.mhigh_byte = data[21].unicode();
	f.float_byte.high_byte = data[22].unicode();
	json.insert("VolumWaterCon", QJsonValue(f.value));

	//空气温度
	f.float_byte.low_byte = data[23].unicode();
	f.float_byte.mlow_byte = data[24].unicode();
	f.float_byte.mhigh_byte = data[25].unicode();
	f.float_byte.high_byte = data[26].unicode();
	json.insert("AT_AT1", QJsonValue(f.value));

	//空气湿度
	f.float_byte.low_byte = data[27].unicode();
	f.float_byte.mlow_byte = data[28].unicode();
	f.float_byte.mhigh_byte = data[29].unicode();
	f.float_byte.high_byte = data[30].unicode();
	json.insert("AH_RH1", QJsonValue(f.value));

	//大气压力
	f.float_byte.low_byte = data[31].unicode();
	f.float_byte.mlow_byte = data[32].unicode();
	f.float_byte.mhigh_byte = data[33].unicode();
	f.float_byte.high_byte = data[34].unicode();
	json.insert("StationPressure", QJsonValue(f.value));

	//10CM小时整点土壤体积含水量
	f.float_byte.low_byte = data[35].unicode();
	f.float_byte.mlow_byte = data[36].unicode();
	f.float_byte.mhigh_byte = data[37].unicode();
	f.float_byte.high_byte = data[38].unicode();
	json.insert("SoilVolume10", QJsonValue(f.value));
	////10CM小时整点频率
	//f.float_byte.low_byte = data[39].unicode();
	//f.float_byte.mlow_byte = data[40].unicode();
	//f.float_byte.mhigh_byte = data[41].unicode();
	//f.float_byte.high_byte = data[42].unicode();
	//json.insert("SoilFrequency10", QJsonValue(f.value));

	//20CM小时整点土壤体积含水量
	f.float_byte.low_byte = data[43].unicode();
	f.float_byte.mlow_byte = data[44].unicode();
	f.float_byte.mhigh_byte = data[45].unicode();
	f.float_byte.high_byte = data[46].unicode();
	json.insert("SoilVolume20", QJsonValue(f.value));
	////20CM小时整点频率
	//f.float_byte.low_byte = data[47].unicode();
	//f.float_byte.mlow_byte = data[48].unicode();
	//f.float_byte.mhigh_byte = data[49].unicode();
	//f.float_byte.high_byte = data[50].unicode();
	//json.insert("SoilFrequency20", QJsonValue(f.value));

	//30CM小时整点土壤体积含水量
	f.float_byte.low_byte = data[51].unicode();
	f.float_byte.mlow_byte = data[52].unicode();
	f.float_byte.mhigh_byte = data[53].unicode();
	f.float_byte.high_byte = data[54].unicode();
	json.insert("SoilVolume30", QJsonValue(f.value));
	////30CM小时整点频率
	//f.float_byte.low_byte = data[55].unicode();
	//f.float_byte.mlow_byte = data[56].unicode();
	//f.float_byte.mhigh_byte = data[57].unicode();
	//f.float_byte.high_byte = data[58].unicode();
	//json.insert("SoilFrequency30", QJsonValue(f.value));

	//40CM小时整点土壤体积含水量
	f.float_byte.low_byte = data[59].unicode();
	f.float_byte.mlow_byte = data[60].unicode();
	f.float_byte.mhigh_byte = data[61].unicode();
	f.float_byte.high_byte = data[62].unicode();
	json.insert("SoilVolume40", QJsonValue(f.value));
	////40CM小时整点频率
	//f.float_byte.low_byte = data[63].unicode();
	//f.float_byte.mlow_byte = data[64].unicode();
	//f.float_byte.mhigh_byte = data[65].unicode();
	//f.float_byte.high_byte = data[66].unicode();
	//json.insert("SoilFrequency40", QJsonValue(f.value));


	//50CM小时整点土壤体积含水量
	f.float_byte.low_byte = data[67].unicode();
	f.float_byte.mlow_byte = data[68].unicode();
	f.float_byte.mhigh_byte = data[69].unicode();
	f.float_byte.high_byte = data[70].unicode();
	json.insert("SoilVolume50", QJsonValue(f.value));
	////50CM小时整点频率
	//f.float_byte.low_byte = data[71].unicode();
	//f.float_byte.mlow_byte = data[72].unicode();
	//f.float_byte.mhigh_byte = data[73].unicode();
	//f.float_byte.high_byte = data[74].unicode();
	//json.insert("SoilFrequency50", QJsonValue(f.value));

	//60CM小时整点土壤体积含水量
	f.float_byte.low_byte = data[75].unicode();
	f.float_byte.mlow_byte = data[76].unicode();
	f.float_byte.mhigh_byte = data[77].unicode();
	f.float_byte.high_byte = data[78].unicode();
	json.insert("SoilVolume60", QJsonValue(f.value));
	//60CM小时整点频率
	//f.float_byte.low_byte = data[79].unicode();
	//f.float_byte.mlow_byte = data[80].unicode();
	//f.float_byte.mhigh_byte = data[81].unicode();
	//f.float_byte.high_byte = data[82].unicode();
	//json.insert("SoilFrequency60", QJsonValue(f.value));


	//80CM小时整点土壤体积含水量
	f.float_byte.low_byte = data[83].unicode();
	f.float_byte.mlow_byte = data[84].unicode();
	f.float_byte.mhigh_byte = data[85].unicode();
	f.float_byte.high_byte = data[86].unicode();
	json.insert("SoilVolume80", QJsonValue(f.value));
	//80CM小时整点频率
	//f.float_byte.low_byte = data[87].unicode();
	//f.float_byte.mlow_byte = data[88].unicode();
	//f.float_byte.mhigh_byte = data[89].unicode();
	//f.float_byte.high_byte = data[90].unicode();
	//json.insert("SoilFrequency80", QJsonValue(f.value));

	//100CM小时整点土壤体积含水量
	f.float_byte.low_byte = data[91].unicode();
	f.float_byte.mlow_byte = data[92].unicode();
	f.float_byte.mhigh_byte = data[93].unicode();
	f.float_byte.high_byte = data[94].unicode();
	json.insert("SoilVolume100", QJsonValue(f.value));
	//100CM小时整点频率
	//f.float_byte.low_byte = data[95].unicode();
	//f.float_byte.mlow_byte = data[96].unicode();
	//f.float_byte.mhigh_byte = data[97].unicode();
	//f.float_byte.high_byte = data[98].unicode();
	//json.insert("SoilFrequency100", QJsonValue(f.value));

}

//错误数据
void Transform2ErrorData(QString data, QJsonObject &json)
{
	json.insert("DataType", 2);//数据类型 观测数据
	json.insert("ValueCount", 1);
	json.insert("RecvValue1", QString::fromLocal8Bit("错误数据"));
}

//接收到心跳包
void Transform2Heartbeat(QString data, QJsonObject &json)
{
	json.insert("DataType", 3);//数据类型 3心跳数据

}

//通信结束
void Transform2CloeseConnection(QString data, QJsonObject &json)
{
	json.insert("DataType", 2);//数据类型 观测数据
	json.insert("ValueCount", 1);
	json.insert("RecvValue1", QString::fromLocal8Bit("通信结束"));
}

//返回值反馈
void SetValueToControlWidget(QStringList list)
{
	if (w == nullptr)
		return;
	if (isActive)
		w->setValue(list);
}
//终端命令
void SetCommand(uint Socket, int CommandType, QString Params1, QString Params2,QString StationID) //<>
{
	//设备终端命令
	switch (CommandType)
	{
	case 1301:
		//设置时钟
	{
		QDateTime nowtime = QDateTime::currentDateTime();
		QString datetime = nowtime.toString("yyyy-MM-dd hh:mm:ss");
		QString year, month, day, hour, min, sec;
		year = nowtime.toString("yy");
		month = nowtime.toString("MM");
		day = nowtime.toString("dd");
		hour = nowtime.toString("hh");
		min = nowtime.toString("mm");
		sec = nowtime.toString("ss");

		int chk = 0;
		int SrcAdrr = StationID.toInt();
		BYTE bytes[1024] = { 0 };
		bytes[0] = 0xaa;
		bytes[1] = 0x0d;//帧长度
		bytes[2] = 0x00;//帧长度
		bytes[3] = 0x84;//帧命令
		chk += bytes[3];
		bytes[4] = 0x01;//帧命令
		chk += bytes[4];
		bytes[5] = SrcAdrr & 0xff;//源地址
		chk += bytes[5];
		bytes[6] = (SrcAdrr >> 8) & 0xff;
		chk += bytes[6];
		bytes[7] = 0;//目的地址
		chk += bytes[7];
		bytes[8] = 0;
		chk += bytes[8];
		bytes[9] = 0x14;//20
		chk += bytes[9];
		bytes[10] = year.toInt();
		chk += bytes[10];
		bytes[11] = month.toInt();
		chk += bytes[11];
		bytes[12] = day.toInt();
		chk += bytes[12];
		bytes[13] = hour.toInt();
		chk += bytes[13];
		bytes[14] = min.toInt();
		chk += bytes[14];
		bytes[15] = sec.toInt();
		chk += bytes[15];
		bytes[16] = chk & 0xff;//校验位 低八位
		bytes[17] = (chk >> 8) & 0xff;//高八位
		bytes[18] = 0xdd;
		::send(Socket, (char *)bytes, 19, 0);
	}
		break;
	case 1302:
		//读时钟
	{
		int chk = 0;
		int SrcAdrr = StationID.toInt();
		BYTE bytes[1024] = { 0 };
		bytes[0] = 0xaa;
		bytes[1] = 0x06;//帧长度
		bytes[2] = 0x00;//帧长度
		bytes[3] = 0x84;//帧命令
		chk += bytes[3];
		bytes[4] = 0x02;//帧命令
		chk += bytes[4];
		bytes[5] = SrcAdrr & 0xff;//源地址
		chk += bytes[5];
		bytes[6] = (SrcAdrr >> 8) & 0xff;
		chk += bytes[6];
		bytes[7] = 0;//源地址
		chk += bytes[7];
		bytes[8] = 0;
		chk += bytes[8];
		bytes[9] = chk & 0xff;//校验位 低八位
		bytes[10] = (chk >> 8) & 0xff;//高八位
		bytes[11] = 0xdd;
		::send(Socket, (char *)bytes, 12, 0);
		break;
	}
	case 1303:
		//补抄数据
	{
		int chk = 0;
		int SrcAdrr = StationID.toInt();
		BYTE bytes[1024] = { 0 };
		bytes[0] = 0xaa;
		bytes[1] = 0x14;//帧长度
		bytes[2] = 0x00;//帧长度
		bytes[3] = 0x84;//帧命令
		chk += bytes[3];
		bytes[4] = 0x13;//帧命令
		chk += bytes[4];
		bytes[5] = SrcAdrr & 0xff;//源地址
		chk += bytes[5];
		bytes[6] = (SrcAdrr >> 8) & 0xff;
		chk += bytes[6];
		bytes[7] = 0;//目的地址
		chk += bytes[7];
		bytes[8] = 0;
		chk += bytes[8];
		QDateTime Time_B, Time_E;
		Time_B = QDateTime::fromString(Params1, "yyyy-MM-dd hh:mm:ss");
		Time_E = QDateTime::fromString(Params2, "yyyy-MM-dd hh:mm:ss");
		QString year, month, day, hour, min, sec;
		year = Time_B.toString("yy");//开始时间
		month = Time_B.toString("MM");
		day = Time_B.toString("dd");
		hour = Time_B.toString("hh");
		min = Time_B.toString("mm");
		sec = Time_B.toString("ss");
		bytes[9] = 0x14;//20
		chk += bytes[9];
		bytes[10] = year.toInt();
		chk += bytes[10];
		bytes[11] = month.toInt();
		chk += bytes[11];
		bytes[12] = day.toInt();
		chk += bytes[12];
		bytes[13] = hour.toInt();
		chk += bytes[13];
		bytes[14] = min.toInt();
		chk += bytes[14];
		bytes[15] = sec.toInt();
		chk += bytes[15];
		year = Time_E.toString("yy");//结束时间
		month = Time_E.toString("MM");
		day = Time_E.toString("dd");
		hour = Time_E.toString("hh");
		min = Time_E.toString("mm");
		sec = Time_E.toString("ss");
		bytes[16] = 0x14;//20
		chk += bytes[16];
		bytes[17] = year.toInt();
		chk += bytes[17];
		bytes[18] = month.toInt();
		chk += bytes[18];
		bytes[19] = day.toInt();
		chk += bytes[19];
		bytes[20] = hour.toInt();
		chk += bytes[20];
		bytes[21] = min.toInt();
		chk += bytes[21];
		bytes[22] = sec.toInt();
		chk += bytes[22];
		bytes[23] = chk & 0xff;//校验位 低八位
		bytes[24] = (chk >> 8) & 0xff;//高八位
		bytes[25] = 0xdd;
		::send(Socket, (char *)bytes, 26, 0);
		break;
	}
	default:
		break;
	}
}


