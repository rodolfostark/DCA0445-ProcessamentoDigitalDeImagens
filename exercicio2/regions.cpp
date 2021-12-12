#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char **)
{
    Mat image;
    Vec3b val;

    Point p1, p2;

    image = cv::imread("biel.png", IMREAD_GRAYSCALE);
    if (!image.data)
        cout << "nao abriu biel.png" << endl;

    namedWindow("janela", WINDOW_AUTOSIZE);

    int width = image.rows;
    int heigth = image.cols;

    cout << "Informe as coordenadas de x e y de dois pontos. Lembrando que tamanho da image eh: " << width << "x" << heigth << endl;
    cin >> p1.x >> p1.y >> p2.x >> p2.y;
    imshow("janela", image);

    waitKey();
    image = imread("biel.png", IMREAD_COLOR);
    if (!image.data)
        cout << "nao abriu biel.png" << endl;
    
    for (int i = p1.x; i < p2.x; i++)
    {
        for (int j = p1.y; j < p2.y; j++)
        {
            cv::Vec3b cor = image.at<Vec3b>(i, j);
            cor.val[0] = 255 - cor.val[0];
            cor.val[1] = 255 - cor.val[1];
            cor.val[2] = 255 - cor.val[2];
            image.at<Vec3b>(i, j) = cor;
        }
    }

    imshow("janela", image);
    imwrite("bielnegativo.png", image);
    waitKey();
    
    return 0;
}