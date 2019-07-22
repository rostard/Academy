

#include "Matrix.h"

int main()
{
    Matrix<int, 4, 4> m(4);
    m.print();

    Matrix<float, 4, 4> m2 = 1. / m;
    m2.print();

    Matrix<int, 8, 2> m3(m);
    m3.print();

    Matrix<float, 4, 1> m4(2);

    (m * m4).print();
}