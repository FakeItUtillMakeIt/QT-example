
#include "MutliThreadOp.h"


//
//FileRunnable::FileRunnable(QObject* parent) :QObject(parent), QRunnable() {
//
//	setAutoDelete(false);
//
//}
//
//FileRunnable::~FileRunnable() {
//
//}
//
//void FileRunnable::dealFile(QStringList fileList) {
//	dealFileList = fileList;
//}
//
////读取文件至AppCache里
//void FileRunnable::run() {
//
//
//}

WorkThread::WorkThread(QObject* parent) {

}

WorkThread::~WorkThread() {


}


void WorkThread::doWork() {

	//把设备的状态及相应参数的配置进行初始化
	QString initStat = QString("断电");
	for (Device* eachDev : AppCache::instance()->m_allDevice)
	{
		eachDev->m_sCurStatus = initStat.toStdString();
	}



	DeviceDBConfigInfo::getInstance()->readStatusInfoDB2UI();
	QString statusFilePath;
	auto curPath = QCoreApplication::applicationDirPath();
	statusFilePath = curPath + "/device/";
	//设备参数初始化  四个需要初始化的值
	for (auto ele = AppCache::instance()->m_allDeviceParam.begin(); ele != AppCache::instance()->m_allDeviceParam.end(); ele++)
	{
		//初始化状态不需要读取实时值列表
		/*在这里启动定时器刷数*/



		for (auto eachStat : AppCache::instance()->m_dev2DeviceStatusID[ele->second->m_deviceId])
		{
			QFile file(statusFilePath + QString::fromStdString(AppCache::instance()->m_allDeviceStatus[eachStat]->m_dataPath));
			auto ret = file.open(QIODevice::ReadOnly | QIODevice::Text);
			if (!ret)
			{
				continue;
			}
			int row = 1;
			QTextStream in(&file);
			in.setCodec("UTF-8");
			QStringList paramNameList;

			unordered_map<string, vector<double>> tempData;

			while (!in.atEnd())
			{
				QString lineData = in.readLine();
				if (row == 1)
				{
					paramNameList = lineData.split(QRegExp("[,]"));
					paramNameList.pop_back();
					row--;
				}
				auto lineDataList = lineData.split(QRegExp("[,]"));
				lineDataList.pop_back();
				//舍弃数据不足的行
				if (lineDataList.size() < paramNameList.size())
				{
					continue;
				}
				for (int i = 0; i < paramNameList.size(); i++)
				{

					tempData[Utils::UTF8ToGBK(paramNameList[i].toStdString().c_str())].push_back(lineDataList[i].toDouble());

				}
			}
			//状态id->状态名称
			//string tmpKey = DeviceDBConfigInfo::getInstance()->statusInfo[eachStat][1];
			//ele->second->m_statusVal[eachStat] = tempData;
			ele->second->m_statusRealData[AppCache::instance()->m_allDeviceStatus[eachStat]->m_statusName] = tempData[ele->second->m_subParameterName];
		}

		//ele->second->timer->start(50);

	}

	emit workFinished();
}