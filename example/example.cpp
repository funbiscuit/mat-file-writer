
#include <iostream>

// have to put implementation in one of source files
#define MAT_FILE_WRITER_IMPLEMENTATION
#include "mat-file-writer.h"

int main()
{
    // By default most libraries store data in row major format, so matrix
    // [0 1 2
    //  3 4 5
    //  6 7 8]
    // will be stored in array
    // [0 1 2 3 4 5 6 7 8]
    // the same matrix in column major format will be stored as
    // [0 3 6 1 4 7 2 5 8]
    // when you call writer.matrix() you need to send false to last parameter if
    // your matrix is stored in column major. By default it will be row major format
    // all test matrices will have 4 rows and 2 columns
    char chars[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

    int8_t vals8i[] = {1, 2, INT8_MIN, 4, 5, 6, INT8_MAX, 8};
    uint8_t vals8ui[] = {1, 2, 3, 4, 5, 6, UINT8_MAX, 8};

    int16_t vals16i[] = {1, 2, INT16_MIN, 4, 5, 6, INT16_MAX, 8};
    uint16_t vals16ui[] = {1, 2, 3, 4, 5, 6, UINT16_MAX, 8};

    int32_t vals32i[] = {1, 2, INT32_MIN, 4, 5, 6, INT32_MAX, 8};
    uint32_t vals32ui[] = {1, 2, 3, 4, 5, 6, UINT32_MAX, 8};

    int64_t vals64i[] = {1, 2, INT64_MIN, 4, 5, 6, INT64_MAX, 8};
    uint64_t vals64ui[] = {1, 2, 3, 4, 5, 6, UINT64_MAX, 8};

    float valsf[] = {1.f, 0.f, FLT_MIN, -FLT_MIN, 5.f, 6.f, FLT_MAX, -FLT_MAX};
    double valsd[] = {1.0, 0.0, DBL_MIN, -DBL_MIN, 5.0, 6.0, DBL_MAX, -DBL_MAX};

    //also test the same input in column major format
    char chars_cm[] = {'a', 'c', 'e', 'g', 'b', 'd', 'f', 'h'};

    int8_t vals8i_cm[] = {1, INT8_MIN, 5, INT8_MAX, 2, 4, 6, 8};
    uint8_t vals8ui_cm[] = {1, 3, 5, UINT8_MAX, 2, 4, 6, 8};

    int16_t vals16i_cm[] = {1, INT16_MIN, 5, INT16_MAX, 2, 4, 6, 8};
    uint16_t vals16ui_cm[] = {1, 3, 5, UINT16_MAX, 2, 4, 6, 8};

    int32_t vals32i_cm[] = {1, INT32_MIN, 5, INT32_MAX, 2, 4, 6, 8};
    uint32_t vals32ui_cm[] = {1, 3, 5, UINT32_MAX, 2, 4, 6, 8};

    int64_t vals64i_cm[] = {1, INT64_MIN, 5, INT64_MAX, 2, 4, 6, 8};
    uint64_t vals64ui_cm[] = {1, 3, 5, UINT64_MAX, 2, 4, 6, 8};

    float valsf_cm[] = {1.f, FLT_MIN, 5.f, FLT_MAX, 0.f, -FLT_MIN, 6.f, -FLT_MAX};
    double valsd_cm[] = {1.0, DBL_MIN, 5.0, DBL_MAX, 0.0, -DBL_MIN, 6.0, -DBL_MAX};

    MatFileWriter writer("test.mat");

    //write row major matrices (don't need the last argument)
    writer.matrix("chars", chars, 4, 2)
            .matrix("vals8i", vals8i, 4, 2)
            .matrix("vals8ui", vals8ui, 4, 2)
            .matrix("vals16i", vals16i, 4, 2)
            .matrix("vals16ui", vals16ui, 4, 2)
            .matrix("vals32i", vals32i, 4, 2)
            .matrix("vals32ui", vals32ui, 4, 2)
            .matrix("vals64i", vals64i, 4, 2)
            .matrix("vals64ui", vals64ui, 4, 2)
            .matrix("valsf", valsf, 4, 2)
            .matrix("valsd", valsd, 4, 2)
            .matrix("literal", "Literal can also be written!", 4, 7)
            .matrix("literal2", "Literal can also be written!", 7, 4)
                    //write column major matrices
            .matrixCM("chars_cm", chars_cm, 4, 2)
            .matrixCM("vals8i_cm", vals8i_cm, 4, 2)
            .matrixCM("vals8ui_cm", vals8ui_cm, 4, 2)
            .matrixCM("vals16i_cm", vals16i_cm, 4, 2)
            .matrixCM("vals16ui_cm", vals16ui_cm, 4, 2)
            .matrixCM("vals32i_cm", vals32i_cm, 4, 2)
            .matrixCM("vals32ui_cm", vals32ui_cm, 4, 2)
            .matrixCM("vals64i_cm", vals64i_cm, 4, 2)
            .matrixCM("vals64ui_cm", vals64ui_cm, 4, 2)
            .matrixCM("valsf_cm", valsf_cm, 4, 2)
            .matrixCM("valsd_cm", valsd_cm, 4, 2)
            .matrixCM("literal_cm", "Literal can also be written!", 4, 7)
            .matrixCM("literal2_cm", "Literal can also be written!", 7, 4);

    // close is optional, file will be closed on destruction of object
    // writer.close();

    return 0;
}
