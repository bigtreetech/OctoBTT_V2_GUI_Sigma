#include "../headers/machineprofile.h"

MachineProfile::MachineProfile(QObject *parent) : QObject(parent)
{
}

void MachineProfile::getMachineProfileInfo()
{
    APIDisposeThread *newThread = new APIDisposeThread("get", "printerprofiles/_default", globalAccess->userinfo->cookies, QJsonObject(), globalAccess->MainUrl() + "printerprofiles/_default");
    APIDisposeThread::runNewThread(newThread, "printerprofiles/_default", false);
    //接收线程返回的信息
    connect(newThread, &APIDisposeThread::sentRequest, this, [=](int _statusCode, QJsonObject _QObj, bool _requestState){
        Q_UNUSED(_statusCode)
        Q_UNUSED(_requestState)
        emit machineProfile_Json(QJsonDocument(_QObj).toJson());
        QObj = _QObj;
        //对接收的Json包解析, 提取出各项机器参数信息
        if(_QObj.contains("axes"))
        {
            QJsonValue axesValue = _QObj.value("axes");
            if(axesValue.isObject())
            {
                QJsonObject axesObject = axesValue.toObject();
                if(axesObject.contains("e"))
                {
                    QJsonValue eValue = axesObject.value("e");
                    if(eValue.isObject())
                    {
                        QJsonObject eObject = eValue.toObject();
                        if(eObject.contains("inverted"))
                        {
                            QJsonValue invertedValue = eObject.value("inverted");
                            if(invertedValue.isBool())
                            {
                                MechanicalParameter_default.inverted_e = invertedValue.toBool();
                            }
                        }
                        if(eObject.contains("speed"))
                        {
                            QJsonValue speedValue = eObject.value("speed");
                            if(speedValue.isDouble())
                            {
                                MechanicalParameter_default.speed_e = speedValue.toDouble();
                            }
                        }
                    }
                }
                if(axesObject.contains("z"))
                {
                    QJsonValue eValue = axesObject.value("z");
                    if(eValue.isObject())
                    {
                        QJsonObject eObject = eValue.toObject();
                        if(eObject.contains("inverted"))
                        {
                            QJsonValue invertedValue = eObject.value("inverted");
                            if(invertedValue.isBool())
                            {
                                MechanicalParameter_default.inverted_z = invertedValue.toBool();
                            }
                        }
                        if(eObject.contains("speed"))
                        {
                            QJsonValue speedValue = eObject.value("speed");
                            if(speedValue.isDouble())
                            {
                                MechanicalParameter_default.speed_z = speedValue.toDouble();
                            }
                        }
                    }
                }
                if(axesObject.contains("y"))
                {
                    QJsonValue eValue = axesObject.value("y");
                    if(eValue.isObject())
                    {
                        QJsonObject eObject = eValue.toObject();
                        if(eObject.contains("inverted"))
                        {
                            QJsonValue invertedValue = eObject.value("inverted");
                            if(invertedValue.isBool())
                            {
                                MechanicalParameter_default.inverted_y = invertedValue.toBool();
                            }
                        }
                        if(eObject.contains("speed"))
                        {
                            QJsonValue speedValue = eObject.value("speed");
                            if(speedValue.isDouble())
                            {
                                MechanicalParameter_default.speed_y = speedValue.toDouble();
                            }
                        }
                    }
                }
                if(axesObject.contains("x"))
                {
                    QJsonValue eValue = axesObject.value("x");
                    if(eValue.isObject())
                    {
                        QJsonObject eObject = eValue.toObject();
                        if(eObject.contains("inverted"))
                        {
                            QJsonValue invertedValue = eObject.value("inverted");
                            if(invertedValue.isBool())
                            {
                                MechanicalParameter_default.inverted_x = invertedValue.toBool();
                            }
                        }
                        if(eObject.contains("speed"))
                        {
                            QJsonValue speedValue = eObject.value("speed");
                            if(speedValue.isDouble())
                            {
                                MechanicalParameter_default.speed_x = speedValue.toDouble();
                            }
                        }
                    }
                }
            }
        }
        if(_QObj.contains("model"))
        {
            QJsonValue modelValue = _QObj.value("model");
            if(modelValue.isString())
            {
                MechanicalParameter_default.model = modelValue.toString();
            }
        }
        if(_QObj.contains("volume"))
        {
            QJsonValue volumeValue = _QObj.value("volume");
            if(volumeValue.isObject())
            {
                QJsonObject volumeObject = volumeValue.toObject();
                if(volumeObject.contains("width"))
                {
                    QJsonValue widthValue = volumeObject.value("width");
                    if(widthValue.isDouble())
                    {
                        MechanicalParameter_default.width = widthValue.toDouble();
                    }
                }
                if(volumeObject.contains("depth"))
                {
                    QJsonValue depthValue = volumeObject.value("depth");
                    if(depthValue.isDouble())
                    {
                        MechanicalParameter_default.depth = depthValue.toDouble();
                    }
                }
                if(volumeObject.contains("height"))
                {
                    QJsonValue heightValue = volumeObject.value("height");
                    if(heightValue.isDouble())
                    {
                        MechanicalParameter_default.height = heightValue.toDouble();
                    }
                }
                if(volumeObject.contains("origin"))
                {
                    QJsonValue originValue = volumeObject.value("origin");
                    if(originValue.isString())
                    {
                        MechanicalParameter_default.origin = originValue.toString();
                    }
                }
            }
        }
        if(_QObj.contains("extruder"))
        {
            QJsonValue extruderValue = _QObj.value("extruder");
            if(extruderValue.isObject())
            {
                QJsonObject extruderObject =  extruderValue.toObject();
                if(extruderObject.contains("nozzleDiameter"))
                {
                    QJsonValue nozzleDiameterValue = extruderObject.value("nozzleDiameter");
                    if(nozzleDiameterValue.isDouble())
                    {
                        MechanicalParameter_default.nozzleDiameter = nozzleDiameterValue.toDouble();
                    }
                }
            }
        }
        //信号无法直接抛出结构体, 需要先用QVariant进行封包
        QVariant tempVar;
        tempVar.setValue(MechanicalParameter_default);
        emit machineProfileInfo(tempVar);
    });
}

