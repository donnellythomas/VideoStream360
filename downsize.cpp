#include <stdio.h>
#include <opencv2/opencv.hpp>

#include <chrono>
using namespace std::chrono;
using namespace std;
using namespace cv;
Mat importImage(String path) {
    Mat image = imread(path, CV_32F);
    if (!image.data) {
        printf("No image data \n");
    }
    return image;
}
float faceTransform[6][2] = {{0, 0},         {M_PI / 2, 0},  {M_PI, 0},
                             {-M_PI / 2, 0}, {0, -M_PI / 2}, {0, M_PI / 2}}; // front right back left top bottom
void computeFaceMap(float inWidth, float inHeight, Mat &mx, Mat &my, int cubeSize, int faceID){
    int height = cubeSize;
    int width = cubeSize;
    
    // Calculate adjacent (ak) and opposite (an) of the
    // triangle that is spanned from the sphere center
    // to our cube face.
    const float an = sin(M_PI / 4);
    const float ak = cos(M_PI / 4);

    float ftu = faceTransform[faceID][0];
    float ftv = faceTransform[faceID][1];

    // For each point in the target image,
    // calculate the corresponding source coordinates.
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Map face pixel coordinates to [-1, 1] on plane
            float nx = ((float)(y) / (float)height - 0.5f);
            float ny = ((float)(x) / (float)width - 0.5f);

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
Mat** computeFaceMaps(float inWidth, float inHeight, int cubeSize){
     Mat ** mxy = new Mat*[6];
    for (int i = 0; i < 6; i++)
    {   
        mxy[i] =new Mat[2];
        cubeSize = cubeSize;
        mxy[i][0] =  Mat(cubeSize, cubeSize, CV_32F);
        mxy[i][1] =  Mat(cubeSize, cubeSize, CV_32F); 
       
        computeFaceMap(inWidth, inHeight , mxy[i][0], mxy[i][1],cubeSize,i);
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
void assemblePacked(Mat &packed, Mat *faces, float cubeSize, float packedCoords[][2]){
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
void unpackAssembleCubemap(Mat &unpacked, Mat &packed , float cubeSize, float packedCoords[][2], float cubeCoords[][2]){
    Mat face;
    
    for(int i = 0; i <4; i++){
        face = packed(Range(packedCoords[i][1],packedCoords[i][1]+cubeSize/2),Range(packedCoords[i][0],packedCoords[i][0]+cubeSize));
        face.copyTo(unpacked(Rect(cubeCoords[i][0],  cubeCoords[i][1],face.cols, face.rows)));
        
    }
    for(int i = 4; i <12; i++){
        face = packed(Range(packedCoords[i][1],packedCoords[i][1]+cubeSize/16),Range(packedCoords[i][0],packedCoords[i][0]+cubeSize/4));
        resize(face, face, Size(),4, 4, INTER_CUBIC); //upscale 4
        face.copyTo(unpacked(Rect(cubeCoords[i][0],  cubeCoords[i][1],face.cols, face.rows)));
    }
    int i = 12;
    Mat top = packed(Range(packedCoords[i][1],packedCoords[i][1]+cubeSize/4),Range(packedCoords[i][0],packedCoords[i][0]+cubeSize/4));
    resize(top, top, Size(),4, 4, INTER_CUBIC); //upscale 4
    rotate(top, top, ROTATE_180);
    top.copyTo(unpacked(Rect(cubeCoords[i][0],  cubeCoords[i][1],top.cols, top.rows)));
    
    i = 13;
    Mat bottom = packed(Range(packedCoords[i][1],packedCoords[i][1]+cubeSize/4),Range(packedCoords[i][0],packedCoords[i][0]+cubeSize/4));
    resize(bottom, bottom, Size(),4, 4, INTER_CUBIC); //upscale 4
    rotate(bottom, bottom, ROTATE_90_CLOCKWISE);
    bottom.copyTo(unpacked(Rect(cubeCoords[i][0],  cubeCoords[i][1],bottom.cols, bottom.rows)));
}
int main(int argc, char **argv) {
    if (argc != 2) {
        printf("usage: ./project <Input_Path>\n");
        return -1;
    }
    // Mat in = importImage(argv[1]);
    // Mat ref = importImage(argv[2]);
    
    VideoCapture cap(argv[1]);
    if(!cap.isOpened()){
        printf("Error opening video\n");
    }

    float cubeSize = 16 * 20;
    
    float packedCoords[14][2] = {
        {0,0},                          
        {cubeSize,0},                   
        {cubeSize*2,0},
        {cubeSize*3,0},
        {cubeSize*4,0}, 
        {cubeSize*4,cubeSize/16},
        {cubeSize*4,cubeSize/16*2},
        {cubeSize*4,cubeSize/16*3},
        {cubeSize*4,cubeSize/16*4},
        {cubeSize*4,cubeSize/16*5},
        {cubeSize*4,cubeSize/16*6},
        {cubeSize*4,cubeSize/16*7},
        {cubeSize*4+cubeSize/4,0},
        {cubeSize*4+cubeSize/4,cubeSize/4}};
    float cubeCoords[14][2] = {
        {0,cubeSize*1.25f},             //front
        {cubeSize, cubeSize*1.25f},     //right
        {cubeSize*2, cubeSize*1.25f},   //back
        {cubeSize*3, cubeSize*1.25f},   //left
        {0,cubeSize},                   //frontTop
        {0,cubeSize*1.75f},             //frontBottom
        {cubeSize,cubeSize},            //rightTop
        {cubeSize,cubeSize*1.75f},      //rightBottom
        {cubeSize*2,cubeSize},          //backTop
        {cubeSize*2,cubeSize*1.75f},    //backBottom
        {cubeSize*3,cubeSize},          //leftTop
        {cubeSize*3,cubeSize*1.75f},    //leftBottom
        {cubeSize,0},                   //top
        {cubeSize,cubeSize*2} };        //bottom
    Mat frame;
    cap >> frame;
    auto start = high_resolution_clock::now();
    Mat ** mxy = computeFaceMaps(frame.cols, frame.rows, cubeSize);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop-start);
    printf("creating mappings: %ld\n", duration.count());



    while(1){
        Mat in;
        cap >> in;
        if(in.empty()) break;
       
    
    start = high_resolution_clock::now();
    
    Mat faces[6];
    for (int i = 0; i < 6; i++)
    {
        Mat face(cubeSize, cubeSize, frame.type());
        mapFace(in, face, mxy[i][0], mxy[i][1]);
        faces[i] = face;
    }
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop-start);
    printf("Remapping: %ld\n", duration.count());
     start = high_resolution_clock::now();
        
        Mat packed(cubeSize/2, cubeSize*4.5, faces[0].type());
        assemblePacked(packed, faces, cubeSize, packedCoords);
        Mat unpacked(cubeSize * 3, cubeSize * 4, packed.type());
        unpackAssembleCubemap(unpacked,packed,cubeSize,packedCoords,cubeCoords);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop-start);
        // printf("pack and unpack: %ld\n", duration.count());
        // imshow("packed", packed);
        // imwrite("packed.png", packed);
        imshow("unpacked", unpacked);
        char c=(char)waitKey(25);
        if(c==27) break;
    }
    cap.release();
        destroyAllWindows();
    // imshow("unpacked", unpacked);
    // imwrite("unpacked.png", unpacked);
    // waitKey(0);
    return 0;
}