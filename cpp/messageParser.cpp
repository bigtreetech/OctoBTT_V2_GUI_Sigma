#include "./headers/messageParser.h"

/// Find_List_Name : Retrieved List Sequence --> [Grandma,Mother,Me]
/// Support wildcard *
/// _QObj:Retrieved Json Objecet
QJsonValue MessageParser::searchJsonValue(QList<QString> Find_List_Name , QJsonObject _QObj)
{
    QJsonValue returnValue;
    QList<QString> SearchLink ;
    SearchLink << Find_List_Name;
    if(SearchLink.count() > 1)
    {
        QString MotherKey = SearchLink[0];
        SearchLink.removeFirst();
        if(MotherKey.indexOf("*") >= 0)//wildcard
        {
            QJsonArray ArrayValue;
            for(int i = 0 ; i <= 9 ; i++)
            {
                QString TempMotherKey = MotherKey;
                if(!_QObj.contains(TempMotherKey.replace("*",QString::number(i))))continue;
                QJsonValue data = searchJsonValue(SearchLink , _QObj.constFind(TempMotherKey.replace("*",QString::number(i))).value().toObject());
                ArrayValue.append(data);
            }
            returnValue = ArrayValue;
        }
        else
            returnValue = searchJsonValue(SearchLink , _QObj.constFind(MotherKey).value().toObject());
    }
    else
    {
        if(SearchLink[0].indexOf("*") >= 0)//wildcard
        {
            QJsonArray ArrayValue;
            for(int i = 0 ; i <= 9 ; i++)
            {
                QString TempMotherKey = SearchLink[0];
                if(!_QObj.contains(TempMotherKey.replace("*",QString::number(i))))continue;
                QJsonValue data = searchJsonValue(SearchLink , _QObj.constFind(TempMotherKey.replace("*",QString::number(i))).value().toObject());
                ArrayValue.append(data);
            }
            returnValue = ArrayValue;
        }
        else
            returnValue = _QObj.constFind(SearchLink[0]).value();
    }
    return returnValue;
}
/// Find_List_Name : Retrieved List Sequence -->
/// [
/// [Grandma_0,Mother_0,Me_0], <-- reference base unit
/// [Grandma_1,Mother_1,Me_1],
/// [Mother_2,Me_2]
/// ]
/// _QObj:Retrieved Json List Objecet
QList<QJsonValue> MessageParser::searchJsonValue(QList<QList<QString>> Find_List_Names , QJsonObject _QObj)
{
    QList<QJsonValue> _returnValue ;
    foreach(QList<QString> item , Find_List_Names)
    {
        _returnValue.append(searchJsonValue(item,_QObj));
    }
    return _returnValue;
}

/// Find_List_Name : Retrieved List Sequence --> [Grandma,Mother,Me]
/// Support wildcard *
/// _QObj:Retrieved Json Objecet
bool MessageParser::searchJsonKey(QList<QString> Find_List_Name , QJsonObject _QObj)
{
    bool returnValue = true;
    QList<QString> SearchLink ;
    SearchLink << Find_List_Name;
    if(SearchLink.count() > 1)
    {
        if(!_QObj.contains(SearchLink[0]))
            return false;
        QString MotherKey = SearchLink[0];
        SearchLink.removeFirst();
        if(MotherKey.indexOf("*") >= 0)//wildcard
        {
            bool tempbool = false;
            for(int i = 0 ; i <= 9 ; i++)
            {
                QString TempMotherKey = MotherKey;
                if(!_QObj.contains(TempMotherKey.replace("*",QString::number(i))))continue;
                bool data = searchJsonKey(SearchLink , _QObj.constFind(TempMotherKey.replace("*",QString::number(i))).value().toObject());
                tempbool |= data;
            }
            returnValue &= tempbool;
        }
        else
            returnValue &= searchJsonKey(SearchLink , _QObj.constFind(MotherKey).value().toObject());
    }
    else
    {
        if(SearchLink[0].indexOf("*") >= 0)//wildcard
        {
            bool tempbool = false;
            for(int i = 0 ; i <= 9 ; i++)
            {
                QString TempMotherKey = SearchLink[0];
                if(!_QObj.contains(TempMotherKey.replace("*",QString::number(i))))continue;
                bool data = searchJsonKey(SearchLink , _QObj.constFind(TempMotherKey.replace("*",QString::number(i))).value().toObject());
                tempbool |= data;
            }
            returnValue &= tempbool;
        }
        else
            returnValue &= _QObj.contains(SearchLink[0]);
    }
    return returnValue;
}
/// Find_List_Name : Retrieved List Sequence -->
/// [
/// [Grandma_0,Mother_0,Me_0], <-- reference base unit
/// [Grandma_1,Mother_1,Me_1],
/// [Mother_2,Me_2]
/// ]
/// _QObj:Retrieved Json List Objecet
QList<bool> MessageParser::searchJsonKey(QList<QList<QString>> Find_List_Names , QJsonObject _QObj)
{
    QList<bool> _returnValue ;
    foreach(QList<QString> item , Find_List_Names)
    {
        _returnValue.append(searchJsonKey(item,_QObj));
    }
    return _returnValue;
}


/// Find_List_Name : Retrieved List Sequence --> [Grandma,Mother,Me]
/// Support wildcard *
/// _QObj:Retrieved Json Objecet
QJsonValue MessageParser::searchJsonValueWithCheck(QList<QString> Find_List_Name , QJsonObject _QObj)
{
    if(!searchJsonKey(Find_List_Name , _QObj))
        return
                QJsonValue();
    else
        return searchJsonValue(Find_List_Name , _QObj);
}
/// Find_List_Name : Retrieved List Sequence -->
/// [
/// [Grandma_0,Mother_0,Me_0], <-- reference base unit
/// [Grandma_1,Mother_1,Me_1],
/// [Mother_2,Me_2]
/// ]
/// _QObj:Retrieved Json List Objecet
QList<QJsonValue> MessageParser::searchJsonValueWithCheck(QList<QList<QString>> Find_List_Names , QJsonObject _QObj)
{
    QList<QJsonValue> _returnValue ;
    foreach(QList<QString> item , Find_List_Names)
    {
        _returnValue.append(searchJsonKey(item , _QObj) ? searchJsonValue(item,_QObj) :
                                                          QJsonValue());
    }
    return _returnValue;
}
