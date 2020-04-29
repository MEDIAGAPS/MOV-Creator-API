#include "MediaSampleImpl.h"

#include "MediaGaps/Media/MediaSampleSharedPtr.h"

using namespace MediaGaps::Media;
using namespace MediaGaps;

using namespace MOVCreator;

bool MediaSampleImpl::GetIsReferenceFrameOnly() const
{
    // All frames sent to QuicktimeWriter will have to be playable.
    return true;
}

MediaSampleSharedPtr MediaSampleImpl::Clone()
{
    return MediaSampleSharedPtr(new MediaSampleImpl(*this));
}

UInt64 MediaSampleImpl::GetTimestamp() const
{
    //For now QuicktimeWriter only supports IFrame only essence so it'll calculate this.
    return 0;
}

UInt64 MediaSampleImpl::GetDurationTimestamp() const
{
    //For now QuicktimeWriter only supports IFrame only essence so it'll calculate this.
    return 0;
}

bool MediaSampleImpl::GetIsSingleBufferData() const
{
    return this->GetListSize() == 1;
}
