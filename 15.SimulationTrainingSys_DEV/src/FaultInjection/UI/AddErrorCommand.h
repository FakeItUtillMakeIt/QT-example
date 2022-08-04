#pragma once

#include <QWidget>
#include<QEvent>
#include<QPainter> 
#include <QMouseEvent>
#include <qmath.h>
#include <QToolButton>
#include "ui_AddErrorCommand.h"
#include "../Controls/MyFaultParameters.h"
#include "../Controls/MyFaultTree.h" 
#include "../AppCache.h"
#include "SearchFault.h"




class AddErrorCommand : public QWidget
{
	Q_OBJECT

public:
	AddErrorCommand(QWidget *parent = nullptr);
	~AddErrorCommand();

	void SetAddIndex(int index,vector<ItemStruct> faultItems);

private:
	void Init();
	void AddFault();
	void DelFault();
	void EditFault();
	void OnFaultNode(QString name);
	void IsEnable(bool isOk);
	void onFaultTypeChanged(QString tempName);
	
	void onDeciveTypeChanged(QString tempName);
	void InitTable();
	void FlashParamTable(QString tempName);
	void IsChooseParam();
	
	void SearchFaults();

	void AddErrorOK();
	void AddErrorCanCel();

	void ShowParamsTable(int index,int deviceIndex = 0);


private:

	AppCache* m_app;

	QToolButton* m_chooseBtn;
	MyFaultTree* m_myFaultTree;

	QScrollArea* m_scrollAreaFault;
	QHBoxLayout* m_hlayout;

	//SearchFault* m_searchFaultwgt;

	

protected:

	QPoint mLastMousePosition;
	bool mMoving;
	void paintEvent(QPaintEvent* event);
	void mousePressEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent* e) Q_DECL_OVERRIDE;

private:
	Ui::AddErrorCommandClass ui;
	//QStandardItemModel m_tableModel2;
	bool m_isAddFault;

	vector<ItemStruct> m_faultItems;//�ݴ浱ǰ������
	vector<ItemStruct> searchItems;//������������ʾ
	map<int, int> m_indexDevice;//�������豸id,�ڽ�����cb_deciveType����ָ����ѡ��

	vector<AddOneFaultInfo> m_addFaults;//�洢��������ӵĹ���
	vector<AddOneFaultInfo> m_delFaults;//�洢������Ҫɾ���Ĺ���
	vector<AddOneFaultInfo> m_editFaults;//�洢������Ҫ�޸ĵĹ���

	vector<int> m_tempCommandId;//���϶�Ӧ��ǰ�������Ϲ����Ĳ���  
	vector<int> m_tempParamId;//���϶�Ӧ��ǰָ����Ϲ����Ĳ��� 

signals:
	
	void AddFaultItem(vector<AddOneFaultInfo> addFaults);
	void DelFaultItem(vector<AddOneFaultInfo> delFaults);
	void EditFaultItem(vector<AddOneFaultInfo> editFaults);
	

};
