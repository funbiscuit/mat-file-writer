#ifndef LIB_MAT_FILE_WRITER_H
#define LIB_MAT_FILE_WRITER_H

#include <cstdint>
#include <fstream>
#include <vector>

/**
 * MatFileWriter creates MAT-file (Level 5) with matrices/vectors (only 2d)
 * of all supported numeric types
 * Documentation for file format:
 * https://www.mathworks.com/help/pdf_doc/matlab/matfile_format.pdf
 */
class MatFileWriter
{
public:
    enum class MatrixOrder
    {
        ROW_MAJOR,
        COLUMN_MAJOR,
    };

    explicit MatFileWriter(const std::string& path);
    ~MatFileWriter();

    /**
     * Closes file if it was opened (it's safe to call this multiple times)
     * File will be closed automatically on object destruction
     */
    void close();


    /**
     * Write 2D matrix to opened MAT-file
     * Suppose we have matrix:
     * 0 1 2
     * 3 4 5
     * In row major format it will be stored as:
     * 0 1 2 3 4 5
     * In column major format it will be stored as:
     * 0 3 1 4 2 5
     * @tparam T - type of element
     * @param name - name of matrix in file
     * @param first - pointer to first element in matrix
     * @param rows - number of rows
     * @param cols - number of columns
     * @param order - order of elements
     * @return
     */
    template<typename T>
    MatFileWriter& matrix(const std::string& name, const T* first,
                          size_t rows, size_t cols = 1,
                          MatrixOrder order = MatrixOrder::ROW_MAJOR)
    {
        // check that type of elements is supported
        static_assert(std::is_same<T, char>::value ||
                      std::is_same<T, double>::value ||
                      std::is_same<T, float>::value ||
                      std::is_same<T, int8_t>::value ||
                      std::is_same<T, uint8_t>::value ||
                      std::is_same<T, int16_t>::value ||
                      std::is_same<T, uint16_t>::value ||
                      std::is_same<T, int32_t>::value ||
                      std::is_same<T, uint32_t>::value ||
                      std::is_same<T, int64_t>::value ||
                      std::is_same<T, uint64_t>::value,
                      "Supported types are char, integers and float/double");

        if(std::is_same<T, char>::value)
            return matrix(name, first, mxCHAR_CLASS, rows, cols, order);
        else if(std::is_same<T, float>::value)
            return matrix(name, first, mxSINGLE_CLASS, rows, cols, order);
        else if(std::is_same<T, double>::value)
            return matrix(name, first, mxDOUBLE_CLASS, rows, cols, order);
        else if(std::is_same<T, int8_t>::value)
            return matrix(name, first, mxINT8_CLASS, rows, cols, order);
        else if(std::is_same<T, uint8_t>::value)
            return matrix(name, first, mxUINT8_CLASS, rows, cols, order);
        else if(std::is_same<T, int16_t>::value)
            return matrix(name, first, mxINT16_CLASS, rows, cols, order);
        else if(std::is_same<T, uint16_t>::value)
            return matrix(name, first, mxUINT16_CLASS, rows, cols, order);
        else if(std::is_same<T, int32_t>::value)
            return matrix(name, first, mxINT32_CLASS, rows, cols, order);
        else if(std::is_same<T, uint32_t>::value)
            return matrix(name, first, mxUINT32_CLASS, rows, cols, order);
        else if(std::is_same<T, int64_t>::value)
            return matrix(name, first, mxINT64_CLASS, rows, cols, order);
        else if(std::is_same<T, uint64_t>::value)
            return matrix(name, first, mxUINT64_CLASS, rows, cols, order);
    }

    template<typename T>
    MatFileWriter& matrixCM(const std::string& name, const T* first,
                            size_t rows, size_t cols = 1)
    {
        return matrix(name, first, rows, cols, MatrixOrder::COLUMN_MAJOR);
    }

private:
    enum mxCLASS : uint32_t
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

    std::ofstream outFile;

    MatFileWriter& matrix(const std::string& name, const void* first,
                          mxCLASS dataClass, int rows, int cols,
                          MatrixOrder order);

    void writeHeader();

    void writeDataElement(int type, const void *data, int dataItemSize,
                          int nDataItems, bool charClass);

    void write(const void* bytes, size_t num);

