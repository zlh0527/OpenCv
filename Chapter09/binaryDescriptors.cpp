/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 9 of the cookbook:  
   Computer Vision Programming using the OpenCV Library 
   Second Edition 
   by Robert Laganiere, Packt Publishing, 2013.

   This program is free software; permission is hereby granted to use, copy, modify, 
   and distribute this source code, or portions thereof, for any purpose, without fee, 
   subject to the restriction that the copyright notice may not be removed 
   or altered from any source or altered source distribution. 
   The software is released on an as-is basis and without any warranties of any kind. 
   In particular, the software is not guaranteed to be fault-tolerant or free from failure. 
   The author disclaims all warranties with regard to this software, any use, 
   and any consequent failure, is purely the responsibility of the user.
 
   Copyright (C) 2013 Robert Laganiere, www.laganiere.name
\*------------------------------------------------------------------------------------------*/

#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/nonfree/features2d.hpp>

int main()
{
	// image matching

	// 1. Read input images
	cv::Mat image1= cv::imread("church01.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat image2= cv::imread("church02.jpg",CV_LOAD_IMAGE_GRAYSCALE);

	// 2. Define keypoints vector
	std::vector<cv::KeyPoint> keypoints1;
	std::vector<cv::KeyPoint> keypoints2;

	// 3. Define feature detector
	// Construct the ORB feature detector object
	cv::Ptr<cv::FeatureDetector> detector = 
//		new cv::BRISK(40); // to detect BRISK points  
		new cv::ORB(100); // detect approx 100 ORB points  

	// 4. Keypoint detection
	// Detect the ORB features
	detector->detect(image1,keypoints1);
	detector->detect(image2,keypoints2);

	// Draw feature points
	cv::Mat featureImage;
	cv::drawKeypoints(image1,keypoints1,featureImage,cv::Scalar(255,255,255),cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    // Display the corners
	cv::namedWindow("ORB");
	cv::imshow("ORB",featureImage);

	std::cout << "Number of ORB keypoints (image 1): " << keypoints1.size() << std::endl; 
	std::cout << "Number of ORB keypoints (image 2): " << keypoints2.size() << std::endl; 

	// ORB includes both the detector and descriptor extractor
	cv::Ptr<cv::DescriptorExtractor> descriptor = detector;
//		 new cv::FREAK(); // to describe with FREAK  

	// 5. Extract the descriptor
    cv::Mat descriptors1;
    cv::Mat descriptors2;
    descriptor->compute(image1,keypoints1,descriptors1);
    descriptor->compute(image2,keypoints2,descriptors2);

   // Construction of the matcher 
   cv::BFMatcher matcher(
	   cv::NORM_HAMMING); // always use hamming norm
                          // for binary descriptors
   // Match the two image descriptors
   std::vector<cv::DMatch> matches;
   matcher.match(descriptors1,descriptors2, matches);

   // draw matches
   cv::Mat imageMatches;
   cv::drawMatches(
     image1,keypoints1, // 1st image and its keypoints
     image2,keypoints2, // 2nd image and its keypoints
     matches,            // the matches
     imageMatches,      // the image produced
     cv::Scalar(255,255,255),  // color of lines
     cv::Scalar(255,255,255)); // color of points

    // Display the image of matches
   cv::namedWindow("ORB Matches");
   cv::imshow("ORB Matches", imageMatches);
//   cv::namedWindow("FREAK Matches");
//   cv::imshow("FREAK Matches", imageMatches);

	std::cout << "Number of matches: " << matches.size() << std::endl; 

   cv::waitKey();
   return 0;
}
