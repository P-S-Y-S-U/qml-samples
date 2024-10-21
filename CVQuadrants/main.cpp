#include <QCoreApplication>

#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/imgproc.hpp>

void SplitQuadrantsAndSaveImg(
    cv::Mat img,
    cv::Mat& topLeft, cv::Mat& topRight,
    cv::Mat& bottomLeft, cv::Mat& bottomRight
)
{
    topLeft = img( cv::Range(0, img.rows/2 - 1), cv::Range(0, img.cols/2 - 1) );
    topRight = img( cv::Range(0, img.rows/2 - 1), cv::Range(img.cols/2, img.cols - 1) );
    bottomLeft = img( cv::Range(img.rows/2, img.rows - 1), cv::Range(0, img.cols/2 - 1) );
    bottomRight = img( cv::Range(img.rows/2, img.rows - 1), cv::Range(img.cols/2, img.cols - 1) );

#if 0
    cv::imwrite("/home/lg/workspace/Qt_Proj/CVQuadrants/TopLeft.png", topLeft);
    cv::imwrite("/home/lg/workspace/Qt_Proj/CVQuadrants/TopRight.png", topRight);
    cv::imwrite("/home/lg/workspace/Qt_Proj/CVQuadrants/BottomLeft.png", bottomLeft);
    cv::imwrite("/home/lg/workspace/Qt_Proj/CVQuadrants/BottomRight.png", bottomRight);
#endif
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    cv::Mat img1 = cv::imread("/home/lg/workspace/Qt_Proj/CVQuadrants/Sample_1.png");
    cv::Mat img2 = cv::imread("/home/lg/workspace/Qt_Proj/CVQuadrants/Sample_2.png");

    cv::Mat gray1 = cv::Mat( img1.rows, img1.cols, CV_8UC1 );
    cv::Mat gray2 = cv::Mat( img2.rows, img2.cols, CV_8UC1 );

    cv::cvtColor( img1, gray1, cv::COLOR_BGR2GRAY );
    cv::cvtColor( img2, gray2, cv::COLOR_BGR2GRAY );

    cv::Mat diffGray = cv::Mat( img1.rows, img1.cols, CV_8UC1 );

    cv::absdiff( gray1, gray2, diffGray );

    cv::Mat thresh = cv::Mat( img1.rows, img1.cols, CV_8UC1 );
    cv::threshold(diffGray, thresh, 0, 255, cv::THRESH_BINARY);

    cv::Mat kernel = cv::getStructuringElement( cv::MORPH_RECT, cv::Size(5, 5));

    cv::Mat dilatedImg = cv::Mat( img1.rows, img1.cols, CV_8UC1 );
    cv::dilate( thresh, dilatedImg, kernel, cv::Point(-1, -1), 2 );

    std::vector<cv::Mat> contours{};
    cv::findContours( dilatedImg, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE );

    cv::Mat finalImg1 = img1.clone(); //cv::Mat( img1.rows, img1.cols, img1.type() );
    cv::Mat finalImg2 = img2.clone(); //cv::Mat( img2.rows, img2.cols, img2.type() );

    for( auto i = 0u; i < contours.size(); i++ )
    {
        cv::Rect boundRect = cv::boundingRect( contours[i] );
        cv::rectangle(finalImg1, boundRect, cv::Scalar(208, 224, 64), 2);
        cv::rectangle(finalImg2, boundRect, cv::Scalar(208, 224, 64), 2);

    }
    cv::imwrite( "/home/lg/workspace/Qt_Proj/CVQuadrants/FinalImg2.png", finalImg2 );
    cv::imwrite( "/home/lg/workspace/Qt_Proj/CVQuadrants/FinalImg1.png", finalImg1 );

    cv::Mat img2TopLeft, img2TopRight, img2BottomLeft, img2BottomRight;

    SplitQuadrantsAndSaveImg(
        finalImg2,
        img2TopLeft, img2TopRight,
        img2BottomLeft, img2BottomRight
    );

    cv::imwrite("/home/lg/workspace/Qt_Proj/CVQuadrants/TopLeft.png", img2TopLeft);
    cv::imwrite("/home/lg/workspace/Qt_Proj/CVQuadrants/TopRight.png", img2TopRight);
    cv::imwrite("/home/lg/workspace/Qt_Proj/CVQuadrants/BottomLeft.png", img2BottomLeft);
    cv::imwrite("/home/lg/workspace/Qt_Proj/CVQuadrants/BottomRight.png", img2BottomRight);
    return a.exec();
}
