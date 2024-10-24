#include <QCoreApplication>

#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <map>
#include <iostream>

enum Quadrant
{
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_RIGHT,
    BOTTOM_LEFT
};

void SplitQuadrants(
    cv::Mat img,
    std::vector<std::vector<cv::Rect>>& quadMatrix,
    const uint32_t& matRow, const uint32_t& matCol
)
{
    quadMatrix.resize(matRow);

    for( auto i = 0u; i < matRow; i++ )
    {
        std::vector<cv::Rect>& rowRects = quadMatrix[i];
        rowRects.resize(matCol);
        for( auto j = 0u; j < matCol; j++ )
        {
            cv::Point2i startExtent = cv::Point2i( (img.cols/matCol) * j, (img.rows/matRow) * i  ) ;
            cv::Size rectSize( ( img.cols / matCol ) - 1 , img.rows / matRow - 1);
            cv::Rect rect{
                startExtent,
                rectSize
            };
            rowRects[j] = rect;
        }
    }

#if 0
    topLeft = img( cv::Range(0, img.rows/2 - 1), cv::Range(0, img.cols/2 - 1) );
    topRight = img( cv::Range(0, img.rows/2 - 1), cv::Range(img.cols/2, img.cols - 1) );
    bottomLeft = img( cv::Range(img.rows/2, img.rows - 1), cv::Range(0, img.cols/2 - 1) );
    bottomRight = img( cv::Range(img.rows/2, img.rows - 1), cv::Range(img.cols/2, img.cols - 1) );


    cv::imwrite("/home/lg/workspace/Qt_Proj/CVQuadrants/TopLeft.png", topLeft);
    cv::imwrite("/home/lg/workspace/Qt_Proj/CVQuadrants/TopRight.png", topRight);
    cv::imwrite("/home/lg/workspace/Qt_Proj/CVQuadrants/BottomLeft.png", bottomLeft);
    cv::imwrite("/home/lg/workspace/Qt_Proj/CVQuadrants/BottomRight.png", bottomRight);
#endif
}


void DetermineQuadrant(
    const cv::Rect& rectangle,
    const uint32_t& matRow, const uint32_t& matCol,
    const std::vector<std::vector<cv::Rect>>& quadMatrix,
    std::vector<std::vector<uint32_t>>& matCount
)
{
    auto l_isInsideRectangle = [&rectangle]( const cv::Rect& quadrantRect ) -> bool{
        bool bP1 = quadrantRect.contains(rectangle.tl());
        bool bP2 = quadrantRect.contains( rectangle.tl() + cv::Point2i( rectangle.width, 0 ) );
        bool bP3 = quadrantRect.contains(rectangle.br());
        bool bP4 = quadrantRect.contains( rectangle.br() - cv::Point2i( rectangle.width, 0 ) );
        return bP1 || bP2 || bP3 || bP4;
    };

    for( auto i = 0; i < matRow; i++ )
    {
        for( auto j = 0; j < matCol; j++ )
        {
            matCount[i][j] += l_isInsideRectangle( quadMatrix[i][j] ) ? 1 : 0;
        }
    }
}

void FindMaxRectQuadrant(
    const std::vector<std::vector<uint32_t>>& matCount,
    const uint32_t& matRow, const uint32_t& matCol,
    uint32_t& maxRow, uint32_t& maxCol
)
{
    uint32_t maxCount = 0;
    for( auto i = 0u; i < matRow; i++ )
    {
        for( auto j = 0u; j < matCol; j++ )
        {
            if( matCount[i][j] > maxCount )
            {
                maxCount = matCount[i][j];
                maxRow = i;
                maxCol = j;
            }
        }
    }
}

void DrawQuadrantImage(
    cv::Mat img,
    const uint32_t& matRow, const uint32_t& matCol,
    const std::vector<std::vector<cv::Rect>>& quadMatrix
)
{
    cv::Mat quadImg = img.clone();

    for( auto i = 0u; i < matRow; i++ )
    {
        for( auto j = 0u; j < matCol; j++ )
        {
            cv::rectangle( quadImg, quadMatrix[i][j], cv::Scalar( 255, 0, 255 ), 1 );
        }
    }

    cv::imwrite( "/home/lg/workspace/Qt_Proj/CVQuadrants/Quadrants.png", quadImg );
}

void printCountMatrix(
    const uint32_t& matRow, const uint32_t& matCol,
    const std::vector<std::vector<uint32_t>>& matCount
)
{
    for( auto i = 0u; i < matRow; i++ )
    {
        for( auto j = 0u; j < matCol; j++ )
        {
            std::cout << matCount[i][j];
            if(j != (matCol - 1))
            {
                std::cout << ", ";
            }
        }
        std::cout << "\n";
    }
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

    cv::Mat kernel = cv::getStructuringElement( cv::MORPH_RECT, cv::Size(10, 10));

    cv::Mat dilatedImg = cv::Mat( img1.rows, img1.cols, CV_8UC1 );
    cv::dilate( thresh, dilatedImg, kernel, cv::Point(-1, -1), 2 );

    std::vector<cv::Mat> contours{};
    cv::findContours( dilatedImg, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE );

    cv::Mat finalImg1 = img1.clone(); //cv::Mat( img1.rows, img1.cols, img1.type() );
    cv::Mat finalImg2 = img2.clone(); //cv::Mat( img2.rows, img2.cols, img2.type() );

    int matRow = 4;
    int matCol = 4;
    std::vector<std::vector<cv::Rect>> quadMatrix;
    SplitQuadrants(
        finalImg2,
        quadMatrix,
        matRow, matCol
    );
    std::vector<std::vector<uint32_t>> matCount;
    matCount.resize( matRow );
    for( auto i = 0u; i < matRow; i++ )
    {
        matCount[i].resize(matCol);
    }

    for( auto i = 0u; i < contours.size(); i++ )
    {
        cv::Rect boundRect = cv::boundingRect( contours[i] );
        cv::rectangle(finalImg1, boundRect, cv::Scalar(208, 224, 64), 2);
        cv::rectangle(finalImg2, boundRect, cv::Scalar(208, 224, 64), 2);
        DetermineQuadrant(
            boundRect,
            matRow, matCol,
            quadMatrix, matCount
        );
    }

    printCountMatrix( matRow, matCol, matCount );
    uint32_t maxRow, maxCol;

    DrawQuadrantImage(
        finalImg2,
        matRow, matCol,
        quadMatrix
    );
    FindMaxRectQuadrant(
        matCount,
        matRow, matCol,
        maxRow, maxCol
    );

    cv::Mat focusImg = finalImg2.clone();
    cv::rectangle( focusImg, quadMatrix[maxRow][maxCol], cv::Scalar( 0, 255, 0 ), 2 );

    cv::imwrite( "/home/lg/workspace/Qt_Proj/CVQuadrants/FinalImg2.png", finalImg2 );
    cv::imwrite( "/home/lg/workspace/Qt_Proj/CVQuadrants/FinalImg1.png", finalImg1 );
    cv::imwrite( "/home/lg/workspace/Qt_Proj/CVQuadrants/FocusImg.png", focusImg );

    return 0;
}
