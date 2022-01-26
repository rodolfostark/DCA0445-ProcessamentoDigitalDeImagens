#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

#define STEP 6
#define JITTER 4
#define RAIO 4

int top_slider = 10;
int top_slider_max = 200;

char TrackbarName[50];

Mat image, border, points;

int main(int argc, char **argv)
{
    vector<int> yrange;
    vector<int> xrange;

    Mat frame;

    int width, height, gray, canny_cor;
    int x, y;

    image = imread(argv[1], IMREAD_GRAYSCALE);

    srand(time(0));

    if (!image.data)
    {
        cout << "nao abriu" << argv[1] << endl;
        cout << argv[0] << " imagem.jpg";
        exit(0);
    }

    width = image.size().width;
    height = image.size().height;

    xrange.resize(height / STEP);
    yrange.resize(width / STEP);

    iota(xrange.begin(), xrange.end(), 0);
    iota(yrange.begin(), yrange.end(), 0);

    for (uint i = 0; i < xrange.size(); i++)
    {
        xrange[i] = xrange[i] * STEP + STEP / 2;
    }
    for (uint i = 0; i < yrange.size(); i++)
    {
        yrange[i] = yrange[i] * STEP + STEP / 2;
    }
    points = Mat(height, width, CV_8U, Scalar(255));
    random_shuffle(xrange.begin(), xrange.end());

    for (auto i : xrange)
    {
        random_shuffle(yrange.begin(), yrange.end());
        for (auto j : yrange)
        {
            x = i + rand() % (2 * JITTER) - JITTER + 1;
            y = j + rand() % (2 * JITTER) - JITTER + 1;
            gray = image.at<uchar>(x, y);
            circle(points,
                   cv::Point(y, x),
                   RAIO,
                   CV_RGB(gray, gray, gray),
                   -1,
                   LINE_AA);
        }
    }
    imshow("ponts", points);
    imwrite("pontos.jpg", points);
    Canny(points, border, 200, 600);
    imwrite("canny1.jpg", border);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            canny_cor = border.at<uchar>(i, j);
            if (canny_cor == 255)
            {
                gray = image.at<uchar>(i, j);
                circle(points,
                       cv::Point(j, i),
                       3,
                       CV_RGB(gray, gray, gray),
                       -1,
                       LINE_AA);
            }
        }
    }
    Canny(points, border, 100, 300);
    imwrite("canny2.jpg", border);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            canny_cor = border.at<uchar>(i, j);
            if (canny_cor == 255)
            {
                gray = image.at<uchar>(i, j);
                circle(points,
                       Point(j, i),
                       2,
                       CV_RGB(gray, gray, gray),
                       -1,
                       LINE_AA);
            }
        }
    }

    Canny(points, border, 50, 150);
    imwrite("canny3.jpg", border);
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            canny_cor = border.at<uchar>(i, j);
            if (canny_cor == 255)
            {
                gray = image.at<uchar>(i, j);
                circle(points,
                       Point(j, i),
                       1,
                       CV_RGB(gray, gray, gray),
                       -1,
                       LINE_AA);
            }
        }
    }

    Canny(points, border, 20, 60);
    imwrite("canny4.jpg", border);
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            canny_cor = border.at<uchar>(i, j);
            if (canny_cor == 255)
            {
                gray = image.at<uchar>(i, j);
                circle(points,
                       Point(j, i),
                       1,
                       CV_RGB(gray, gray, gray),
                       -1,
                       LINE_AA);
            }
        }
    }
    waitKey();
    imshow("result", points);
    imwrite("cannyborders.jpg", points);
    waitKey();

    return 0;
}