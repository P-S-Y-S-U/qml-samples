#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <iostream>

int main(int argc, char *argv[])
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    cv::Mat img1 = cv::imread("/home/lg/workspace/Qt_Proj/SampleCV/Sample_1.png");
    cv::Mat img2 = cv::imread("/home/lg/workspace/Qt_Proj/SampleCV/Sample_2.png");

    cv::Mat gray1 = cv::Mat( img1.rows, img1.cols, CV_8UC1 );
    cv::Mat gray2 = cv::Mat( img2.rows, img2.cols, CV_8UC1 );

    cv::cvtColor( img1, gray1, cv::COLOR_BGR2GRAY );
    cv::cvtColor( img2, gray2, cv::COLOR_BGR2GRAY );

    cv::Mat diffGray = cv::Mat( img1.rows, img1.cols, CV_8UC1 );

    cv::absdiff( gray1, gray2, diffGray );

    bool bCheck = cv::imwrite( "/home/lg/workspace/Qt_Proj/SampleCV/DiffGray.png", diffGray );

    if( bCheck )
        std::cout << "Write Sucess" << '\n';
    else
        std::cout << "Write Failed" << '\n';

    cv::Mat thresh = cv::Mat( img1.rows, img1.cols, CV_8UC1 );
    cv::threshold(diffGray, thresh, 0, 255, cv::THRESH_BINARY);

    bCheck = cv::imwrite( "/home/lg/workspace/Qt_Proj/SampleCV/Threshold.png", thresh );

    cv::Mat kernel = cv::getStructuringElement( cv::MORPH_RECT, cv::Size(50, 50));

    cv::Mat dilatedImg = cv::Mat( img1.rows, img1.cols, CV_8UC1 );
    cv::dilate( thresh, dilatedImg, kernel, cv::Point(-1, -1), 2 );

    bCheck = cv::imwrite( "/home/lg/workspace/Qt_Proj/SampleCV/Dilated.png", dilatedImg );

    if( bCheck )
        std::cout << "Write Sucess" << '\n';
    else
        std::cout << "Write Failed" << '\n';

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

    bCheck = cv::imwrite( "/home/lg/workspace/Qt_Proj/SampleCV/FinalImg1.png", finalImg1 );
    if( bCheck )
        std::cout << "Write Sucess" << '\n';
    else
        std::cout << "Write Failed" << '\n';

    bCheck = cv::imwrite( "/home/lg/workspace/Qt_Proj/SampleCV/FinalImg2.png", finalImg2 );
    if( bCheck )
        std::cout << "Write Sucess" << '\n';
    else
        std::cout << "Write Failed" << '\n';

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
