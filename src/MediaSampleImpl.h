#ifndef _MEDIASAMPLEIMPL_H_
#define _MEDIASAMPLEIMPL_H_

#include "MediaGaps/Media/IMediaSample.h"

namespace MOVCreator
{
    /** This class implements a media sample.
     *  A media sample is composed by the a set of buffers with the data and some additional information.
     */
    class MediaSampleImpl : public MediaGaps::Media::IMediaSample
    {
    public:
        bool GetIsReferenceFrameOnly() const override;

        MediaGaps::Media::MediaSampleSharedPtr Clone() override;

        MediaGaps::UInt64 GetTimestamp() const override;
 
        MediaGaps::UInt64 GetDurationTimestamp() const override;

        bool GetIsSingleBufferData() const override;
    };
}


#endif //_MEDIASAMPLEIMPL_H_
