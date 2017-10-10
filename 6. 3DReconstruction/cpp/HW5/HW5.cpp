// HW5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "comp_F.h"

#define  MAX_NUM 24

using namespace cv;
using namespace std;

Mat img_1;
Mat img_2;
vector<Point2f> points_1;
vector<Point2f> points_2;

vector<Point3d> points_wall_1;
vector<Point3d> points_wall_2;
vector<Point3d> points_floor_1;
vector<Point3d> points_floor_2;

int index0 = 0;
int index1 = 0;

void CallBackFunc_1(int event, int x, int y, int flags, void* userdata)
{

	if (event == EVENT_LBUTTONDOWN)
	{

		if (index0 > MAX_NUM - 1) {
			return;
		}
		else {
			points_1.push_back(Point2f((float)x, (float)y));
			circle(img_1, Point(x, y), 2, CV_RGB(255, 0, 0), -1);
		}
		index0++;

		cout << "points_1.push_back(Point2f(" << x << "," << y << "));" << endl;
	}

	imshow("image", img_1);
}

void CallBackFunc_2(int event, int x, int y, int flags, void* userdata)
{

	if (event == EVENT_LBUTTONDOWN)
	{

		if (index1 > MAX_NUM - 1) {
			return;
		}
		else {
			points_2.push_back(Point2f((float)x, (float)y));
			circle(img_2, Point(x, y), 2, CV_RGB(255, 0, 0), -1);
		}
		index1++;
		cout << "points_2.push_back(Point2f(" << x << "," << y << "));" << endl;
	}
	imshow("image", img_2);

}


void input_data() {
	/*points_1.push_back(Point2f(278, 175));
	points_1.push_back(Point2f(276, 298));
	points_1.push_back(Point2f(275, 429));
	points_1.push_back(Point2f(273, 551));
	points_1.push_back(Point2f(464, 157));
	points_1.push_back(Point2f(461, 306));
	points_1.push_back(Point2f(454, 465));
	points_1.push_back(Point2f(446, 611));
	points_1.push_back(Point2f(547, 405));
	points_1.push_back(Point2f(544, 438));
	points_1.push_back(Point2f(566, 407));
	points_1.push_back(Point2f(565, 443));
	points_1.push_back(Point2f(274, 577));
	points_1.push_back(Point2f(302, 589));
	points_1.push_back(Point2f(327, 598));
	points_1.push_back(Point2f(358, 610));
	points_1.push_back(Point2f(389, 622));
	points_1.push_back(Point2f(421, 632));
	points_1.push_back(Point2f(251, 586));
	points_1.push_back(Point2f(274, 597));
	points_1.push_back(Point2f(300, 608));
	points_1.push_back(Point2f(330, 619));
	points_1.push_back(Point2f(360, 632));
	points_1.push_back(Point2f(394, 645));

	points_2.push_back(Point2f(221, 30));
	points_2.push_back(Point2f(226, 183));
	points_2.push_back(Point2f(231, 340));
	points_2.push_back(Point2f(238, 488));
	points_2.push_back(Point2f(419, 51));
	points_2.push_back(Point2f(423, 175));
	points_2.push_back(Point2f(423, 306));
	points_2.push_back(Point2f(424, 433));
	points_2.push_back(Point2f(481, 240));
	points_2.push_back(Point2f(481, 264));
	points_2.push_back(Point2f(493, 239));
	points_2.push_back(Point2f(493, 262));
	points_2.push_back(Point2f(265, 510));
	points_2.push_back(Point2f(299, 498));
	points_2.push_back(Point2f(333, 487));
	points_2.push_back(Point2f(365, 477));
	points_2.push_back(Point2f(395, 468));
	points_2.push_back(Point2f(420, 458));
	points_2.push_back(Point2f(289, 524));
	points_2.push_back(Point2f(323, 510));
	points_2.push_back(Point2f(358, 499));
	points_2.push_back(Point2f(390, 487));
	points_2.push_back(Point2f(420, 477));
	points_2.push_back(Point2f(448, 467));*/

	points_1.push_back(Point2f(278, 177));
	points_1.push_back(Point2f(276, 300));
	points_1.push_back(Point2f(275, 429));
	points_1.push_back(Point2f(275, 553));
	points_1.push_back(Point2f(464, 155));
	points_1.push_back(Point2f(461, 308));
	points_1.push_back(Point2f(454, 467));
	points_1.push_back(Point2f(446, 612));
	points_1.push_back(Point2f(220, 533));
	points_1.push_back(Point2f(242, 540));
	points_1.push_back(Point2f(509, 633));
	points_1.push_back(Point2f(549, 648));
	points_1.push_back(Point2f(275, 579));
	points_1.push_back(Point2f(300, 589));
	points_1.push_back(Point2f(328, 598));
	points_1.push_back(Point2f(356, 609));
	points_1.push_back(Point2f(388, 620));
	points_1.push_back(Point2f(422, 633));
	points_1.push_back(Point2f(217, 595));
	points_1.push_back(Point2f(243, 607));
	points_1.push_back(Point2f(271, 618));
	points_1.push_back(Point2f(298, 631));
	points_1.push_back(Point2f(329, 644));
	points_1.push_back(Point2f(362, 658));

	points_2.push_back(Point2f(220, 31));
	points_2.push_back(Point2f(225, 180));
	points_2.push_back(Point2f(232, 341));
	points_2.push_back(Point2f(238, 488));
	points_2.push_back(Point2f(420, 51));
	points_2.push_back(Point2f(421, 175));
	points_2.push_back(Point2f(423, 308));
	points_2.push_back(Point2f(423, 431));
	points_2.push_back(Point2f(127, 520));
	points_2.push_back(Point2f(172, 509));
	points_2.push_back(Point2f(460, 420));
	points_2.push_back(Point2f(484, 413));
	points_2.push_back(Point2f(263, 511));
	points_2.push_back(Point2f(299, 498));
	points_2.push_back(Point2f(333, 489));
	points_2.push_back(Point2f(362, 476));
	points_2.push_back(Point2f(394, 467));
	points_2.push_back(Point2f(420, 457));
	points_2.push_back(Point2f(317, 538));
	points_2.push_back(Point2f(353, 523));
	points_2.push_back(Point2f(388, 510));
	points_2.push_back(Point2f(420, 498));
	points_2.push_back(Point2f(449, 487));
	points_2.push_back(Point2f(474, 476));

	cout << points_1 << endl;
	cout << endl;
	cout << points_2 << endl;
}

