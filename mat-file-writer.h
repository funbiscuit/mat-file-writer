#ifndef LIB_MAT_FILE_WRITER_H
#define LIB_MAT_FILE_WRITER_H

#include <cstdint>
#include <fstream>

/**
 * MatFileWriter creates MAT-file (version 5) with matrices/vectors (only 2d)
 * of all supported numeric types
 * Documentation for file format:
 * https://www.mathworks.com/help/pdf_doc/matlab/matfile_format.pdf
 */
class MatFileWriter
{
public:
    enum NumberType
    {
        FLOAT = 1,
        DOUBLE = 2,
        INT8 = 3,
        UINT8 = 4,
        INT16 = 5,
        UINT16 = 6,
        INT32 = 7,
        UINT32 = 8,
        INT64 = 9,
        UINT64 = 10,
        CHAR =11
    };

    explicit MatFileWriter(const std::string& path);
    ~MatFileWriter();

    MatFileWriter& matrix(const char *name, const float *first, int rows, int cols = 1, bool bRowMajor = true);
    MatFileWriter& matrix(const char *name, const double *first, int rows, int cols = 1, bool bRowMajor = true);

    MatFileWriter& matrix(const char *name, const int8_t *first, int rows, int cols = 1, bool bRowMajor = true);
    MatFileWriter& matrix(const char *name, const uint8_t *first, int rows, int cols = 1, bool bRowMajor = true);

    MatFileWriter& matrix(const char *name, const int16_t *first, int rows, int cols = 1, bool bRowMajor = true);
    MatFileWriter& matrix(const char *name, const uint16_t *first, int rows, int cols = 1, bool bRowMajor = true);

    MatFileWriter& matrix(const char *name, const int32_t *first, int rows, int cols = 1, bool bRowMajor = true);
    MatFileWriter& matrix(const char *name, const uint32_t *first, int rows, int cols = 1, bool bRowMajor = true);

    MatFileWriter& matrix(const char *name, const int64_t *first, int rows, int cols = 1, bool bRowMajor = true);
    MatFileWriter& matrix(const char *name, const uint64_t *first, int rows, int cols = 1, bool bRowMajor = true);

    MatFileWriter& matrix(const char *name, const char *first, int rows, int cols = 1, bool bRowMajor = true);

    /**
     * Closes file if it was opened (it's safe to call this multiple times)
     * File will be closed automatically on object destruction
     */
    void close();

private:
    std::ofstream outFile;

    MatFileWriter& matrix(const char *name, const void* first, NumberType dataType, int rows, int cols, bool bRowMajor);

    void writeHeader();

    void write_data_element(int type, const void *data, int dataItemSize, int nDataItems, bool charClass);

    static uint8_t* transpose(const void* data, int dataItemSize, int rows, int cols);
    static inline uint32_t get_padding(uint32_t size);
};



#ifdef MAT_FILE_WRITER_IMPLEMENTATION
#include <algorithm>

enum miTYPE
{
    miINT8 = 1,
    miUINT8 = 2,
    miINT16 = 3,
    miUINT16 = 4,
    miINT32 = 5,
    miUINT32 = 6,
    miSINGLE = 7,
    miDOUBLE = 9,
    miINT64 = 12,
    miUINT64 = 13,
    miMATRIX = 14,
};

enum mxCLASS
{
    mxCHAR_CLASS = 4,
    mxDOUBLE_CLASS = 6,
    mxSINGLE_CLASS = 7,
    mxINT8_CLASS = 8,
    mxUINT8_CLASS = 9,
    mxINT16_CLASS = 10,
    mxUINT16_CLASS = 11,
    mxINT32_CLASS = 12,
    mxUINT32_CLASS = 13,
    mxINT64_CLASS = 14,
    mxUINT64_CLASS = 15
};

MatFileWriter::MatFileWriter(const std::string &path)
{
    outFile.open(path, std::ios::binary | std::ios::out);
    if(outFile.is_open())
        writeHeader();
}

MatFileWriter::~MatFileWriter()
{
    close();
}

MatFileWriter& MatFileWriter::matrix(const char *name, const float *first, int rows, int cols, bool bRowMajor)
{
    return matrix(name, first, FLOAT, rows, cols, bRowMajor);
}

MatFileWriter& MatFileWriter::matrix(const char *name, const double *first, int rows, int cols, bool bRowMajor)
{
    return matrix(name, first, DOUBLE, rows, cols, bRowMajor);
}

MatFileWriter& MatFileWriter::matrix(const char *name, const int8_t *first, int rows, int cols, bool bRowMajor)
{
    return matrix(name, first, INT8, rows, cols, bRowMajor);
}

