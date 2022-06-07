#pragma execution_character_set("utf-8")

#ifndef POSTTYPECONFIG_H
#define POSTTYPECONFIG_H

#include <QWidget>
#include <QtWidgets>


class PostTypeConfig : public QWidget
{
    Q_OBJECT
private:
    explicit PostTypeConfig(QWidget *parent = nullptr);

public:
    static PostTypeConfig* getInstance(){
        if(instance==nullptr){
            instance=new PostTypeConfig();
        }
        return  instance;
    }

QMap<QString,QVector<qreal>> postTypeConfigData;

private:
    static PostTypeConfig* instance;

    QPushButton* btnAdd;
    QPushButton* btnEdit;
    QPushButton* btnRemove;
    QLabel* labelInfo;
    QStackedWidget* stackWidget;
    QTableWidget* tableConfig;
    QPushButton* btnCancel;
    QPushButton* btnOK;
    QStringList configData;

private slots:
    void editConfig();
    void addNewConfig();
    void deleteConfig();
    void clickConfigOK();
    void clickConfigCancle();

signals:
    void configOK();
    void configCancle();

};

#endif // POSTTYPECONFIG_H
