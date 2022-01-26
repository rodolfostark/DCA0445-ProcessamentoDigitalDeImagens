#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

#define RADIUS 20

using namespace cv;
using namespace std;

Mat imaginaryInput, complexImage, multsp;
Mat padded, filter, mag, imFiltrado;
Mat image, imagegray, tmp, magI;
Mat_<float> realInput, log_realInput, zeros, exp_idft, ones;
Mat backgroundImage;
vector<Mat> planos;

int gammaL_slider = 2, gammaH_slider = 20, C_slider = 1, D0_slider = 5;
const int gammaL_max = 10, gammaH_max = 50, C_max = 100, D0_max = 200;
double gammaL, gammaH, C, D0;

int dft_M, dft_N;

// troca os quadrantes da imagem da DFT
void deslocaDFT(cv::Mat &image)
{
    Mat tmp, A, B, C, D;

    // se a imagem tiver tamanho impar, recorta a regiao para
    // evitar cópias de tamanho desigual
    image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
    int cx = image.cols / 2;
    int cy = image.rows / 2;

    // reorganiza os quadrantes da transformada
    // A B   ->  D C
    // C D       B A
    A = image(Rect(0, 0, cx, cy));
    B = image(Rect(cx, 0, cx, cy));
    C = image(Rect(0, cy, cx, cy));
    D = image(Rect(cx, cy, cx, cy));

    // A <-> D
    A.copyTo(tmp);
    D.copyTo(A);
    tmp.copyTo(D);

    // C <-> B
    C.copyTo(tmp);
    B.copyTo(C);
    tmp.copyTo(B);
}

void filtragem()
{
    // parte imaginaria da matriz complexa (preenchida com zeros)
    zeros = Mat_<float>::zeros(padded.size());
    ones = Mat_<float>::zeros(padded.size());

    // prepara a matriz complexa para ser preenchida
    complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

    // limpa o array de matrizes que vao compor a
    // imagem complexa
    planos.clear();
    // cria a compoente real
    realInput = Mat_<float>(padded);

    //log
    realInput += Scalar::all(1);
    log(realInput, realInput);

    // insere as duas componentes no array de matrizes
    planos.push_back(realInput);
    planos.push_back(zeros);

    // combina o array de matrizes em uma unica
    // componente complexa
    merge(planos, complexImage);

    // calcula o dft
    dft(complexImage, complexImage);
    // realiza a troca de quadrantes
    deslocaDFT(complexImage);
    resize(complexImage, complexImage, padded.size());
    normalize(complexImage, complexImage, 0, 1, NORM_MINMAX);

    // a função de transferencia (filtro de frequencia) deve ter o
    // mesmo tamanho e tipo da matriz complexa
    filter = complexImage.clone();

    // cria uma matriz temporária para criar as componentes real
    // e imaginaria do filtro passa alta gaussiano
    tmp = Mat(dft_M, dft_N, CV_32F);
    float D;
    // prepara o filtro passa-alta ideal
    for (int i = 0; i < dft_M; i++)
    {
        for (int j = 0; j < dft_N; j++)
        {
            D = (i - dft_M / 2) * (i - dft_M / 2) + (j - dft_N / 2) * (j - dft_N / 2);
            tmp.at<float>(i, j) = (gammaH - gammaL) * (1 - exp(-C * (D / (D0 * D0)))) + gammaL;
        }
    }

    // cria a matriz com as componentes do filtro e junta
    // ambas em uma matriz multicanal complexa
    Mat comps[] = {tmp, tmp};
    merge(comps, 2, filter);

    // aplica o filtro de frequencia
    mulSpectrums(complexImage, filter, complexImage, 0);

    // troca novamente os quadrantes
    deslocaDFT(complexImage);

    // calcula a DFT inversa
    idft(complexImage, complexImage);

    // limpa o array de planos
    planos.clear();

    // separa as partes real e imaginaria da
    // imagem filtrada
    split(complexImage, planos);

    exp(planos[0], planos[0]);
    // normaliza a parte real para exibicao
    normalize(planos[0], planos[0], 0, 1, NORM_MINMAX);
    imFiltrado = planos[0].clone();
}

void on_trackbar(int, void *)
{
    gammaL = (double)gammaL_slider / 10;
    gammaH = (double)gammaH_slider / 10;
    C = (double)C_slider;
    D0 = (double)D0_slider;
    filtragem();
    imshow("Homomorphic", imFiltrado);
}

int main(int argc, char **argv)
{

    // valores ideais dos tamanhos da imagem
    // para calculo da DFT

    image = imread(argv[1], IMREAD_GRAYSCALE);

    namedWindow("Homomorphic", WINDOW_NORMAL);
    imshow("Original", image);
    // identifica os tamanhos otimos para
    // calculo do FFT
    dft_M = getOptimalDFTSize(image.rows);
    dft_N = getOptimalDFTSize(image.cols);

    // realiza o padding da imagem
    copyMakeBorder(image, padded, 0, dft_M - image.rows, 0,
                   dft_N - image.cols, BORDER_CONSTANT,
                   Scalar::all(0));

    char TrackbarName[50];

    sprintf(TrackbarName, "Gamma L x %d", gammaL_max);
    createTrackbar(TrackbarName, "Homomorphic", &gammaL_slider, gammaL_max, on_trackbar);

    sprintf(TrackbarName, "Gamma H x %d", gammaH_max);
    createTrackbar(TrackbarName, "Homomorphic", &gammaH_slider, gammaH_max, on_trackbar);

    sprintf(TrackbarName, "C x %d", C_max);
    createTrackbar(TrackbarName, "Homomorphic", &C_slider, C_max, on_trackbar);

    sprintf(TrackbarName, "D0 x %d", D0_max);
    createTrackbar(TrackbarName, "Homomorphic", &D0_slider, D0_max, on_trackbar);
    
    waitKey(0);
    return 0;
}