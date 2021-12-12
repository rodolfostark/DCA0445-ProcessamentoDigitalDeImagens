#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
  Mat image, realce;
  int width, height;
  int nobjects, nobjectsholes;

  Point p;
  image = imread(argv[1], cv::IMREAD_GRAYSCALE);

  if (!image.data)
  {
    cout << "imagem nao carregou corretamente" << endl;
    return -1;
  }

  width = image.cols;
  height = image.rows;
  cout << "Dimensoes da imagem: " << width << "x" << height << endl;

  p.x = 0;
  p.y = 0;
  // Removendo as bolhas nas bordas verticais da imagem
  for (int i = 0; i < height; i++)
  {
    if (image.at<uchar>(i, 0) == 255)
    {
      p.x = 0;
      p.y = i;
      floodFill(image, p, 0);
    }
    if (image.at<uchar>(i, width - 1) == 255)
    {
      p.x = width - 1;
      p.y = i;
      floodFill(image, p, 0);
    }
  }
  // Removendo as bolhas nas bordas horizontais da imagem
  for (int i = 0; i < width; i++)
  {
    if (image.at<uchar>(0, i) == 255)
    {
      p.x = i;
      p.y = 0;
      floodFill(image, p, 0);
    }
    if (image.at<uchar>(height - 1, i) == 255)
    {
      p.x = i;
      p.y = width - 1;
      floodFill(image, p, 0);
    }
  }
  // busca objetos presentes
  nobjects = 0;
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (image.at<uchar>(i, j) == 255)
      {
        // achou um objeto
        nobjects++;
        p.x = j;
        p.y = i;
        // preenche o objeto com o contador
        cv::floodFill(image, p, nobjects);
      }
    }
  }
  p.x = 0;
  p.y = 0;
  floodFill(image, p, 255);
  nobjectsholes = 0;
  Point pAnterior;
  pAnterior.x = p.x;
  pAnterior.y = p.y;
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (image.at<uchar>(i, j) == 0)
      {
        // achou um objeto
        nobjectsholes++;
        p.x = j;
        p.y = i;
        pAnterior.y = p.y;
        pAnterior.x = p.x - 1;
        // preenche o objeto com o contador
        floodFill(image, p, 255);
        floodFill(image, pAnterior, 255);
      }
    }
  }
  cout << "a figura tem " << nobjects - nobjectsholes << " bolhas sem buracos" << endl;
  cout << "a figura tem " << nobjectsholes << " bolhas com buracos" << endl;
  
  equalizeHist(image, realce);
  imshow("image", image);
  imshow("realce", realce);
  imwrite("labeling.png", image);
  waitKey();
  
  return 0;
}
