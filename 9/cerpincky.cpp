#include <opencv2/opencv.hpp>
#include <iostream>
#include <omp.h>
#include <cmath>

using namespace cv;
using namespace std;


bool isPowerOfThree(int n) {
    if (n < 1) return false;
    double log3 = log(n) / log(3);
    return (pow(3, floor(log3)) == n);
}

void drawSierpinskiCarpet(Mat& img, int x, int y, int size, int depth, int max_depth) noexcept {
    if (depth >= max_depth) return;

    int sub_size = size / 3;

    rectangle(img,
        Point(x + sub_size, y + sub_size),
        Point(x + 2 * sub_size, y + 2 * sub_size),
        Scalar(0, 0, 0),
        FILLED);

#pragma omp parallel for collapse(2)
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i == 1 && j == 1) continue;
            if (sub_size >= 3) { // min size
                drawSierpinskiCarpet(img,
                    x + i * sub_size,
                    y + j * sub_size,
                    sub_size,
                    depth + 1,
                    max_depth);
            }
        }
    }
}

int main() {
    try {
        const int SIZE = 729;     // 3^6
        const int DEPTH = 5;
        const int THREADS = 8;

        if (!isPowerOfThree(SIZE)) {
            throw runtime_error("Размер должен быть степенью тройки");
        }
        if (DEPTH < 1 || THREADS < 1) {
            throw invalid_argument("Некорректные параметры глубины или потоков");
        }

        omp_set_num_threads(THREADS);
        
        Mat image(SIZE, SIZE, CV_8UC3, Scalar(255, 255, 255));

        //fractall
        double start = omp_get_wtime();
        drawSierpinskiCarpet(image, 0, 0, SIZE, 0, DEPTH);
        cout << "Время выполнения: " << omp_get_wtime() - start << " сек.\n";

        imshow("Sierpinski_Carpet", image);
        waitKey(0);

    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}