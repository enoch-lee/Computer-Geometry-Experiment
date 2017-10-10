#include"stdafx.h"
#include"dlt.h"

#define MAX_NUM 20
using namespace cv;
using namespace std;


Mat img_src;
Mat img_dst;
vector<Point2f> points_src;
vector<Point2f> points_dst;

int index00 = 0;

void CallBackFunc_src(int event, int x, int y, int flags, void* userdata)
{

	if (event == EVENT_LBUTTONDOWN)
	{

		if (index00 > MAX_NUM-1) {
			return;
		}
		else {
			points_src.push_back(Point2f((float)x, (float)y));
			circle(img_src, Point(x, y), 2, CV_RGB(255, 0, 0),-1);
		}
		index00++;
		cout << "Point " <<index00<<"- position (" << x << ", " << y << ")" << endl;
	}

	imshow("img_src", img_src);
}

int index01 = 0;

void CallBackFunc_dst(int event, int x, int y, int flags, void* userdata)
{

	if (event == EVENT_LBUTTONDOWN)
	{

		if (index01 > MAX_NUM-1) {
			return;
		}
		else {
			points_dst.push_back(Point2f((float)x, (float)y));
			circle(img_dst, Point(x, y), 2, CV_RGB(255, 0, 0), -1);
		}
		index01++;
		cout << "Point " << index01 << "- position (" << x << ", " << y << ")" << endl;
	}
	imshow("img_dst", img_dst);

}

Mat myWarpPerspective(Mat img_src, Mat H, const string& filename) {

	int i, j;
	vector<Point3d> src_coord;
	//create image coordinate
	src_coord.push_back(Point3d(0, 0, 1));
	src_coord.push_back(Point3d(0, img_src.rows - 1, 1));
	src_coord.push_back(Point3d(img_src.cols - 1, 0, 1));
	src_coord.push_back(Point3d(img_src.cols - 1, img_src.rows - 1, 1));

	Mat mat_src_coord = Mat(src_coord, true);
	//cout << mat_image_coord << endl;
	mat_src_coord = mat_src_coord.reshape(1, 4);
	//use certain homography H to transfer image coordinates to the world coordinate

	/*
	**Not H.inv()
	**Not remove distrotion 
	*/
	Mat mat_dst_coord = H.inv()*mat_src_coord.t();

	//cout << mat_image_coord << endl;
	//cout << mat_image_coord.t() << endl;


	// set world coordnates in inhomogeneous form
	mat_dst_coord.row(0) = mat_dst_coord.row(0).mul(1 / mat_dst_coord.row(2));
	mat_dst_coord.row(1) = mat_dst_coord.row(1).mul(1 / mat_dst_coord.row(2));
	mat_dst_coord.row(2) = 1;

	// get the boundaries of the world coordinate
	double xmin, xmax, ymin, ymax;
	minMaxLoc(mat_dst_coord.row(0), &xmin, &xmax, 0, 0);
	minMaxLoc(mat_dst_coord.row(1), &ymin, &ymax, 0, 0);

	// detemine the size of corrected image
	// keep image width
	double scale_factor = img_src.cols / (xmax - xmin);
	int height = (int)((ymax - ymin) * scale_factor);

	// create corrected image 
	Mat image_corrected(height, img_src.cols, CV_8UC3);
	// Find values for corrected image by taking set of coordinates from world plane
	// to image plane and interpolating values if necessary

	Mat points_world_temp(3, 1, CV_64FC1);
	Mat points_image_temp(3, 1, CV_64FC1);

	// Loop through size of corrected image at step = 1/scale
	points_world_temp.at<double>(2, 0) = 1; // Set third component to 1
	double step = 1.0 / scale_factor;

	for (j = 0; j < image_corrected.cols; j++) {
		// set x coordinate
		points_world_temp.at<double>(0, 0) = (double)j * step + xmin;
		for (i = 0; i < image_corrected.rows; i++) {
			double x, y;
			// set y coordinate
			points_world_temp.at<double>(1, 0) = (double)i * step + ymin;
			// find point(x,y) in image plane
			points_image_temp = H * points_world_temp;
			// set image points in inhomogeneous form
			x = points_image_temp.at<double>(0, 0) / points_image_temp.at<double>(2, 0);
			y = points_image_temp.at<double>(1, 0) / points_image_temp.at<double>(2, 0);
			// check if points cross image boundaries
			if (x < 0 || x > img_src.cols - 1 || y < 0 || y > img_src.rows - 1) {
				image_corrected.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
				continue;
			}

			//obtain four points arount (x,y)
			double x1 = floor(x);
			double y1 = floor(y);
			double x2 = ceil(x);
			double y2 = ceil(y);

			// If x or y is not integer, interpolate pixel value using bilinear interpolation
			if (x - (int)x != 0.0 || (y - (int)y) != 0.0) {
				image_corrected.at<Vec3b>(i, j) =
					(x2 - x)*(y2 - y) / ((x2 - x1)*(y2 - y1))*img_src.at<Vec3b>(int(y1), int(x1)) +
					(x - x1)*(y2 - y) / ((x2 - x1)*(y2 - y1))*img_src.at<Vec3b>(int(y1), int(x2)) +
					(x2 - x)*(y - y1) / ((x2 - x1)*(y2 - y1))*img_src.at<Vec3b>(int(y2), int(x1)) +
					(x - x1)*(y - y1) / ((x2 - x1)*(y2 - y1))*img_src.at<Vec3b>(int(y2), int(x2));
			}
			else {
				image_corrected.at<Vec3b>(i, j) = img_src.at<Vec3b>(int(y), int(x));
			}
		}
	}
	imwrite(filename, image_corrected);
	cout << "Corrected Image Saved !" << endl;
	return image_corrected;
}


