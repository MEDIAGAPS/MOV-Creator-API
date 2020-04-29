#include "MediaChunkSizeProviderImpl.h"

using namespace MediaGaps::Media;
using namespace MediaGaps;

using namespace MOVCreator;

UInt32 MediaChunkSizeProviderImpl::chunk_size(const TrackDescriptor& /*track_descriptor*/) const
{
    //Note: this may not be optimized for certain combination of format resolution
    return 9;
}
;