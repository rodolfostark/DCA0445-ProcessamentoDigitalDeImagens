#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace std;
using namespace cv;

double x0 = 0;
int slider_x0 = 0;
int slider_x0_max = 100;

double band = 0;
int band_slider = 0;
int band_slider_max = 100;

double decaimento = 1;
int d_slider = 1;
int d_slider_max = 100;

Mat image1, image2, blended, mask, peso1, peso2, blend1, blend2, matriz_alfa1, matriz_alfa2;

char TrackbarName[50];

int altura, largura;
float alfa1, alfa2;

// Calculo do peso de cada imagem para aplicar o tilt shift na imagem final (blended)
float alpha(float ponto, float x0, float b, float d)
{
    return 0.5 * (tanhf((ponto - (x0 + (b / 2))) / d) - tanhf((ponto - (x0 - (b / 2))) / d));
}

// Toda mudanca no slider modifica a imagem blended e a mascara mask e o peso peso1 e peso2  e a matriz de alfa matriz_alfa1 e matriz_alfa2
void montagem_de_tiltshift()
{
    // Inicializa as imagens blended e mask com a imagem original image1 e image2 respectivamente
    matriz_alfa1 = Mat::zeros(altura, largura, CV_32F);
    matriz_alfa2 = Mat::zeros(altura, largura, CV_32F);

    // Preenche valores de alfa1 e alfa2 com os valores de alpha calculados com os valores de x0, b e d do slider
    for (int i = 0; i < altura; i++)
    {
        alfa1 = alpha(i, x0, band, decaimento);
        alfa2 = 1.0 - alfa1;
        for (int j = 0; j < largura; j++)
        {
            matriz_alfa1.at<float>(i, j) = alfa1;
            matriz_alfa2.at<float>(i, j) = alfa2;
        }
    }

    // Replica alfa pra matrizes auxiliares
    Mat aux1[] = {matriz_alfa1, matriz_alfa1, matriz_alfa1};
    Mat aux2[] = {matriz_alfa2, matriz_alfa2, matriz_alfa2};
    merge(aux1, 3, matriz_alfa1);
    merge(aux2, 3, matriz_alfa2);
    // Instancia matrizes auxiliares
    Mat copia1, copia2, result_32fc;
    // Convertendo imagens para 32FC3 para multiplicar as matrizes auxiliares com as imagens image1 e image2 e depois convertendo para 8UC3
    image1.convertTo(copia1, CV_32FC3);
    image2.convertTo(copia2, CV_32FC3);
    // Multiplicando as matrizes auxiliares com as imagens image1 e image2 e convertendo para 8UC3 para a imagem blended e a mascara mask
    multiply(copia1, matriz_alfa1, copia1);
    multiply(copia2, matriz_alfa2, copia2);
    // Somando as imagens blended e mask e convertendo para 8UC3 para blended e a mascara mask
    add(copia1, copia2, result_32fc);
    result_32fc.convertTo(blended, CV_8UC3);
    // Mostrar as duas imagens blended e mask na tela (blended e mask)
    imshow("imagem original", image1);
    imshow("borrada", image2);
    imwrite("borrada.jpg", image2);
    imshow("tiltshift", blended);
    imwrite("tiltshift.jpg", blended);
}

// Funcao que atualiza os valores de x0, b e d do slider e chama a funcao montagem_de_tiltshift para atualizar a imagem blended e a mascara mask
void on_trackbar_x0(int, void *)
{
    x0 = (double)slider_x0 * altura / slider_x0_max;
    cout << "x0" << x0 << endl;
    montagem_de_tiltshift();
    imshow("tiltshift", blended);
}

void on_trackbar_band(int, void *)
{
    band = (double)band_slider * altura / band_slider_max;
    cout << "band" << band << endl;
    montagem_de_tiltshift();
    imshow("tiltshift", blended);
}

void on_trackbar_d(int, void *)
{
    decaimento = (double)d_slider;
    cout << "decaimento" << decaimento << endl;
    montagem_de_tiltshift();
    imshow("tiltshift", blended);
}

int main(int argvc, char **argv)
{

    // Carrega imagem 1 e 2
    image1 = imread("blend1.jpg");
    image2 = image1.clone();

    Mat imageAux;

    // Aplica filtro gaussiano na imagem 1 e 2 e mostra na tela (image1 e image2)
    GaussianBlur(image1, image2, Size(5, 5), 5.0, 5.0);
    GaussianBlur(image2, imageAux, Size(5, 5), 5.0, 5.0);
    GaussianBlur(imageAux, image2, Size(5, 5), 5.0, 5.0);

    // Calcula altura e largura da imagem 1
    altura = image1.rows;
    largura = image1.cols;

    namedWindow("tiltshift", 1);
    sprintf(TrackbarName, "x0 x %d", slider_x0_max);
    createTrackbar(TrackbarName, "tiltshift",
                   &slider_x0,
                   slider_x0_max,
                   on_trackbar_x0);
    on_trackbar_x0(slider_x0, 0);

    sprintf(TrackbarName, "band x %d", band_slider_max);
    createTrackbar(TrackbarName, "tiltshift",
                   &band_slider,
                   band_slider_max,
                   on_trackbar_band);
    on_trackbar_band(band_slider, 0);

    sprintf(TrackbarName, "d x %d", d_slider_max);
    createTrackbar(TrackbarName, "tiltshift",
                   &d_slider,
                   d_slider_max,
                   on_trackbar_d);
    on_trackbar_d(d_slider, 0);

    waitKey(0);
    return 0;
}