#include <iostream>
#include "Eigen/Dense"

using namespace std;
using namespace Eigen;

namespace skyi {
    void project(cv::Mat src, cv::Mat dest, std::vector<Target> point) {
        clock_t startTime = clock();

        Matrix<double, 4, 4> A;
        A << 1, 0, 0, 0, 1, src.cols, 0, 0, 1, 0, src.rows, 0, 1, src.cols, src.rows, src.cols * src.rows;
        Matrix<double, 4, 1> c;
        c << point[0].x, point[1].x, point[2].x, point[3].x;
        Matrix<double, 4, 1> d;
        d << point[0].y, point[1].y, point[2].y, point[3].y;

        Matrix<double, 4, 1> a = A.ldlt().solve(c);
        Matrix<double, 4, 1> b = A.ldlt().solve(d);

        float secsElapsed = (float)(clock() - startTime)/CLOCKS_PER_SEC;
        std::cout << "project matrix " << secsElapsed << "\n";

        startTime = clock();
        for (int x = 0; x < src.cols; x++)
            for (int y = 0; y < src.rows; y++) {
                int dx = a[0] * 1 + a[1] * x + a[2] * y + a[3] * x * y;
                int dy = b[0] * 1 + b[1] * x + b[2] * y + b[3] * x * y;
                if (dx < 0 || dy < 0 || dx >= dest.cols || dy >= dest.rows)
                    continue;

                *(dest.data + (dest.cols*dy+dx)*3+0) = *(src.data + (src.cols*y+x)*3+0);
                *(dest.data + (dest.cols*dy+dx)*3+1) = *(src.data + (src.cols*y+x)*3+1);
                *(dest.data + (dest.cols*dy+dx)*3+2) = *(src.data + (src.cols*y+x)*3+2);
            }
        
        secsElapsed = (float)(clock() - startTime)/CLOCKS_PER_SEC;
        std::cout << "project copy " << secsElapsed << "\n";
    }
}
