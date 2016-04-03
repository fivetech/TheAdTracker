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


void detectAndDisplay( Mat frame );


String face_cascade_name = "haarcascade_frontalface_alt2.xml";
String eyes_cascade_name = "haarcascade_eye.xml";

CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

String window_name = "Capture - Face and eye detection";

int main(int argc, const char * argv[]) {
    
    VideoCapture capture;
    Mat frame;
    
    
    if( !face_cascade.load( face_cascade_name ) ) {
        printf("--(!)Error loading face cascade\n");
        return -1; };
    if( !eyes_cascade.load( eyes_cascade_name ) ) {
        printf("--(!)Error loading eyes cascade\n");
        return -1; };
    
    
    capture.open( -1 );
    if ( ! capture.isOpened() ) { printf("--(!)Error opening video capture\n"); return -1; }
    
    capture.set(CV_CAP_PROP_FRAME_WIDTH,640);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,480);
    
    while ( capture.read(frame) )
    {
        if( frame.empty() )
        {
            printf(" --(!) No captured frame -- Break!");
            break;
        }
        
        detectAndDisplay( frame );
        
        int c = waitKey(10);
        if( (char)c == 27 ) { break; }
    }
    
    return 0;
}

void detectAndDisplay( Mat frame )
{
    std::vector<Rect> faces;
    Mat faceROI;
    Mat frame_gray;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    
    
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0, Size(80, 80) );
    
    for( size_t i = 0; i < faces.size(); i++ ) {
        faceROI = frame_gray( faces[i] );
        std::vector<Rect> eyes;
        eyes_cascade.detectMultiScale( faceROI, eyes, 2.5, 6, 0 |CASCADE_SCALE_IMAGE, Size(30, 30) );
        
        rectangle(frame, faces[i], Scalar( 0, 255, 0 ), 1); // Displays face rectange
 
        
            for( size_t j = 0; j < eyes.size(); j++ ) {
           /* Point pt1(faces[i].x+eyes[j].x, faces[i].y+eyes[j].y);
            Point pt2(faces[i].x + eyes[j].x + eyes[j].width, faces[i].y + eyes[j].y + eyes[j].height);
            rectangle( frame, pt1, pt2, Scalar(0, 255,0), 1);*/
               
                   Rect rect = eyes[j] + cv::Point(faces[i].x, faces[i].y);
                    
                    rectangle(frame, rect, Scalar( 0, 255, 0 ), 1);
                
       /* Rect rect = eyes[j] + cv::Point(faces[i].x, faces[i].y);
                rectangle(frame, rect, Scalar( 0, 255, 0 ), 1);*/
            }
        
    }
    
    imshow( window_name, frame );
}
