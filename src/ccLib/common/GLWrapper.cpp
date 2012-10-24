#include "GLWrapper.h"
#include "UIGLView.h"
#include "VideoDef.h"

namespace CCPlayer
{

CCGLWrapper::CCGLWrapper()
:m_pGLRenderView(NULL)
{
}

CCGLWrapper::~CCGLWrapper()
{
}

void CCGLWrapper::SetGLRenderView(CCUIGLView* pGLRenderView)
{
    m_pGLRenderView = pGLRenderView;
}

void CCGLWrapper::InitGL()
{
    glViewport(0, 0, 800, 600);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)800/(GLfloat)600, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

int CCGLWrapper::CreateGLContext()
{
    m_pGLRenderView->CreateGLContext();

    glGenTextures(1, &m_glTexture);
    glBindTexture(GL_TEXTURE_2D, m_glTexture);
    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S,
                    GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T,
                    GL_CLAMP);

    //Init the render rect
	InitGL();

    return 0;
}

int CCGLWrapper::DrawFrame(VideoFrame* pVideoFrame, int width, int height)
{
    glBindTexture(GL_TEXTURE_2D, m_glTexture);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 VIDEO_OUTPUT_WIDTH,
                 VIDEO_OUTPUT_WIDTH,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 pVideoFrame->GetFrameData());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);

    //Draw the image
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -4.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -4.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, -4.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -4.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    m_pGLRenderView->SwapBuffers();
	return 0;
}

}
