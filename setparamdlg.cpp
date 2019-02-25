#include "setparamdlg.h"
#include<QMessageBox>
SetParamDlg::SetParamDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);//<>
	setWindowFlags(Qt::WindowCloseButtonHint | Qt::FramelessWindowHint);
	ui.CloseBtn->setToolTipName(QString::fromLocal8Bit("�ر�"));
	ui.MinBtn->setToolTipName(QString::fromLocal8Bit("��С��"));

	connect(ui.MinBtn, SIGNAL(clicked()), this, SLOT(slot_minWindow()));//��С��
	connect(ui.CloseBtn, SIGNAL(clicked()), this, SLOT(close()));//�رմ���
	connect(ui.WTimeBtn, SIGNAL(clicked()), this, SLOT(OnWTimeBtnClick()));
	connect(ui.RTimeBtn, SIGNAL(clicked()), this, SLOT(OnRTimeBtnClick()));
	connect(ui.DownBtn, SIGNAL(clicked()), this, SLOT(OnDownBtnClick()));
}

SetParamDlg::~SetParamDlg()
{

}

void SetParamDlg::closeEvent(QCloseEvent *event)
{
	QWidget::close();
	*isActive = false;
	delete this;
}
void SetParamDlg::slot_minWindow()
{
	this->showMinimized();
}

void SetParamDlg::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		m_Drag = true;
		m_DragPosition = event->globalPos() - this->pos();
		event->accept();
	}
}

void SetParamDlg::mouseMoveEvent(QMouseEvent *event)
{
	if (m_Drag && (event->buttons() && Qt::LeftButton)) {
		move(event->globalPos() - m_DragPosition);
		event->accept();
	}
}

void SetParamDlg::mouseReleaseEvent(QMouseEvent *event)
{
	m_Drag = false;
}
//����ʱ��
void SetParamDlg::OnWTimeBtnClick()
{
	cmmIndex = 1301;//<>
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

//��ʱ��
void SetParamDlg::OnRTimeBtnClick()
{
	cmmIndex = 1302;//<>
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
	bytes[8] =0;
	chk += bytes[8];
	bytes[9] = chk & 0xff;//У��λ �Ͱ�λ
	bytes[10] = (chk >> 8) & 0xff;//�߰�λ
	bytes[11] = 0xdd;
	::send(Socket, (char *)bytes, 12, 0);
}

//��������
void SetParamDlg::OnDownBtnClick()
{
	cmmIndex = 1303;//<>
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
	Time_B = ui.BdateTimeEdit->dateTime();
	Time_E = ui.EdateTimeEdit->dateTime();
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
}

void SetParamDlg::setValue(QStringList list)
{
	switch (cmmIndex)
	{
	case 1302://��ȡʱ�� //<>
	{
		if (list.count() < 1)
			break;
		QDateTime time = QDateTime::fromString(list.at(0), "yyyy-MM-dd hh:mm:ss");
		ui.StationTimeEdit->setDateTime(time);
		break;
	}
	default://<>
	{
		if (list.count() < 1)
			break;
		QString r = list.at(0);
		if (r.compare(QString::fromLocal8Bit("ͨ�Ž���")) == 0)
		{
			QMessageBox::about(this, QString::fromLocal8Bit("�豸״̬"), QString::fromLocal8Bit("�����ɹ���"));
		}
	
	}
		break;
	}
}
