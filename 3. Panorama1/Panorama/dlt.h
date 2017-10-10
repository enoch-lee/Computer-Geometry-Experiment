#pragma once
#define DLT
#ifdef DLT

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/calib3d.hpp>
#include<opencv2/stitching.hpp>
#include<opencv2/imgcodecs.hpp>

#include "opencv2/features2d.hpp"
//#include "opencv2/xfeatures2d.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"

#include<iostream>
#include<cmath>
#include<stdlib.h>
#include<ctime>
#include<random>
#include<chrono>

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;


//void Get_global_var();

extern Mat img_src;
extern Mat img_dst;
extern vector<Point2f> points_src;
extern vector<Point2f> points_dst;

void CallBackFunc_src(int event, int x, int y, int flags, void* userdata);
void CallBackFunc_dst(int event, int x, int y, int flags, void* userdata);
//Mat myWarpPerspective(Mat img_original, Mat H, const string& filename);

Mat build_A_matrix();
Mat build_A_matrix(vector<Point2f> norm_pts_src, vector<Point2f> norm_pts_dst);

void set_pts();

vector<Point2f> norm_pts(vector<Point2f> pts);
Mat denorm_mat(vector<Point2f> pts);

double sampson_error(Mat A, Mat h,  vector<Point2f> points_src, vector<Point2f> points_dst);
Mat ini_Ch(Mat A, Mat h, int i);
Mat jacob(Point2f point_src, Point2f point_dst, Mat mat_h);
Mat jacob_h(vector<Point2f> point_src, vector<Point2f> point_dst );


Mat newton_iteration(Mat A, Mat h, vector<Point2f> points_src, vector<Point2f> points_dst);
Mat findH_dlt(vector<Point2f> src, vector<Point2f> dst);
double sym_error(Point2f src_pts, Point2f dst_pts, Mat H);

//vector<vector<double>> mat2vector(Mat A);

#endif // DLT