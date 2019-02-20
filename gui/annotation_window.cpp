#define CVUI_IMPLEMENTATION
#include "annotation_window.h"

AnnotationWindow::AnnotationWindow(cv::String name)
{
	main_view_name_ = name;

	frame_ = cv::Mat(200, 400, CV_8UC3);
	frame_ = cv::Scalar(50, 50, 50);

	state_ = AnnotationState::point;

	visualize_view_ = new VisualizeView("Visualize View");
}

AnnotationWindow::~AnnotationWindow() {}

void AnnotationWindow::setFeatureAnnotationView(FeatureAnnotationView* image_view)
{
	feature_annotation_view_ = image_view;
}

void AnnotationWindow::setCourtView(CourtView* image_view)
{
	court_view_ = image_view;
}

void AnnotationWindow::calibButtonFunc()
{
	vector<vgl_point_2d<double>> points_annotation = feature_annotation_view_->getPoints();
	vector<vgl_point_2d<double>> points_court = court_view_->getPoints();

	printf("Annotations in image:\n");
	for (auto iter = points_annotation.begin(); iter != points_annotation.end(); ++iter)
	{
		printf("(%f, %f)\n", iter->x(), iter->y());
	}

	printf("points in world coordinates:\n");
	for (auto iter = points_court.begin(); iter != points_court.end(); ++iter)
	{
		printf("(%f, %f)\n", iter->x(), iter->y());
	}

	if (points_annotation.size() == points_court.size() &&
		points_annotation.size() >= 4) {
		vgl_point_2d<double> principal_point(640, 360);
		vpgl_perspective_camera<double> camera;

		bool is_calib = cvx::init_calib(points_court, points_annotation, principal_point, camera);
		if (is_calib) {
			printf("successfully init calib.\n");
			// draw annotation
			// save camera

		}
		else {
			printf("Warning: init calib failed.\n");

		}

	}



}


void AnnotationWindow::clearButtonFunc()
{
	feature_annotation_view_->clearAnnotations();
	court_view_->clearAnnotations();
}

void AnnotationWindow::mainControlHandler()
{
	if (cvui::button(frame_, 100, 30, "Do Calibration")) {
		calibButtonFunc();
	}

	if (cvui::button(frame_, 100, 60, "Clear Annotations"))
	{
		clearButtonFunc();
	}


	annotationStateFunc();
}

void AnnotationWindow::annotationStateFunc()
{
	bool point_checkbox = false;
	bool line_checkbox = false;
	bool circle_checkbox = false;

	switch (state_)
	{
	case AnnotationState::point:
		point_checkbox = true;
		break;
	case AnnotationState::line_intersection:
		line_checkbox = true;
		break;
	case AnnotationState::circle:
		circle_checkbox = true;
		break;
	}

	cvui::checkbox(frame_, 100, 100, "point", &point_checkbox);
	cvui::checkbox(frame_, 100, 120, "line_intersection", &line_checkbox);
	cvui::checkbox(frame_, 100, 140, "circle", &circle_checkbox);

	switch (state_)
	{
	case AnnotationState::point:
		if (line_checkbox)
			state_ = AnnotationState::line_intersection;
		else if (circle_checkbox)
			state_ = AnnotationState::circle;
		break;
	case AnnotationState::line_intersection:
		if (point_checkbox)
			state_ = AnnotationState::point;
		else if (circle_checkbox)
			state_ = AnnotationState::circle;
		break;
	case AnnotationState::circle:
		if (point_checkbox)
			state_ = AnnotationState::point;
		else if (line_checkbox)
			state_ = AnnotationState::line_intersection;
		break;
	}


}

void AnnotationWindow::startLoop() {

	const int view_number = 4;

	const cv::String feature_view_name = feature_annotation_view_->getWindowName();
	const cv::String court_view_name = court_view_->getWindowName();
	const cv::String visualize_view_name = visualize_view_->getWindowName();

	const cv::String view_names[] =
	{
		feature_view_name,
		court_view_name,
		main_view_name_,
		visualize_view_name
	};

	// init multiple windows
	cvui::init(view_names, view_number);


	while (true) {
		frame_ = cv::Scalar(50, 50, 50);

		cvui::context(feature_view_name);
		feature_annotation_view_->drawFrame();
		feature_annotation_view_->annotate();
		cvui::imshow(feature_view_name, feature_annotation_view_->frame_);

		cvui::context(court_view_name);
		court_view_->drawFrame();
		court_view_->annotate();
		cvui::imshow(court_view_name, court_view_->frame_);

		cvui::context(main_view_name_);
		mainControlHandler();
		cvui::imshow(main_view_name_, frame_);

		cvui::context(visualize_view_name);
		visualize_view_->drawFrame();
		cvui::imshow(visualize_view_name, visualize_view_->frame_);


		// Check if ESC key was pressed
		if (cv::waitKey(20) == 27) {
			break;
		}
	}
}

