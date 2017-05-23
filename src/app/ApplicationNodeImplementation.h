/**
 * @file   ApplicationNodeImplementation.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.11.30
 *
 * @brief  Declaration of the application node implementation common for master and slave nodes.
 */

#pragma once

#include <sgct/Engine.h>
#include "core/ApplicationNodeInternal.h"

namespace viscom {

    class MeshRenderable;
    class FullscreenQuad;

    struct SimulationData {
        /** The distance the simulation will be drawn at. */
        float simulationDrawDistance_ = 15.0f;
        /** The simulation height field height. */
        float simulationHeight_ = 1.0f;
        /** The relative index of refraction used for raycasting. */
        float eta_ = 1.5f;
        /** The absorption coefficient. */
        float sigma_a_ = 2.0f;
        /** The current global iteration count. */
        std::uint64_t currentGlobalIterationCount_ = 0;
    };

    class ApplicationNodeImplementation
    {
    public:
        explicit ApplicationNodeImplementation(ApplicationNodeInternal* appNode);
        ApplicationNodeImplementation(const ApplicationNodeImplementation&) = delete;
        ApplicationNodeImplementation(ApplicationNodeImplementation&&) = delete;
        ApplicationNodeImplementation& operator=(const ApplicationNodeImplementation&) = delete;
        ApplicationNodeImplementation& operator=(ApplicationNodeImplementation&&) = delete;
        virtual ~ApplicationNodeImplementation();

        virtual void PreWindow();
        virtual void InitOpenGL();
        virtual void PreSync();
        virtual void UpdateSyncedInfo();
        virtual void UpdateFrame(double currentTime, double elapsedTime);
        virtual void ClearBuffer(FrameBuffer& fbo);
        virtual void DrawFrame(FrameBuffer& fbo);
        virtual void Draw2D(FrameBuffer& fbo);
        virtual void PostDraw();
        virtual void CleanUp();

        virtual void KeyboardCallback(int key, int scancode, int action, int mods);
        virtual void CharCallback(unsigned int character, int mods);
        virtual void MouseButtonCallback(int button, int action);
        virtual void MousePosCallback(double x, double y);
        virtual void MouseScrollCallback(double xoffset, double yoffset);

        virtual void EncodeData();
        virtual void DecodeData();

    protected:
        sgct::Engine* GetEngine() const { return appNode_->GetEngine(); }
        const FWConfiguration& GetConfig() const { return appNode_->GetConfig(); }
        ApplicationNodeInternal* GetApplication() const { return appNode_; }

        const Viewport& GetViewportScreen(size_t windowId) const { return appNode_->GetViewportScreen(windowId); }
        Viewport& GetViewportScreen(size_t windowId) { return appNode_->GetViewportScreen(windowId); }
        const glm::ivec2& GetViewportQuadSize(size_t windowId) const { return appNode_->GetViewportQuadSize(windowId); }
        glm::ivec2& GetViewportQuadSize(size_t windowId) { return appNode_->GetViewportQuadSize(windowId); }
        const glm::vec2& GetViewportScaling(size_t windowId) const { return appNode_->GetViewportScaling(windowId); }
        glm::vec2& GetViewportScaling(size_t windowId) { return appNode_->GetViewportScaling(windowId); }

        double GetCurrentAppTime() const { return appNode_->GetCurrentAppTime(); }
        double GetElapsedTime() const { return appNode_->GetElapsedTime(); }

        std::uint64_t& GetCurrentLocalIterationCount() { return currentLocalIterationCount_; }
        SimulationData& GetSimulationData() { return simData_; }

        /** The maximum iteration count per frame. */
        static constexpr std::uint64_t MAX_FRAME_ITERATIONS = 20;

        /** The simulation frame buffer size (x). */
        static constexpr unsigned int SIMULATION_SIZE_X = 1920;
        /** The simulation frame buffer size (y). */
        static constexpr unsigned int SIMULATION_SIZE_Y = 1080;

    private:
        /** Holds the application node. */
        ApplicationNodeInternal* appNode_;

        /** The current local iteration count. */
        std::uint64_t currentLocalIterationCount_ = 0;
        /** Holds the simulation data. */
        SimulationData simData_;

        /** Toggle switch for iteration step */
        bool iterationToggle_ = true;
        /** seed points to draw into simulation */
        std::vector<glm::vec2> rdSeedPoints;
        /** Uniform Location for texture sampler of previous iteration step */
        GLint rdPrevIterationTextureLoc_ = -1;
        GLint rdDiffusionRateALoc_ = -1;
        GLint rdDiffusionRateBLoc_ = -1;
        GLint rdFeedRateLoc_ = -1;
        GLint rdKillRateLoc_ = -1;
        GLint rdDtLoc_ = -1;
        GLint rdSeedPointRadiusLoc_ = -1;
        GLint rdNumSeedPointsLoc_ = -1;
        GLint rdSeedPointsLoc_ = -1;
        GLint rdUseManhattenDistanceLoc_ = -1;

        /** Program to compute reaction diffusion step */
        std::unique_ptr<FullscreenQuad> reactionDiffusionFullScreenQuad_;

        /** The frame buffer object for the simulation. */
        std::unique_ptr<FrameBuffer> reactDiffuseFBO_;
        /** The frame buffer objects for the simulation height field back. */
        std::vector<FrameBuffer> simulationBackFBOs_;

        /** Holds the shader program for raycasting the height field back side. */
        std::shared_ptr<GPUProgram> raycastBackProgram_;
        /** Holds the location of the VP matrix. */
        GLint raycastBackVPLoc_ = -1;
        /** Holds the location of the simulation quad size. */
        GLint raycastBackQuadSizeLoc_ = -1;
        /** Holds the location of the simulation quad distance. */
        GLint raycastBackDistanceLoc_ = -1;

        /** Holds the shader program for raycasting the height field. */
        std::shared_ptr<GPUProgram> raycastProgram_;
        /** Holds the location of the VP matrix. */
        GLint raycastVPLoc_ = -1;
        /** Holds the location of the simulation quad size. */
        GLint raycastQuadSizeLoc_ = -1;
        /** Holds the location of the simulation quad distance. */
        GLint raycastDistanceLoc_ = -1;
        /** Holds the location of the simulation height. */
        GLint raycastSimHeightLoc_ = -1;
        /** Holds the location of the camera position. */
        GLint raycastCamPosLoc_ = -1;
        /** Holds the location of index of refraction. */
        GLint raycastEtaLoc_ = -1;
        /** Holds the location of the absorption coefficient. */
        GLint raycastSigmaALoc_ = -1;
        /** Holds the location of the environment map. */
        GLint raycastEnvMapLoc_ = -1;
        /** Holds the location of the background texture. */
        GLint raycastBGTexLoc_ = -1;
        /** Holds the location of the height texture. */
        GLint raycastHeightTextureLoc_ = -1;
        /** Holds the location of the back position texture. */
        GLint raycastPositionBackTexLoc_ = -1;

        /** Holds the dummy VAO for the simulation quad. */
        GLuint simDummyVAO_ = 0;
        /** Holds the background texture for the simulation. */
        std::shared_ptr<Texture> backgroundTexture_;
        /** Holds the environment map texture. */
        std::shared_ptr<Texture> environmentMap_;

#ifndef VISCOM_LOCAL_ONLY
    protected:
        unsigned int GetGlobalProjectorId(int nodeId, int windowId) const { return appNode_->GetGlobalProjectorId(nodeId, windowId); }
#endif
    };
}
