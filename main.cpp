#include <iostream>
#include <stdio.h>
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

char *window = (char*) "Image";
bool leftButtonDown = false;
int k;
Point2f *pt;

void menu();
void L1_norm();
void sel_methods();
static void points(int event, int x, int y, int, void*);


int main(int argc, char *argv[]){

    /// Read the image
    Mat img = imread(argv[1]);

    if (img.empty()){
        cout << "Error loading the image" << endl;
        return -1;
    }

    /// Create first image window
    namedWindow(window, WINDOW_NORMAL);
    imshow(window, img);

    while(pt == NULL);
    cout << pt->x <<",," << pt->y << endl;
    waitKey(0);

    /// Set mouse callback
    setMouseCallback(window, points, &img);

    /// Display info
    menu();

    sel_methods();

    waitKey(0);

    return 0;
}

void menu(){
    cout << "Method: " << endl;
    cout << "1 - L1-norm (cube)" << endl;
    cout << "2 - L2-norm (sphere)" << endl;
    cout << "3 - Mahalanobis" << endl;
    cout << "4 - k nearest neighbor" << endl;
    cout << "Enter - Reset" << endl;
    cout << "Esc - Exit" << endl;
}

void L1_norm(){
    int th;

    cout << "Select one point" << endl;
    cout << "Threshold: ";
    cin >> th;

    while(pt == NULL);



}

void sel_methods(){

    for(;;){
        k = waitKey(0);

        switch(k){
            case 49:        /// Key = 1
                L1_norm();
                break;
            default:
                break;
        }

        if(k == 23){        ///Key = Esc
            destroyAllWindows();
            break;
        }
    }
}


static void points(int event, int x, int y, int, void* img_){
    Vec3b color(255, 255, 255);
    Mat &img = *(Mat*) img_;

    if(event == CV_EVENT_LBUTTONDOWN && (k == 49 || k == 50)){
        pt->x = x;
        pt->y = y;
        return;
    }

    if (event == CV_EVENT_LBUTTONDOWN)
        leftButtonDown = true;
    else if(event == CV_EVENT_MOUSEMOVE && leftButtonDown)
        img.at<Vec3b>(Point(x, y)) = color;
    else if(event == CV_EVENT_LBUTTONUP)
        leftButtonDown = false;

    imshow(window, img);

}