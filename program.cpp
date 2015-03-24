#include "opencv2/core/core_c.h"
#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"

#include <iostream>

using namespace cv;

int height, width, small_height, small_width;
Mat frame, gray, mask, edges, small, small_temp, temp, cartoonized;
VideoCapture cap;

void cartoonize() {

  cvtColor(frame, gray, CV_BGR2GRAY);
  medianBlur(gray, gray, 7);
  Laplacian(gray, edges, CV_8U, 5);
  threshold(edges, mask, 80, 255, THRESH_BINARY_INV);

  resize(frame, small, cv::Size(small_width, small_height), 0, 0, INTER_LINEAR);

  for (int i = 0; i < 7; ++i) {
    bilateralFilter(small, small_temp, 9, 9, 7);
    bilateralFilter(small_temp, small, 9, 9, 7);
  }

  resize(small, temp, cv::Size(width, height), 0, 0, INTER_LINEAR);
  cartoonized.setTo(0);
  temp.copyTo(cartoonized, mask);
}

int main(int argc, char** argv) {

  cap.open(0);

  height = (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT);
  width = (int)cap.get(CV_CAP_PROP_FRAME_WIDTH);

  cartoonized = Mat(height, width, CV_8UC3);
  temp = Mat(height, width, CV_8UC3);
  gray = Mat(height, width, CV_8U);
  edges = Mat(height, width, CV_8U);

  small_height = height / 4;
  small_width = width / 4;

  small = Mat(small_height, small_width, CV_8UC3);
  small_temp = Mat(small_height, small_width, CV_8UC3);

  while(true) {
    
    cap >> frame;
    
    cartoonize();

    cv::imshow("CARTOON", cartoonized);

    char key = waitKey(10);
    if (key == 27) break;
  }
  
  return 0;

}
