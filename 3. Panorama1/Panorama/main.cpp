#include "stdafx.h"
#include"dlt.h"


using namespace cv;
using namespace std;


//int unifRand(int ubound, default_random_engine generator) {
//	
//	//default_random_engine generator(seed);
//	uniform_int_distribution<int> distribution(0, ubound);
//	return distribution(generator);
//}

int main() {
	time_t start = clock();


	img_src = imread("image01.jpg", 1);
	img_dst = imread("image00.jpg", 1);

	if (true) {
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

	//srand(time(0));


	default_random_engine generator(time(0));
	uniform_int_distribution<int> distribution_src_rows(0, img_src.rows);
	uniform_int_distribution<int> distribution_src_cols(0, img_src.cols);
	uniform_int_distribution<int> distribution_dst_rows(0, img_dst.rows);
	uniform_int_distribution<int> distribution_dst_cols(0, img_dst.cols);



	/********RANSAC*********/
	/***********************/

	//number of false corresponding points
	const int outlier = 100;
	double N = log(1 - 0.99) / log(1 - pow(1 - (double)outlier / (20 + (double)outlier), 4));
	N = ceil(N);
	cout << "N=" << N << endl;

	for (int i = 0; i < outlier; ++i) {
		points_src.push_back(Point2d(distribution_src_cols(generator), distribution_src_rows(generator)));
		points_dst.push_back(Point2d(distribution_dst_cols(generator), distribution_dst_rows(generator)));
	}

	uniform_int_distribution<int> rand_index(0, points_src.size() - 1);
	int index;				//index of random corresponding points
	int num_in = 0;			//number of inlier
	int thres = 100;		//threshold of symmetric transfer error
	//int max_in_index[4];	//max inlier index
	int temp_index[4];		//temperate index
	int max_in_num = 0;		//max inliner number

	Mat temp_H;				//homography calculated by randomly selected 4 points
	Mat maxIn_H;			//homography with most inlier

	vector<Point2f> rand_pts_src(4);
	vector<Point2f> rand_pts_dst(4);
	
	vector<Point2f> cons_set_src;
	vector<Point2f> cons_set_dst;



	while (N > 0) {
		//cout << N << endl;
		for (int i = 0; i < 4; ++i) {
			index = rand_index(generator);
			temp_index[i] = index;
			rand_pts_src[i] = points_src[index];
			rand_pts_dst[i] = points_dst[index];
			//cout << index << " ";
		}
		//cout << endl;
		
		//find a homography using 4 corresponding points
		temp_H = findH_dlt(rand_pts_src, rand_pts_dst);

		//calculate number of inlier
		for (int i = 0; i < points_src.size(); ++i) {
			if (sym_error(points_src[i], points_dst[i], temp_H) < thres) {
				num_in++;
			};
		}
		if (num_in > max_in_num) {
			max_in_num = num_in;
			maxIn_H = temp_H;
		}

		N--;
		num_in = 0;
	}


	for (int i = 0; i < points_src.size(); ++i) {
		if (sym_error(points_src[i], points_dst[i], maxIn_H) < thres) {
			cons_set_src.push_back(points_src[i]);
			cons_set_dst.push_back(points_dst[i]);
		};
	}


	Mat final_H = findH_dlt(cons_set_src, cons_set_dst);
	cout << "Inlier number=" << max_in_num << endl;
	cout << "Ransac_H=\n" << final_H << endl;
	//Mat H = findHomography(points_src, points_dst);
	//temp_H = findH_dlt(points_src, points_dst);
	//cout << temp_H << endl;



	//for (int i = 0; i < N; ++i) {
	//	points_src.push_back(Point2d(distribution_src_cols(generator), distribution_src_rows(generator)));
	//	points_dst.push_back(Point2d(distribution_dst_cols(generator), distribution_dst_rows(generator)));
	//}
	//for (int i = 0; i < points_src.size(); ++i) {
	//	cout << sym_error(points_src[i], points_dst[i], H1) << endl;
	//}
	//cout << H << endl << H1 << endl;

	//cout << rand_pts_src << endl;
	//cout << endl;
	//cout << rand_pts_dst << endl;
	time_t end = clock();

	cout << difftime(end, start)/1000 << endl;
	//while (true);
	return(0);
}



////cv::solvePnP();

////right hand 
////Mat rhs = Mat::zeros(40, 1, CV_64F);
////cout << rhs << endl;
////backSubst(InputArray w, InputArray u, InputArray vt, InputArray rhs, OutputArray dst)
////SVD::backSubst(D,U,Vt,rhs,H);
//

//int main()
//{
//	//img_src = imread("image01.jpg", IMREAD_GRAYSCALE);
//	//img_dst = imread("image00.jpg", IMREAD_GRAYSCALE);
//	points_src.push_back(Point2f(1723, 496));
//	points_src.push_back(Point2f(1802, 480));
//	points_src.push_back(Point2f(1809, 625));
//	points_src.push_back(Point2f(1729, 641));
//
//
//	points_src.push_back(Point2f(1819, 494));
//	points_src.push_back(Point2f(1851, 488));
//	points_src.push_back(Point2f(1823, 581));
//	points_src.push_back(Point2f(1858, 574));
//	points_src.push_back(Point2f(1765, 655));
//	points_src.push_back(Point2f(1838, 642));
//
//	points_src.push_back(Point2f(1902, 1112));
//	points_src.push_back(Point2f(1987, 1113));
//	points_src.push_back(Point2f(1987, 1172));
//	points_src.push_back(Point2f(1900, 1172));
//	points_src.push_back(Point2f(1925, 1043));
//	points_src.push_back(Point2f(1966, 1042));
//	points_src.push_back(Point2f(1967, 1073));
//	points_src.push_back(Point2f(1925, 1072));
//	points_src.push_back(Point2f(1773, 1192));
//	points_src.push_back(Point2f(1797, 1189));
//
//
//	points_dst.push_back(Point2f(156, 489));
//	points_dst.push_back(Point2f(238, 485));
//	points_dst.push_back(Point2f(228, 632));
//	points_dst.push_back(Point2f(146, 635));
//
//	points_dst.push_back(Point2f(255, 502));
//	points_dst.push_back(Point2f(286, 500));
//	points_dst.push_back(Point2f(249, 588));
//	points_dst.push_back(Point2f(282, 586));
//	points_dst.push_back(Point2f(181, 655));
//	points_dst.push_back(Point2f(254, 652));
//
//	points_dst.push_back(Point2f(275, 1119));
//	points_dst.push_back(Point2f(356, 1120));
//	points_dst.push_back(Point2f(354, 1176));
//	points_dst.push_back(Point2f(271, 1181));
//	points_dst.push_back(Point2f(303, 1052));
//	points_dst.push_back(Point2f(344, 1052));
//	points_dst.push_back(Point2f(342, 1082));
//	points_dst.push_back(Point2f(301, 1080));
//	points_dst.push_back(Point2f(136, 1203));
//	points_dst.push_back(Point2f(156, 1203));
//	//int minHessian = 400;
//	//
//	////-- Step 1: Detect the keypoints using SURF Detector
//
//
//	//Ptr<SURF> detector = SURF::create(minHessian);
//
//	//std::vector<KeyPoint> keypoints_1, keypoints_2;
//
//	//detector->detect(img_src, keypoints_1);
//	//detector->detect(img_dst, keypoints_2);
//
//	////-- Draw keypoints
//	//Mat img_keypoints_1; Mat img_keypoints_2;
//
//	//drawKeypoints(img_src, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
//	//drawKeypoints(img_dst, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
//	//cout << points_dst << endl;
//
//	//-- Show detected (drawn) keypoints
//	/*imshow("Keypoints 1", img_keypoints_1);
//	imshow("Keypoints 2", img_keypoints_2);*/
//
//	waitKey(0);
//	while (1);
//	return 0;
//}

