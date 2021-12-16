#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void printmask(Mat &m)
{
    for (int i = 0; i < m.size().height; i++)
    {
        for (int j = 0; j < m.size().width; j++)
        {
            cout << m.at<float>(i, j) << ",";
        }
        cout << "\n";
    }
}

int main(int argc, char **argv)
{
    VideoCapture cap; // open the default camera
    float media[] = {0.1111, 0.1111, 0.1111, 0.1111, 0.1111,
                     0.1111, 0.1111, 0.1111, 0.1111};
    float gauss[] = {0.0625, 0.125, 0.0625, 0.125, 0.25,
                     0.125, 0.0625, 0.125, 0.0625};
    float horizontal[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    float vertical[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    float laplacian[] = {0, -1, 0, -1, 4, -1, 0, -1, 0};
    float boost[] = {0, -1, 0, -1, 5.2, -1, 0, -1, 0};

    Mat frame, framegray, frame32f, frameFiltered, frameFiltered1;
    Mat mask(3, 3, CV_32F);
    Mat result;
    double width, height;
    int absolut;
    char key;
    bool laplgauss = 0;

    cap.open(0);
    if (!cap.isOpened()) // check if we succeeded
    {
        return -1;
    }

    cap.set(CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CAP_PROP_FRAME_HEIGHT, 480);
    width = cap.get(CAP_PROP_FRAME_WIDTH);
    height = cap.get(CAP_PROP_FRAME_HEIGHT);
    cout << "largura=" << width << "\n";
    cout << "altura =" << height << "\n";
    cout << "fps    =" << cap.get(CAP_PROP_FPS) << "\n";
    cout << "format =" << cap.get(CAP_PROP_FORMAT) << "\n";

    namedWindow("filtroespacial", WINDOW_NORMAL);
    namedWindow("original", WINDOW_NORMAL);

    mask = Mat(3, 3, CV_32F, media);

    absolut = 1; // Calcula o valor absoluto do resultado

    for (;;)
    {
        cap >> frame; // get a new frame from camera
        cvtColor(frame, framegray, COLOR_BGR2GRAY);
        flip(framegray, framegray, 1);
        imshow("original", framegray);
        framegray.convertTo(frame32f, CV_32F);

        if (laplgauss == 1)
        {
            // Seleciona o filtro a ser usado (laplaciano ou gaussiano)
            mask = Mat(3, 3, CV_32F, gauss);
            // Aplica o filtro Gaussiano na imagem e converte para float para poder usar o cv::filter2D
            filter2D(frame32f, frameFiltered1, frame32f.depth(), mask, Point(1, 1), 0);
            mask = Mat(3, 3, CV_32F, laplacian);
            // Aplica o filtro Laplaciano na imagem e converte para float para poder usar o cv::filter2D
            filter2D(frameFiltered1, frameFiltered, frame32f.depth(), mask, Point(1, 1), 0);
        }
        else
        {
            filter2D(frame32f, frameFiltered, frame32f.depth(), mask,
                     Point(1, 1), 0);
        }
        if (absolut)
        {
            frameFiltered = abs(frameFiltered);
        }

        frameFiltered.convertTo(result, CV_8U);
        imshow("Filtro Espacial", result);

        key = (char)waitKey(10);
        if (key == 27)
            break; // esc pressed!
        switch (key)
        {
        case 'a':
            absolut = !absolut;
            laplgauss = 0;
            break;
        case 'm':
            // Aplica o filtro da media na imagem e converte para float para poder usar o cv::filter2D
            mask = Mat(3, 3, CV_32F, media);
            printmask(mask);
            laplgauss = 0;
            break;
        case 'g':
            // Aplica o filtro Gaussiano na imagem e converte para float para poder usar o cv::filter2D
            mask = Mat(3, 3, CV_32F, gauss);
            printmask(mask);
            laplgauss = 0;
            break;
        case 'h':
            // Aplica o filtro horizontal na imagem e converte para float para poder usar o cv::filter2D
            mask = Mat(3, 3, CV_32F, horizontal);
            printmask(mask);
            laplgauss = 0;
            break;
        case 'v':
            // Aplica o filtro vertical na imagem e converte para float para poder usar o cv::filter2D
            mask = Mat(3, 3, CV_32F, vertical);
            printmask(mask);
            laplgauss = 0;
            break;
        case 'l':
            // Aplica o filtro Laplaciano na imagem e converte para float para poder usar o cv::filter2D
            mask = Mat(3, 3, CV_32F, laplacian);
            laplgauss = 0;
            printmask(mask);
            break;
        case 'b':
            // Aplica o filtro de boost na imagem e converte para float para poder usar o cv::filter2D
            laplgauss = 0;
            mask = Mat(3, 3, CV_32F, boost);
            break;
        case 'p':
            // Aplica o filtro de Laplaciano e Gaussiano na imagem e converte para float para poder usar o cv::filter2D
            laplgauss = 1;
            break;
        default:
            break;
        }
    }
    return 0;
}