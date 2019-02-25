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

//��ȡҵ���
int GetServiceTypeID()
{
	return 13;
}

//��ȡҵ������
QString GetServiceTypeName()
{
	QString name = QString::fromLocal8Bit("ʪ������ˮ������ҵ��");
	return name;
}

//��ȡ�汾��
QString GetVersionNo()
{
	QString Version = QString::fromLocal8Bit("1.0");
	return Version;
}

//����ʹ����¼���Ӧ
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

//��ȡ�˿ں�
int GetPort()
{
	return 23003;
}

//����ʱ��
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
	bytes[1] = 0x0d;//֡����
	bytes[2] = 0x00;//֡����
	bytes[3] = 0x84;//֡����
	chk += bytes[3];
	bytes[4] = 0x01;//֡����
	chk += bytes[4];
	bytes[5] = SrcAdrr & 0xff;//Դ��ַ
	chk += bytes[5];
	bytes[6] = (SrcAdrr >> 8) & 0xff;
	chk += bytes[6];
	bytes[7] = 0;//Դ��ַ
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
	bytes[16] = chk & 0xff;//У��λ �Ͱ�λ
	bytes[17] = (chk >> 8) & 0xff;//�߰�λ
	bytes[18] = 0xdd;
	::send(Socket, (char *)bytes, 19, 0);
}

//��������
LRESULT Char2Json(QString &buff, QJsonObject &json)
{
	int Count = 0;//���ݸ���
	int Current_P = buff.length();//��ǰ����ָ

								  //������������
	for (int i = 0; i < buff.length(); i++)
	{
		//֡ͷ  aa
		if (buff[i] == 0xaa)
		{
			Current_P = i;//ָ��ָ��֡ͷ
			for (int j = i +1; j < buff.length(); j++)
			{
				if (buff[j] == 0xdd)
				{
					Current_P = j + 1;//ָ���ƶ���֡β��һ���ַ�
					
					QString strBuff = buff.mid(i, j - i +1);
					QJsonObject SubJson;
					SubJson.insert("ServiceTypeID", QString::number(SH_TRSF_QY));

					Frame frame = { 0,NULL,NULL };
					//��ȡ֡����
					frame.len = ((strBuff[1]).unicode() & 0xFF + strBuff[2].unicode() * 256) - 6;
					//�ж�֡���� 12Ϊ���� 0Ϊ����
					if (strBuff.length() - frame.len != 12|| strBuff.length() - frame.len==0)
						continue;
					//��ȡ֡����
					frame.Command = strBuff[3].unicode() + strBuff[4].unicode() * 256;
					//��ȡԴ��ַ
					frame.SrcAddr = strBuff[5].unicode() + strBuff[6].unicode() * 256;
					//��ȡĿ�ĵ�ַ
					frame.DesAddr = strBuff[7].unicode() + strBuff[8].unicode() * 256;
					//��վ��
					SubJson.insert("StationID", "NULL");
					//�豸��
					SubJson.insert("DeviceID", QString::number(frame.SrcAddr));
		
					frame.data = strBuff.mid(9, frame.len);
					//�жϽ�����������
					switch (frame.Command)
					{
						//��ȡ�ɼ�ʱ�ӷ������� 0x0204
					case 516:
						Transform2Time(frame.data, SubJson);
						break;
						//GPRSƽ�������ˮ����Ƶ������ 0x0404
					case 1028:
						Transform2GPRSPerVolume(frame.data, SubJson);
						break;
						//���ݴ���
					case 65028:
						Transform2ErrorData(frame.data, SubJson);
						break;
						//ͨѶ���� Oxff04
					case 65284:
						Transform2CloeseConnection(frame.data, SubJson);
						break;
						//���������� 0x3A
					case 1082:
						Transform2Heartbeat(frame.data, SubJson);
						break;
					default:
						SubJson.insert("Command", frame.Command);
						break;
					}
					Count += 1;//���ݸ���
					//���ݱ���
					QDateTime current_date_time = QDateTime::currentDateTime();
					QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss");
					QString current_day = current_date_time.toString("yyyy-MM-dd");
					QString fileName = QCoreApplication::applicationDirPath() + "\\SH\\TRSF_QY\\" + QString::number(frame.SrcAddr) + "\\" + current_day;
					QDir dir(fileName);
					if (!dir.exists())
						dir.mkpath(fileName);//�����༶Ŀ¼
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
					i = j;//��ǰѭ��
					break;
				}
			}
		}
	}
	json.insert("DataLength", Count);//JSON���ݸ���
	if (Current_P >= buff.length())//�жϵ�ǰָ��λ��
	{
		buff.clear();
	}//����ڴ�
	else
	{
		buff.remove(0, Current_P);
	}//��ʣ���ֽڴ��뻺��
	return 1;
}

