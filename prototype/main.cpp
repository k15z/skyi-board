#include <tuple>
#include <stack>
#include "target.cpp"
#include "project.cpp"
#include "opencv2/opencv.hpp"
#include "Eigen/Dense"

using namespace cv;
using namespace std;

int main(int, char**)
{
    VideoCapture src("source.mp4");
    if(!src.isOpened())
        return -1;

    VideoWriter vw("out.mp4", CV_FOURCC('m', 'p', '4', 'v'), 10, cv::Size(960*2, 540*2), true);
    for (int epoch = 0; epoch < 2; epoch++) {
        VideoCapture cap("1.mp4");
        if(!cap.isOpened())
            return -1;

        Mat frame;
        cap >> frame;

//        namedWindow("edges",1);
        for (;;)
        {
            Mat frame;
            cap >> frame;
            if (frame.empty())
                break;

            Mat small;
            cv::resize(frame, small, cv::Size(0,0), 1, 1);

            assert(small.channels() == 3);
            assert(small.elemSize1() == 1);

            Mat source;
            src >> source;
            src >> source;

            std::vector<skyi::Target> targets = skyi::detect(small);
            project(source, small, targets);

    /*        for (int i = 0; i < targets.size(); i++)
                for (int j = i+1; j < targets.size(); j++) {
                    cv::line(small, cv::Point(targets[i].x, targets[i].y), cv::Point(targets[j].x, targets[j].y), cv::Scalar(255, 255, 0));
                }*/
            vw.write(small);
    //        imshow("edges", small);
    //        if(waitKey(1) >= 0) break;
        }
    }

    return 0;
}
