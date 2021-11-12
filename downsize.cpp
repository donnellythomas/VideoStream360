#include <stdio.h>
#include <opencv2/opencv.hpp>

#include <chrono>
using namespace std::chrono;
using namespace std;
using namespace cv;

const float CUBESIZE = 16 * 40;
Mat importImage(String path) {
    Mat image = imread(path, CV_32F);
    if (!image.data) {
        printf("No image data \n");
    }
    return image;
}
// float faceTransform[6][2] = {{0, 0},         {M_PI / 2, 0},  {M_PI, 0},
//                              {-M_PI / 2, 0}, {0, -M_PI / 2}, {0, M_PI / 2}}; // front right back left top bottom
float faceTransform[6][2] = {{-M_PI / 2, 0},{0, 0},{M_PI / 2, 0},{M_PI, 0}, {0, -M_PI / 2}, {0, M_PI / 2}}; // left front right back top bottom
float packedCoords[14][2] = {
        {0,0},                          
        {CUBESIZE,0},                   
        {CUBESIZE*2,0},
        {CUBESIZE*3,0},
        {CUBESIZE*4,0}, 
        {CUBESIZE*4,CUBESIZE/16},
        {CUBESIZE*4,CUBESIZE/16*2},
        {CUBESIZE*4,CUBESIZE/16*3},
        {CUBESIZE*4,CUBESIZE/16*4},
        {CUBESIZE*4,CUBESIZE/16*5},
        {CUBESIZE*4,CUBESIZE/16*6},
        {CUBESIZE*4,CUBESIZE/16*7},
        {CUBESIZE*4+CUBESIZE/4,0},
        {CUBESIZE*4+CUBESIZE/4,CUBESIZE/4}};
    float cubeCoords[14][2] = {
        {0,CUBESIZE*1.25f},             //front
        {CUBESIZE, CUBESIZE*1.25f},     //right
        {CUBESIZE*2, CUBESIZE*1.25f},   //back
        {CUBESIZE*3, CUBESIZE*1.25f},   //left
        {0,CUBESIZE},                   //frontTop
        {0,CUBESIZE*1.75f},             //frontBottom
        {CUBESIZE,CUBESIZE},            //rightTop
        {CUBESIZE,CUBESIZE*1.75f},      //rightBottom
        {CUBESIZE*2,CUBESIZE},          //backTop
        {CUBESIZE*2,CUBESIZE*1.75f},    //backBottom
        {CUBESIZE*3,CUBESIZE},          //leftTop
        {CUBESIZE*3,CUBESIZE*1.75f},    //leftBottom
        {CUBESIZE,0},                   //top
        {CUBESIZE,CUBESIZE*2} };        //bottom
