#include <cstddef>
#include "Base.h"
#include <iconv.h>
#include <iomanip>
#include <stdio.h>

void Encoder::HtoN(void *p, size_t size, size_t byteCount)
{
    if(size % byteCount != 0)
        return;
    switch (byteCount)
    {
        case 1:
            break;
        case 2:
            for(std::size_t index = 0;index < size / byteCount;++index)
            {
                *(static_cast<UInt16*>(p)+index) = htons(*(static_cast<UInt16*>(p)+index));
            }
            break;
        case 4:
            for(std::size_t index = 0;index < size / byteCount;++index)
            {
                *(static_cast<UInt32*>(p)+index) = htonl(*(static_cast<UInt32*>(p)+index));
            }
            break;
        case 8:
        {
            for(std::size_t index = 0;index < size / byteCount;++index)
            {
                *(static_cast<UInt64*>(p) + index) = htonll(*(static_cast<UInt64*>(p) + index));
            }
        }
            break;
        default:
            break;
    }
}

void Encoder::NtoH(void *p, size_t size, size_t byteCount)
{
    if(size % byteCount != 0)
        return;
    switch (byteCount)
    {
        case 1:
            break;
        case 2:
            for(std::size_t index = 0;index < size / byteCount;++index)
            {
                *(static_cast<UInt16*>(p)+index) = ntohs(*(static_cast<UInt16*>(p)+index));
            }
            break;
        case 4:
            for(std::size_t index = 0;index < size / byteCount;++index)
            {
                *(static_cast<UInt32*>(p)+index) = ntohl(*(static_cast<UInt32*>(p)+index));
            }
            break;
        case 8:
        {
            for(std::size_t index = 0;index < size / byteCount;++index)
            {
                *(static_cast<UInt64*>(p) + index) = ntohll(*(static_cast<UInt64*>(p) + index));
            }
        }
            break;
        default:
            break;
    }
}


void TranscodOut(char *inbuf,char *outbuf,const char *tocode, const char *fromcode)
{
    size_t len = strlen(inbuf);
    size_t outlen = 50;
    iconv_t cd = iconv_open(tocode,fromcode);
    if(cd == (iconv_t)-1)
    {
        LOG_WARN << "Conversion Failed";
        exit(3);
    }
    iconv(cd,&inbuf,&len,&outbuf,&outlen);//将转换后的文件放入缓存区
    iconv_close(cd);
}

std::string ToHexStr(const char* value,int size)
{
    assert(value != NULL);
    std::string str(value,value + 4);
    if(!str.compare(std::string("null")))
    {
        return std::string("null");
    }
    else
    {
        std::ostringstream os;
        for(int i = 0;i < size;++i)
        {
            os << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (int)value[i];
        }
        return os.str();
    }
}


std::string ToString(const char* value,int size)
{
    assert(value != NULL);
    std::string str(value,value + 4);
    if(!str.compare(std::string("null")))
    {
        return std::string("null");
    }
    else
    {
        return std::string(value,value + size);
    }
}