MatFileWriter& MatFileWriter::matrix(const char *name, const uint8_t *first, int rows, int cols, bool bRowMajor)
{
    return matrix(name, first, UINT8, rows, cols, bRowMajor);
}

MatFileWriter& MatFileWriter::matrix(const char *name, const int16_t *first, int rows, int cols, bool bRowMajor)
{
    return matrix(name, first, INT16, rows, cols, bRowMajor);
}

MatFileWriter& MatFileWriter::matrix(const char *name, const uint16_t *first, int rows, int cols, bool bRowMajor)
{
    return matrix(name, first, UINT16, rows, cols, bRowMajor);
}

MatFileWriter& MatFileWriter::matrix(const char *name, const int32_t *first, int rows, int cols, bool bRowMajor)
{
    return matrix(name, first, INT32, rows, cols, bRowMajor);
}

MatFileWriter& MatFileWriter::matrix(const char *name, const uint32_t *first, int rows, int cols, bool bRowMajor)
{
    return matrix(name, first, UINT32, rows, cols, bRowMajor);
}

MatFileWriter& MatFileWriter::matrix(const char *name, const int64_t *first, int rows, int cols, bool bRowMajor)
{
    return matrix(name, first, INT64, rows, cols, bRowMajor);
}

MatFileWriter& MatFileWriter::matrix(const char *name, const uint64_t *first, int rows, int cols, bool bRowMajor)
{
    return matrix(name, first, UINT64, rows, cols, bRowMajor);
}

MatFileWriter& MatFileWriter::matrix(const char *name, const char *first, int rows, int cols, bool bRowMajor)
{
    return matrix(name, first, CHAR, rows, cols, bRowMajor);
}

MatFileWriter& MatFileWriter::matrix(const char *name, const void* first, NumberType dataType, int rows, int cols, bool bRowMajor)
{
    if(!outFile.is_open())
        return *this;

    //write data element
    uint32_t data_type = miMATRIX;
    uint32_t bytes_num = 0;

    int32_t dims[2]={rows, cols}; //1xn - row; nx1 - column

    int dataItemSize=sizeof(uint8_t);
    int dataItemType=miUINT8;
    uint32_t dataItemClass=mxUINT8_CLASS;

    switch (dataType)
    {
        case FLOAT:
            dataItemSize=sizeof(float);
            dataItemType=miSINGLE;
            dataItemClass=mxSINGLE_CLASS;
            break;
        case DOUBLE:
            dataItemSize=sizeof(double);
            dataItemType=miDOUBLE;
            dataItemClass=mxDOUBLE_CLASS;
            break;

        case INT8:
            dataItemSize=sizeof(int8_t);
            dataItemType=miINT8;
            dataItemClass=mxINT8_CLASS;
            break;
        case UINT8:
            dataItemSize=sizeof(uint8_t);
            dataItemType=miUINT8;
            dataItemClass=mxUINT8_CLASS;
            break;

        case INT16:
            dataItemSize=sizeof(int16_t);
            dataItemType=miINT16;
            dataItemClass=mxINT16_CLASS;
            break;
        case UINT16:
            dataItemSize=sizeof(uint16_t);
            dataItemType=miUINT16;
            dataItemClass=mxUINT16_CLASS;
            break;

        case INT32:
            dataItemSize=sizeof(int32_t);
            dataItemType=miINT32;
            dataItemClass=mxINT32_CLASS;
            break;
        case UINT32:
            dataItemSize=sizeof(uint32_t);
            dataItemType=miUINT32;
            dataItemClass=mxUINT32_CLASS;
            break;

        case INT64:
            dataItemSize=sizeof(int64_t);
            dataItemType=miINT64;
            dataItemClass=mxINT64_CLASS;
            break;
        case UINT64:
            dataItemSize=sizeof(uint64_t);
            dataItemType=miUINT64;
            dataItemClass=mxUINT64_CLASS;
            break;

        case CHAR:
            dataItemSize=sizeof(int8_t);
            //data type will be set to miUINT16 automatically later
            //because char array is written as array of uint16
            dataItemType=miUINT16;
            dataItemClass=mxCHAR_CLASS;
            break;
    }

    const void* data=first;
    uint8_t* transposed=nullptr;

    //since matlab expects matrices in column major order, we need to transpose it first
    if (bRowMajor)
        transposed=transpose(first, dataItemSize, rows, cols);
    if(transposed)
        data=transposed;

    auto name_size = (uint32_t) strlen(name) * sizeof(char);

    //add padding bytes
    name_size += get_padding((uint32_t)name_size);

    auto dataCount = static_cast<uint32_t>(rows * cols);
    uint32_t dataTotalSize = dataCount * dataItemSize;
    //chars will be written as uint16, so size is twice as big
    if(dataType==CHAR)
        dataTotalSize <<= 1u;

    dataTotalSize += get_padding(dataTotalSize);

    bytes_num = 16 //array flags subelement
                + 16 //dimensions subelement
                + name_size + 8 //name subelement (8 for tag)
                + dataTotalSize + 8; //data subelement (8 for tag)


    outFile.write((const char*) &data_type, sizeof(uint32_t));
    outFile.write((const char*) &bytes_num, sizeof(uint32_t));


    //write subelements

    //write array flags block (8 bytes)
    data_type = miUINT32;
    bytes_num = 8;
    outFile.write((const char*) &data_type, sizeof(uint32_t));
    outFile.write((const char*) &bytes_num, sizeof(uint32_t));

    uint32_t flags = 0x00;
    flags |= dataItemClass;//we use only data class flag
    outFile.write((const char*) &flags, sizeof(uint32_t));
    //write 4 bytes of undefined (0 in this case) data to array flags block
    for(int i=0;i<4;++i)
        outFile << 0x00;

    //write dimensions
    write_data_element(miINT32, dims, sizeof(int32_t), 2, false);

    //write array name and data
    write_data_element(miINT8, (void *) name, sizeof(char), static_cast<uint32_t>(strlen(name)), false);
    write_data_element(dataItemType, data, dataItemSize, dataCount, dataType==CHAR);

    //delete transposed data
    if (transposed)
        delete[](transposed);

    return *this;
}

