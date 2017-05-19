/**
 * @file   SlaveNode.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.11.25
 *
 * @brief  Declaration of the ApplicationNodeImplementation for the slave node.
 */

#pragma once

#include "core/SlaveNodeInternal.h"

namespace viscom {

    class SlaveNode final : public SlaveNodeInternal
    {
    public:
        explicit SlaveNode(ApplicationNode* appNode);
        virtual ~SlaveNode() override;

        void Draw2D(FrameBuffer& fbo) override;
        virtual void UpdateSyncedInfo() override;

        virtual void EncodeData() override;
        virtual void DecodeData() override;

    private:
        /** Holds the data shared by the master. */
        sgct::SharedObject<SimulationData> sharedData_;

    };
}
