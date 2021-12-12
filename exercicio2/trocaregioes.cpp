#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    Mat image;
    Vec3b val;

    image = imread("biel.png", cv::IMREAD_GRAYSCALE);
    if (!image.data)
        cout << "nao abriu biel.png" << endl;
    namedWindow("janela", cv::WINDOW_AUTOSIZE);

    imshow("janela", image);
    waitKey();

    int width = image.rows / 2;
    int heigth = image.cols / 2;

    Rect Q1_rect(0, 0, width, heigth);
    Rect Q2_rect(width, 0, width, heigth);
    Rect Q3_rect(0, heigth, width, heigth);
    Rect Q4_rect(width, heigth, width, heigth);

    Mat Q1 = image(Q1_rect);
    Mat Q2 = image(Q2_rect);
    Mat Q3 = image(Q3_rect);
    Mat Q4 = image(Q4_rect);

    Mat img_trocada = image.clone();

    Q1.copyTo(img_trocada(Q4_rect)); //Q1->Q4
    Q2.copyTo(img_trocada(Q3_rect)); //Q2->Q3
    Q3.copyTo(img_trocada(Q2_rect)); //Q3->Q2
    Q4.copyTo(img_trocada(Q1_rect)); //Q4->Q1

    cv::namedWindow("janela", cv::WINDOW_AUTOSIZE);
    cv::imshow("janela", img_trocada);
    cv::imwrite("trocaregioes.png", img_trocada);
    cv::waitKey();

    return 0;
}