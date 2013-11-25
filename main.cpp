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
#include <string.h>

using namespace std;

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
    void evalSubscores();
    unsigned getScore();
private:
    static const unsigned defaultSize = 10;
    unsigned rows, cols;
    unsigned maxHeight, maxWidth;
    unsigned ** data;
    unsigned totalScore;
    unsigned * subscores;
    unsigned subscoreRecords;
    unsigned subscoreMaxRecords;
    unsigned targetDiff;
    unsigned solutions;
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
    subscores = new unsigned[subscoreMaxRecords];
    subscoreRecords = 0;
    solutions = 0;
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
    solutions = 0;
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

    out << "Vysledku: " << matrix.solutions << endl;
    unsigned i = 0;
    for (unsigned y = 0; y < matrix.rows; y++) {
        for (unsigned x = 0; x < matrix.cols; x++) {
            for (unsigned height = 1; y + height - 1 < matrix.rows; height++) {
                for (unsigned width = 1; x + width - 1 < matrix.cols; width++) {
                    unsigned localDiff = (unsigned) abs((long int) matrix.subscores[i] - (matrix.totalScore - matrix.subscores[i]));
                    if (localDiff == matrix.targetDiff)
                        out << width << " x " << height
                            << " @ (" << x << ", " << y
                            << "): " << matrix.subscores[i] << " : " << matrix.totalScore - matrix.subscores[i]
                            << endl;
                    i++;
                }
            }
        }
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
    unsigned * oldSubscores = subscores;
    subscoreMaxRecords *= 2;
    subscores = new unsigned[subscoreMaxRecords];
    for (unsigned i = 0; i < subscoreRecords; i++) {
        subscores[i] = oldSubscores[i];
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
    unsigned value, checkSum, readingCol = 0;
    char separator;
    while (EOF != (checkSum = scanf("%u%c", &value, &separator))) {
        if ((separator != ',' && separator != '\n') ||
                checkSum != 2 ||
                value < 1 ||
                (rows && readingCol >= cols)) return 0;
        if (!rows && readingCol == maxWidth) reallocCols();
        if (rows == maxHeight) reallocRows();
        data[rows][readingCol] = value;
        totalScore += value;
        if (separator == ',') readingCol++;
        else {
            if (!rows) cols = readingCol + 1;
            else if (cols != readingCol + 1) return 0;
            rows++;
            readingCol = 0;
        }
    }
    return 1;
}

void CMatrix::evalSubscores() {
    targetDiff = totalScore - (totalScore - data[0][0]);
    for (unsigned y = 0; y < rows; y++) {
        for (unsigned x = 0; x < cols; x++) {
            for (unsigned height = 1; y + height - 1 < rows; height++) {
                unsigned rowScore = 0;
                for (unsigned width = 1; x + width - 1 < cols; width++) {
                    if (subscoreRecords > 0.9 * subscoreMaxRecords) reallocSubscores();
                    unsigned lastRow;
                    if (height > 1) lastRow = subscores[subscoreRecords - cols + x];
                    else lastRow = 0;
                    rowScore += data[y + height - 1][x + width - 1];
                    subscores[subscoreRecords] = rowScore + lastRow;
                    unsigned localDiff = (unsigned) abs((long int) subscores[subscoreRecords] - (totalScore - subscores[subscoreRecords]));
                    if (localDiff < targetDiff) {
                        targetDiff = localDiff;
                        solutions = 1;
                    } else if (localDiff == targetDiff) {
                        solutions++;
                    }
                    subscoreRecords++;
                }
            }
        }
    }
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

