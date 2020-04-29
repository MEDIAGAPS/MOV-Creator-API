#ifndef _MEDIAGAPS_MEDIA_QUICKTIME_QUICKTIMEWRITER_H_
#define _MEDIAGAPS_MEDIA_QUICKTIME_QUICKTIMEWRITER_H_

#include "MediaGaps/Media/IWriter.h"

#include "MediaGaps/Timecode/TimecodeInfo.h"
#include "MediaGaps/Media/IProgressIndicator.h"
#include "MediaGaps/IO/SinkNameListener.h"
#include "MediaGaps/Media/IMediaChunkSizeProvider.h"

namespace MediaGaps
{
namespace Media
{
namespace Quicktime
{
    class MEDIAGAPS_EXPORTS_KEYWORD QuicktimeWriter : public IWriter, public TimecodeInfo, public IProgressIndicator
    {
        struct Impl;
        Impl* impl_;

    public:
        /** Constructor
        * \param name the name for the asset.
        * \param target_folder the destination folder for the media files.
        */
        QuicktimeWriter(const IMediaChunkSizeProvider& mediaChunkProvider,
            const wchar_t* targetFolder,
            const wchar_t* name,
            bool selfContained = true,
            bool growing = false);

        /** Copy constructor. Not implemented. */
        QuicktimeWriter(const QuicktimeWriter&) = delete;

        /** Assignement operator. Not implemented. */
        QuicktimeWriter& operator= (const QuicktimeWriter&) = delete;

        ~QuicktimeWriter();

        void AddTrack(const TrackDescriptor& trackDescriptor, SampleProviderSharedPtr sampleProviderSharedPtr) override;

        void Start() override;

        void Abort() override;

        State GetState() const override;

        ExceptionCode GetErrorCode() const override;

        const char* GetErrorMsg() const override;

        void SetErrorListener(IErrorListener* listener) override;

        void SetStateListener(IStateListener* listener) override;

        UInt64 GetProcessedFrames() const override;

        void SetSinkNameListener(IO::SinkNameListener* listener);
    };
}
}
}

#endif //_MEDIAGAPS_MEDIA_QUICKTIME_QUICKTIMEWRITER_H_
