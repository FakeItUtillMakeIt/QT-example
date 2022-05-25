#pragma once
#ifndef ROCKET_BASE_DEVICE_H
#define ROCKET_BASE_DEVICE_H


#include <QtCore>
#include "RocketGroundBaseProtol.h"


class RocketBaseDevice
{
};

/**

    @class   GeneralControlMachineDevice
    @brief   综控机设备
    @details ~

**/
class GeneralControlMachineDevice
{
public:
    /**
        @brief  综控机上电
        @retval 上电结果
    **/
    bool powerOn();//上电
    /**
        @brief  综控机下电
        @retval 下电结果
    **/
    bool powerOff();//下电
    /**
        @brief  综控机自检
        @retval 自检结果
    **/
    bool selfCheck();//自检
    /**
        @brief  地面测发控系统可向综控机发送状态查询指令，综控机接收指令后，反馈当前运行状态
        @retval 当前运行状态
    **/
    quint32 FlightControlStatusQuery();//飞控状态
    /**
        @brief  单机配电
        @retval 配电电压信息
    **/
    quint32 singleDistribution();
    /**
        @brief  惯组数据测量
        @retval 光纤惯组当前测量信息
    **/
    quint32 intertialGroupDataMeasure();
    /**
        @brief  GPS数据测量
        @retval GPS当前测量信息
    **/
    bool gpsDataMeasure();
    /**
        @brief  伺服零位装订
        @retval 装订结果
    **/
    quint32 servoZeroBinding();

    /**
        @brief  接收伺服测量信息并转发信息
        接收伺服反馈信息并转发给地面测发控
        @retval 伺服反馈信息
    **/
    quint32 servoMeasure();
 
 
    /**
        @brief  地面测发控系统将伺服小回路测试信息发送给综控机，综控机接收到该数据后，飞控软件执行具体舵偏指令见下表（由飞控软件编程并执行）。
        飞控软件以5毫秒为周期实时将伺服关键测量数据发遥测，执行完舵偏指令后，飞控软件向地面测发控发送伺服测量结果
        @retval 伺服测量结果
    **/
    quint32 servoSmallLoopMeasure();
    /**
        @brief 地面测发控设备将伺服电池激活指令发送给综控机，综控机接收到该数据后，向伺服电池输出时序信号进行电池激活。
        完成后，综控机向地面测发控设备反馈伺服电池激活信息
        @retval 电池激活信息
    **/
    bool servoBatteryOn();
    /**
        @brief  地面测发控系统发送诸元烧写控制（控制字为0x11）指令给箭上，箭上响应后进入诸元数据烧写状态，
        而后地面测发控系统才可向箭上上传诸元数据，否则箭上不予以执行诸元数据烧写动作；当完成诸元数据烧写后，
        地面测发控系统发送诸元数据烧写控制（控制字为0x22），
        箭上响应后退出诸元数据烧写状态，等待地面测发控系统其他控制指令
        @retval 烧写状态反馈
    **/
    quint32 metaDataControl();

private:

};


/**

    @class   GeneralControlMachineDevice
    @brief   伺服设备
    @details ~

**/
class ServoDevice
{
public:


private:

};


/**

    @class   GeneralControlMachineDevice
    @brief   电源管理模块
    @details ~

**/
class PowerManageModule
{
public:


private:

};

#endif