void MatFileWriter::writeHeader()
{
    //write header (128 bytes)

    const size_t maxTextLen = 124;	//matlab uses 124 bytes for header text and 4 bytes for version+endian

    const char head[] = "Version 5 MAT-file, created by MatFileWriter";
    size_t text_len = strlen(head);

    //write text in header (124 bytes max)
    outFile.write(head, sizeof(char) * std::min(maxTextLen, text_len));
    for (size_t k = text_len; k<maxTextLen; k++)
        outFile << ' ';

    uint16_t version = 0x0100;
    uint16_t endian = 'M';
    endian <<= 8u;
    endian += 'I';

    //(version+endian 4 bytes)
    outFile.write((const char*) &version, sizeof(uint16_t));
    outFile.write((const char*) &endian, sizeof(uint16_t));
}

void MatFileWriter::write_data_element(int type, const void* data, int dataItemSize, int nDataItems, bool charClass)
{
    //write data element tag (8 bytes)

    //calculate the number of data bytes:
    uint32_t N = dataItemSize * nDataItems;
    if (charClass)
    {
        //mxCHAR_CLASS data is 8bit, but is written as 16bit uints
        type = miUINT16;
        N <<= 1u;
    }

    //write the datatype identifier
    outFile.write((const char*) &type, sizeof(uint32_t));
    //write the number of data bytes to the data element's tag:
    outFile.write((const char*) &N, sizeof(uint32_t));

    //write data element body (N bytes)
    // write data
    if (charClass)
    {
        //we have to write one char at a time,
        //each followed by a null byte
        auto *chars = reinterpret_cast<const char*>(data);

        for (int i = 0; i<nDataItems; ++i)
        {
            outFile << *chars << 0x00;
            ++chars;
        }
    }
    else
        outFile.write((const char*) data, dataItemSize * nDataItems);

    /*
    * padding is required to ensure 64bit boundaries between
    * data elements.
    */
    uint32_t paddingBytes = get_padding(N);

    for (uint32_t i = 0; i<paddingBytes; ++i)
        outFile << 0x00;
}


void MatFileWriter::close()
{
    if(outFile.is_open())
        outFile.close();
}

uint8_t* MatFileWriter::transpose(const void *data, int dataItemSize, int rows, int cols)
{
    //transposed result will be the same
    if(rows==1 || cols==1)
        return nullptr;

    auto* tr = new uint8_t[rows*cols*dataItemSize];
    auto* src = (uint8_t*) data;

    for(int i=0;i<cols;++i)
        for(int j=0;j<rows;++j)
            memcpy(&tr[(i*rows+j)*dataItemSize], &src[(j*cols+i)*dataItemSize], (size_t)dataItemSize);

    return tr;
}

uint32_t MatFileWriter::get_padding(uint32_t size)
{
    //short way to write (8 - (size % 8)) % 8;
    return uint32_t(-size%8);
}

#endif // MAT_FILE_WRITER_IMPLEMENTATION
#endif //LIB_MAT_FILE_WRITER_H
