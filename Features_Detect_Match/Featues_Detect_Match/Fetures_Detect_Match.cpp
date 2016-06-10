#include<opencv2/opencv.hpp>
#include<opencv2/nonfree/nonfree.hpp>
#include<iostream>
using namespace std;
using namespace cv;

int main(){
	system("color 3F");
	Mat srcImage1 = imread("C:\\Users\\jqb\\Pictures\\jiangqb_blue.jpg");
	Mat srcImage2 = imread("C:\\Users\\jqb\\Pictures\\jiangqb_red.jpg");
	if (!srcImage1.data || !srcImage2.data){
		cout << "There are someting wrong with loading the pictures!\n";
		return false;
	}
	//使用surf算子检测关键点
	int minHessian = 400;
	SurfFeatureDetector detector(minHessian);
	vector<KeyPoint> keypoints_object, keypoints_sence;
	detector.detect(srcImage1, keypoints_object);
	detector.detect(srcImage2, keypoints_sence);
	//计算描述符（特征向量）
	Mat descriptors_object, descriptors_sence;
	SurfDescriptorExtractor extractor;
	extractor.compute(srcImage1, keypoints_object, descriptors_object);
	extractor.compute(srcImage2, keypoints_sence, descriptors_sence);
	//使用FLANN算子进行特征匹配
	FlannBasedMatcher matcher;
	vector<DMatch> matches;
	matcher.match(descriptors_object, descriptors_sence, matches);
	//计算出关键点之间最大距离和最小距离
	double max_dist = 0, min_dist = 100;
	for (int i = 0; i < descriptors_object.rows; i++){
		double dist = matches[i].distance;
		if (dist < min_dist)
			min_dist = dist;
		if (dist>max_dist)
			max_dist = dist;
	}
	cout << ">Max dist 最大距离：" << max_dist << endl;
	cout << "Min dist 最小距离：" << min_dist << endl;

	//存下匹配距离小于3*min_dist的点对
	vector<DMatch> good_matches;
	for (int i = 0; i < descriptors_object.rows; i++){
		if (matches[i].distance < 3 * min_dist)
			good_matches.push_back(matches[i]);
	}

	//绘制匹配到的关键点
	Mat img_matches;
	drawMatches(srcImage1, keypoints_object, srcImage2, keypoints_sence, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	vector<Point2f> obj;
	vector<Point2f> sence;
	//从匹配成功的匹配对中获取关键点
	for (size_t i = 0; i < good_matches.size(); i++){
		obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
		sence.push_back(keypoints_sence[good_matches[i].trainIdx].pt);
	}

	Mat H = findHomography(obj, sence, CV_RANSAC);//计算透视变换
	//从待检测图片中获取角点
	vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0);
	obj_corners[1] = cvPoint(srcImage1.cols, 0);
	obj_corners[2] = cvPoint(srcImage1.cols, srcImage1.rows);
	obj_corners[3] = cvPoint(0, srcImage1.rows);
	vector<Point2f> sence_corner(4);

	//进行透视变换
	perspectiveTransform(obj_corners, sence_corner, H);
	//绘制出角点之间的直线
	line(img_matches, sence_corner[0] + Point2f(static_cast<float>(srcImage1.cols), 0), sence_corner[1] + Point2f(static_cast<float>(srcImage1.cols), 0), Scalar(255, 0, 123), 4);
	line(img_matches, sence_corner[1] + Point2f(static_cast<float>(srcImage1.cols), 0), sence_corner[2] + Point2f(static_cast<float>(srcImage1.cols), 0), Scalar(255, 0, 123), 4);
	line(img_matches, sence_corner[2] + Point2f(static_cast<float>(srcImage1.cols), 0), sence_corner[3] + Point2f(static_cast<float>(srcImage1.cols), 0), Scalar(255, 0, 123), 4);
	line(img_matches, sence_corner[3] + Point2f(static_cast<float>(srcImage1.cols), 0), sence_corner[0] + Point2f(static_cast<float>(srcImage1.cols), 0), Scalar(255, 0, 123), 4);

	//显示最终结果
	imshow("【效果图】", img_matches);
	waitKey(0);
}