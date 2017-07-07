//
// Created by wangqi on 17-6-30.
//

#ifndef STGPSPROTOCOL_BASE_H_H
#define STGPSPROTOCOL_BASE_H_H

#include <netinet/in.h>
#include "Common.h"
#include "CCMS.h"
#include "base/Logging.h"
#include <vector>
#include <iostream>
#include <sstream>

#define ASSERT_BEGIN(buf,bufLen,value)\
        if(buf == NULL || bufLen == NULL) \
        { \
             LOG_WARN << "invalid param!";\
             return CCMS_ERRORMSG_INVALIDPARAM;\
        } \
        if((int)(*bufLen) < (int)sizeof(value))\
        {\
            LOG_WARN << "invalid len = " << *bufLen << " bodyLen = " << sizeof(value);\
            return CCMS_ERRORMSG_FAIL;\
        }

#define ASSERT_END(buf,end)\
        if (buf > end)\
        {\
             LOG_WARN << "wrong pBuf = " << buf << " pEnd = " << end;\
             return CCMS_ERRORMSG_FAIL;\
        }


class Encoder {
public:
    static void HtoN(void* p, size_t size, size_t byteCount);
    static void NtoH(void* p, size_t size, size_t byteCount);
private:
    static uint64_t htonll(uint64_t n)
    {
        return (((uint64_t)htonl(n)) << 32) | htonl(n >> 32);
    }
    static uint64_t ntohll(uint64_t n)
    {
        return (((uint64_t)ntohl(n)) << 32) | ntohl(n >> 32);
    }
};



template <typename T>
std::size_t DECODE(T* data,char** buf,std::size_t bufLen,bool changeByteOrder)
{
    assert((data != NULL) || (buf != NULL) && (*buf != NULL) && (bufLen > 0) && (bufLen< 65536));
    const std::size_t bytes = sizeof(T);
    memcpy(data,*buf,bytes);
    (*buf) += bytes;
    if (changeByteOrder)
        Encoder::NtoH(data, bytes, bytes);
    return CCMS_RETURN_OK;
}

template <typename T>
std::size_t DECODEARR(T* obj,std::size_t size,char** buf, std::size_t bufLen)
{
    assert((obj != NULL) && (buf != NULL) && (*buf != NULL) && (size <= bufLen) && (bufLen > 0) && (bufLen< 65536));
    memcpy(obj,*buf,size);
    (*buf) += size;
    return CCMS_RETURN_OK;
}


template <typename T>
std::size_t ENCODE(T* data,char** buf,std::size_t bufLen,bool changeByteOrder)
{
    assert((data != NULL) || (buf != NULL) && (*buf != NULL) && (bufLen > 0) && (bufLen< 65536));
    const std::size_t bytes = sizeof(T);
    memcpy(*buf,data,bytes);
    if (changeByteOrder)
        Encoder::HtoN(*buf,bytes, bytes);
    (*buf) += bytes;
    return CCMS_RETURN_OK;
}

template <typename T>
std::size_t ENCODEARR(T* obj,std::size_t size,char** buf, std::size_t bufLen)
{
    assert((obj != NULL) && (buf != NULL) && (*buf != NULL) && (bufLen > 0) && (bufLen< 65536));
    std::copy(obj,obj + size,*buf);
    (*buf) += size;
    return CCMS_RETURN_OK;
}

template <typename T>
std::size_t Byte2Hex(T& obj,char** buf, std::size_t size,std::size_t bufLen)
{
    assert((buf != NULL) && (*buf != NULL) && (bufLen > 0) && (bufLen< 65536));
    int temp;
    std::string hex("0123456789abcdef");
    std::ostringstream os;
    for(int i = 0;i < size;++i)
    {
        os << hex.at(((*buf)[i] >> 4) & 0x0f);
        os << hex.at(((*buf)[i]) & 0x0f);
    }
    std::istringstream in(os.str());
    in >> temp;
    obj = temp;
    (*buf) += size;
    return CCMS_RETURN_OK;
}


void TranscodOut(char *inbuf,char *outbuf,const char *tocode, const char *fromcode);

std::string ToHexStr(const char* value,int size);

std::string ToString(const char* value,int size);

#endif //STGPSPROTOCOL_BASE_H_H
