#ifndef SURFACEDUMPER_H
#define SURFACEDUMPER_H

#include <QImage>
#include <QFile>

#include "TexReader.h"

class SurfaceDumper : public QObject
{
    Q_OBJECT
public:
    SurfaceDumper() = default;
    virtual ~SurfaceDumper() = default;

    Q_INVOKABLE void acquireStates()
    {
            bValidCapture = true;
    }

public slots:

    void ReadAndDump()
    {
        uint32_t imgWidth = 1920;
        uint32_t imgHeight = 1080;

        QFile imgFile{"SurfaceDump.png"};

        if( !imgFile.exists() && bValidCapture )
        {
            QImage dumpImage{ imgWidth, imgHeight, QImage::Format_ARGB32_Premultiplied};

            TexReader::GetInstance().DownloadPixelsFromGPU(
                   TexReader::DEFAULT_FRAME_BUFFER,
                   TexReader::FBO,
                   0, 0, 1920, 1080, -1,
                   GL_RGBA,
                   dumpImage.bits(), dumpImage.sizeInBytes()
            );

            dumpImage.save("SurfaceDump.png");
        }
    }

private:
    bool bValidCapture;
};

#endif // SURFACEDUMPER_H
