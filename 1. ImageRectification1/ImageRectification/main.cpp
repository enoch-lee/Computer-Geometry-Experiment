//rm_proj_distortion.cc
//Remove image projective distortion using four points
//By Zishuo Li 
//2/12/2017
//#include"stdafx.h"
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>


#include<iostream>
#include<conio.h>           

#define MATRIX_MAX 8
using namespace std;
using namespace cv;

Mat image_original;
double xi[4]; //points in image plane defined by the user
double yi[4];
double xd[4]; //points in the world plane
double yd[4];



//Set the world coordnates
void set_worldcoords() {

	xd[0] = xi[0];
	yd[0] = yi[0];

	xd[1] = xi[1];
	yd[1] = yi[0];

	xd[2] = xi[1];
	yd[2] = yi[2];

	xd[3] = xd[0];
	yd[3] = yd[2];
	return;
}

int index = 0;

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{

	if (event == EVENT_LBUTTONDOWN)
	{
		if (index > 3) return;
		xi[index] = x;
		yi[index] = y;
		//red circle used to determine four points in image coordnates
		circle(image_original, Point(x, y), 3, CV_RGB(255, 0, 0), -1);
		index++;
		cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	imshow("image_original", image_original);
}

void remove_distortion(Mat img_original, Mat H, const string& filename) {

	int i, j;
	vector<Point3d> image_coord;
	//create image coordinate
	image_coord.push_back(Point3d(0, 0, 1));
	image_coord.push_back(Point3d(0, img_original.rows - 1, 1));
	image_coord.push_back(Point3d(img_original.cols - 1, 0, 1));
	image_coord.push_back(Point3d(img_original.cols - 1, img_original.rows - 1, 1));

	Mat mat_image_coord = Mat(image_coord, true);
	//cout << mat_image_coord << endl;
	mat_image_coord = mat_image_coord.reshape(1, 4);
	//use certain homography H to transfer image coordinates to the world coordinate
	Mat mat_world_coord = H.inv()*mat_image_coord.t();

	//cout << mat_image_coord << endl;
	//cout << mat_image_coord.t() << endl;


	// set world coordnates in inhomogeneous form
	mat_world_coord.row(0) = mat_world_coord.row(0).mul(1 / mat_world_coord.row(2));
	mat_world_coord.row(1) = mat_world_coord.row(1).mul(1 / mat_world_coord.row(2));
	mat_world_coord.row(2) = 1;

	// get the boundaries of the world coordinate
	double xmin, xmax, ymin, ymax;
	minMaxLoc(mat_world_coord.row(0), &xmin, &xmax, 0, 0);
	minMaxLoc(mat_world_coord.row(1), &ymin, &ymax, 0, 0);

	// detemine the size of corrected image
	// maintain image width
	double scale_factor = img_original.cols / (xmax - xmin);
	int height = (int)((ymax - ymin) * scale_factor);

	// create corrected image 
	Mat image_corrected(height, img_original.cols, CV_8UC3);

	Mat points_world_temp(3, 1, CV_64FC1);
	Mat points_image_temp(3, 1, CV_64FC1);

	// Loop through corrected image at step = 1/scale
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
			if (x < 0 || x > img_original.cols - 1 || y < 0 || y > img_original.rows - 1) {
				continue;
			}

			//obtain four points around (x,y)
			double x1 = floor(x);
			double y1 = floor(y);
			double x2 = ceil(x);
			double y2 = ceil(y);

			// If x or y is not integer, interpolate pixel value using bilinear interpolation
			if (x - (int)x != 0.0 || (y - (int)y) != 0.0) {
				image_corrected.at<Vec3b>(i, j) =
					(x2 - x)*(y2 - y) / ((x2 - x1)*(y2 - y1))*img_original.at<Vec3b>(int(y1), int(x1)) +
					(x - x1)*(y2 - y) / ((x2 - x1)*(y2 - y1))*img_original.at<Vec3b>(int(y1), int(x2)) +
					(x2 - x)*(y - y1) / ((x2 - x1)*(y2 - y1))*img_original.at<Vec3b>(int(y2), int(x1)) +
					(x - x1)*(y - y1) / ((x2 - x1)*(y2 - y1))*img_original.at<Vec3b>(int(y2), int(x2));
			}
			else {
				image_corrected.at<Vec3b>(i, j) = img_original.at<Vec3b>(int(y), int(x));
			}
		}
	}

	// Save corrected image
	imwrite(filename, image_corrected);
	cout << "Corrected Image Saved !" << endl;

}


int main() {


	double A_matrix[MATRIX_MAX][MATRIX_MAX];
	double b_matrix[MATRIX_MAX];

	cout << "Please click left button to select 4 points clockwise in the image" << endl;
	image_original = cv::imread("image01.jpg", 1);          // open image

	if (image_original.empty()) {                               // if unable to open image
		std::cout << "error: image not read from file\n\n";     // show error message on command line
		_getch();                                               // may have to modify this line if not using Windows
		return(0);
	}



	//resize the original image make it suitable to display
	resize(image_original, image_original, Size(image_original.cols / 2, image_original.rows / 2));
	cv::namedWindow("image_original", CV_WINDOW_AUTOSIZE);
	cv::imshow("image_original", image_original);			// show windows
	setMouseCallback("image_original", CallBackFunc, NULL);
	cv::waitKey(0);                 // hold windows open until user presses a key

									//Set word coordnates
	set_worldcoords();

	//Solve the linear system: b=Ax
	for (int i = 0; i < 4; ++i) {

		b_matrix[i * 2] = xi[i];
		b_matrix[i * 2 + 1] = yi[i];


		A_matrix[i * 2][0] = xd[i];
		A_matrix[i * 2][1] = yd[i];
		A_matrix[i * 2][2] = 1;
		A_matrix[i * 2][3] = 0;
		A_matrix[i * 2][4] = 0;
		A_matrix[i * 2][5] = 0;
		A_matrix[i * 2][6] = -xi[i] * xd[i];
		A_matrix[i * 2][7] = -xi[i] * yd[i];

		A_matrix[i * 2 + 1][0] = 0;
		A_matrix[i * 2 + 1][1] = 0;
		A_matrix[i * 2 + 1][2] = 0;
		A_matrix[i * 2 + 1][3] = xd[i];
		A_matrix[i * 2 + 1][4] = yd[i];
		A_matrix[i * 2 + 1][5] = 1;
		A_matrix[i * 2 + 1][6] = -yi[i] * xd[i];
		A_matrix[i * 2 + 1][7] = -yi[i] * yd[i];

	}

	Mat A_ = Mat(8, 8, CV_64F, A_matrix);
	Mat b_ = Mat(8, 1, CV_64F, b_matrix);
	Mat x = A_.inv()*b_;
	Mat H = Mat(3, 3, CV_64F);

	//build H matrix
	int t = 0;
	for (auto i = 0; i < H.rows; ++i) {
		for (auto j = 0; j < H.cols; ++j) {
			if (i == H.rows - 1 && j == H.cols - 1) {
				H.at<double>(i, j) = 1.0;
				break;
			}
			H.at<double>(i, j) = x.at<double>(t++, 0);
		}
	}
	cout << "Correcting Image..." << endl;
	remove_distortion(image_original, H, "test.jpg");
	while (1);
	return(0);
}
