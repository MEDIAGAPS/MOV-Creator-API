<p align="center">
  <a href="https://www.mediagaps.com">
    <img alt="MEDIAGAPS MOV Creator API SDK" src="http://www.mediagaps.com/wp-content/uploads/2020/04/GitHub-01.png" >
  </a>
  <h4 align="center">
    MOV creator api c++ enables you to easily create .mov files in win64.
  </h4>
  <p align="center">
	<a>
	<img src="https://img.shields.io/badge/license-Apache 2.0-9cf" alt=""></img></a>
  </p>

## Overview
The MOV creator API is a C++ SDK that enables the creation of a MOV file in win64.
During sometime we were looking for a way to create MOV files in win64, but we only found win32 solutions, so Mediagaps team decided to create our own library and share a restricted portion of it with the community. 

This tool receives raw bitstream and multiplexes it inside an MOV container using MediaGaps internal libraries.

Inside this project, you can find a simple solution example to explore how to use the internal libraries api or directly use the Demo tool to generate a MOV file from raw prores stream and an WAVE file.

## Features
Here you can find a list of formats available from the internal library.

### Video Essence Kind
|  Essence Kind  | Included |
| ---    | ---       |
| Uncompressed | :x: |
| DNxHD  | :x:  |
| AVCIntra | :x:  |
| ProRes_422Proxy | :heavy_check_mark:  |
| ProRes_422LT | :heavy_check_mark:  |
| ProRes_422 | :heavy_check_mark:  |
| ProRes_422HQ | :x:  |
| ProRes_4444 | :x:  |
| ProRes_4444XQ | :x:  |
| DNxHR_444 | :x:  |
| DNxHR_HQX | :x:  |
| DNxHR_HQ | :x:  |
| DNxHR_SQ | :x:  |
| DNxHR_LB | :x:  |

### Audio essence kind

|  Essence Kind  | Included |
| ---    | ---       |
| PCM  | :heavy_check_mark:  |


Support for **Growing files** (edit-while-write) and **Quicktime reference files** are not enabled in this pack, for more information or question please contact us at info@mediagaps.com


## Building

### Using command line

```
inside root ./MOVCreator
mkdir build
cd build
cmake -G "Visual Studio 15 2017" -A x64 ../
At the end copy the bin folder from MOVcreator\bin to your build folder 
```

### Using the graphical version of CMake:

```
Open CMake GUI.
Under Where to build the binaries, same path as source plus build.
Click Configure and select Visual Studio 15 2017 Win64, then press Generate.
At the end copy the bin folder from MOVcreator\bin to your build folder 
```

### Build solution
```
Inside the build folder, open the MOVCreator.sln and build the solution
After that, inside the bin folder (MOVCreator\build\bin) you may find the MOVCreatorDemo.exe to test the library.
```

## Demo Usage
After building the MovCreator solution, inside the bin folder you should find the MOVCreatorDemo.exe.

MOVCreatorDemo usage
```
Use program as:

MOVCreatorDemo.exe <output_dir> <output_filename> <input_raw_video> <input_wave_file> <video_type> <edit_rate>

  video_type [ prores422 | prores422lt | prores422proxy]
  edit_rate [23.98 | 24 | 25 | 29.97 | 30 | 50 | 59.94 | 60 ]
```

Open a command line inside movCreator\bin and run the following line

```
MOVCreatorDemo.exe ..\..\data fromDemo ..\..\data\sample_video.prores ..\..\data\sample_audio.wav prores422 30
```

This example will use a sample file (prores essence and audio pcm) and output to the same data folder an output file called fromdemo.

```
Starting demo.
--> API log: Creating file handle to: ..\..\data\sample_video.prores
--> API log: Creating file handle to: ..\..\data\sample_audio.wav
--> API log: Creating file handle to: ..\..\data/fromDemo.mov
--> API log: Updating movie storage
--> API log: Setting state to finished
Success while generating quicktime file
--> API log: Returned frames is SampleProviderAudioResampler: 0
Processed ProRes samples: 361
Leaving demo.
```

Open the fromdemo.mov using the quicktime player

![Image of Quicktime player](/images/QT_file.png)

### API Usage
For a simple example, check main.cpp

```cpp
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
```

## License
MOV Creator Demo is distributed under the terms of the Apache License Version 2. See the accompanying license file for details. The license enables usage of MOV Creator Demo in both commercial and non-commercial applications, without restrictions on the licensing applied to the combined work.

## Third Party
Third-party included:
* boost

## Contacts
Looking for a different solutions or help in your development, get in contact with us - info@mediagaps.com. 

We would love to hear your story and know if we can help you fill the gap.

