#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    Mat image;
    int width, height;
    VideoCapture cap;
    vector<Mat> planes;
    Mat histR, histG, histB, histR_anterior, histG_anterior, histB_anterior;

    int nbins = 64;
    float range[] = {0, 255};
    const float *histrange = {range};
    bool uniform = true;
    bool acummulate = false;
    int key;
    double correlacaoR, correlacaoG, correlacaoB, threshold = 0.99;

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

    cout << "largura = " << width << endl;
    cout << "altura  = " << height << endl;

    int histw = nbins, histh = nbins / 2;
    Mat histImgR(histh, histw, CV_8UC3, Scalar(0, 0, 0));
    Mat histImgG(histh, histw, CV_8UC3, Scalar(0, 0, 0));
    Mat histImgB(histh, histw, CV_8UC3, Scalar(0, 0, 0));
    
    // Efetua a leitura da imagem e a conversao para o espectro de cinza (1 canal) e aplica o equalizacao de histograma (1 canal) para cada canal da imagem
    cap >> image;
    split(image, planes);
    calcHist(&planes[0], 1, 0, Mat(), histR_anterior, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[1], 1, 0, Mat(), histG_anterior, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[2], 1, 0, Mat(), histB_anterior, 1,
             &nbins, &histrange,
             uniform, acummulate);

    while (1)
    {
        cap >> image;
        split(image, planes);
        calcHist(&planes[0], 1, 0, Mat(), histR, 1,
                 &nbins, &histrange,
                 uniform, acummulate);
        calcHist(&planes[1], 1, 0, Mat(), histG, 1,
                 &nbins, &histrange,
                 uniform, acummulate);
        calcHist(&planes[2], 1, 0, Mat(), histB, 1,
                 &nbins, &histrange,
                 uniform, acummulate);

        // Correlacao entre histogramas de cores da imagem atual e anterior (como se fosse um filtro)
        correlacaoR = compareHist(histR, histR_anterior, HISTCMP_CORREL);
        correlacaoG = compareHist(histG, histG_anterior, HISTCMP_CORREL);
        correlacaoB = compareHist(histB, histB_anterior, HISTCMP_CORREL);
        // Se a correlacao entre os histogramas for maior que o threshold, entao a imagem atual e detectada como movimento
        if (correlacaoR <= threshold || correlacaoG <= threshold || correlacaoB <= threshold)
        {
            cout << "Movimento Detectado!" << endl;
        }
        // Atualiza os histogramas para a proxima iteracao do loop
        normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
        normalize(histG, histG, 0, histImgG.rows, NORM_MINMAX, -1, Mat());
        normalize(histB, histB, 0, histImgB.rows, NORM_MINMAX, -1, Mat());
        
        histImgR.setTo(Scalar(0));
        histImgG.setTo(Scalar(0));
        histImgB.setTo(Scalar(0));
        // Desenha os histogramas na imagem de saida (histImg) e mostra a imagem de saida na tela (image)
        for (int i = 0; i < nbins; i++)
        {
            line(histImgR,
                 Point(i, histh),
                 Point(i, histh - cvRound(histR.at<float>(i))),
                 Scalar(0, 0, 255), 1, 8, 0);
            line(histImgG,
                 Point(i, histh),
                 Point(i, histh - cvRound(histG.at<float>(i))),
                 Scalar(0, 255, 0), 1, 8, 0);
            line(histImgB,
                 Point(i, histh),
                 Point(i, histh - cvRound(histB.at<float>(i))),
                 Scalar(255, 0, 0), 1, 8, 0);
        }
        histImgR.copyTo(image(Rect(0, 0, nbins, histh)));
        histImgG.copyTo(image(Rect(0, histh, nbins, histh)));
        histImgB.copyTo(image(Rect(0, 2 * histh, nbins, histh)));
        imshow("image", image);

        calcHist(&planes[0], 1, 0, Mat(), histR_anterior, 1,
                 &nbins, &histrange,
                 uniform, acummulate);
        calcHist(&planes[1], 1, 0, Mat(), histG_anterior, 1,
                 &nbins, &histrange,
                 uniform, acummulate);
        calcHist(&planes[2], 1, 0, Mat(), histB_anterior, 1,
                 &nbins, &histrange,
                 uniform, acummulate);
        // Espera uma tecla ser pressionada para continuar o loop (ou seja, para a proxima iteracao)
        key = waitKey(30);
        if (key == 27)
            break;
    }
    return 0;
}