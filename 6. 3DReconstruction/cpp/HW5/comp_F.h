#pragma once
#define DLT
#ifdef DLT

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/calib3d.hpp>
#include <iostream>


using namespace cv;
using namespace std;

vector<Point2f> norm_pts(vector<Point2f> pts);

Mat denorm_mat(vector<Point2f> pts);

Mat findF_dlt(vector<Point2f> src, vector<Point2f> dst);

Mat build_A_matrix(vector<Point2f> norm_pts_1, vector<Point2f> norm_pts_2);

Mat skew_symmetric(Mat mat);

#endif