void read_data() {
	img_1 = imread("image01.jpg", 1);
	img_2 = imread("image02.jpg", 1);
	namedWindow("image", CV_WINDOW_AUTOSIZE);
	resize(img_2, img_2, Size(img_2.cols / 3, img_2.rows / 3));
	resize(img_1, img_1, Size(img_1.cols / 3, img_1.rows / 3));

	//resizeWindow("image", img_1.cols / 3, img_1.rows / 3);
	setMouseCallback("image", CallBackFunc_2, NULL);
	cv::waitKey(0);

	//imwrite("image01_points.jpg", img_1);
}

void CallBackFunc_vanishPoints(int event, int x, int y, int flags, void* userdata)
{

	if (event == EVENT_LBUTTONDOWN)
	{

		if (index0 > 8 - 1) {
			return;
		}
		else {
			points_1.push_back(Point2f((float)x, (float)y));
			circle(img_2, Point(x, y), 2, CV_RGB(255, 0, 0), -1);
		}
		index0++;

		cout << "points_src.push_back(Point2f(" << x << "," << y << "));" << endl;
	}

	imshow("image", img_2);
}

void read_wall_floor_points() {

	img_1 = imread("image01.jpg", 1);
	img_2 = imread("image02.jpg", 1);
	namedWindow("image", CV_WINDOW_AUTOSIZE);
	resize(img_2, img_2, Size(img_2.cols / 3, img_2.rows / 3));
	resize(img_1, img_1, Size(img_1.cols / 3, img_1.rows / 3));

	//resizeWindow("image", img_1.cols / 3, img_1.rows / 3);
	setMouseCallback("image", CallBackFunc_vanishPoints, NULL);
	cv::waitKey(0);

	//imwrite("image02_points.jpg", img_2);



}


void input_wall_floor_points() {

	points_wall_1.push_back(Point3d(125, 196, 1));
	points_wall_1.push_back(Point3d(466, 156, 1));
	points_wall_1.push_back(Point3d(126, 293, 1));
	points_wall_1.push_back(Point3d(462, 307, 1));
	points_wall_1.push_back(Point3d(128, 399, 1));
	points_wall_1.push_back(Point3d(455, 467, 1));
	points_wall_1.push_back(Point3d(131, 499, 1));
	points_wall_1.push_back(Point3d(447, 610, 1));


	points_floor_1.push_back(Point3d(250, 586, 1));
	points_floor_1.push_back(Point3d(393, 645, 1));
	points_floor_1.push_back(Point3d(218, 597, 1));
	points_floor_1.push_back(Point3d(362, 660, 1));
	points_floor_1.push_back(Point3d(184, 605, 1));
	points_floor_1.push_back(Point3d(331, 673, 1));
	points_floor_1.push_back(Point3d(151, 615, 1));
	points_floor_1.push_back(Point3d(293, 688, 1));

	points_wall_2.push_back(Point3d(220, 32, 1));
	points_wall_2.push_back(Point3d(543, 65, 1));
	points_wall_2.push_back(Point3d(227, 181, 1));
	points_wall_2.push_back(Point3d(541, 171, 1));
	points_wall_2.push_back(Point3d(231, 341, 1));
	points_wall_2.push_back(Point3d(539, 286, 1));
	points_wall_2.push_back(Point3d(238, 485, 1));
	points_wall_2.push_back(Point3d(539, 395, 1));

	points_floor_2.push_back(Point3d(264, 511, 1));
	points_floor_2.push_back(Point3d(420, 457, 1));
	points_floor_2.push_back(Point3d(288, 524, 1));
	points_floor_2.push_back(Point3d(446, 466, 1));
	points_floor_2.push_back(Point3d(316, 537, 1));
	points_floor_2.push_back(Point3d(476, 476, 1));
	points_floor_2.push_back(Point3d(347, 552, 1));
	points_floor_2.push_back(Point3d(505, 487, 1));



}

