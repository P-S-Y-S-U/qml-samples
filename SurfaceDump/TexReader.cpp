#include "TexReader.h"
#include <string>
#include <cstring>
#include <memory>
#include <limits>
#include <EGL/egl.h>

#include <unistd.h>

std::unique_ptr<TexReader, std::default_delete<TexReader>> TexReader::m_pTexReaderSingleton = nullptr;

GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        //qInfo() << "Mohan Log " << error.c_str() << " | " << file << " (" << line << ")";
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

#ifndef TEXREADER_PBO_NOT_USED
TexReader::TexReader()
    :m_glPBOarray{ nullptr }
    ,m_pboBufferSizeInBytes{ 0 }
    ,m_numOfPBOs{ 1 }
    ,m_pbobuffertype{ GL_PIXEL_PACK_BUFFER_NV }
    ,m_currentDownload{ 0 }
    ,m_totalDownload{ 0 }
    ,m_asyncDownloadLimit{ 2 }
{
    gl_map_buffer_range_EXT = reinterpret_cast<PFNGLMAPBUFFERRANGEEXTPROC>( eglGetProcAddress("glMapBufferRangeEXT") );
    gl_unmap_buffer_oes = reinterpret_cast<PFNGLUNMAPBUFFEROESPROC>( eglGetProcAddress( "glUnmapBufferOES" ) );
}
TexReader::~TexReader()
{
    DestroyPBOs();
}

void TexReader::Init(
    const std::uint32_t& numOfPBOs,
    const std::uint32_t& asyncDownloadLimit
)
{
    m_numOfPBOs = numOfPBOs;
    m_asyncDownloadLimit = asyncDownloadLimit;
}

#else
TexReader::TexReader()
{}

TexReader::~TexReader()
{}
#endif

TexReader& TexReader::GetInstance()
{
    if( !m_pTexReaderSingleton.get() )
    {
        m_pTexReaderSingleton = std::move( std::unique_ptr<TexReader, std::default_delete<TexReader>>{ new TexReader } );
    }
    return *m_pTexReaderSingleton;
}

bool TexReader::DownloadPixelsFromGPU(
    const CaptureTarget& target,
    const TexDownloadApproach& downloadApproach,
    const std::uint32_t& xOffset, const std::uint32_t& yOffset,
    const std::uint32_t& imgWidth, const std::uint32_t& imgHeight,
    const GLuint& texId, const GLenum& pixelFormatToPack,
    uint8_t* pCPUpixeldump, const std::size_t& pixelDumpSizeInBytes
)
{
    switch(downloadApproach)
    {
    case TexDownloadApproach::FBO:
        return DownloadUsingFBO( 
            target, 
            xOffset, yOffset,
            imgWidth, imgHeight,
            texId, pixelFormatToPack,
            pCPUpixeldump, pixelDumpSizeInBytes
        );
    case TexDownloadApproach::PBO:
        return DownloadUsingPBO(
            target,
            xOffset, yOffset,
            imgWidth, imgHeight,
            texId, pixelFormatToPack,
            pCPUpixeldump, pixelDumpSizeInBytes
        );
    default:
        return false;
    }
}

bool TexReader::DownloadUsingFBO(
    const CaptureTarget& target,
    const std::uint32_t& xOffset, const std::uint32_t& yOffset,
    const std::uint32_t& imgWidth, const std::uint32_t& imgHeight,
    const GLuint& texId, const GLenum& pixelFormatToPack,
    uint8_t* pCPUpixeldump, const std::size_t& pixelDumpSizeInBytes
)
{
    bool downloadReslt = false;

    GLenum status;
    GLuint fbo;

    auto l_revertGLState = [&fbo](){
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &fbo);
    };

    if( target == CaptureTarget::DEFAULT_FRAME_BUFFER )
    {
        /*
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        status = glCheckFramebufferStatus( GL_FRAMEBUFFER );

        if (status != GL_FRAMEBUFFER_COMPLETE) {
            //qCritical() << "TexReader::DownloadUsingFBO " << "fbo error: " << status;
            return false;
	    }
        */

        glReadPixels(
            xOffset, yOffset,
            imgWidth, imgHeight,
            pixelFormatToPack,
            GL_UNSIGNED_BYTE,
            pCPUpixeldump
        );

        if( glCheckError() )
        {
            //qWarning() << "TexReader::DownloadUsingFBO "<< "Surface Dump Using FBO failed" << status;
            return false;
        }

        downloadReslt = true;
    }
    else if( target == CaptureTarget::TEX_ID )
    {
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glBindTexture(GL_TEXTURE_2D, texId);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);

        status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	    if (status != GL_FRAMEBUFFER_COMPLETE) {
            //qCritical() << "TexReader::DownloadUsingFBO " << "fbo error: " << status;

            l_revertGLState();
            return false;
	    }

        glReadPixels(
            xOffset, yOffset,
            imgWidth, imgHeight,
            pixelFormatToPack,
            GL_UNSIGNED_BYTE,
            pCPUpixeldump
        );

        if( glCheckError() )
        {
            //qWarning() << "TexReader::DownloadUsingFBO " << "Surface Dump Using FBO failed" << status;
            l_revertGLState();
            return false;
        }

        l_revertGLState();
        downloadReslt = true;
    }

    return downloadReslt;
}

#ifndef TEXREADER_PBO_NOT_USED