    static std::vector<uint8_t> transpose(const void* data, int dataItemSize,
                                          int rows, int cols);
    static inline uint32_t getPadding(uint32_t size);
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

MatFileWriter& MatFileWriter::matrix(const std::string& name, const void* first,
                                     mxCLASS dataClass, int rows, int cols,
                                     MatrixOrder order)
{
    if(!outFile.is_open())
        return *this;

    //write data element
    uint32_t dataType = miMATRIX;
    uint32_t bytesNum = 0;

    int32_t dims[2]={rows, cols}; //1xn - row; nx1 - column

    int dataItemSize=sizeof(uint8_t);
    int dataItemType=miUINT8;

    switch (dataClass)
    {
        case mxSINGLE_CLASS:
            dataItemSize=sizeof(float);
            dataItemType=miSINGLE;
            break;
        case mxDOUBLE_CLASS:
            dataItemSize=sizeof(double);
            dataItemType=miDOUBLE;
            break;
        case mxINT8_CLASS:
            dataItemSize=sizeof(int8_t);
            dataItemType=miINT8;
            break;
        case mxUINT8_CLASS:
            dataItemSize=sizeof(uint8_t);
            dataItemType=miUINT8;
            break;

        case mxINT16_CLASS:
            dataItemSize=sizeof(int16_t);
            dataItemType=miINT16;
            break;
        case mxUINT16_CLASS:
            dataItemSize=sizeof(uint16_t);
            dataItemType=miUINT16;
            break;

        case mxINT32_CLASS:
            dataItemSize=sizeof(int32_t);
            dataItemType=miINT32;
            break;
        case mxUINT32_CLASS:
            dataItemSize=sizeof(uint32_t);
            dataItemType=miUINT32;
            break;

        case mxINT64_CLASS:
            dataItemSize=sizeof(int64_t);
            dataItemType=miINT64;
            break;
        case mxUINT64_CLASS:
            dataItemSize=sizeof(uint64_t);
            dataItemType=miUINT64;
            break;

        case mxCHAR_CLASS:
            dataItemSize=sizeof(int8_t);
            //data type will be set to miUINT16 automatically later
            //because char array is written as array of uint16
            dataItemType=miUINT16;
            break;
    }

    const void* data=first;
    std::vector<uint8_t> transposed;

    // since matlab expects matrices in column major order,
    // we need to transpose it first
    if (order == MatrixOrder::ROW_MAJOR)
    {
        transposed=transpose(first, dataItemSize, rows, cols);
        if(!transposed.empty())
            data = transposed.data();
    }

    auto nameSize = name.size();

    //add padding bytes
    nameSize += getPadding((uint32_t) nameSize);

    auto dataCount = static_cast<uint32_t>(rows * cols);
    uint32_t dataTotalSize = dataCount * dataItemSize;
    //chars will be written as uint16, so size is twice as big
    if(dataClass==mxCHAR_CLASS)
        dataTotalSize <<= 1u;

    dataTotalSize += getPadding(dataTotalSize);

    bytesNum = 16 //array flags subelement
                + 16 //dimensions subelement
                + nameSize + 8 //name subelement (8 for tag)
                + dataTotalSize + 8; //data subelement (8 for tag)


    write(&dataType, 4);
    write(&bytesNum, 4);


    //write subelements

    //write array flags block (8 bytes)
    dataType = miUINT32;
    bytesNum = 8;
    write(&dataType, 4);
    write(&bytesNum, 4);

    uint32_t flags = 0x00;
    flags |= dataClass;//we use only data class flag
    write(&flags, 4);
    //write 4 bytes of undefined (0 in this case) data to array flags block
    write(nullptr, 4);

    //write dimensions
    writeDataElement(miINT32, dims, sizeof(int32_t), 2, false);

    //write array name and data
    writeDataElement(miINT8, name.c_str(), sizeof(char), name.size(), false);
    writeDataElement(dataItemType, data, dataItemSize, dataCount, dataClass == mxCHAR_CLASS);

    return *this;
}

void MatFileWriter::writeHeader()
{
    //write header (128 bytes)

    const size_t maxTextLen = 124;	//matlab uses 124 bytes for header text and 4 bytes for version+endian

    const char head[] = "MATLAB 5.0 MAT-file";
    size_t textLen = strlen(head);

    //write text in header (124 bytes max)
    write(head, std::min(maxTextLen, textLen));
    for (size_t k = textLen; k < maxTextLen; k++)
        outFile << ' ';

    uint16_t version = 0x0100;
    uint16_t endian = 'M';
    endian <<= 8u;
    endian += 'I';

    //(version+endian 4 bytes)
    write(&version, 2);
    write(&endian, 2);
}

void MatFileWriter::writeDataElement(int type, const void* data, int dataItemSize, int nDataItems, bool charClass)
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
    write(&type, 4);
    //write the number of data bytes to the data element's tag:
    write(&N, 4);

    //write data element body (N bytes)
    // write data
    if (charClass)
    {
        //we have to write one char at a time,
        //each followed by a null byte
        auto *chars = reinterpret_cast<const char*>(data);

        for (int i = 0; i<nDataItems; ++i)
        {
            outFile << *chars << (uint8_t) 0x00;
            ++chars;
        }
    }
    else
        write(data, dataItemSize * nDataItems);

    // padding is required to ensure 64bit boundaries between data elements
    write(nullptr, getPadding(N));
}

void MatFileWriter::write(const void* bytes, size_t num)
{
    if(bytes)
        outFile.write((const char*) bytes, num);
    else
        for(size_t i = 0; i < num; ++i)
            outFile << (uint8_t) 0x00;
}

void MatFileWriter::close()
{
    if(outFile.is_open())
        outFile.close();
}

std::vector<uint8_t> MatFileWriter::transpose(const void *data, int dataItemSize, int rows, int cols)
{
    std::vector<uint8_t> tr;
    //transposed result will be the same
    if(rows==1 || cols==1)
        return tr;

    tr.resize(rows*cols*dataItemSize);
    auto* src = (uint8_t*) data;

    for(int i=0;i<cols;++i)
        for(int j=0;j<rows;++j)
            memcpy(&tr[(i*rows+j)*dataItemSize], &src[(j*cols+i)*dataItemSize], (size_t)dataItemSize);

    return tr;
}

uint32_t MatFileWriter::getPadding(uint32_t size)
{
    //short way to write (8 - (size % 8)) % 8;
    return uint32_t(-size%8);
}

#endif // MAT_FILE_WRITER_IMPLEMENTATION
#endif // LIB_MAT_FILE_WRITER_H
