#include <gtest/gtest.h>
#include <src/utils.h>
#include <opencv2/opencv.hpp>

using namespace cv;

TEST(Gaussian, KernelSumOne){
  float* kernel;
  int window;
  float sum = 0;

  createGaussianKernel(kernel,0.5,&window);
  for(int i = 0; i < window; i++){
    sum += kernel[i];
  }
  EXPECT_EQ(fabs(sum-1) < FLT_EPSILON, true);

  delete[] kernel;
}

TEST(Gaussian, KernelValues){
  float* kernel;
  int window;
  float expected[5]{0.0002638651, 0.1064507720, 0.7865707259, 0.1064507720, 0.0002638651};

  createGaussianKernel(kernel,0.5,&window);
  for(int i = 0; i < window; i++){
    EXPECT_EQ(fabs(expected[i]-kernel[i]) < FLT_EPSILON,true);
  }

  delete[] kernel;
}

TEST(Gaussian, KernelCreation) {
  float* kernel;
  int window;

  createGaussianKernel(kernel,2,&window);
  for(int i = 0; i < 7; i++){
    EXPECT_EQ(kernel[i],kernel[12-i]);
  }
  EXPECT_EQ(window, 13);

  delete[] kernel;
}

TEST(Gaussian, IsNonzero){
  std::string image_path = std::string(PROJECT_DIR) + "/tests/test.jpg";
  Mat img = cv::imread(image_path, IMREAD_GRAYSCALE);
  unsigned char* data = img.data;
  float sigma = 0.5;
  int rows = 256;
  int columns = 256;
  short int* smoothed_img;

  int sum = 0;

  gaussian(img.data,sigma,rows,columns,smoothed_img);

  for(int i = 0; i < (rows*columns); i++){
    sum += smoothed_img[i];
  }

  EXPECT_NE(sum,0);

  delete[] smoothed_img;
}

TEST(Gaussian, InRange){
  std::string image_path = std::string(PROJECT_DIR) + "/tests/test.jpg";
  Mat img = imread(image_path, IMREAD_GRAYSCALE);
  unsigned char* data = img.data;
  float sigma = 0.5;
  int rows = 256;
  int columns = 256;
  short int* smoothed_img;

  gaussian(img.data,sigma,rows,columns,smoothed_img);

  for(int i = 0; i < (rows*columns); i++){
    EXPECT_LE(smoothed_img[i], 255);
    EXPECT_GE(smoothed_img[i], 0);
  }

  delete[] smoothed_img;
}

TEST(Gaussian, GaussianDimensions){
  std::string image_path = std::string(PROJECT_DIR) + "/tests/test.jpg";
  Mat img = imread(image_path, IMREAD_GRAYSCALE);
  unsigned char* data = img.data;
  float sigma = 0.5;
  int rows = 256;
  int columns = 256;
  short int* smoothed_img;

  gaussian(img.data,sigma,rows,columns,smoothed_img);

  Mat smoothedMat(rows, columns, CV_16S, smoothed_img);
  EXPECT_EQ(smoothedMat.rows, rows);
  EXPECT_EQ(smoothedMat.cols, columns);
  
  delete[] smoothed_img;
}

TEST(Gradient, GradientDimensions){
  short int* grad_x;
  short int* grad_y;
  int rows = 3;
  int columns = 3;

  short int* smoothed_img = new short int[9]{1,2,1,2,3,2,3,4,3};
  calculateXYGradient(smoothed_img,rows,columns,grad_x,grad_y);

  Mat x(rows, columns, CV_16S, grad_x);
  EXPECT_EQ(x.rows, rows);
  EXPECT_EQ(x.cols, columns);

  Mat y(rows, columns, CV_16S, grad_y);
  EXPECT_EQ(y.rows, rows);
  EXPECT_EQ(y.cols, columns);

  delete[] grad_x;
  delete[] grad_y;
  delete[] smoothed_img;
}