QString Convert2Time(QString strTime)
{
	QString tmp;
	tmp = strTime.mid(0, 4) + "-" + strTime.mid(4, 2) + "-" + strTime.mid(6, 2) + " " + strTime.mid(8, 2) + ":" + strTime.mid(10, 2) + ":" + strTime.mid(12, 2);
	return tmp;
}

//�ɼ�ʱ�ӷ�������
void Transform2Time(QString data, QJsonObject &json)
{
	json.insert("DataType", 2);//�������� �۲�����
	json.insert("ValueCount", 1);//����ֵ����
								 //ʱ��
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

//GPRS����Сʱƽ�������ˮ����Ƶ������
void Transform2GPRSPerVolume(QString data, QJsonObject &json)
{
	//�������� �۲�����
	json.insert("DataType", 1);

	//bytesתfloat
	FLOAT_UNION  f;
	//ʱ��
	QString century = QString("%1").arg((int)data[0].unicode(), 2, 10, QChar('0'));
	QString year = QString("%1").arg((int)data[1].unicode(), 2, 10, QChar('0'));
	QString month = QString("%1").arg((int)data[2].unicode(), 2, 10, QChar('0'));
	QString day = QString("%1").arg((int)data[3].unicode(), 2, 10, QChar('0'));
	QString hour = QString("%1").arg((int)data[4].unicode(), 2, 10, QChar('0'));
	QString min = QString("%1").arg((int)data[5].unicode(), 2, 10, QChar('0'));
	QString sec = QString("%1").arg((int)data[6].unicode(), 2, 10, QChar('0'));
	QString time = Convert2Time(century + year + month + day + hour + min + sec);
	json.insert("ObserveTime", time);

	//��ȡ��ѹֵ������
	f.float_byte.low_byte = data[7].unicode();
	f.float_byte.mlow_byte = data[8].unicode();
	f.float_byte.mhigh_byte = data[9].unicode();
	f.float_byte.high_byte = data[10].unicode();
	json.insert("MainClctrVltgVal", QJsonValue(f.value));

	//ԭʼ������
	int I = data[11].unicode();
	I |= (data[12].unicode() << 8);
	I |= (data[13].unicode() << 16);
	I |= (data[14].unicode() << 24);
	json.insert("NeutronNum", I);

	//У��������
	I = data[15].unicode();
	I |= (data[16].unicode() << 8);
	I |= (data[17].unicode() << 16);
	I |= (data[18].unicode() << 24);
	json.insert("CorredNNum", I);

	//��������ˮ��
	f.float_byte.low_byte = data[19].unicode();
	f.float_byte.mlow_byte = data[20].unicode();
	f.float_byte.mhigh_byte = data[21].unicode();
	f.float_byte.high_byte = data[22].unicode();
	json.insert("VolumWaterCon", QJsonValue(f.value));

	//�����¶�
	f.float_byte.low_byte = data[23].unicode();
	f.float_byte.mlow_byte = data[24].unicode();
	f.float_byte.mhigh_byte = data[25].unicode();
	f.float_byte.high_byte = data[26].unicode();
	json.insert("AT_AT1", QJsonValue(f.value));

	//����ʪ��
	f.float_byte.low_byte = data[27].unicode();
	f.float_byte.mlow_byte = data[28].unicode();
	f.float_byte.mhigh_byte = data[29].unicode();
	f.float_byte.high_byte = data[30].unicode();
	json.insert("AH_RH1", QJsonValue(f.value));

	//����ѹ��
	f.float_byte.low_byte = data[31].unicode();
	f.float_byte.mlow_byte = data[32].unicode();
	f.float_byte.mhigh_byte = data[33].unicode();
	f.float_byte.high_byte = data[34].unicode();
	json.insert("StationPressure", QJsonValue(f.value));

	//10CMСʱ�������������ˮ��
	f.float_byte.low_byte = data[35].unicode();
	f.float_byte.mlow_byte = data[36].unicode();
	f.float_byte.mhigh_byte = data[37].unicode();
	f.float_byte.high_byte = data[38].unicode();
	json.insert("SoilVolume10", QJsonValue(f.value));
	////10CMСʱ����Ƶ��
	//f.float_byte.low_byte = data[39].unicode();
	//f.float_byte.mlow_byte = data[40].unicode();
	//f.float_byte.mhigh_byte = data[41].unicode();
	//f.float_byte.high_byte = data[42].unicode();
	//json.insert("SoilFrequency10", QJsonValue(f.value));

	//20CMСʱ�������������ˮ��
	f.float_byte.low_byte = data[43].unicode();
	f.float_byte.mlow_byte = data[44].unicode();
	f.float_byte.mhigh_byte = data[45].unicode();
	f.float_byte.high_byte = data[46].unicode();
	json.insert("SoilVolume20", QJsonValue(f.value));
	////20CMСʱ����Ƶ��
	//f.float_byte.low_byte = data[47].unicode();
	//f.float_byte.mlow_byte = data[48].unicode();
	//f.float_byte.mhigh_byte = data[49].unicode();
	//f.float_byte.high_byte = data[50].unicode();
	//json.insert("SoilFrequency20", QJsonValue(f.value));

	//30CMСʱ�������������ˮ��
	f.float_byte.low_byte = data[51].unicode();
	f.float_byte.mlow_byte = data[52].unicode();
	f.float_byte.mhigh_byte = data[53].unicode();
	f.float_byte.high_byte = data[54].unicode();
	json.insert("SoilVolume30", QJsonValue(f.value));
	////30CMСʱ����Ƶ��
	//f.float_byte.low_byte = data[55].unicode();
	//f.float_byte.mlow_byte = data[56].unicode();
	//f.float_byte.mhigh_byte = data[57].unicode();
	//f.float_byte.high_byte = data[58].unicode();
	//json.insert("SoilFrequency30", QJsonValue(f.value));

	//40CMСʱ�������������ˮ��
	f.float_byte.low_byte = data[59].unicode();
	f.float_byte.mlow_byte = data[60].unicode();
	f.float_byte.mhigh_byte = data[61].unicode();
	f.float_byte.high_byte = data[62].unicode();
	json.insert("SoilVolume40", QJsonValue(f.value));
	////40CMСʱ����Ƶ��
	//f.float_byte.low_byte = data[63].unicode();
	//f.float_byte.mlow_byte = data[64].unicode();
	//f.float_byte.mhigh_byte = data[65].unicode();
	//f.float_byte.high_byte = data[66].unicode();
	//json.insert("SoilFrequency40", QJsonValue(f.value));


	//50CMСʱ�������������ˮ��
	f.float_byte.low_byte = data[67].unicode();
	f.float_byte.mlow_byte = data[68].unicode();
	f.float_byte.mhigh_byte = data[69].unicode();
	f.float_byte.high_byte = data[70].unicode();
	json.insert("SoilVolume50", QJsonValue(f.value));
	////50CMСʱ����Ƶ��
	//f.float_byte.low_byte = data[71].unicode();
	//f.float_byte.mlow_byte = data[72].unicode();
	//f.float_byte.mhigh_byte = data[73].unicode();
	//f.float_byte.high_byte = data[74].unicode();
	//json.insert("SoilFrequency50", QJsonValue(f.value));

	//60CMСʱ�������������ˮ��
	f.float_byte.low_byte = data[75].unicode();
	f.float_byte.mlow_byte = data[76].unicode();
	f.float_byte.mhigh_byte = data[77].unicode();
	f.float_byte.high_byte = data[78].unicode();
	json.insert("SoilVolume60", QJsonValue(f.value));
	//60CMСʱ����Ƶ��
	//f.float_byte.low_byte = data[79].unicode();
	//f.float_byte.mlow_byte = data[80].unicode();
	//f.float_byte.mhigh_byte = data[81].unicode();
	//f.float_byte.high_byte = data[82].unicode();
	//json.insert("SoilFrequency60", QJsonValue(f.value));


	//80CMСʱ�������������ˮ��
	f.float_byte.low_byte = data[83].unicode();
	f.float_byte.mlow_byte = data[84].unicode();
	f.float_byte.mhigh_byte = data[85].unicode();
	f.float_byte.high_byte = data[86].unicode();
	json.insert("SoilVolume80", QJsonValue(f.value));
	//80CMСʱ����Ƶ��
	//f.float_byte.low_byte = data[87].unicode();
	//f.float_byte.mlow_byte = data[88].unicode();
	//f.float_byte.mhigh_byte = data[89].unicode();
	//f.float_byte.high_byte = data[90].unicode();
	//json.insert("SoilFrequency80", QJsonValue(f.value));

	//100CMСʱ�������������ˮ��
	f.float_byte.low_byte = data[91].unicode();
	f.float_byte.mlow_byte = data[92].unicode();
	f.float_byte.mhigh_byte = data[93].unicode();
	f.float_byte.high_byte = data[94].unicode();
	json.insert("SoilVolume100", QJsonValue(f.value));
	//100CMСʱ����Ƶ��
	//f.float_byte.low_byte = data[95].unicode();
	//f.float_byte.mlow_byte = data[96].unicode();
	//f.float_byte.mhigh_byte = data[97].unicode();
	//f.float_byte.high_byte = data[98].unicode();
	//json.insert("SoilFrequency100", QJsonValue(f.value));

}

//��������
void Transform2ErrorData(QString data, QJsonObject &json)
{
	json.insert("DataType", 2);//�������� �۲�����
	json.insert("ValueCount", 1);
	json.insert("RecvValue1", QString::fromLocal8Bit("��������"));
}

//���յ�������
void Transform2Heartbeat(QString data, QJsonObject &json)
{
	json.insert("DataType", 3);//�������� 3��������

}

//ͨ�Ž���
void Transform2CloeseConnection(QString data, QJsonObject &json)
{
	json.insert("DataType", 2);//�������� �۲�����
	json.insert("ValueCount", 1);
	json.insert("RecvValue1", QString::fromLocal8Bit("ͨ�Ž���"));
}

//����ֵ����
void SetValueToControlWidget(QStringList list)
{
	if (w == nullptr)
		return;
	if (isActive)
		w->setValue(list);
}
//�ն�����
void SetCommand(uint Socket, int CommandType, QString Params1, QString Params2,QString StationID) //<>
{
	//�豸�ն�����
	switch (CommandType)
	{
	case 1301:
		//����ʱ��
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
		bytes[1] = 0x0d;//֡����
		bytes[2] = 0x00;//֡����
		bytes[3] = 0x84;//֡����
		chk += bytes[3];
		bytes[4] = 0x01;//֡����
		chk += bytes[4];
		bytes[5] = SrcAdrr & 0xff;//Դ��ַ
		chk += bytes[5];
		bytes[6] = (SrcAdrr >> 8) & 0xff;
		chk += bytes[6];
		bytes[7] = 0;//Ŀ�ĵ�ַ
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
		bytes[16] = chk & 0xff;//У��λ �Ͱ�λ
		bytes[17] = (chk >> 8) & 0xff;//�߰�λ
		bytes[18] = 0xdd;
		::send(Socket, (char *)bytes, 19, 0);
	}
		break;
	case 1302:
		//��ʱ��
	{
		int chk = 0;
		int SrcAdrr = StationID.toInt();
		BYTE bytes[1024] = { 0 };
		bytes[0] = 0xaa;
		bytes[1] = 0x06;//֡����
		bytes[2] = 0x00;//֡����
		bytes[3] = 0x84;//֡����
		chk += bytes[3];
		bytes[4] = 0x02;//֡����
		chk += bytes[4];
		bytes[5] = SrcAdrr & 0xff;//Դ��ַ
		chk += bytes[5];
		bytes[6] = (SrcAdrr >> 8) & 0xff;
		chk += bytes[6];
		bytes[7] = 0;//Դ��ַ
		chk += bytes[7];
		bytes[8] = 0;
		chk += bytes[8];
		bytes[9] = chk & 0xff;//У��λ �Ͱ�λ
		bytes[10] = (chk >> 8) & 0xff;//�߰�λ
		bytes[11] = 0xdd;
		::send(Socket, (char *)bytes, 12, 0);
		break;
	}
	case 1303:
		//��������
	{
		int chk = 0;
		int SrcAdrr = StationID.toInt();
		BYTE bytes[1024] = { 0 };
		bytes[0] = 0xaa;
		bytes[1] = 0x14;//֡����
		bytes[2] = 0x00;//֡����
		bytes[3] = 0x84;//֡����
		chk += bytes[3];
		bytes[4] = 0x13;//֡����
		chk += bytes[4];
		bytes[5] = SrcAdrr & 0xff;//Դ��ַ
		chk += bytes[5];
		bytes[6] = (SrcAdrr >> 8) & 0xff;
		chk += bytes[6];
		bytes[7] = 0;//Ŀ�ĵ�ַ
		chk += bytes[7];
		bytes[8] = 0;
		chk += bytes[8];
		QDateTime Time_B, Time_E;
		Time_B = QDateTime::fromString(Params1, "yyyy-MM-dd hh:mm:ss");
		Time_E = QDateTime::fromString(Params2, "yyyy-MM-dd hh:mm:ss");
		QString year, month, day, hour, min, sec;
		year = Time_B.toString("yy");//��ʼʱ��
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
		year = Time_E.toString("yy");//����ʱ��
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
		bytes[23] = chk & 0xff;//У��λ �Ͱ�λ
		bytes[24] = (chk >> 8) & 0xff;//�߰�λ
		bytes[25] = 0xdd;
		::send(Socket, (char *)bytes, 26, 0);
		break;
	}
	default:
		break;
	}
}


