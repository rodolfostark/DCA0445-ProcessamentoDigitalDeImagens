#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    Mat image, img_cinza, img_cnz_eq, hist_img, hist_eq;
    int width, height;
    VideoCapture cap;
    int nbins = 64;
    float range[] = {0, 255};
    const float *histrange = {range};
    bool uniform = true;
    bool acummulate = false;
    int key;

    // Abre a Camera, entrada de imagens, e verifica se ela foi aberta com sucesso
    cap.open(0);
    if (!cap.isOpened())
    {
        std::cout << "cameras indisponiveis";
        return -1;
    }

    cap.set(CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CAP_PROP_FRAME_HEIGHT, 480);
    width = cap.get(CAP_PROP_FRAME_WIDTH);
    height = cap.get(CAP_PROP_FRAME_HEIGHT);

    int hist_w = nbins, hist_h = nbins / 2;

    Mat display_hist(hist_h, hist_w, CV_8UC1, Scalar(0));
    Mat display_hist_eq(hist_h, hist_w, CV_8UC1, Scalar(0));

    while (1)
    {
        cap >> image;
        cvtColor(image, img_cinza, 6); // Imagem em tons de cinza
        // Equalização de histograma
        equalizeHist(img_cinza, img_cnz_eq);
        // Criação do histograma da imagem original e da imagem equalizada
        calcHist(&img_cinza, 1, 0, Mat(), hist_img, 1, &nbins, &histrange, uniform, acummulate);
        calcHist(&img_cnz_eq, 1, 0, Mat(), hist_eq, 1, &nbins, &histrange, uniform, acummulate);
        // Normalização do histograma para o tamanho da imagem de saída (display_hist) e aplicação do mesmo no histograma da imagem equalizada (hist_eq)
        normalize(hist_img, hist_img, 0, display_hist.rows, NORM_MINMAX, -1, Mat());
        normalize(hist_eq, hist_eq, 0, display_hist_eq.rows, NORM_MINMAX, -1, Mat());
        // Desenho do histograma na imagem de saída (display_hist) e na imagem de saída (display_hist_eq)
        display_hist.setTo(Scalar(0));
        display_hist_eq.setTo(Scalar(0));
        // Desenho do histograma na imagem de saída (display_hist) e na imagem de saída (display_hist_eq)
        for (int i = 0; i < nbins; i++)
        {
            line(display_hist,
                 Point(i, hist_h),
                 Point(i, hist_h - cvRound(hist_img.at<float>(i))), Scalar(255), 1, 8, 0);
            line(display_hist_eq,
                 Point(i, hist_h),
                 Point(i, hist_h - cvRound(hist_eq.at<float>(i))), Scalar(255), 1, 8, 0);
        }

        display_hist.copyTo(img_cinza(Rect(0, 0, nbins, hist_h)));
        display_hist_eq.copyTo(img_cnz_eq(Rect(0, 0, nbins, hist_h)));

        // Exibição da imagem de saída (img_cinza) e da imagem de saída (img_cnz_eq)
        imshow("imagem_cinza", img_cinza);
        imshow("imagem_cinza_equalizada", img_cnz_eq);
        // Espera tecla pressionada para sair do programa ou para continuar a captura de imagens (key = 'q')
        key = waitKey(30);
        if (key == 27)
            break;
    }
    return 0;
}