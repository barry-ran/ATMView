﻿#include "DataModelReader.h"
#include "UserConfig.h"

DataModelReader* DataModelReader::m_pInstance = NULL;

DataModelReader *DataModelReader::instance()
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new DataModelReader;
    }
    return m_pInstance;
}

QVariantMap DataModelReader::getInterfaceHead() const
{
    QVariantMap map;
    map.clear();

    map.insert("sessionId", m_interfaceInputHead.sessionId);
    map.insert("terminalId", m_interfaceInputHead.terminalId);
    map.insert("terminalType", m_interfaceInputHead.terminalType);
    map.insert("terminalVersion", m_interfaceInputHead.terminalVersion);
    map.insert("sysType", m_interfaceInputHead.sysType);
    map.insert("sysVersion", m_interfaceInputHead.sysVersion);
    map.insert("userId", m_interfaceInputHead.userId);
    map.insert("portalVersion", m_interfaceInputHead.portalVersion);
    map.insert("invokeTime", m_interfaceInputHead.invokeTime);

    return map;
}

QVariantMap DataModelReader::getOperatorInfo() const
{
    QVariantMap map;
    map.clear();

    map.insert("userId", m_operatorInfo.userId);
    map.insert("passwd", m_operatorInfo.passwd);
    map.insert("userName", m_operatorInfo.userName);
    map.insert("idCard", m_operatorInfo.idCard);
    map.insert("type", m_operatorInfo.type);

    return map;
}

QVariantMap DataModelReader::getHeartbeatInfo() const
{
    QVariantMap map;
    map.clear();

    map.insert("offlineCount", m_heartbeatData.offlineCount);

    return map;
}

QVariantMap DataModelReader::getModelStatus() const
{
    QVariantMap map;
    map.clear();

    map.insert("visModelStatus", m_modelStatus.visModelStatus);
    map.insert("nirModelStatus", m_modelStatus.nirModelStatus);
    map.insert("faceModelStatus", m_modelStatus.faceModelStatus);
    map.insert("fveinModelStatus", m_modelStatus.fveinModelStatus);
    map.insert("fprintModelStatus", m_modelStatus.fprintModelStatus);

    return map;
}

void DataModelReader::setInterfaceHead(QVariantMap map)
{
    m_interfaceInputHead.sessionId = map.value("sessionId").toString();
    m_interfaceInputHead.terminalId = map.value("terminalId").toString();
    m_interfaceInputHead.terminalType = map.value("terminalType").toString();
    m_interfaceInputHead.terminalVersion = map.value("terminalVersion").toString();
    m_interfaceInputHead.sysType = map.value("sysType").toString();
    m_interfaceInputHead.sysVersion = map.value("sysVersion").toString();
    m_interfaceInputHead.userId = map.value("userId").toString();
    m_interfaceInputHead.portalVersion = map.value("portalVersion").toString();
    m_interfaceInputHead.invokeTime = map.value("invokeTime").toString();
}

void DataModelReader::setOperatorInfo(QVariantMap map)
{
    m_operatorInfo.userId = map.value("userId").toString();
    m_operatorInfo.passwd = map.value("passwd").toString();
    m_operatorInfo.userName = map.value("userName").toString();
    m_operatorInfo.idCard = map.value("idCard").toString();
    m_operatorInfo.type = map.value("type").toString();
}

void DataModelReader::setHeartbeatData(QVariantMap map)
{
    m_heartbeatData.offlineCount = map.value("offlineCount").toInt();
}

void DataModelReader::setModelStatus(QVariantMap map)
{
    m_modelStatus.visModelStatus = map.value("visModelStatus").toInt();
    m_modelStatus.nirModelStatus = map.value("nirModelStatus").toInt();
    m_modelStatus.faceModelStatus = map.value("faceModelStatus").toInt();
    m_modelStatus.fveinModelStatus = map.value("fveinModelStatus").toInt();
    m_modelStatus.fprintModelStatus = map.value("fprintModelStatus").toInt();
}

InterfaceInputHead &DataModelReader::interfaceHead()
{
    return m_interfaceInputHead;
}

OperatorInfo &DataModelReader::operatorInfo()
{
    return m_operatorInfo;
}

HeartbeatData &DataModelReader::heartbeatData()
{
    return m_heartbeatData;
}

UserBaseInfo &DataModelReader::userBaseInfo()
{
    return m_userBaseInfo;
}

IdCardPicData &DataModelReader::idCardPicData()
{
    return m_idCardPicData;
}

void DataModelReader::setInterfaceHead(InterfaceInputHead interfaceHead)
{
    m_interfaceInputHead = interfaceHead;
}

void DataModelReader::setOperatorInfo(OperatorInfo operatorInfo)
{
    m_operatorInfo = operatorInfo;
}

void DataModelReader::setHeartbeatData(HeartbeatData heartbeatData)
{
    m_heartbeatData = heartbeatData;
}

void DataModelReader::setUserBaseInfo(UserBaseInfo userBaseInfo)
{
    m_userBaseInfo = userBaseInfo;
}

void DataModelReader::setIdCardPicData(IdCardPicData idCardPicData)
{
    m_idCardPicData = idCardPicData;
}

void DataModelReader::setModelStatus(ModelStatus modelStatus)
{
    m_modelStatus = modelStatus;
}

DataModelReader::DataModelReader(QObject *parent) : QObject(parent)
{
    INSTANCE_USER_CONFIG->getOperatorInfo(m_operatorInfo.userId, m_operatorInfo.passwd);

    m_interfaceInputHead.userId = m_operatorInfo.userId;
    m_interfaceInputHead.terminalVersion = INSTANCE_USER_CONFIG->getVersion();
    m_interfaceInputHead.terminalId = INSTANCE_USER_CONFIG->getMac();
}
