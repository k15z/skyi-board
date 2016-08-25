// Copyright 2016, All Rights Reserverd
// Kevin Zhang <kevz@mit.edu>

#include <stack>
#include <vector>
#include "opencv2/opencv.hpp"

namespace skyiBoard {
    struct Point {
        int x;
        int y;
        Point(int x, int y)
        {
            this->x = x;
            this->y = y;
        }
    };

    struct Target {
        int c;
        int x;
        int y;
        Target(int c, int x, int y)
        {
            this->c = c;
            this->x = x;
            this->y = y;
        }
    };

    std::vector<Target> prev;
    std::vector<Target> detect (cv::Mat frame) {
        std::vector<Target> targets;

        int width = frame.cols;
        int height = frame.rows;
        bool* valid = new bool[width*height];
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++) {
                int b = (int)*(frame.data + (width*y + x)*3 + 0);
                int g = (int)*(frame.data + (width*y + x)*3 + 1);
                int r = (int)*(frame.data + (width*y + x)*3 + 2);
                if (r - g > 50 && r - b > 40)
                    valid[y*width+x] = true;
                else
                    valid[y*width+x] = false;
            }

        int c_id = 255;
        int c_id2 = 255;
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
                if (valid[y*width + x]) {
                    std::stack<Point> points;
                    points.push(Point(x,y));
                    Target target(0, 0, 0);
                    while (points.size() > 0) {
                        Point point = points.top();
                        points.pop();
                        if (point.x < 0 || width <= point.x)
                            continue;
                        if (point.y < 0 || height <= point.y)
                            continue;
                        if (valid[point.y*width+point.x]) {
                            valid[point.y*width+point.x] = false;
                            target.c += 1;
                            target.x += point.x;
                            target.y += point.y;
                            points.push(Point(point.x+1,point.y));
                            points.push(Point(point.x-1,point.y));
                            points.push(Point(point.x,point.y+1));
                            points.push(Point(point.x,point.y-1));
                        }
                    }
                    target.x /= target.c;
                    target.y /= target.c;
                    targets.push_back(target);
                }

        for (int i = 0; i < targets.size(); i++) {
            for (int j = i+1; j < targets.size(); j++) {
                cv::line(frame, cv::Point(targets[i].x, targets[i].y), cv::Point(targets[j].x, targets[j].y), cv::Scalar(255, 255, 0));
            }
        }

        delete[] valid;
        prev = targets;
        return targets;
    }
}
