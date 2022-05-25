#pragma once

#ifndef BASE_LAUNCH_COMMAND_H
#define BASE_LAUNCH_COMMAND_H

#include "CrcCompute.h"
#include "RocketGroundProtolDataFormat.h"

#include <QMap>
#include <QVector>



class BaseRequestText
{
public:
    BaseRequestText();

    QMap<int, QString> datamap;
    QVector<QString> boxText;

};


class BaseLuanchCommand
{
public:
    BaseLuanchCommand();

public:
    /**
    @brief  综控机上电请求
    测发控软件通过RS422向电源管理模块发送综控机上电指令，电源管理模块控制输出28V上电脉冲信号，使综控机上电。该协议无对应遥测回令，
    @retval 请求数据
**/
    QByteArray generalMachinePowerOnRequest();//上电
    /**
        @brief  综控机下电请求
        测发控软件通过RS422向电源管理模块发送综控机下电指令，使综控机下电。该协议无对应遥测协议
        @retval 请求数据
    **/
    QByteArray generalMachinePowerOffRequest();//下电
    /**
        @brief  综控机自检请求
        地面测发控系统以100ms为周期，向综控机发送自检请求指令，综控机受令获得自身自检信息，并与箭上其他设备逐个通讯，获得自检结果信息。
        综控机收到指令后立即反馈自检流程完成状态，地面测发控对结果进行判断，自检结果是否正常
        @retval
    **/
    QByteArray generalMachineSelfCheckRequest();//自检
    
    /**
        @brief  地面测发控系统可向综控机发送状态查询指令，综控机接收指令后，反馈当前运行状态
        @retval 当前运行状态
    **/
    QByteArray flightControlSoftwareStatusQueryRequest();//飞控状态
  
    /**
        @brief  单机配电请求
        地面测发控设备将单机配电指令发送给综控机，综控机接收到该数据后，向综控机配电单元以及时序模块进行单机配电。
        完成后，综控机向地面测发控设备反馈配电电压信息。
        @retval
    **/
    QByteArray singleDistributionRequest();
   
    /**
        @brief  惯组数据测量
        地面测发控设备向综控机发送惯组数据测量请求，综控机将光纤惯组当前测量信息反馈给地面设备
        @retval
    **/
    QByteArray intertialGroupDataMeasureRequest();
   
    /**
        @brief  GPS数据测量
        地面测发控设备向综控机发送GPS数据测量请求，综控机将GPS当前测量信息反馈给地面设备
        @retval
    **/
    QByteArray gpsDataMeasureRequest();
   
    /**
        @brief  伺服零位装订
        地面测发控系统将零位信息发送给综控机，综控机接收到该数据后，向伺服系统进行零位数据装订，完成后，综控机向地面测发控设备反馈装订结果
        @retval
    **/
    QByteArray servoZeroBindingRequest();
    
    /**
        @brief  接收伺服测量信息并转发信息
        地面测发控系统将伺服控制信息发送给综控机，综控机接收到该数据后向伺服系统转发该信息，并将伺服反馈信息转发给地面测发控系统
        @retval
    **/
    QByteArray servoMeasureRequest();
    

    /**
        @brief  地面测发控系统将伺服小回路测试信息发送给综控机，综控机接收到该数据后，飞控软件执行具体舵偏指令见下表（由飞控软件编程并执行）。
        飞控软件以5毫秒为周期实时将伺服关键测量数据发遥测，执行完舵偏指令后，飞控软件向地面测发控发送伺服测量结果
        @retval
    **/
    QByteArray servoSmallLoopMeasureRequest();
    
    /**
        @brief 地面测发控设备将伺服电池激活指令发送给综控机，综控机接收到该数据后，向伺服电池输出时序信号进行电池激活。
        完成后，综控机向地面测发控设备反馈伺服电池激活信息
        @retval
    **/
    QByteArray servoBatteryActivateRequest();
   
