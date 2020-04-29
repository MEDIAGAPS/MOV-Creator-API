#include "MediaGaps/Media/Quicktime/QuicktimeWriter.h"

#include "BufferDataImpl.h"
#include "CustomLoggerAdapter.h"
#include "Helpers.h"
#include "MediaChunkSizeProviderImpl.h"
#include "ProResSampleProvider.h"
#include "WaveSampleProvider.h"

#include <Windows.h>

#include <iostream>
#include <codecvt>
#include <locale>

using namespace MediaGaps::Memory;
using namespace MediaGaps::Media::Quicktime;
using namespace MediaGaps::Media;
using namespace MediaGaps;

using namespace MOVCreator;

void program_usage (const char* progrname)
{
    std::cout << "Invalid program usage. Use program as:" << std::endl << std::endl;
    std::cout << progrname << " <output_dir> <output_filename> <input_raw_video> <input_wave_file> <video_type> <edit_rate>" << std::endl << std::endl;
    std::cout << "  video_type [ prores422 | prores422lt | prores422proxy]" << std::endl;
    std::cout << "  edit_rate [23.98 | 24 | 25 | 29.97 | 30 | 50 | 59.94 | 60 ]" << std::endl;

    //Tip: Use the following command line from the location of the executable to use the demo samples
    // MOVCreatorDemo.exe ..\data fromDemo ..\data\sample_video.prores ..\data\sample_audio.wav prores422 30
}

std::wstring toWstring(std::string str)
{
    return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
}

TrackDescriptorEssenceKind essenceKindFromArg (std::string essenceArg)
{
    if (essenceArg == "prores422") return TrackDescriptorEssenceKind_ProRes_422;
    if (essenceArg == "prores422lt") return TrackDescriptorEssenceKind_ProRes_422LT;
    if (essenceArg == "prores422proxy") return TrackDescriptorEssenceKind_ProRes_422Proxy;

    return TrackDescriptorEssenceKind_Unknown;    
}

Rational32 editRateFromArg(std::string essenceArg)
{
    if (essenceArg == "23.98") return { 24000, 1001 };
    if (essenceArg == "24") return { 24, 1 };
    if (essenceArg == "25") return { 25, 1};
    if (essenceArg == "29.97") return { 30000, 1001 };
    if (essenceArg == "30") return { 30, 1 };
    if (essenceArg == "50") return { 50, 1 };
    if (essenceArg == "59.94") return { 60000, 1001 };
    if (essenceArg == "60") return { 60, 1 };

    return { 0, 1};
}

int main(int argc, char* argv[])
{
    if (argc != 7)
    {
        program_usage(argv[0]);
        return -1;
    }

    try
    {
        const std::wstring outputDir = toWstring(argv[1]);
        const std::wstring outputFilename = toWstring(argv[2]);

        const std::wstring sourceVideoFile = toWstring(argv[3]);
        const std::wstring sourceAudioFile = toWstring(argv[4]);

        const TrackDescriptorEssenceKind essenceKind = essenceKindFromArg(argv[5]);
        if (essenceKind == TrackDescriptorEssenceKind_Unknown)
        {
            program_usage(argv[0]);
            return -1;
        }

        const Rational32 editRate = editRateFromArg(argv[6]);
        if (editRate.Numerator == 0)
        {
            program_usage(argv[0]);
            return -1;
        }

        std::cout << "Starting demo." << std::endl;

        // Start receiving  internal logs from the API
        CustomLoggerAdapter customLoggerAdapter;

        // Instantiate object that will configure the number of video samples per chunk
        const MediaChunkSizeProviderImpl mediaChunkSizeProvider;

        // Instantiate Quicktime writing API by indicating the location and name of the target file
        QuicktimeWriter quicktimeWriter(mediaChunkSizeProvider, outputDir.c_str(), outputFilename.c_str());

        //Set the initial timecode of the file
        const Timecode initialTimecode( static_cast<UInt16> (editRate.Round()), 1, 0, 0, 0, editRate.Denominator != 1);
        quicktimeWriter.SetInitialTimecode(initialTimecode);


        // Create an instance to provide video samples
        SampleProviderSharedPtr video_sample_provider(new ProResSampleProvider(sourceVideoFile.c_str(), essenceKind, editRate));
        const auto video_sample_provider_with_descriptor = dynamic_cast<ISampleProviderWithDescriptor*> (video_sample_provider.Get());

        // Add the video provider to the writing API
        quicktimeWriter.AddTrack(video_sample_provider_with_descriptor->Descriptor(), video_sample_provider);

        // Create an instance to provide audio samples
        SampleProviderSharedPtr audio_sample_provider(new WaveSampleProvider(sourceAudioFile.c_str()));
        const auto audio_sample_provider_with_descriptor = dynamic_cast<ISampleProviderWithDescriptor*> (audio_sample_provider.Get());

        // Add the audio provider to the writing API
        quicktimeWriter.AddTrack(audio_sample_provider_with_descriptor->Descriptor(), audio_sample_provider);

        // Start the creation of the file
        quicktimeWriter.Start();

        //Wait until the process as finished
        while (!Helpers::IsFinishedState(quicktimeWriter.GetState()))
        {
            ::Sleep(1000);
        }

        // Check de final result
        if (quicktimeWriter.GetState() == IStateProvider::State_Finished)
        {
            std::cout << "Success while generating quicktime file" << std::endl;
        }
        else
        {
            std::cout << "Failed to generated quicktime file with message; " << quicktimeWriter.GetErrorMsg() << std::endl;
        }
    }
    catch (const Exception& ex)
    {
        std::cout << "Error while processing:" << ex.GetMGExceptionMessage() << std::endl;
    }
    catch (...)
    {        
        std::cout << "Unknown error while processing:" << std::endl;
    }

    std::cout << "Leaving demo." << std::endl;
}