TEST(Gradient, xOnes){
  short int* grad_x;
  short int* grad_y;
  int rows = 3;
  int columns = 3;

  short int* smoothed_img = new short int[9]{1,1,1,1,1,1,1,1,1};
  calculateXYGradient(smoothed_img,rows,columns,grad_x,grad_y);

  int expected[9]{0,0,0,0,0,0,0,0,0};

  for(int i = 0; i < 9; i++){
    EXPECT_EQ(grad_x[i],expected[i]);
    if(grad_x[i] != expected[i]){ std::cout << "X " << i << std::endl;}
  }

  delete[] grad_x;
  delete[] grad_y;
  delete[] smoothed_img;
}

TEST(Gradient, yOnes){
  short int* grad_x;
  short int* grad_y;
  int rows = 3;
  int columns = 3;

  short int* smoothed_img = new short int[9]{1,1,1,1,1,1,1,1,1};
  calculateXYGradient(smoothed_img,rows,columns,grad_x,grad_y);

  int expected[9]{0,0,0,0,0,0,0,0,0};

  for(int i = 0; i < 9; i++){
    EXPECT_EQ(grad_y[i],expected[i]);
    if(grad_y[i] != expected[i]){ std::cout << "Y " << i << std::endl;}
  }

  delete[] grad_x;
  delete[] grad_y;
  delete[] smoothed_img;
}

TEST(Gradient, xCorrect){
  short int* grad_x;
  short int* grad_y;
  int rows = 3;
  int columns = 3;

  short int* smoothed_img = new short int[9]{1,2,1,2,3,2,3,4,3};
  calculateXYGradient(smoothed_img,rows,columns,grad_x,grad_y);

  int expected[9]{3,0,-3,4,0,-4,3,0,-3};

  for(int i = 0; i < 9; i++){
    EXPECT_EQ(grad_x[i],expected[i]);
  }

  delete[] grad_x;
  delete[] grad_y;
  delete[] smoothed_img;
}

TEST(Gradient, yCorrect){
  short int* grad_x;
  short int* grad_y;
  int rows = 3;
  int columns = 3;

  short int* smoothed_img = new short int[9]{1,2,1,2,3,2,3,4,3};
  calculateXYGradient(smoothed_img,rows,columns,grad_x,grad_y);

  int expected[9]{3,4,3,6,8,6,3,4,3};

  for(int i = 0; i < 9; i++){
    EXPECT_EQ(grad_y[i],expected[i]);
  }

  delete[] grad_x;
  delete[] grad_y;
  delete[] smoothed_img;
}

TEST(SobelOperator, GradientDimensions){
  short int* img = new short int [9]{1,1,1,1,1,1,1,1,1};
  int rows = 3;
  int columns = 3;

  short int* magnitude;
  short int* angle;

  sobelOperator(img, rows, columns, magnitude, angle);

  Mat x(rows, columns, CV_16S, magnitude);
  EXPECT_EQ(x.rows, rows);
  EXPECT_EQ(x.cols, columns);

  Mat y(rows, columns, CV_16S, angle);
  EXPECT_EQ(y.rows, rows);
  EXPECT_EQ(y.cols, columns);

  delete[] magnitude;
  delete[] angle;
}


// TODO: Fix tests for refactored sobel operator
// TEST(SobelOperator, GradientCalculation){
//   short int* grad_x = new short int [9]{1,1,1,1,1,1,1,1,1};
//   short int* grad_y = new short int [9]{1,1,1,1,1,1,1,1,1};
//   int rows = 3;
//   int columns = 3;

//   short int expectation[9]{1,1,1,1,1,1,1,1,1};
//   short int* magnitude;
//   short int* angle;
//   sobelOperator(grad_x, grad_y, rows, columns, magnitude, angle);

//   for(int i = 0; i < (rows * columns); i++){
//     EXPECT_EQ(fabs(magnitude[i]-expectation[i]) < FLT_EPSILON, true);
//   }

//   delete[] magnitude;
//   delete[] angle;
// }

// TEST(SobelOperator, AngleCalculation){
//   short int* grad_x = new short int [5]{1,1,1,1,1};
//   short int* grad_y = new short int [5]{0,-1,1,3,-3};
//   int rows = 1;
//   int columns = 5;

//   short int expectation[5]{0,135,45,90,90};
//   short int* magnitude;
//   short int* angle;
//   sobelOperator(grad_x, grad_y, rows, columns, magnitude, angle);

