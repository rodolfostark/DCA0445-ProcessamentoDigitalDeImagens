#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv){
  Mat image;
  image = imread(argv[1], IMREAD_GRAYSCALE);
  imshow("foto biel", image);
  waitKey();
  return 0;
}