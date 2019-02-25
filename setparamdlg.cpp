#include "setparamdlg.h"
#include<QMessageBox>
SetParamDlg::SetParamDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);//<>
	setWindowFlags(Qt::WindowCloseButtonHint | Qt::FramelessWindowHint);
	ui.CloseBtn->setToolTipName(QString::fromLocal8Bit("关闭"));
	ui.MinBtn->setToolTipName(QString::fromLocal8Bit("最小化"));

	connect(ui.MinBtn, SIGNAL(clicked()), this, SLOT(slot_minWindow()));//最小化
	connect(ui.CloseBtn, SIGNAL(clicked()), this, SLOT(close()));//关闭窗体
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
//设置时钟
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

//读时钟
void SetParamDlg::OnRTimeBtnClick()
{
	cmmIndex = 1302;//<>
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
	bytes[8] =0;
	chk += bytes[8];
	bytes[9] = chk & 0xff;//校验位 低八位
	bytes[10] = (chk >> 8) & 0xff;//高八位
	bytes[11] = 0xdd;
	::send(Socket, (char *)bytes, 12, 0);
}

//补抄数据
void SetParamDlg::OnDownBtnClick()
{
	cmmIndex = 1303;//<>
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
	Time_B = ui.BdateTimeEdit->dateTime();
	Time_E = ui.EdateTimeEdit->dateTime();
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
}

void SetParamDlg::setValue(QStringList list)
{
	switch (cmmIndex)
	{
	case 1302://读取时钟 //<>
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
		if (r.compare(QString::fromLocal8Bit("通信结束")) == 0)
		{
			QMessageBox::about(this, QString::fromLocal8Bit("设备状态"), QString::fromLocal8Bit("操作成功！"));
		}
	
	}
		break;
	}
}
