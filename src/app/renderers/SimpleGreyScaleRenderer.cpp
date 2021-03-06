/**
 * @file   SimpleGreyScaleRenderer.cpp
 * @author Sebastian Maisch <sebastian.maisch@googlemail.com>
 * @date   2017.05.28
 *
 * @brief  Implementation of the simple greyscale renderer.
 */

#include "SimpleGreyScaleRenderer.h"
#include "app/ApplicationNodeImplementation.h"
#include <glm/gtc/type_ptr.hpp>
#include "core/open_gl.h"

namespace viscom::renderers {

    SimpleGreyScaleRenderer::SimpleGreyScaleRenderer(ApplicationNodeImplementation* appNode) :
        RDRenderer{ "SimpleGreyScaleRenderer", appNode }
    {
        drawGSProgram_ = appNode_->GetGPUProgramManager().GetResource("simpleGreyscaleRD", std::vector<std::string>{ "raycastHeightfield.vert", "drawGreyscale.frag" });
        drawGSVPLoc_ = drawGSProgram_->getUniformLocation("viewProjectionMatrix");
        drawGSQuadSizeLoc_ = drawGSProgram_->getUniformLocation("quadSize");
        drawGSDistanceLoc_ = drawGSProgram_->getUniformLocation("distance");
        drawGSHeightTextureLoc_ = drawGSProgram_->getUniformLocation("heightTexture");

        glGenVertexArrays(1, &simDummyVAO_);
    }

    SimpleGreyScaleRenderer::~SimpleGreyScaleRenderer()
    {
        if (simDummyVAO_ != 0) glDeleteVertexArrays(1, &simDummyVAO_);
        simDummyVAO_ = 0;
    }

    void SimpleGreyScaleRenderer::ClearBuffers(FrameBuffer& fbo)
    {
        fbo.DrawToFBO([]() {
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        });
    }

    void SimpleGreyScaleRenderer::UpdateFrame(double, double, const SimulationData& simData, const glm::vec2& nearPlaneSize)
    {
    }

    void SimpleGreyScaleRenderer::RenderRDResults(FrameBuffer& fbo, const SimulationData& simData, const glm::mat4& perspectiveMatrix, GLuint rdTexture)
    {
        fbo.DrawToFBO([this, &perspectiveMatrix, &simData, rdTexture]() {
            glBindVertexArray(simDummyVAO_);
            glUseProgram(drawGSProgram_->getProgramId());
            glUniformMatrix4fv(drawGSVPLoc_, 1, GL_FALSE, glm::value_ptr(perspectiveMatrix));
            glUniform2fv(drawGSQuadSizeLoc_, 1, glm::value_ptr(appNode_->GetSimulationOutputSize()));
            glUniform1f(drawGSDistanceLoc_, 10.0f);

            glActiveTexture(GL_TEXTURE0 + 2);
            glBindTexture(GL_TEXTURE_2D, rdTexture);
            glUniform1i(drawGSHeightTextureLoc_, 2);

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        });
    }

    void SimpleGreyScaleRenderer::DrawOptionsGUI(SimulationData& simData) const
    {
        // No parameters yet that make any difference. [5/28/2017 Sebastian Maisch]
    }
}
