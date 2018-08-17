#include <iostream>
#include <stdio.h>
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

char *window = (char*) "Image";
FILE* file;
bool leftButtonDown = false;

static void points(int event, int x, int y, int, void*);

int main(int argc, char *argv[]){

    Mat img = imread(argv[1]);
    file = fopen(argv[2], "w");

    if (img.empty()){
        cout << "Error loading the image" << endl;
        return -1;
    }

    namedWindow(window, 1);
    imshow(window, img);

    cout << "Select the points to be analyzed" << endl;

    setMouseCallback(window, points, &img);

    for(;;){
        int c = waitKey(0);

        if (c == 13){
            cout << "Exit" << endl;
            destroyAllWindows();
            break;
        }
    }
    waitKey(0);

    fclose(file);

    return 0;
}

static void points(int event, int x, int y, int, void* img_){
    Vec3b color(255, 255, 255);
    Mat &img = *(Mat*) img_;

    if (event == CV_EVENT_LBUTTONDOWN)
        leftButtonDown = true;
    else if(event == CV_EVENT_MOUSEMOVE && leftButtonDown){
        img.at<Vec3b>(Point(x, y)) = color;
        fprintf(file, "%d %d\n", x, y);
    }
    else if(event == CV_EVENT_LBUTTONUP)
        leftButtonDown = false;

    imshow(window, img);

}