//   for(int i = 0; i < (rows * columns); i++){
//     EXPECT_EQ(angle[i],expectation[i]);
//   }

//   delete[] grad_x;
//   delete[] grad_y;
//   delete[] angle;
// }

TEST(NonmaximalSuppression, SuppressionCalculation0){
  short int* grad = new short int[9]{0,0,0,0,10,0,50,20,50};
  short int* angle = new short int[9]{0,0,0,0,0,0,0,0,0};

  short int expectation[9]{0,0,0,0,10,0,50,0,50};

  int rows = 3;
  int columns = 3;
  short int* suppress;

  nonmaximalSuppression(grad,angle,rows,columns,suppress);

  for(int i = 0; i < rows*columns; i++){
    EXPECT_EQ(suppress[i],expectation[i]);
  }

  delete[] suppress;
}

TEST(NonmaximalSuppression, SuppressionCalculation45){
  short int* grad = new short int[9]{0,1,1,0,2,0,1,1,0};
  short int* angle = new short int[9]{0,45,45,45,45,45,45,45,0};

  short int expectation[9]{0,1,0,0,2,0,0,1,0};

  int rows = 3;
  int columns = 3;
  short int* suppress;

  nonmaximalSuppression(grad,angle,rows,columns,suppress);

  for(int i = 0; i < rows*columns; i++){
    EXPECT_EQ(suppress[i],expectation[i]);
  }

  delete[] suppress;
}

TEST(NonmaximalSuppression, SuppressionCalculation90){
  short int* grad = new short int[9]{1,0,0,0,1,0,0,0,1};
  short int* angle = new short int[9]{90,90,90,90,90,90,90,90,90};

  short int expectation[9]{1,0,0,0,1,0,0,0,1};

  int rows = 3;
  int columns = 3;
  short int* suppress;

  nonmaximalSuppression(grad,angle,rows,columns,suppress);

  for(int i = 0; i < rows*columns; i++){
    EXPECT_EQ(suppress[i],expectation[i]);
  }

  delete[] suppress;
}

TEST(NonmaximalSuppression, SuppressionCalculation135){
  short int* grad = new short int[9]{0,1,1,0,2,0,1,1,0};
  short int* angle = new short int[9]{135,135,0,135,135,135,0,135,135};

  short int expectation[9]{0,1,0,0,2,0,0,1,0};

  int rows = 3;
  int columns = 3;
  short int* suppress;

  nonmaximalSuppression(grad,angle,rows,columns,suppress);

  for(int i = 0; i < rows*columns; i++){
    EXPECT_EQ(suppress[i],expectation[i]);
  }
  
  delete[] suppress;
}

TEST(findEdgePixels,CorrectCalculations){
  short int* suppress = new short int[25]{
    5,6,0,5,5,
    4,1,0,1,4,
    1,3,7,0,0,
    10,9,8,0,0
  };

  short int expectation[25]{
    EDGE,EDGE,0,5,5,
    EDGE,1,0,1,4,
    1,EDGE,EDGE,0,0,
    EDGE,EDGE,EDGE,0,0
  };

  bool* visited = new bool[5*5];
  std::fill_n(visited,5*5,false);
  
  findEdgePixels(suppress,visited,1,2,10,5,5);

  for(int i = 0; i < 5*5; i++){
    EXPECT_EQ(suppress[i],expectation[i]);
  }

  delete[] suppress;
  delete[] visited;
}

TEST(Hysteresis, CorrectFunction){
  short int* suppress = new short int[25]{
    5,6,0,5,10,
    4,1,0,1,4,
    1,3,7,0,0,
    10,9,8,0,0
  };
  short int expectation[25]{
    EDGE,EDGE,0,EDGE,EDGE,
    EDGE,0,0,0,EDGE,
    0,EDGE,EDGE,0,0,
    EDGE,EDGE,EDGE,0,0
  };
  hysteresis(suppress,5,5,2,10);

  for(int i = 0; i < 5*5; i++){
    EXPECT_EQ(suppress[i], expectation[i]);
  }

  delete[] suppress;
}