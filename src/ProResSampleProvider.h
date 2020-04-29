#ifndef _PRORESSAMPLEPROVIDER_H_
#define _PRORESSAMPLEPROVIDER_H_

#include "ISampleProviderWithDescriptor.h"
#include "MediaGaps/IO/FileAlignedSource.h"
#include "MediaGaps/Media/PictureTrackDescriptor.h"

namespace MOVCreator
{
    class ProResSampleProvider : public ISampleProviderWithDescriptor
    {
        MediaGaps::IO::FileAlignedSource _source;

        MediaGaps::Media::MediaSampleSharedPtr _cache;

        bool _finished = false;

        MediaGaps::UInt64 _readSamples = 0;

        MediaGaps::Media::PictureTrackDescriptor _pictureTrackDescriptor;

        void LoadCache(bool loadTrackDescriptor = false);

    public:
        ProResSampleProvider(const wchar_t* source_name, MediaGaps::Media::TrackDescriptorEssenceKind essencekind, MediaGaps::Rational32 editRate);

        ~ProResSampleProvider();

        MediaGaps::Media::MediaSampleSharedPtr GetNextSample() override;

        bool IsNextSampleAvailable() const override;

        void Abort() override;

        const MediaGaps::Media::TrackDescriptor& Descriptor() const override;
    };
}

#endif //_PRORESSAMPLEPROVIDER_H_
