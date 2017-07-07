/*
 * STGPSProtocol.cpp
 *
 *  Created on: 2017年6月30日
 *      Author: wong
 */

#include "STGPSProtocolAPI.h"
#include "Base.h"

SZGPSPROTOCOL_API int STGPS_ParseCarBasicInfo(char* buf, int *bufLen, CarInfo* value)
{
    ASSERT_BEGIN(buf,bufLen,value);
    const char* begin = buf;
    const char* end = buf + *bufLen;
    DECODEARR(value->vendorId,10,&buf,end - buf);
    DECODEARR(value->factoryId,10,&buf,end - buf);
    DECODEARR(value->deviceId,10,&buf,end - buf);
    DECODEARR(value->commNo,11,&buf,end - buf);
    ASSERT_END(buf,end);
    *bufLen = static_cast<std::size_t>(buf - begin);
    return CCMS_RETURN_OK;
}

SZGPSPROTOCOL_API int STGPS_ParseGPSInfo(char* buf, int* len, int packLen, GPSInfo* value)
{
    if((int)(*len) < 56)
    {
        LOG_WARN << "invalid len = " << *len << " bodyLen = " << sizeof(value) - 8;
        return CCMS_ERRORMSG_FAIL;
    }
    const char* begin = buf;
    const char* end = buf + *len;
    Byte2Hex(value->time.nYear,&buf,2,end - buf);
    Byte2Hex(value->time.nMonth,&buf,1,end - buf);
    Byte2Hex(value->time.nDay,&buf,1,end - buf);
    Byte2Hex(value->time.nHour,&buf,1,end - buf);
    Byte2Hex(value->time.nMinute,&buf,1,end - buf);
    Byte2Hex(value->time.nSecond,&buf,1,end - buf);
    DECODE(&value->latitude,&buf,end - buf,true);
    DECODE(&value->longitude,&buf,end - buf,true);
    DECODE(&value->speed,&buf,end - buf,true);
    DECODE(&value->direction,&buf,end - buf,true);
    DECODE(&value->altitude,&buf,end - buf,true);
    DECODE(&value->mileage,&buf,end - buf,true);
    char DriverName[12];
    DECODEARR(DriverName,12,&buf,end - buf);
    std::string null("null");
    if(DriverName[0] == 0x00)
    {
        memcpy(&value->driverName,null.c_str(),null.length());
    }
    else
    {
        TranscodOut(DriverName, value->driverName,"utf-8", "gbk");
    }
    char DriverId[18];
    DECODEARR(DriverId,18,&buf,end - buf);
    if(DriverId[0] == 0x00)
    {
        memcpy(&value->driverId,null.c_str(),null.length());
    }
    else
    {
        TranscodOut(DriverId, value->driverId,"utf-8", "gbk");
    }
    DECODE(&value->valid,&buf,end - buf,false);
    if((int)packLen >= 64)
    {
        DECODEARR(value->state,8,&buf,end - buf);
    }
    else
    {
        memcpy(&value->state,null.c_str(),null.length());
    }
    ASSERT_END(buf,end);
    *len = static_cast<std::size_t>(buf - begin);
    return CCMS_RETURN_OK;
}

SZGPSPROTOCOL_API int STGPS_PackCarBasicInfo(char* buf, int *bufLen, CarInfo* value)
{
    ASSERT_BEGIN(buf,bufLen,value);
    const char* begin = buf;
    const char* end = buf + *bufLen;
    ENCODEARR(value->vendorId,10,&buf,end - buf);
    ENCODEARR(value->factoryId,10,&buf,end - buf);
    ENCODEARR(value->deviceId,10,&buf,end - buf);
    ENCODEARR(value->commNo,11,&buf,end - buf);
    ASSERT_END(buf,end);
    *bufLen = static_cast<std::size_t>(buf - begin);
    return CCMS_RETURN_OK;
}

SZGPSPROTOCOL_API int STGPS_PackGPSInfo(char* buf, int* len, int packLen,GPSInfo* value)
{
    ASSERT_BEGIN(buf,len,value);
    const char* begin = buf;
    const char* end = buf + *len;
    ENCODE(&value->time.nYear,&buf,end - buf,true);
    ENCODE(&value->time.nMonth,&buf,end - buf, false);
    ENCODE(&value->time.nDay,&buf,end - buf,false);
    ENCODE(&value->time.nHour,&buf,end - buf,false);
    ENCODE(&value->time.nMinute,&buf,end - buf,false);
    ENCODE(&value->time.nSecond,&buf,end - buf,false);
    ENCODE(&value->latitude,&buf,end - buf,true);
    ENCODE(&value->longitude,&buf,end - buf,true);
    ENCODE(&value->speed,&buf,end - buf,true);
    ENCODE(&value->direction,&buf,end - buf,true);
    ENCODE(&value->altitude,&buf,end - buf,true);
    ENCODE(&value->mileage,&buf,end - buf,true);
    char DriverName[12];
    std::string null('0',18);
    if(value->driverName[0] == 0)
    {
        memcpy(DriverName,null.c_str(),12);
    }
    else
    {
        TranscodOut(value->driverName, DriverName, "gbk","utf-8");
    }
    ENCODEARR(DriverName,12,&buf,end - buf);
    char DriverID[18];
    if(value->driverId[0] == 0)
    {
        memcpy(DriverID,null.c_str(),null.length());
    }
    else
    {
        TranscodOut(value->driverId, DriverID, "gbk","utf-8");
    }
    ENCODEARR(DriverID,18,&buf,end - buf);
    ENCODE(&value->valid,&buf,end - buf,false);
    if(packLen  >= 64)
    {
        ENCODEARR(value->state,8,&buf,end - buf);
    }
    else
    {
        memcpy(&buf,null.c_str(),8);
    }
    ASSERT_END(buf,end);
    *len = static_cast<std::size_t>(buf - begin);
    return CCMS_RETURN_OK;
}

SZGPSPROTOCOL_API int STGPS_ParseHead(char* buf, int* len, Head* value)
{
    ASSERT_BEGIN(buf,len,value);
    const char* begin = buf;
    const char* end = buf + *len;
    DECODE(&value->check,&buf,end - buf,true);
    DECODE(&value->packLen,&buf,end - buf,true);
    DECODE(&value->cityCode,&buf,end - buf, false);
    DECODE(&value->cmdId,&buf,end - buf,true);
    char temp[8];
    DECODEARR(temp,8,&buf,end - buf);
    TranscodOut(temp, value->vehileId,"utf-8", "gbk");
    DECODE(&value->nonsense,&buf,end - buf,true);
    DECODE(&value->vehileColor,&buf,end - buf, false);
    DECODE(&value->cmdLength,&buf,end - buf,true);
    ASSERT_END(buf,end);
    *len = static_cast<std::size_t>(buf - begin);
    return CCMS_RETURN_OK;
}