    /**
        @brief  地面测发控系统发送诸元烧写控制（控制字为0x11）指令给箭上，箭上响应后进入诸元数据烧写状态，
        而后地面测发控系统才可向箭上上传诸元数据，否则箭上不予以执行诸元数据烧写动作；当完成诸元数据烧写后，
        地面测发控系统发送诸元数据烧写控制（控制字为0x22），
        箭上响应后退出诸元数据烧写状态，等待地面测发控系统其他控制指令
        @retval
    **/
    QByteArray metaDataControlRequest();
    
    /**
        @brief  诸元数据上传
        诸元数据进入软件烧写状态后，接收并响应地面测发控系统发送的诸元数据，地面测发控系统依据可执行程序进行数据分包，计算所需软件总包数，并开始上传第一包，包序号从0号开始，直至最后一包，每包有效数据最多为250个字节。
        重发间隔时间100ms，若连续3次未接收到综控机反馈，则停止流程
        @retval 上传结果  返回结果应该是协议基本格式
        箭上--->地面
    **/
    QByteArray metaDataUploadRequest();
   
    /**
        @brief  诸元数据下传
        综控机在任何状态下都可以进行诸元数据下传，诸元数据依据地面测发控系统发送的需查询数据包序号进行数据下传，每包数据最多一次下传250个字节。
        重发间隔时间100ms，若连续3次未接收到综控机反馈，则停止流程
        @retval 下传结果  返回结果应该是协议基本格式
        箭上--->地面
    **/
    QByteArray metaDataDownloadRequest();
   
    /**
        @brief  诸元数据退出烧写状态后，地面测发控系统可发送诸元数据校验指令，诸元数据读取flash中可执行程序软件，并计算校验和，反馈给地面测发控系统。
        在校验过程中，飞控功能软件可能无法响应地面指令。
        诸元数据完成校验后，反馈给地面测发控系统
        @retval
    **/
    QByteArray metaDataCheckRequest();

    /**
        @brief  诸元数据加载
        诸元数据退出烧写状态后，地面测发控系统可发送诸元数据加载指令，诸元数据加载Flash中的诸元数据到飞控软件中，飞控使用加载后的诸元数据。
        诸元数据加载完成后，反馈执行结果给地面测发控系统
        @retval
    **/
    QByteArray metaDataLoadingRequest();
   
    /**
        @brief  模飞数据上传
        地面测发控系统依据可执行程序进行数据分包，计算所需软件总包数，并开始上传第一包，包序号从0号开始，直至最后一包，每包有效数据最多为250个字节。
        重发间隔时间100ms，若连续3次未接收到综控机反馈，则停止流程
        @retval
    **/
    QByteArray simulationFlightDataUploadRequest();
   
    /**
        @brief  火工品端口测试
        地面测发控系统向综控机发送火工品端口测试请求，综控机依据指令进行火工品端口测试，飞控软件查询时序2板的端口状态，30个接口如果无输出（30个端口置位均为“0”），返回测试成功；
        如果有输出（该输出端口此时置位为“1”），则返回测试失败。
        从接收到火工品端口测试请求开始，飞控软件以5毫秒为周期实时采集DRV_STATUS1和DRV_IN_STATSU1，并将寄存器状态发送遥测，该步操作时间跨度要求从接受到火工品端口测试请求后持续15秒。
        15秒测试结果均返回测试成功，则将测试结果发送地面测发控系统

        @retval
    **/
    QByteArray explosivePortTestRequest();
   
    /**
        @brief  火工品阻值测量
        地面测发控系统向综控机发送火工品阻值测量请求，综控机将每一路的火工品当前测量阻值反馈给地面测发控系统。
        @retval
    **/
    QByteArray explosiveResistMeasureRequest();
   
    /**
        @brief  水平计算
        地面测发控系统向综控机发送水平计算指令，综控机依据指令进行水平计算，在60秒时间内，期间地面测发控系统以100ms为周期向综控机发送该指令，并将惯组数据转发遥测，
        同时进行积分解算合成自转角速度 与合成加速度 ，180秒后将解算结果合成自转角速度 与合成加速度 发送地面测发控系统，同时反馈水平计算结果。
        若需要重新进行水平计算指令，则在原执行ID值加1。
        @retval
    **/
    QByteArray levelComputeRequest();

