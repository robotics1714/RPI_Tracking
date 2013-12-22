#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main()
{
    Mat img, hsv;
    VideoCapture cap("http://10.17.14.11/mjpg/video.mjpg");
    vector< vector< Point > > contours, filteredContours;
    vector<Vec4i> hierarchy;
    namedWindow("IMG", CV_WINDOW_AUTOSIZE);
    namedWindow("HSV", CV_WINDOW_AUTOSIZE);

    while(true)
    {
        //Get the newest image
        cap>>img;
        if(img.empty())cout<<"no\n";

        //Blur the image and Convery it to hsv
        GaussianBlur(img, img, Size(3, 3), 0, 0);
        cvtColor(img, hsv, CV_BGR2HSV);

        //Detect the goals in range
        inRange(hsv, Scalar(140, 80, 45), Scalar(160, 100, 65), hsv);

        //find the contours
        findContours(hsv, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

        //Determine which of the found contours are good
        for(int i = 0; i < contours.size(); i++)
        {
            double area = contourArea(contours[i]);
            vector<Point> simpleContour;
            approxPolyDP(contours[i], simpleContour, 5, true);
            if((area >= 1000) && (simpleContour.size() == 4))
            {
                filteredContours.push_back(simpleContour);
            }
        }

        drawContours(hsv, filteredContours, -1, Scalar(255, 255, 255), CV_FILLED);

        imshow("IMG", img);
        imshow("HSV", hsv);
        cout<<"hello\n";
        if(waitKey(1)>=0)break;
    }
}
