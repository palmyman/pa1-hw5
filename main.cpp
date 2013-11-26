/* 
 * File:   main.cpp
 * Author: palmyman
 *
 * Created on November 16, 2013, 10:24 PM
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <stdio.h>

using namespace std;

struct TScore {
    unsigned width, height, x, y, in;
};

class CMatrix {
public:
    CMatrix();
    ~CMatrix();
    friend ostream& operator <<(ostream&, const CMatrix&);
    //    bool addRow(string);
    //    bool read();
    bool readMatrix();
    bool reallocRows();
    bool reallocCols();
    bool reallocSubscores();
    bool evalSubscores();
    unsigned getScore();
private:
    static const unsigned defaultSize = 2;
    unsigned rows, cols;
    unsigned maxHeight, maxWidth;
    unsigned ** data;
    unsigned totalScore;
    TScore * subscores;
    unsigned subscoreRecords;
    unsigned subscoreMaxRecords;
    unsigned targetDiff;
};

CMatrix::CMatrix() {
    maxHeight = defaultSize;
    maxWidth = defaultSize;
    cols = 0;
    rows = 0;
    totalScore = 0;
    targetDiff = 0;
    data = new unsigned*[maxHeight];
    for (unsigned i = 0; i < maxHeight; i++) {
        data[i] = new unsigned[maxWidth];
    }
    subscoreMaxRecords = defaultSize;
    subscores = new TScore[subscoreMaxRecords];
    subscoreRecords = 0;
}

CMatrix::~CMatrix() {
    for (unsigned i = 0; i < maxHeight; i++) {
        delete[] data[i];
    }
    delete[] data;
    data = NULL;
    maxHeight = 0;
    maxWidth = 0;
    rows = 0;
    cols = 0;
    delete [] subscores;
    subscoreRecords = 0;
    subscoreMaxRecords = 0;
}

ostream& operator <<(ostream& out, const CMatrix& matrix) {
    //    MATRIX PRINT
    //    if (!matrix.cols || !matrix.rows) {
    //        out << "Empty matrix" << endl;
    //        return out;
    //    }
    //    for (unsigned i = 0; i < matrix.rows; i++) {
    //        for (unsigned j = 0; j < matrix.cols; j++) {
    //            out << setw(4) << matrix.data[i][j];
    //        }
    //        out << endl;
    //    }

    out << "Vysledku: " << matrix.subscoreRecords << endl;
    for (unsigned i = 0; i < matrix.subscoreRecords; i++) {
        out << matrix.subscores[i].width << " x " << matrix.subscores[i].height
                << " @ (" << matrix.subscores[i].x << ", " << matrix.subscores[i].y
                << "): " << matrix.subscores[i].in << " : " << matrix.totalScore - matrix.subscores[i].in
                << endl;
    }
    return out;
}

bool CMatrix::reallocCols() {
    unsigned ** oldData = data;
    maxWidth *= 2;
    data = new unsigned*[maxHeight];
    for (unsigned i = 0; i < maxHeight; i++) {
        data[i] = new unsigned[maxWidth];
    }
    //copy old data
    for (unsigned i = 0; i < cols; i++) {
        data[0][i] = oldData[0][i];
    }
    //free old memory
    for (unsigned i = 0; i < maxHeight; i++) {
        delete[] oldData[i];
    }
    delete[] oldData;
    return 1;
}

bool CMatrix::reallocRows() {
    unsigned ** oldData = data;
    maxHeight *= 2;
    data = new unsigned*[maxHeight];
    for (unsigned i = 0; i < maxHeight; i++) {
        if (i < maxHeight / 2)
            data[i] = oldData[i];
        else
            data[i] = new unsigned[maxWidth];
    }
    delete[] oldData;
    return 1;
}

bool CMatrix::reallocSubscores() {
    TScore * oldSubscores = subscores;
    subscoreMaxRecords *= 2;
    subscores = new TScore[subscoreMaxRecords];
    for (unsigned i = 0; i < subscoreRecords; i++) {
        subscores[i].height = oldSubscores[i].height;
        subscores[i].in = oldSubscores[i].in;
        subscores[i].width = oldSubscores[i].width;
        subscores[i].x = oldSubscores[i].x;
        subscores[i].y = oldSubscores[i].y;
    }
    delete[] oldSubscores;
    return 1;
}

//bool CMatrix::addRow(string row) {
//    unsigned x = 0, value;
//    size_t current, next = -1;
//    string colData;
//    do {
//        current = next + 1;
//        next = row.find_first_of(",", current);
//        colData = row.substr(current, next - current);
//        if (rows && x > cols) return 0;
//        if (!rows && x == maxWidth) {
//            reallocCols();
//        }
//        value = atoi(colData.c_str());
//        if (value < 1) return 0;
//        if (!rows) cols++;
//        data[rows][x] = value;
//        totalScore += data[rows][x];
//        x++;
//    } while (next != string::npos);
//    if (rows && x != cols) return 0;
//    rows++;
//    return 1;
//}
//
//bool CMatrix::read() {
//    string row;
//    while (!cin.eof()) {
//        getline(cin, row);
//        if (rows == maxHeight) reallocRows();
//        if (!addRow(row)) return 0;
//    }
//    return 1;
//}

bool CMatrix::readMatrix() {
    cout << "Zadejte matici:" << endl;
    unsigned value, readingCol = 0;
    int checkSum;
    char separator;
    while (EOF != (checkSum = scanf("%u%c", &value, &separator))) {
        if ((separator != ',' && separator != '\n') ||
                checkSum != 2 ||
                value < 1 ||
                (rows && readingCol >= cols))
            return 0;
        if (readingCol == maxWidth) reallocCols();
        if (rows == maxHeight) reallocRows();
        if (!rows) cols++;
        data[rows][readingCol] = value;
        totalScore += value;
        if (separator == ',') readingCol++;
        else {
            if (rows && cols != readingCol + 1)
                return 0;
            rows++;
            readingCol = 0;
        }
    }
    targetDiff = totalScore;
    if (rows + cols < 3 || separator != '\n')
        return 0;
    return 1;
}

bool CMatrix::evalSubscores() {
    unsigned * buffer = new unsigned[cols];
    unsigned localScore;
    for (unsigned y = 0; y < rows; y++) {
        for (unsigned x = 0; x < cols; x++) {
            for (unsigned height = 1; y + height - 1 < rows; height++) {
                unsigned rowScore = 0;
                unsigned modI = 0;
                for (unsigned width = 1; x + width - 1 < cols; width++) {
                    if (subscoreRecords == subscoreMaxRecords) reallocSubscores();
                    unsigned lastRow;
                    if (height > 1) lastRow = buffer[modI];
                    else lastRow = 0;
                    rowScore += data[y + height - 1][x + width - 1];
                    localScore = rowScore + lastRow;
                    buffer[modI] = localScore;
                    unsigned localDiff = (unsigned) abs((long int) localScore - (totalScore - localScore));
                    if (localDiff < targetDiff) {
                        targetDiff = localDiff;
                        subscoreRecords = 0;
                        subscoreMaxRecords = defaultSize;
                    }
                    if (localDiff == targetDiff) {
                        subscores[subscoreRecords].x = x;
                        subscores[subscoreRecords].y = y;
                        subscores[subscoreRecords].height = height;
                        subscores[subscoreRecords].width = width;
                        subscores[subscoreRecords].in = localScore;
                        subscoreRecords++;
                    }
                    modI++;
                    if (localScore > totalScore / 2 + targetDiff) {
                        break;
                    }
                }
            }
            if (!x && localScore < totalScore / 2 - targetDiff) {
                delete [] buffer;
                return 0;
            }
        }
    }
    delete [] buffer;
    return 1;
}

unsigned CMatrix::getScore() {
    return totalScore;
}

/*
 * 
 */
int main(int argc, char** argv) {
    CMatrix testMatrix;
    if (!testMatrix.readMatrix()) {
        cout << "Nespravny vstup." << endl;
        return 0;
    }

    testMatrix.evalSubscores();

    cout << testMatrix << flush;
    return 0;
}