void MachineProfile::postMachineProfile(MechanicalParameter profileInfo)
{
    //profile必须至少存在一个
    //生成一个临时profile
    QJsonObject tempPostObject = QObj;
    QJsonValueRef tempPostRef_1 = tempPostObject.find("id").value();
    tempPostRef_1 = QJsonValue("tempDefault");

    QJsonObject tempPostProfileObject;
    tempPostProfileObject.insert("profile", tempPostObject);

    APIDisposeThread *postTempThread = new APIDisposeThread("post_0", "printerprofiles", globalAccess->userinfo->cookies, tempPostProfileObject, globalAccess->MainUrl() + "printerprofiles");
    APIDisposeThread::runNewThread(postTempThread, "printerprofiles", false);

    connect(postTempThread, &APIDisposeThread::sentRequest, this, [=](){
        //删除旧的默认profile
        APIDisposeThread *delFaultThread = new APIDisposeThread("deleteResource", "printerprofiles/_default", globalAccess->userinfo->cookies, QJsonObject (), globalAccess->MainUrl() + "printerprofiles/_default");
        APIDisposeThread::runNewThread(delFaultThread, "printerprofiles/_default", false);
        connect(delFaultThread, &APIDisposeThread::sentRequest, this, [=](){
            //构建新的默认profile
            QJsonValueRef tempRef_0 = QObj.find("model").value();
            tempRef_0 = QJsonValue(profileInfo.model);

            QJsonValueRef tempRef_1 = QObj.find("id").value();
            tempRef_1 = QJsonValue("_default");

            QJsonObject eObjcet;
            eObjcet.insert("inverted", profileInfo.inverted_e);
            eObjcet.insert("speed", profileInfo.speed_e);

            QJsonObject zObjcet;
            zObjcet.insert("inverted", profileInfo.inverted_z);
            zObjcet.insert("speed", profileInfo.speed_z);

            QJsonObject yObjcet;
            yObjcet.insert("inverted", profileInfo.inverted_y);
            yObjcet.insert("speed", profileInfo.speed_y);

            QJsonObject xObjcet;
            xObjcet.insert("inverted", profileInfo.inverted_x);
            xObjcet.insert("speed", profileInfo.speed_x);

            QJsonObject axesObject;
            axesObject.insert("e", eObjcet);
            axesObject.insert("z", zObjcet);
            axesObject.insert("y", yObjcet);
            axesObject.insert("x", xObjcet);
            QObj.insert("axes", axesObject);

            QJsonValueRef tempRef_2 = QObj.find("extruder").value();
            QJsonObject tempObj_0 = tempRef_2.toObject();
            tempObj_0["nozzleDiameter"] = profileInfo.nozzleDiameter;
            tempRef_2 = tempObj_0;

            QJsonValueRef tempRef_3 = QObj.find("volume").value();
            QJsonObject tempObj_1 = tempRef_3.toObject();
            tempObj_1["depth"] = profileInfo.depth;
            tempRef_3 = tempObj_1;

            QJsonValueRef tempRef_4 = QObj.find("volume").value();
            QJsonObject tempObj_2 = tempRef_4.toObject();
            tempObj_2["width"] = profileInfo.width;
            tempRef_4 = tempObj_2;

            QJsonValueRef tempRef_5 = QObj.find("volume").value();
            QJsonObject tempObj_3 = tempRef_5.toObject();
            tempObj_3["height"] = profileInfo.height;
            tempRef_5 = tempObj_3;

            QJsonValueRef tempRef_6 = QObj.find("volume").value();
            QJsonObject tempObj_4 = tempRef_6.toObject();
            tempObj_4["origin"] = profileInfo.origin;
            tempRef_6 = tempObj_4;

            QObj.remove("resource");

            QJsonObject profileObject;
            profileObject.insert("profile", QObj);

            //生成新的profile
            APIDisposeThread *newThread_1 = new APIDisposeThread("post_0", "printerprofiles", globalAccess->userinfo->cookies, profileObject, globalAccess->MainUrl() + "printerprofiles");
            APIDisposeThread::runNewThread(newThread_1, "printerprofiles", false);

            //删除临时的profile
            connect(newThread_1, &APIDisposeThread::sentRequest, this, [=](){
                APIDisposeThread *delTempThread = new APIDisposeThread("deleteResource", "printerprofiles/tempDefault", globalAccess->userinfo->cookies, QJsonObject (), globalAccess->MainUrl() + "printerprofiles/tempDefault");
                APIDisposeThread::runNewThread(delTempThread, "printerprofiles/tempDefault", false);
            });
        });
    });
}