void compute_vanish_plane() {

	vector<Point3d> lines_wall_1;
	vector<Point3d> lines_wall_2;
	vector<Point3d> lines_floor_1;
	vector<Point3d> lines_floor_2;

	vector<Point3d> vanish_points_wall_1;
	vector<Point3d> vanish_points_wall_2;
	vector<Point3d> vanish_points_floor_1;
	vector<Point3d> vanish_points_floor_2;



	for (int i = 0; i < points_wall_1.size()-1; i+=2) {
		lines_wall_1.push_back(points_wall_1[i].cross(points_wall_1[i + 1]));
		lines_floor_1.push_back(points_floor_1[i].cross(points_floor_1[i + 1]));
		lines_wall_2.push_back(points_wall_2[i].cross(points_wall_2[i + 1]));
		lines_floor_2.push_back(points_floor_2[i].cross(points_floor_2[i + 1]));
	}
	
	for (int i = 0; i < 3; ++i) {
		vanish_points_wall_1.push_back(lines_wall_1[i].cross(lines_wall_1[i + 1]));
		
		vanish_points_wall_2.push_back(lines_wall_2[i].cross(lines_wall_2[i + 1]));
		vanish_points_floor_1.push_back(points_floor_1[i].cross(points_floor_1[i + 1]));
		vanish_points_floor_2.push_back(points_floor_2[i].cross(points_floor_2[i + 1]));
	}

	cout << vanish_points_wall_1 << endl;

}


int main()
{
	//compute_vanish_plane();
	//read_wall_floor_points();
	//read_data();
	input_data();

	if (false)
	{
		vector<Point2f> norm_pts_1 = norm_pts(points_1);
		vector<Point2f> norm_pts_2 = norm_pts(points_2);
		//build T and T`
		Mat T_1 = denorm_mat(points_1);
		Mat T_2 = denorm_mat(points_2);

		Mat A = build_A_matrix(norm_pts_1, norm_pts_2);

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
		Mat F_ = (Mat_<double>(3, 3) << h.at<double>(0, 0), h.at<double>(1, 0), h.at<double>(2, 0),
			h.at<double>(3, 0), h.at<double>(4, 0), h.at<double>(5, 0),
			h.at<double>(6, 0), h.at<double>(7, 0), h.at<double>(8, 0));

		Mat F = T_2.inv()*F_*T_1;
		F = F / F.at<double>(2, 2);

		cout << "F= "<<F << endl;
	}


	Mat F = findFundamentalMat(points_1, points_2, CV_FM_8POINT);
	cout << "F="<< F << endl;
	//F.at<double>(8) = 1.;

	//Mat  D1, U1, Vt1;
	Mat  D2, U2, Vt2;

	//SVD::compute(F, D1, U1, Vt1, SVD::FULL_UV);

	SVD::compute(F.t(), D2, U2, Vt2, SVD::FULL_UV);
	Mat ep2 = Vt2.row(2);

	//cout << ep2 << endl;

	Mat M = skew_symmetric(ep2)*F;

	//cout << M << endl;
	Mat P2;
	hconcat(M, ep2.t(), P2);

	cout << "P2=" << P2 << endl;

	Mat P1 = Mat::zeros(3, 4, CV_64FC1);
	P1.at<double>(0, 0) = 1;
	P1.at<double>(1, 1) = 1;
	P1.at<double>(2, 2) = 1;

	cout << "P1=" << P1 << endl;

	Mat points_3d(4, 24, CV_64FC1);

	triangulatePoints(P1, P2, points_1, points_2, points_3d);

	cout << "points_3d= "<<points_3d << endl;

	while (true);
	return 0;
}