#ifndef SETPARAMDLG_H
#define SETPARAMDLG_H

#include<windows.h>
#include <QWidget>
#include "ui_setparamdlg.h"
#include<QMouseEvent>
class SetParamDlg : public QWidget
{
	Q_OBJECT

public:
	SetParamDlg(QWidget *parent );
	~SetParamDlg();
	void setValue(QStringList); //<>
	bool *isActive; //<>
private:
	Ui::SetParamDlg ui;
	//窗体移动
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	bool m_Drag;                //判断鼠标左键是否按下
	QPoint m_DragPosition;
	int cmmIndex;//记录当前操作类型 //
protected:
	void closeEvent(QCloseEvent *event);
private slots:

	void OnWTimeBtnClick();//设置时钟
	void OnRTimeBtnClick();//读时钟
	void OnDownBtnClick();//补抄数据
	void slot_minWindow();
public:
	QString StationID;
	uint Socket;
};

#endif // SETPARAMDLG_H
