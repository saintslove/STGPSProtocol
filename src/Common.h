
#include <memory>
#include <sstream>
#include <iomanip>


template <typename T, std::size_t N>
struct OctetString
{
    OctetString(): pData(std::move(make_shared_array(N))) {};
    inline std::shared_ptr<T> make_shared_array(std::size_t size)
    {
        return std::shared_ptr<T>(new T[size],std::default_delete<T[]>());
    }
    inline std::shared_ptr<OctetString<T,N> > make_shared()
    {
        return std::make_shared<OctetString<T,N> >(*this);
    }
    inline void memcpy(const void* source,size_t len)
    {
        assert(source != NULL && 0 < len <= N);
        ::memcpy(pData.get(),source,len);
    }
    void clear()
    {
        std::shared_ptr<T> temp = make_shared_array(N);
        pData.swap(temp);
    }
    std::string toString()
    {
        if(std::string(pData.get(),pData.get() + 4) == "null")
        {
            return std::string("null");
        }
        else
        {
            return std::string(pData.get(),pData.get() + size());
        }
    }
    std::string toHexString()
    {
        std::ostringstream os;
        for(int i = 0;i < N;++i)
        {
            os << std::hex << std::setw(2) << std::setfill('0') << (int)(*this)[i];
        }
        return os.str();
    }
    const T& operator[](int index) const { return pData.get()[index]; }
    T& operator[](int index)  { return pData.get()[index]; }
    const std::size_t size() const { return N*sizeof(T);}
    const std::size_t capacity() const { return N;};
    const T* get() const { return pData.get();}
private:
    std::shared_ptr<T> pData;
};







template <typename T>
std::size_t DECODE(T* data,char** buf,std::size_t bufLen,bool changeByteOrder)
{
    assert(data != NULL || buf != NULL && *buf != NULL && 0 < bufLen < 65536);
    const std::size_t bytes = sizeof(T);
    memcpy(data,*buf,bytes);
    (*buf) += bytes;
    if (changeByteOrder)
        Encoder::NtoH(data, bytes, bytes);
    return CCMS_RETURN_OK;
}

template <std::size_t size>
std::size_t DECODEOCT(std::shared_ptr<OctetString<char,size> > octet,char** buf, std::size_t bufLen)
{
    assert(octet != NULL && buf != NULL && *buf != NULL && 0 < bufLen < 65536);
    (*octet).memcpy(*buf,size);
    (*buf) += size;
    return CCMS_RETURN_OK;
}


template <typename T>
std::size_t ENCODE(T* data,char** buf,std::size_t bufLen,bool changeByteOrder)
{
    assert(data != NULL || buf != NULL && *buf != NULL && 0 < bufLen < 65536);
    const std::size_t bytes = sizeof(T);
    memcpy(*buf,data,bytes);
    if (changeByteOrder)
        Encoder::HtoN(*buf,bytes, bytes);
    (*buf) += bytes;
    return CCMS_RETURN_OK;
}

template <std::size_t size>
std::size_t ENCODEOCT(std::shared_ptr<OctetString<char,size> > octet,char** buf, std::size_t bufLen)
{
    assert(octet != NULL && buf != NULL && *buf != NULL && 0 < bufLen < 65536);
    std::copy((*octet).get(),(*octet).get() + size,*buf);
    (*buf) += size;
    return CCMS_RETURN_OK;
}


std::size_t DECODEVEC(std::shared_ptr<char> data,char** buf, std::size_t bufLen,std::size_t size);

std::size_t ENCODEVEC(std::shared_ptr<char> data,char** buf, std::size_t bufLen,std::size_t  size);

std::shared_ptr<std::vector<char> > make_shared_vector(const std::vector<char>& obj);






























