//
//  main.cpp
//  TheAdTracker
//
//  Created by Tolga Durak on 25/02/16.
//  Copyright © 2016 Tolga Durak. All rights reserved.
//

#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

// Function header that takes original image and detects faces and eyes.
void detectAndDisplay( Mat frame );


String face_cascade_name = "lbpcascade_frontalface.xml";
String eyes_cascade_name = "haarcascade_eye.xml";

// Cascade Classifiers
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

// Window Name
String window_name = "Capture - Face and eye detection";

int main(int argc, const char * argv[]) {
    
    VideoCapture capture; // Camera source
    Mat frame; // image object
    
    // Load face and eyes cascade
    if( !face_cascade.load( face_cascade_name ) ) {
        printf("--(!)Error loading face cascade\n");
        return -1; };
    if( !eyes_cascade.load( eyes_cascade_name ) ) {
        printf("--(!)Error loading eyes cascade\n");
        return -1; };
    
    // Open camera stream
    // -1 for default camera
    capture.open( -1 );
    
    // Check if it is opened properly
    if ( ! capture.isOpened() ) { printf("--(!)Error opening video capture\n"); return -1; }
    
    // Set camera resolution
    capture.set(CV_CAP_PROP_FRAME_WIDTH,640);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,480);
    
    // Keep read from camera stream until specified key is pressed
    while ( capture.read(frame) )
    {
        // Check if captured frame is empty
        if( frame.empty() )
        {
            printf(" --(!) No captured frame -- Break!");
            break;
        }
        
        // Detect face and eyes region
        detectAndDisplay( frame );
        
        // Get char from keyboard
        int c = waitKey(1);
        
        // if char is esc break the loop
        if( (char)c == 27 ) { break; }
    }
    
    return 0;
}

void detectAndDisplay( Mat frame )
{
    
    std::vector<Rect> faces; // Region of faces
    Mat faceROI; // Gray image of region of face
    Mat frame_gray; // Gray image of original frame
   
    // Convert image to GRAY image
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    
    // Equalize the histogram to normalize the brightness and increases the contrast of the image
    equalizeHist( frame_gray, frame_gray );
    
    // Detect faces objects of different sizes in the input image and return a list of rectangles
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0, Size(80, 80) );
    
    // Iterate through the list of faces
    for( size_t i = 0; i < faces.size(); i++ ) {
       
        // Create faceROI in GRAY
        faceROI = frame_gray( faces[i] );
        
        std::vector<Rect> eyes; // Region of eyes
        
        // Detect eyes objects of different sizes in the input image and return a list of rectangles
        eyes_cascade.detectMultiScale( faceROI, eyes, 2.5, 7, 0 |CASCADE_SCALE_IMAGE, Size(30, 30) );
        
        // Draw face region on to original image
        rectangle(frame, faces[i], Scalar( 0, 255, 0 ), 1);
        
        // Iterate through the list of eyes for a face
        for( size_t j = 0; j < eyes.size(); j++ ) {
           
            // Create rectangle object that hold region of an eye
            Rect rect = eyes[j] + cv::Point(faces[i].x, faces[i].y);
            
            // Draw eye region on to original image
            rectangle(frame, rect, Scalar( 0, 255, 0 ), 1);
        }
        
    }
    // Show frame object in a window
    imshow( window_name, frame );
}
