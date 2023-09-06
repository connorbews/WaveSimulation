#include "../../../include/WaveSimProj/Setup/OpenGLSetup.h"

OpenGLSetup::OpenGLSetup()
{
	gladLoadGL();
	glViewport(0, 0, 800, 800);

    glEnable(GL_DEPTH_TEST);
}

void OpenGLSetup::ProcessErrors()
{
    GLenum err = glGetError();
    while (err != GL_NO_ERROR)
    {
        std::cout << "error: " << err << std::endl;
        err = glGetError();

    }
}

void OpenGLSetup::SetBackgroundColour(float x, float y, float z, float w)
{
    glClearColor(x, y, z, w);
}

void OpenGLSetup::CleanBuffer()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLSetup::GetMaxWorkGroupSize()
{
    int work_grp_cnt[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);
	std::cout << "Max work groups per compute shader" << 
		" x:" << work_grp_cnt[0] <<
		" y:" << work_grp_cnt[1] <<
		" z:" << work_grp_cnt[2] << std::endl;

	int work_grp_size[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);
	std::cout << "Max work group sizes" <<
		" x:" << work_grp_size[0] <<
		" y:" << work_grp_size[1] <<
		" z:" << work_grp_size[2] << std::endl;
}

void OpenGLSetup::GetMaxInvocations()
{
    int work_grp_inv;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
	std::cout << "Max invocations count per work group: " << work_grp_inv << std::endl;
}