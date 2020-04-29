#ifndef _MEDIACHUNKSIZEPROVIDERIMPL_H_
#define _MEDIACHUNKSIZEPROVIDERIMPL_H_

#include "MediaGaps/Media/IMediaChunkSizeProvider.h"

namespace MOVCreator
{
    /** Im the Quicktime file essence is written in block, meaning that you'll have consecutive number of images.
     *  This class allows to define what's the size according the details of the track.
     *  Note: This is only relevant for video, audio will always write in chunks of half a second
     */
    class MediaChunkSizeProviderImpl : public MediaGaps::Media::IMediaChunkSizeProvider
    {
    public:
        MediaGaps::UInt32 chunk_size(const MediaGaps::Media::TrackDescriptor& /*track_descriptor*/) const override;
    };
}

#endif //_MEDIACHUNKSIZEPROVIDERIMPL_H_