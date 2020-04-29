#ifndef _WAVESAMPLEPROVIDER_H_
#define _WAVESAMPLEPROVIDER_H_

#include "ISampleProviderWithDescriptor.h"

#include "MediaGaps/IO/FileAlignedSource.h"
#include "MediaGaps/Media/SoundTrackDescriptor.h"

namespace MOVCreator
{
    class WaveSampleProvider : public ISampleProviderWithDescriptor
    {
        MediaGaps::IO::FileAlignedSource _source;

        MediaGaps::Media::MediaSampleSharedPtr _cache;

        bool _finished = false;

        MediaGaps::Media::SoundTrackDescriptor _soundTrackDescriptor;

        MediaGaps::UInt64 _maxReadSize = 0;

        void LoadCache(bool loadTrackDescriptor = false);;

    public:
        WaveSampleProvider(const wchar_t* source_name);

        MediaGaps::Media::MediaSampleSharedPtr GetNextSample() override;

        bool IsNextSampleAvailable() const override;

        void Abort() override;

        const MediaGaps::Media::TrackDescriptor& Descriptor() const;
    };
}

#endif //_WAVESAMPLEPROVIDER_H_