    /**
        @brief  转导航
        地面测发控系统在完成水平计算后，可向综控机发送转导航指令，综控机进入导航状态成功后，地面测发控系统可随时再向综控机发送转导航指令，转导航指令向地面测发控系统反馈当前导航状态。
        @retval
    **/
    QByteArray trunNavigateRequest();
   
    /**
        @brief  飞控软件烧写控制
        地面测发控系统发送飞控软件烧写控制（控制字为0x11）指令给箭上，箭上响应后进入飞控软件烧写状态，而后地面测发控系统才可向箭上上传飞控软件可执行程序数据，否则箭上不予以执行飞控软件数据烧写动作；当完成飞控软件烧写后，地面测发控系统发送飞控软件烧写控制（控制字为0x22），箭上响应后退出软件烧写状态，等待地面测发控系统其他控制指令；需要飞控软件重启时，地面测发控系统向飞控软件发送飞控软件烧写控制（控制字为0x33）。
        重发间隔时间100ms，若连续3次未接收到综控机反馈，则停止流程
        @retval
    **/
    QByteArray flightControlSoftwareUploadControlRequest();
   

    /**
        @brief  飞控软件数据上传
        飞控软件进入软件烧写状态后，接收并响应地面测发控系统发送的飞控软件数据，地面测发控系统依据可执行程序进行数据分包，计算所需软件总包数，并开始上传第一包，包序号从0号开始，直至最后一包，每包有效数据最多为250个字节。重发间隔时间100ms，若连续3次未接收到综控机反馈，则停止流程
        @retval
    **/
    QByteArray flightControlSoftwareDataUploadRequest();
   
    /**
        @brief  飞控软件数据校验
        地面测发控系统可发送飞控软件数据校验下载指令，其中包含飞控软件的CRC，飞控软件接收到指令后会读取DDR中上传的可执行程序软件，并计算校验和，如果计算的CRC和飞控软件的CRC相同，则将DDR中的飞控软件程序下载到Flash中，校验和下载的过程估计会持续40s左右，地面建议设置1分钟的超时判断，综控机下载完后将下载结果反馈给地面测发控系统。在校验下载过程中，飞控功能软件可能无法响应地面指令。
        飞控软件完成校验后，反馈给地面测发控系统
        @retval
    **/
    QByteArray flightControlSoftwareDataCheckRequest();
    
    /**
        @brief  发射预令
        地面测发控设备给飞控机发射预令，飞控机接收正常后，而后地面测发控系统才可向箭上发送发射主令，否则箭上不予以执行发射点火指令
        @retval
    **/
    QByteArray preLaunchCommandRequest();

    /**
        @brief  发射点火主令
        地面测发控设备连续发送3次给飞控机发射主令，飞控机接收正常后，进入正式发射流程（进行开栓点火）。发射主令只在飞控接收到发射预令后，方可响应
        @retval 请求数据
    **/
    QByteArray launchFireCommandRequest();
    /**
        @brief 安控预令
        在飞行异常情况下，地面测发控设备向综控机发送安控预令，飞控机接收后，进入安控流程。安控动令只在飞控接收到安控预令后，方可响应
        @retval 请求数据
    **/
    QByteArray safeControlPreCommandRequest();
    /**
        @brief 安控动令
        在飞行异常情况下，地面测发控设备连续3次向综控机发送安控预令，飞控机接收判定通过后，进入安控流程。安控动令只在飞控接收到安控预令后，方可响应
        @retval 请求数据
    **/
    QByteArray safeControlActivateCommandRequest();

    private:
        RocketGroundProtolDataFormat* m_requestData;
        QByteArray RocketFormat2ByteArray(RocketGroundProtolDataFormat* data);

};




#endif