//一定要注意 数组初始化 是否与问题一致！！！！
//MAX_NUM=4 但是线性方程数是8个！！！
double A1[2*MAX_NUM][9];
double A2[2 * MAX_NUM][9];
Mat build_A_matrix() {

	for (int i = 0; i < 2*MAX_NUM; i += 2) {
		A1[i][0] = 0;
		A1[i][1] = 0;
		A1[i][2] = 0;
		A1[i][3] = -points_src[i / 2].x;
		A1[i][4] = -points_src[i / 2].y;
		A1[i][5] = -1.;
		A1[i][6] = points_dst[i / 2].y*points_src[i / 2].x;
		A1[i][7] = points_dst[i / 2].y*points_src[i / 2].y;
		A1[i][8] = points_dst[i / 2].y;

		A1[i + 1][0] = points_src[i / 2].x;;
		A1[i + 1][1] = points_src[i / 2].y;
		A1[i + 1][2] = 1.;
		A1[i + 1][3] = 0;
		A1[i + 1][4] = 0;
		A1[i + 1][5] = 0;
		A1[i + 1][6] = -1.*points_dst[i / 2].x*points_src[i / 2].x;
		A1[i + 1][7] = -1.*points_dst[i / 2].x*points_src[i / 2].y;
		A1[i + 1][8] = -1.*points_dst[i / 2].x;
	}

	Mat A(2*MAX_NUM, 9, CV_64F, A1);
	return A;
}

