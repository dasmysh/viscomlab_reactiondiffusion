/**
 * @file   SlaveNode.cpp
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.11.25
 *
 * @brief  Implementation of the slave application node.
 */

#include "SlaveNode.h"

namespace viscom {

    SlaveNode::SlaveNode(ApplicationNode* appNode) :
        SlaveNodeInternal{ appNode }
    {
    }

    SlaveNode::~SlaveNode() = default;

    void SlaveNode::Draw2D(FrameBuffer& fbo)
    {
        // always do this call last!
        SlaveNodeInternal::Draw2D(fbo);
    }

    void SlaveNode::UpdateSyncedInfo()
    {
        SlaveNodeInternal::UpdateSyncedInfo();
        GetGlobalIterationCount() = GetGlobalIterationCountShared().getVal();
    }

}
