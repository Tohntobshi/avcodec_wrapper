#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/core.hpp>
#include "src/videoEncoder.h"
#include "src/videoDecoder.h"


using std::cout;
using std::cin;

#define WIDTH 320
#define HEIGHT 240

int main()
{
  VideoEncoder encoder(WIDTH, HEIGHT, 150000, 30);

  VideoDecoder decoder(WIDTH, HEIGHT);

  cv::setUseOptimized(true);
  cv::VideoCapture cap("test.mp4");
  if(!cap.isOpened())
  {
    cout << "cant open test file\n";
    return 0;
  }

  cv::Mat image;

  while (true)
  {
    {
      if (!cap.read(image))
      {
        cout << "cant read\n";
        break;
      }
    }

    cv::imshow("Display Image", image);
    int rawSize = image.elemSize() * image.total();
    auto [encodedData, encodedSize] = encoder.encode(image.data, rawSize);

    cout << "Raw frame size " << rawSize << ", encoded frame size " << encodedSize << "\n";

    auto [decodedData, decodedSize] = decoder.decode(encodedData, encodedSize);
    cout << "Decoded frame size " << decodedSize << "\n";

    if (decodedSize != 0)
    {
      cv::Mat reconstructed(HEIGHT, WIDTH, CV_8UC3, decodedData);
      cv::imshow("Decoded Image", reconstructed);
    }
    
    delete[] encodedData;
    delete[] decodedData;
    cv::waitKey(0);
  }

  cap.release();
  cv::destroyAllWindows();
  return 0;
}


