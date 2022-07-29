#ifndef SOFTWARE_H
#define SOFTWARE_H
#pragma once
#include <QObject>
#include <QVariant>
#include <QVariantMap>
using namespace std;
 
class LogInfo
{
public: 
    int level;
    int days;//表示最大遗留天数
    int auto_clear;//表示是否自动清除，1表示自动，0表示不自动
};


class  OutputPath
{
public:  
    string m_strType;
    string m_strRoot;
    string m_server;
    unsigned short m_port;
    string m_db_name;
    string m_user_name;
    string m_password; 
};


class PeerInfo
{
public:  
    string m_strNetworkType;
    string m_strIP;
    unsigned short m_iPort;
    string m_name; 
};


class Software : public QObject
{
    Q_OBJECT  
public:
    Software(QObject* parent = nullptr);
    ~Software();
      
    struct BaseSize
    {
        int m_width;
        int m_height;
    };


    void setCode(const QString& code)
    {
        m_code = code;
    }
    QString code() const {
        return m_code;
    }

    void setName(const QString& name)
    {
        m_name = name;
    }
    QString GetName() const {
        return m_name;
    }
    void setType(const QString& type)
    {
        m_type = type;
    }
    QString GetType() const {
        return m_type;
    }

    void setVertionNode(const QString& vertion_node)
    {
        m_vertion_node = vertion_node;
    }
    QString vertionNode() const {
        return m_vertion_node;
    }

    void setLogoIcon(const QString& logo_icon)
    {
        m_logo_icon = logo_icon;
    }
    QString logoIcon() const 
    {
        return m_logo_icon;
    }
  
    void setLogLevel(const int& log_level)
    {
        m_log_level = log_level;
    }
    int logLevel() const {
        return m_log_level;
    } 

    void setBaseSizeInfo(int width, int height)
    {
        m_base_width = width;
        m_base_height = height;
    }
    void setFullScreenSizeInfo(int width, int height)
    {
        m_full_width = width;
        m_full_height = height;
    }
    int baseWidth() const {
        return m_base_width;
    }
    int baseHeight() const {
        return m_base_height;
    }
    int fullWidth() const {
        return m_full_width;
    }
    int fullHeight() const {
        return m_full_height;
    }
private:  
    QString m_code;
    QString m_name;
    QString m_type;
    QString m_vertion_node;
    QString m_logo_icon;
    int m_log_level; 
    //原始尺寸 
    int m_base_width;
    int m_base_height;
    //全屏尺寸 
    int m_full_width;
    int m_full_height;
}; 
 
#endif