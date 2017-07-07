

std::size_t DECODEVEC(std::shared_ptr<char> data,char** buf, std::size_t bufLen,std::size_t size)
{
    assert(data != NULL && buf != NULL && *buf != NULL && 0 < bufLen < 65536);
    memcpy(data.get(),*buf,size);
    (*buf) += size;
    return CCMS_RETURN_OK;
}


std::size_t ENCODEVEC(std::shared_ptr<char> data,char** buf, std::size_t bufLen,std::size_t  size)
{
    assert(data != NULL && buf != NULL && *buf != NULL && 0 < bufLen < 65536);
    std::copy(data.get(),data.get()+size,*buf);
    (*buf) += size;
    return CCMS_RETURN_OK;
}

std::shared_ptr<std::vector<char> > make_shared_vector(const std::vector<char>& obj)
{
    return std::make_shared<std::vector<char> >(obj);
}

