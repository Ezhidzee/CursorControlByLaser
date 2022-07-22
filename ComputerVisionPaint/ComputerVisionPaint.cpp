#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>

#include <iostream>
#include <algorithm>
#include <windows.h>

bool contourСomparison(std::vector<cv::Point> a, std::vector<cv::Point> b) {
    return a.size() <= b.size();
}

int main(int argc, char** argv) {
    cv::Mat img, bg, hsvImg;
    cv::Moments m;
    std::vector<std::vector<cv::Point>> contours;
    const double MONITOR_WIDTH = 1920.0, MONITOR_HEIGHT = 1080.0,
        VIDEO_WIDTH = 640.0, VIDEO_HEIGHT = 460.0;

    cv::namedWindow("Camera");

    cv::VideoCapture cap(1);
    if (!cap.isOpened()) std::cerr << "Unable to open camera";

    for (;;) {
        cap >> img;
        img.copyTo(bg);

        cv::cvtColor(img, img, cv::COLOR_BGR2HSV);
        cv::inRange(img, cv::Scalar(160, 60, 66), cv::Scalar(180, 255, 255), hsvImg);

        contours.clear();
        cv::findContours(hsvImg, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        std::sort(contours.begin(), contours.end(), contourСomparison);

        if (!contours.empty()) {
            cv::Rect rec = cv::boundingRect(contours.back());
            m = moments(contours.back());
            
            SetCursorPos((m.m10 / m.m00) * (MONITOR_WIDTH / VIDEO_WIDTH), (m.m01 / m.m00) * (MONITOR_HEIGHT / VIDEO_HEIGHT));
            cv::circle(bg, cv::Point(m.m10 / m.m00, m.m01 / m.m00), 1, cv::Scalar(0, 0, 255), 5);
            cv::rectangle(bg, rec.tl(), rec.br(), cv::Scalar(0, 0, 255), 2);
        }

        cv::imshow("Camera", bg);
        cv::waitKey(25);
    }
}