Mat build_A_matrix(vector<Point2f> norm_pts_src, vector<Point2f> norm_pts_dst) {

	for (int i = 0; i < 2 * MAX_NUM; i += 2) {
		A2[i][0] = 0;
		A2[i][1] = 0;
		A2[i][2] = 0;
		A2[i][3] = -1.*norm_pts_src[i / 2].x;
		A2[i][4] = -norm_pts_src[i / 2].y;
		A2[i][5] = -1.;
		A2[i][6] = norm_pts_dst[i / 2].y*norm_pts_src[i / 2].x;
		A2[i][7] = norm_pts_dst[i / 2].y*norm_pts_src[i / 2].y;
		A2[i][8] = norm_pts_dst[i / 2].y;

		A2[i + 1][0] = norm_pts_src[i / 2].x;;
		A2[i + 1][1] = norm_pts_src[i / 2].y;
		A2[i + 1][2] = 1.;
		A2[i + 1][3] = 0;
		A2[i + 1][4] = 0;
		A2[i + 1][5] = 0;
		A2[i + 1][6] = -norm_pts_dst[i / 2].x*norm_pts_src[i / 2].x;
		A2[i + 1][7] = -norm_pts_dst[i / 2].x*norm_pts_src[i / 2].y;
		A2[i + 1][8] = -norm_pts_dst[i / 2].x;
	}

	Mat A(2 * MAX_NUM, 9, CV_64F, A2);
	return A;
}
//get denormalization matrix T
Mat denorm_mat(vector<Point2f> pts) {
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


//normalize points
vector<Point2f> norm_pts(vector<Point2f> pts) {
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
//select 20 points from src image and dst image
void set_pts() {

	points_src.push_back(Point2f(1723, 496));
	points_src.push_back(Point2f(1802, 480));
	points_src.push_back(Point2f(1809, 625));
	points_src.push_back(Point2f(1729, 641));


	points_src.push_back(Point2f(1819, 494));
	points_src.push_back(Point2f(1851, 488));
	points_src.push_back(Point2f(1823, 581));
	points_src.push_back(Point2f(1858, 574));
	points_src.push_back(Point2f(1765, 655));
	points_src.push_back(Point2f(1838, 642));

	points_src.push_back(Point2f(1902, 1112));
	points_src.push_back(Point2f(1987, 1113));
	points_src.push_back(Point2f(1987, 1172));
	points_src.push_back(Point2f(1900, 1172));
	points_src.push_back(Point2f(1925, 1043));
	points_src.push_back(Point2f(1966, 1042));
	points_src.push_back(Point2f(1967, 1073));
	points_src.push_back(Point2f(1925, 1072));
	points_src.push_back(Point2f(1773, 1192));
	points_src.push_back(Point2f(1797, 1189));


	points_dst.push_back(Point2f(156, 489));
	points_dst.push_back(Point2f(238, 485));
	points_dst.push_back(Point2f(228, 632));
	points_dst.push_back(Point2f(146, 635));

	points_dst.push_back(Point2f(255, 502));
	points_dst.push_back(Point2f(286, 500));
	points_dst.push_back(Point2f(249, 588));
	points_dst.push_back(Point2f(282, 586));
	points_dst.push_back(Point2f(181, 655));
	points_dst.push_back(Point2f(254, 652));

	points_dst.push_back(Point2f(275, 1119));
	points_dst.push_back(Point2f(356, 1120));
	points_dst.push_back(Point2f(354, 1176));
	points_dst.push_back(Point2f(271, 1181));
	points_dst.push_back(Point2f(303, 1052));
	points_dst.push_back(Point2f(344, 1052));
	points_dst.push_back(Point2f(342, 1082));
	points_dst.push_back(Point2f(301, 1080));
	points_dst.push_back(Point2f(136, 1203));
	points_dst.push_back(Point2f(156, 1203));
}


//build jacobain matrix namely partial-derivative matrix
Mat jacob(Point2f point_src, Point2f point_dst, Mat mat_h) {
	vector<double> h;
	Mat jacob(2,4,CV_64F);
	mat_h.col(0).copyTo(h);
	jacob.at<double>(0, 0) = -1. * h[3] + point_dst.y*h[6];
	jacob.at<double>(0, 1) = -1. * h[4] + point_dst.y*h[7];
	jacob.at<double>(0, 2) = 0;
	jacob.at<double>(0, 3) = point_src.x * h[6] + point_src.y*h[7] + h[8];
	jacob.at<double>(1, 0) = h[0] - point_dst.x*h[6];
	jacob.at<double>(1, 1) = h[1] - point_dst.x*h[7];
	jacob.at<double>(1, 2) = -1. * point_src.x * h[6] - point_src.y*h[7] - h[8];
	jacob.at<double>(1, 3) = 0;
	return jacob;
}

Mat jacob_h(vector<Point2f> point_src, vector<Point2f> point_dst) {
	//vector<double> h;
	Mat jacob = Mat::zeros(2 * MAX_NUM, 9, CV_64F);
	//mat_h.col(0).copyTo(h);
	int n = 0;
	for (int i = 0; i < point_src.size(); ++i) {
		jacob.at<double>(n, 3) = -1. * point_src[i].x;
		jacob.at<double>(n, 4) = -1. * point_src[i].y;
		jacob.at<double>(n, 5) = -1.;
		jacob.at<double>(n, 6) = point_dst[i].y * point_src[i].x;
		jacob.at<double>(n, 7) = point_dst[i].y * point_src[i].y;
		jacob.at<double>(n, 8) = point_dst[i].y;

		jacob.at<double>(n+1, 0) = point_src[i].x;
		jacob.at<double>(n+1, 1) = point_src[i].y;
		jacob.at<double>(n+1, 2) = 1;
		jacob.at<double>(n+1, 6) = -1.*point_dst[i].x * point_src[i].x;
		jacob.at<double>(n+1, 7) = -1.*point_dst[i].x * point_src[i].y;
		jacob.at<double>(n+1, 8) = -1.*point_dst[i].x;
		n += 2;
	}

	return jacob;
}
//initialize CH(x)
Mat ini_Ch(Mat A, Mat h, int i) {
	Mat Ai(2, 9, CV_64F);
	A.row(i).copyTo(Ai.row(0));
	A.row(i + 1).copyTo(Ai.row(1));
	//cout << Ai << endl;
	Mat Ch = Ai*h;
	return Ch;
	//Ai.inv(DECOMP_SVD);
	//invert
}

double sampson_error(Mat A, Mat h, vector<Point2f> points_src, vector<Point2f> points_dst) {
	double sum_sampson_err = 0;
	Mat sampson_err;
	Mat E;
	Mat J,T;
	int n = 0;
	//cout << "h=" << h << endl;
	for (int i = 0; i < points_src.size(); ++i) {
		E = ini_Ch(A, h, n);
		//cout << E << endl;
		J = jacob(points_src[i], points_dst[i], h);
		//cout << J << endl;
		T = J*J.t();
		sampson_err = E.t()*T.inv()*E;

		//cout << sampson_err << endl;
		sum_sampson_err += sqrt(sampson_err.at<double>(0, 0));
		//sampsonDistance(points_src, points_src, h);
		n += 2;
	}
	//matMulDeriv(A, h, J, T);
	//cout << "J=\n" << T << endl;
	//cout << "sampsonDistance" << sampsonDistance(Point3d(points_src[0].x,1,1), Point3d(points_src[0].x, 1, 1), h) << endl;
	//cout << "sum_sampson_err="<< sum_sampson_err << endl;
	return sum_sampson_err;
}

Mat p_inverse(Mat A) {
	SVD svd(A);
	Mat pinvA = svd.vt.t()*Mat::diag(1. / svd.w)*svd.u.t();
	return pinvA;
}

Mat newton_iteration(Mat A, Mat h, vector<Point2f> points_src, vector<Point2f> points_dst) {
	//vector<double> h;
	//vector<vector<double>> A;
	//mat_h.col(0).copyTo(h);
	//A = mat2vector(mat_A);
	double d = 0.01;
	double sampson_err;
	Mat J, pinvJ, e, delta;
	Mat final_H;
	//J = jacob_h(points_src, points_dst);
	invert(A, pinvJ, DECOMP_SVD);
	//pinvJ = p_inverse(A);
	//cout << "s-inverse" << pinvJ <<endl;
	int num = 0;
	do {
		//cout << "sum_sampson_err=" << sampson_err << endl;
		//cout << "sum_sampson_err=" << sampson_err << endl;
		//cout << J << endl;
		//cout << A << endl;

		//cout << "h=" << h << endl;
		e = A*h;
		//cout << "e=\n" << e << endl;
		delta = -1 * pinvJ * e;
		//cout << "delta=" << delta << endl;
		//cout << J << endl;
		//cout << e << endl;
		//cout << "delta=" << sampson_error(A, h, points_src, points_dst) << endl;
		h += delta;
		
		++num;
		//if (num > 1000) {
		//	cout << "cannot converge!" << endl;
		//}
		sampson_err = sampson_error(A, h, points_src, points_dst);
		cout << "sum_sampson_err=" << sampson_err << endl;
	} while (sampson_err > d && num < 500);
	//cout << "h=" << h << endl;
	final_H = (Mat_<double>(3, 3) << 
		h.at<double>(0, 0), h.at<double>(1, 0), h.at<double>(2, 0),
		h.at<double>(3, 0), h.at<double>(4, 0), h.at<double>(5, 0),
		h.at<double>(6, 0), h.at<double>(7, 0), h.at<double>(8, 0));
	final_H = final_H / h.at<double>(8, 0);
	cout << "final_H=" << final_H << endl;
	return final_H;
}

//transfer mat to 2D vector
vector<vector<double>> mat2vector(Mat A){
	vector<vector<double>> vec_A;
	vector<double> vec_A_rows;
	for (int i = 0; i < A.rows; ++i) {
		A.row(i).copyTo(vec_A_rows);
		vec_A.push_back(vec_A_rows);
	}
	return vec_A;
}


inline Point2f mat2pts2f(Mat pts) {
	Point2f t(pts.at<double>(0, 0)/ pts.at<double>(2, 0), pts.at<double>(1, 0)/ pts.at<double>(2, 0));
	return t;
}

//calculate symmetric transfer error
//H is 3*3 homography
double sym_error(Point2f src_pts, Point2f dst_pts, Mat H) {

	Mat src = (Mat_<double>(3, 1) << src_pts.x, src_pts.y, 1);
	Mat dst = (Mat_<double>(3, 1) << dst_pts.x, dst_pts.y, 1);
	//cout << src << endl;
	Mat dst_p = H.inv()*dst;

	Mat src_p = H*src;

	Point2f sym_dst = mat2pts2f(src_p);
	Point2f sym_src = mat2pts2f(dst_p);

	double d1 = pow((src_pts.x - sym_src.x), 2) + pow((src_pts.y - sym_src.y), 2);
	double d2 = pow((dst_pts.x - sym_dst.x), 2) + pow((dst_pts.y - sym_dst.y), 2);

	return sqrt(d1 + d2);
}

//double A3[8][9];
Mat build_A_matrix_ransac(vector<Point2f> norm_pts_src, vector<Point2f> norm_pts_dst) {
	//int num_pts = norm_pts_src.size();
	Mat A = Mat::zeros(2 * (int)norm_pts_src.size(), 9, CV_64F);
	for (int i= 0; i < 2 * norm_pts_src.size(); i += 2) {
		//A.at<double>(i, 0) = 0;
		//A.at<double>(i, 1) = 0;
		//A.at<double>(i, 2) = 0;
		A.at<double>(i, 3) = -1.*norm_pts_src[i / 2].x;
		A.at<double>(i, 4) = -norm_pts_src[i / 2].y;
		A.at<double>(i, 5) = -1.;
		A.at<double>(i, 6) = norm_pts_dst[i / 2].y*norm_pts_src[i / 2].x;
		A.at<double>(i, 7) = norm_pts_dst[i / 2].y*norm_pts_src[i / 2].y;
		A.at<double>(i, 8) = norm_pts_dst[i / 2].y;

		A.at<double>(i + 1, 0) = norm_pts_src[i / 2].x;
		A.at<double>(i + 1, 1) = norm_pts_src[i / 2].y;
		A.at<double>(i + 1, 2) = 1.;
		//A.at<double>(i + 1, 0) = 0;
		//A.at<double>(i + 1, 0) = 0;
		//A.at<double>(i + 1, 0) = 0;
		A.at<double>(i + 1, 6) = -norm_pts_dst[i / 2].x*norm_pts_src[i / 2].x;
		A.at<double>(i + 1, 7) = -norm_pts_dst[i / 2].x*norm_pts_src[i / 2].y;
		A.at<double>(i + 1, 8) = -norm_pts_dst[i / 2].x;
	}
	

	return A;
}

//using 4 points to find Homography by DLT
Mat findH_dlt(vector<Point2f> src,vector<Point2f> dst) {

	Mat A = build_A_matrix_ransac(src, dst);
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