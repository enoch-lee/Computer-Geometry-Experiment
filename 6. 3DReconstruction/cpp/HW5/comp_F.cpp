#include "stdafx.h"

#include "comp_F.h"

using namespace cv;
using namespace std;



vector<cv::Point2f> norm_pts(vector<Point2f> pts)
{
	vector<Point2f> norm_points;
	Point2f pts_temp;
	double pts_size = (double)pts.size();
	double sum_x = 0;
	double sum_y = 0;
	for (int i = 0; i < pts.size(); ++i) {
		sum_x += pts[i].x;
		sum_y += pts[i].y;
	}
	double mean_x = sum_x / pts_size;
	double mean_y = sum_y / pts_size;
	double sum_temp = 0;
	for (int i = 0; i < pts.size(); ++i) {
		sum_temp += pow(pts[i].x - mean_x, 2) + pow(pts[i].y - mean_y, 2);
	}
	double s = sqrt(sum_temp / (2 * pts_size));

	for (int i = 0; i < pts.size(); ++i) {
		pts_temp.x = (pts[i].x - mean_x) / s;
		pts_temp.y = (pts[i].y - mean_y) / s;
		norm_points.push_back(pts_temp);
	}
	return norm_points;
}

Mat denorm_mat(vector<Point2f> pts)
{
	double pts_size = (double)pts.size();
	double sum_x = 0;
	double sum_y = 0;
	for (int i = 0; i < pts.size(); ++i) {
		sum_x += pts[i].x;
		sum_y += pts[i].y;
	}
	double mean_x = sum_x / pts_size;
	double mean_y = sum_y / pts_size;
	double sum_temp = 0;
	for (int i = 0; i < pts.size(); ++i) {
		sum_temp += pow(pts[i].x - mean_x, 2) + pow(pts[i].y - mean_y, 2);
	}
	double s = sqrt(sum_temp / (2 * pts_size));
	Mat T = Mat::eye(3, 3, CV_64F);
	T.at<double>(0, 0) = 1. / s;
	T.at<double>(1, 1) = 1. / s;
	T.at<double>(0, 2) = -mean_x / s;
	T.at<double>(1, 2) = -mean_y / s;
	return T;
}

Mat build_A_matrix(vector<Point2f> norm_pts_1, vector<Point2f> norm_pts_2)
{
	Mat A = Mat::zeros((int)norm_pts_1.size(), 9, CV_64F);
	for (int i = 0; i < norm_pts_1.size(); ++i) {

		A.at<double>(i, 0) = norm_pts_1[i].x*norm_pts_2[i].x;
		A.at<double>(i, 1) = norm_pts_1[i].y*norm_pts_2[i].x;
		A.at<double>(i, 2) = norm_pts_2[i].x;
		A.at<double>(i, 3) = norm_pts_1[i].x*norm_pts_2[i].y;
		A.at<double>(i, 4) = norm_pts_1[i].y*norm_pts_2[i].y;
		A.at<double>(i, 5) = norm_pts_2[i].y;
		A.at<double>(i, 6) = norm_pts_1[i].x;
		A.at<double>(i, 7) = norm_pts_2[i].x;
		A.at<double>(i, 8) = 1.;
	}
	return A;
}



cv::Mat skew_symmetric(Mat mat)
{
	Mat skewSymmetric = (Mat_<double>(3, 3) <<
		0, -mat.at<double>(2), mat.at<double>(1),
		mat.at<double>(2), 0, -mat.at<double>(0),
		-mat.at<double>(1), mat.at<double>(0), 0);
	return skewSymmetric;
}

Mat findF_dlt(vector<Point2f> src, vector<Point2f> dst)
{
	Mat A = build_A_matrix(src, dst);
	//cout << A << endl;
	Mat U, D, Vt, H;
	//A is not a square matrix, need flag = SVD::FULL_UV
	SVD::compute(A, D, U, Vt, SVD::FULL_UV);
	//check the result of process
	//cout << "D=\n" << D << endl;
	//cout << "U=\n" << U << endl;
	//cout << "Vt=\n" << Vt << endl;

	//h is the last column of V
	Mat h = Vt.t().col(8);


	//h.at<double>(8, 0) = 0 could lead to serious problems
	H = (Mat_<double>(3, 3) << h.at<double>(0, 0), h.at<double>(1, 0), h.at<double>(2, 0),
		h.at<double>(3, 0), h.at<double>(4, 0), h.at<double>(5, 0),
		h.at<double>(6, 0), h.at<double>(7, 0), h.at<double>(8, 0));

	H = H / h.at<double>(8, 0);
	//cout << H << endl;
	return H;
}

