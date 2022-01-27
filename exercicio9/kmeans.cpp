#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <stdio.h>

using namespace cv;

int main(int argc, char **argv)
{
  int nClusters = 8;
  Mat rotulos;
  int nRodadas = 1;
  Mat centros;

  if (argc != 3)
  {
    exit(0);
  }
  Mat img = imread(argv[1], IMREAD_COLOR);
  Mat samples(img.rows * img.cols, 3, CV_32F);

  for (int y = 0; y < img.rows; y++)
  {
    for (int x = 0; x < img.cols; x++)
    {
      for (int z = 0; z < 3; z++)
      {
        samples.at<float>(y + x * img.rows, z) = img.at<Vec3b>(y, x)[z];
      }
    }
  }
  char nome1[] = "sushi_kmeans_n.jpg";

  for (int i = 0; i < 10; i++)
  {

    kmeans(samples,
           nClusters,
           rotulos,
           TermCriteria(TermCriteria::MAX_ITER | TermCriteria::EPS, 100, 0.1),
           nRodadas,
           KMEANS_RANDOM_CENTERS,
           centros);

    Mat rotulada(img.size(), img.type());
    for (int y = 0; y < img.rows; y++)
    {
      for (int x = 0; x < img.cols; x++)
      {
        int indice = rotulos.at<int>(y + x * img.rows, 0);
        rotulada.at<Vec3b>(y, x)[0] = (uchar)centros.at<float>(indice, 0);
        rotulada.at<Vec3b>(y, x)[1] = (uchar)centros.at<float>(indice, 1);
        rotulada.at<Vec3b>(y, x)[2] = (uchar)centros.at<float>(indice, 2);
      }
    }
    imshow("clustered image", rotulada);
    nome1[13] = i + '0';
    imwrite(nome1, rotulada);
    waitKey(0);
  }
}