void computeFaceMap(float inWidth, float inHeight, Mat &mx, Mat &my, int faceID){
    
    
    // Calculate adjacent (ak) and opposite (an) of the
    // triangle that is spanned from the sphere center
    // to our cube face.
    const float an = sin(M_PI / 4);
    const float ak = cos(M_PI / 4);

    float ftu = faceTransform[faceID][0];
    float ftv = faceTransform[faceID][1];

    // For each point in the target image,
    // calculate the corresponding source coordinates.
    for (int y = 0; y < CUBESIZE; y++) {
        for (int x = 0; x < CUBESIZE; x++) {
            // Map face pixel coordinates to [-1, 1] on plane
            float nx = ((float)(y) / CUBESIZE - 0.5f);
            float ny = ((float)(x) / CUBESIZE - 0.5f);

            // printf("test");
            nx *= 2;
            ny *= 2;
            nx *= an;
            ny *= an;

            float u, v;

            if (ftv == 0) {
                // Center faces
                u = atan2(nx, ak);
                v = atan2(ny * cos(u), ak);
                u += ftu;
            } else if (ftv > 0) {
                // Bottom face
                float d = sqrt(nx * nx + ny * ny);
                v = M_PI / 2 - atan2(d, ak);
                u = atan2(ny, nx);
            } else {
                // Top face
                float d = sqrt(nx * nx + ny * ny);
                v = -M_PI / 2 + atan2(d, ak);
                u = atan2(-ny, nx);
            }

            u = u / (M_PI);
            v = v / (M_PI / 2);

            // Warp around, if our coordinates are out of bounds.
            while (v < -1) {
                v += 2;
                u += 1;
            }
            while (v > 1) {
                v -= 2;
                u += 1;
            }

            while (u < -1) {
                u += 2;
            }
            while (u > 1) {
                u -= 2;
            }

            u = u / 2.0f + 0.5f;
            v = v / 2.0f + 0.5f;

            u = u * (inWidth - 1);
            v = v * (inHeight - 1);
            int x2,x3;
            mx.at<float>(x, y) = u;
            my.at<float>(x, y) = v;
        }
    }
} 
void mapFace(Mat &in, Mat &out, Mat &mx, Mat &my){
      remap(in, out, mx, my, INTER_LINEAR, BORDER_CONSTANT,
          Scalar(0, 0, 0));
}
Mat** computeFaceMaps(float inWidth, float inHeight){
     Mat ** mxy = new Mat*[6];
    for (int i = 0; i < 6; i++)
    {   
        mxy[i] =new Mat[2];
        mxy[i][0] =  Mat(CUBESIZE, CUBESIZE, CV_32F);
        mxy[i][1] =  Mat(CUBESIZE, CUBESIZE, CV_32F); 
       
        computeFaceMap(inWidth, inHeight , mxy[i][0], mxy[i][1],i);
    }

    
    return mxy;
}
void splitFace(const Mat &full, Mat &top, Mat &mid, Mat &bott){
    top = full(Range(0, full.rows/4), Range(0,full.cols));
    mid = full(Range(full.rows/4, 3*full.rows/4), Range(0,full.cols));
    bott = full(Range(3*full.rows/4, full.rows), Range(0,full.cols));
    resize(top, top, Size(), .25, .25, INTER_AREA); // downscale 4x
    resize(bott, bott, Size(), .25, .25, INTER_AREA); //  downscale 4x
}
void assemblePacked(Mat &packed, Mat *faces){
    Mat packedFaces[14];
    int j = 3;
    for(int i = 0; i < 4; i++){
        int top = j+1;
        int bottom = j+2;
        j = j+2;
        splitFace(faces[i], packedFaces[top], packedFaces[i], packedFaces[bottom]);
    }
    resize(faces[4],packedFaces[12],Size(), .25, .25, INTER_AREA);
    resize(faces[5],packedFaces[13],Size(), .25, .25, INTER_AREA);
    // PACK INTO FRAME
    for(int i = 0; i <14; i++){
        packedFaces[i].copyTo(packed(Rect(packedCoords[i][0],  packedCoords[i][1],packedFaces[i].cols, packedFaces[i].rows)));
    }
    // imshow("packed", packed);
    // imwrite("packed.png", packed);

}
void unpackAssembleCubemap(Mat &unpacked, Mat &packed){
    Mat face;
    
    for(int i = 0; i <4; i++){
        face = packed(Range(packedCoords[i][1],packedCoords[i][1]+CUBESIZE/2),Range(packedCoords[i][0],packedCoords[i][0]+CUBESIZE));
        face.copyTo(unpacked(Rect(cubeCoords[i][0],  cubeCoords[i][1],face.cols, face.rows)));
        
    }
    for(int i = 4; i <12; i++){
        face = packed(Range(packedCoords[i][1],packedCoords[i][1]+CUBESIZE/16),Range(packedCoords[i][0],packedCoords[i][0]+CUBESIZE/4));
        resize(face, face, Size(),4, 4, INTER_CUBIC); //upscale 4
        face.copyTo(unpacked(Rect(cubeCoords[i][0],  cubeCoords[i][1],face.cols, face.rows)));
    }
    int i = 12;
    Mat top = packed(Range(packedCoords[i][1],packedCoords[i][1]+CUBESIZE/4),Range(packedCoords[i][0],packedCoords[i][0]+CUBESIZE/4));
    resize(top, top, Size(),4, 4, INTER_CUBIC); //upscale 4
    rotate(top, top, ROTATE_90_CLOCKWISE);
    top.copyTo(unpacked(Rect(cubeCoords[i][0],  cubeCoords[i][1],top.cols, top.rows)));
    
    i = 13;
    Mat bottom = packed(Range(packedCoords[i][1],packedCoords[i][1]+CUBESIZE/4),Range(packedCoords[i][0],packedCoords[i][0]+CUBESIZE/4));
    resize(bottom, bottom, Size(),4, 4, INTER_CUBIC); //upscale 4
    rotate(bottom, bottom, ROTATE_90_COUNTERCLOCKWISE);
    bottom.copyTo(unpacked(Rect(cubeCoords[i][0],  cubeCoords[i][1],bottom.cols, bottom.rows)));
}
Mat pack(Mat &in, Mat ** mxy){
    Mat faces[6];
    for (int i = 0; i < 6; i++)
    {
        Mat face(CUBESIZE, CUBESIZE, in.type());
        mapFace(in, face, mxy[i][0], mxy[i][1]);
        faces[i] = face;
    }
    Mat packed(CUBESIZE/2, CUBESIZE*4.5, faces[0].type());
    assemblePacked(packed, faces);
    return packed;
}
Mat unpack(Mat packed){
    Mat unpacked(CUBESIZE * 3, CUBESIZE * 4, 16);
    unpackAssembleCubemap(unpacked,packed);
    return unpacked;
}

void runImage(char*path){
    Mat in = importImage(path);
    int height = in.rows;
    int width = in.cols;

    Mat ** mxy = computeFaceMaps(width,height);
    Mat packed = pack(in,mxy);
    Mat unpacked = unpack(packed);
    imshow("packed", packed);
    imwrite("packed.png", packed);
    imshow("unpacked", unpacked);
    imwrite("unpacked.png", unpacked);
    waitKey(0);
}
void runVideo(char* path){
    VideoCapture cap(path);
    if(!cap.isOpened()){
        printf("Error opening video\n");
    }
    // cap >> in;
    int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);;
    int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    Mat ** mxy = computeFaceMaps(width,height);
    int i = 0;
    while(1){
        Mat in;
        cap >> in;
        if(in.empty()) break;
        Mat packed = pack(in,mxy);
        Mat unpacked = unpack(packed);

        imshow("unpacked", packed);
        string index = to_string(i);
        while(i < 5) {
            imwrite(index + ".png", packed);
            i++;
        }
        // while(1);
        char c=(char)waitKey(25);
        if(c==27) break;
    }
    cap.release();

    destroyAllWindows();
}
int main(int argc, char **argv) {
    if (argc != 3) {
        printf("usage: ./project <type> <Input_Path> \n");
        return -1;
    }
    string image = "image";
    if((!image.compare(argv[1]))){
        runImage(argv[2]);
    }
    else{
        runVideo(argv[2]);
    }
   
    return 0;
}