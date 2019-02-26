//
//  cvimage_view.cpp
//  CameraCalibration
//
//  Created by jimmy on 2019-02-02.
//  Copyright © 2019 Nowhere Planet. All rights reserved.
//

#include "cvimage_view.hpp"

CVImageView::CVImageView(std::string name)
{
	// default parameters for CVImageView
	//window_size_ = cv::Size(1300, 740);
	//image_size_ = cv::Size(1280, 720);
	//image_pos_ = cv::Point(10, 10);

	window_size_ = cv::Size(1280, 720);
	image_size_ = cv::Size(1280, 720);
	image_pos_ = cv::Point(0, 0);

	window_name_ = name;

	frame_ = cv::Mat(window_size_.height, window_size_.width, CV_8UC3);
	frame_ = cv::Scalar(50, 50, 50);

	image_ = cv::Mat(image_size_.height, image_size_.width, CV_8UC3, cv::Scalar(0, 0, 0));
	image_after_scale_ = cv::Mat(image_size_.height, image_size_.width, CV_8UC3, cv::Scalar(0, 0, 0));

}

CVImageView::~CVImageView() {}

cv::Size CVImageView::getWindowSize() const
{
	return window_size_;
}

void CVImageView::setWindowSize(cv::Size size)
{
	window_size_ = size;
	frame_ = cv::Mat(window_size_.height, window_size_.width, CV_8UC3);
	frame_ = cv::Scalar(50, 50, 50);
}

std::string CVImageView::getWindowName() const
{
	return window_name_;
}

void CVImageView::setWindowName(std::string name)
{
	window_name_ = name;
}

// set/get image
Mat CVImageView::getImage() const
{
	return image_;
}

void CVImageView::setImage(const Mat& img)
{
	image_ = img;
	cv::resize(image_, image_after_scale_, image_size_, cv::INTER_LINEAR);
}

cv::Point CVImageView::getImagePosition() const
{
	return image_pos_;
}

void CVImageView::setImagePosition(cv::Point p)
{
	image_pos_ = p;
}

cv::Size CVImageView::getImageSize() const
{
	return image_size_;
}

void CVImageView::setImageSize(cv::Size size)
{
	image_size_ = size;
	cv::resize(image_, image_after_scale_, image_size_, cv::INTER_LINEAR);
}

// exchange between window point and image point
cv::Point CVImageView::windowPointForImagePoint(const cv::Point& p) const
{
	cv::Size before_scale = image_.size();
	cv::Size after_scale = image_after_scale_.size();

	double scale_x = 1.0 * (after_scale.width - 1) / (before_scale.width - 1);
	double scale_y = 1.0 * (after_scale.height - 1) / (before_scale.height - 1);

	int x = round(p.x * scale_x + image_pos_.x);
	int y = round(p.y * scale_y + image_pos_.y);

	return cv::Point(x, y);
}

cv::Point CVImageView:: imagePointForWindowPoint(const cv::Point& p) const
{
	cv::Size before_scale = image_.size();
	cv::Size after_scale = image_after_scale_.size();

	double scale_x = 1.0 * (before_scale.width - 1) / (after_scale.width - 1);
	double scale_y = 1.0 * (before_scale.height - 1) / (after_scale.height - 1);


	int x = round((p.x - image_pos_.x) * scale_x);
	int y = round((p.y - image_pos_.y) * scale_y);

	return cv::Point(x, y);
}


void CVImageView::drawPoint(cv::Point p)
{
	cv::Point left_up = cv::Point(p.x - 5, p.y - 5);
	cv::Point left_down = cv::Point(p.x - 5, p.y + 5);
	cv::Point right_up = cv::Point(p.x + 5, p.y - 5);
	cv::Point right_down = cv::Point(p.x + 5, p.y + 5);
	cv::line(frame_, left_up, right_down, cv::Scalar(255, 0, 0), 1);
	cv::line(frame_, left_down, right_up, cv::Scalar(255, 0, 0), 1);
}

void CVImageView::drawFrame()
{
	cvui::image(frame_, image_pos_.x, image_pos_.y, image_after_scale_);

	for (auto iter = windows_points_.begin(); iter != windows_points_.end(); iter++)
	{
		drawPoint(*iter);
	}
}