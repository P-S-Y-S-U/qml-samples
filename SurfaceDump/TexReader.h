#ifndef TEX_READER_H
#define TEX_READER_H

#include <cstdint>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <memory>

/**
 Using PBO to download is not encouraged, since the extension needed for it to do it on OpenGL ES 2
 is not available on TV board.
 ** Extensions needed for PBO **
    - GL_EXT_map_buffer_range
    - GL_OES_mapbuffer
    - GL_NV_pixel_buffer_object
 If these Extensions are advertised in the System's API implementation, remove the below #define
 */
#define TEXREADER_PBO_NOT_USED 

class TexReader
{
public:
    enum CaptureTarget
    {
        DEFAULT_FRAME_BUFFER,
        TEX_ID
    };

    enum TexDownloadApproach
    {
        FBO,
        PBO
    };

    ~TexReader();

    static TexReader& GetInstance();

    bool DownloadPixelsFromGPU(
        const CaptureTarget& target,
        const TexDownloadApproach& downloadApproach,
        const std::uint32_t& xOffset, const std::uint32_t& yOffset,
        const std::uint32_t& imgWidth, const std::uint32_t& imgHeight,
        const GLuint& texId, const GLenum& pixelFormatToPack,
        uint8_t* pCPUpixeldump, const std::size_t& pixelDumpSizeInBytes
    );
#ifndef TEXREADER_PBO_NOT_USED
    void Init(
        const std::uint32_t& numOfPBOs,
        const std::uint32_t& asyncDownloadLimit
    );
    PFNGLMAPBUFFERRANGEEXTPROC gl_map_buffer_range_EXT;
    PFNGLUNMAPBUFFEROESPROC gl_unmap_buffer_oes;
#endif
private:
    TexReader();
    
    static std::unique_ptr<TexReader, std::default_delete<TexReader>> m_pTexReaderSingleton;

#ifndef TEXREADER_PBO_NOT_USED
    GLuint* m_glPBOarray;
    std::size_t m_pboBufferSizeInBytes;
    std::uint32_t m_numOfPBOs;
    GLenum m_pbobuffertype;

    std::uint32_t m_currentDownload;
    std::uint32_t m_totalDownload;
    std::uint32_t m_asyncDownloadLimit;
#endif

    bool DownloadUsingFBO(
        const CaptureTarget& target,
        const std::uint32_t& xOffset, const std::uint32_t& yOffset,
        const std::uint32_t& imgWidth, const std::uint32_t& imgHeight,
        const GLuint& texId, const GLenum& pixelFormatToPack,
        uint8_t* pCPUpixeldump, const std::size_t& pixelDumpSizeInBytes
    );

    bool DownloadUsingPBO(
        const CaptureTarget& target,
        const std::uint32_t& xOffset, const std::uint32_t& yOffset,
        const std::uint32_t& imgWidth, const std::uint32_t& imgHeight,
        const GLuint& texId, const GLenum& pixelFormatToPack,
        uint8_t* pCPUpixeldump, const std::size_t& pixelDumpSizeInBytes
    );

#ifndef TEXREADER_PBO_NOT_USED
    void AllocatePBOs( const std::size_t& bufferSize );
    void DestroyPBOs();
#endif

};

#endif