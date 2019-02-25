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
	//�����ƶ�
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	bool m_Drag;                //�ж��������Ƿ���
	QPoint m_DragPosition;
	int cmmIndex;//��¼��ǰ�������� //
protected:
	void closeEvent(QCloseEvent *event);
private slots:

	void OnWTimeBtnClick();//����ʱ��
	void OnRTimeBtnClick();//��ʱ��
	void OnDownBtnClick();//��������
	void slot_minWindow();
public:
	QString StationID;
	uint Socket;
};

#endif // SETPARAMDLG_H