bool TexReader::DownloadUsingPBO(
    const CaptureTarget& target,
    const std::uint32_t& xOffset, const std::uint32_t& yOffset,
    const std::uint32_t& imgWidth, const std::uint32_t& imgHeight,
    const GLuint& texId, const GLenum& pixelFormatToPack,
    uint8_t* pCPUpixeldump, const std::size_t& pixelDumpSizeInBytes
)
{
    bool downloadReslt = false;

    GLenum status;
    GLuint fbo;

    //qInfo() << "Mohan Log " << pixelDumpSizeInBytes << " vs " << m_pboBufferSizeInBytes;

    if( pixelDumpSizeInBytes != m_pboBufferSizeInBytes )
    {
        if( m_pboBufferSizeInBytes != 0 )
            DestroyPBOs();
        AllocatePBOs( pixelDumpSizeInBytes );
    }

    uint8_t* pMappedBuffer = nullptr;

    auto l_revertGLState = [&fbo](){
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &fbo);
    };

    auto l_triggerOrCaptureUsingPBO = [&]() {
        if( m_totalDownload < m_numOfPBOs )
        {
            glBindBuffer( m_pbobuffertype, m_glPBOarray[m_currentDownload] );

            glReadPixels(
                xOffset, yOffset,
                imgWidth, imgHeight,
                pixelFormatToPack,
                GL_UNSIGNED_BYTE,
                0
            );

            qInfo() << "Mohan Log " << "glReadPixels() with pbo: " << m_glPBOarray[m_currentDownload];
        }
        else 
        {
            glBindBuffer( m_pbobuffertype, m_glPBOarray[m_currentDownload] );
            
            pMappedBuffer = reinterpret_cast<uint8_t*>( gl_map_buffer_range_EXT(
                m_pbobuffertype,
                0,
                m_pboBufferSizeInBytes,
                GL_MAP_READ_BIT_EXT
            ));

            if( pMappedBuffer )
            {
                std::memcpy( pCPUpixeldump, pMappedBuffer, pixelDumpSizeInBytes );
                gl_unmap_buffer_oes( m_pbobuffertype );
            }
            else 
            {
                qInfo() << "Mohan Log " << "Failed to Map the Buffer";
            }

            qInfo() << "Mohan Log " << "Trigger For Next Capture";
            qInfo() << "Mohan Log " << "glReadPixels() with pbo: " << m_glPBOarray[m_currentDownload];

            glReadPixels(
                xOffset, yOffset,
                imgWidth, imgHeight,
                pixelFormatToPack,
                GL_UNSIGNED_BYTE,
                0
            );
        }

        m_currentDownload++;
        m_currentDownload = m_currentDownload % m_numOfPBOs;
        m_totalDownload++;

        if( m_totalDownload == std::numeric_limits<uint32_t>::max() )
        {
            m_totalDownload = m_numOfPBOs;
        }
    };

    if( target == CaptureTarget::TEX_ID )
    {
        glGenFramebuffers(1, &fbo);
        glCheckError();
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glCheckError();
        glBindTexture(GL_TEXTURE_2D, texId);
        glCheckError();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
        glCheckError();

        status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	    if (status != GL_FRAMEBUFFER_COMPLETE) {
	    	qInfo() << "Mohan Log " << "fbo error: " << status;

            l_revertGLState();
            return false;
	    }
    }

    for( auto i = 0u; i < m_asyncDownloadLimit; i++ )
    {
        l_triggerOrCaptureUsingPBO();
    }

    downloadReslt = true;

    if( target == CaptureTarget::TEX_ID )
    {
        l_revertGLState();
    }

    return downloadReslt;
}

void TexReader::AllocatePBOs( const std::size_t& bufferSize )
{
    qInfo() <<"Mohan Log " << "Allocating PBOs";

    m_glPBOarray = new GLuint[m_numOfPBOs];
    m_currentDownload = 0;
    m_totalDownload = 0;
    m_pboBufferSizeInBytes = bufferSize;

    glGenBuffers( m_numOfPBOs, m_glPBOarray );
    glCheckError();
    for( uint32_t i = 0; i < m_numOfPBOs; i++ )
    {
        glBindBuffer( m_pbobuffertype, m_glPBOarray[i]);
        glCheckError();
        glBufferData( 
            m_pbobuffertype, 
            m_pboBufferSizeInBytes,
            NULL,
            GL_STREAM_DRAW
        );
        glCheckError();

        qInfo() << "Mohan Log " << "Binded PBO[" << i << "] = " << m_glPBOarray[i] << ", with " << m_pboBufferSizeInBytes << " bytes\n";
    }

    glBindBuffer( m_pbobuffertype, 0 );
}

void TexReader::DestroyPBOs()
{
    qInfo() << "Mohan Log " << " Deleting PBOs";
    glBindBuffer( m_pbobuffertype, 0 );
    glDeleteBuffers( m_numOfPBOs, m_glPBOarray );
    delete m_glPBOarray;
    m_glPBOarray = nullptr;
}

#else
bool TexReader::DownloadUsingPBO(
    const CaptureTarget& target,
    const std::uint32_t& xOffset, const std::uint32_t& yOffset,
    const std::uint32_t& imgWidth, const std::uint32_t& imgHeight,
    const GLuint& texId, const GLenum& pixelFormatToPack,
    uint8_t* pCPUpixeldump, const std::size_t& pixelDumpSizeInBytes
)
{
    //qWarning() << "PBO Download Approach is not recommended, check whether all the extensions are available that are needed for this approach";
    return false;
}

#endif 
