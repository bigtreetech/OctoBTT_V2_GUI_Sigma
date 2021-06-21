#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>

class MessageParser
{
    public:
    // 检索数据
    static QJsonValue searchJsonValue(QList<QString> Find_List_Name , QJsonObject _QObj);
    static QList<QJsonValue> searchJsonValue(QList<QList<QString>> Find_List_Names , QJsonObject _QObj);
    // 检索键是否存在
    static bool searchJsonKey(QList<QString> Find_List_Name , QJsonObject _QObj);
    static QList<bool> searchJsonKey(QList<QList<QString>> Find_List_Names , QJsonObject _QObj);
    // 检索数据带自检
    static QJsonValue searchJsonValueWithCheck(QList<QString> Find_List_Name , QJsonObject _QObj);
    static QList<QJsonValue> searchJsonValueWithCheck(QList<QList<QString>> Find_List_Names , QJsonObject _QObj);
};

#endif // MESSAGEPARSER_H
