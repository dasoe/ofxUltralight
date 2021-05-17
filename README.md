
[openframeworks](https://openframeworks.cc/) 0.11.0 addon for ultralight-sdk-1.2-win https://ultralig.ht/
Ultralight (c) 2018 Ultralight, Inc. Ultralight is a trademark of Ultralight, Inc.

### About

With the release of the most inspiring [Ultralight 1.2](https://ultralig.ht/), implementation of Ultralight in other Apps has become *much* easier. So I wrote this addon from scratch. This is only a beginning and at the moment mostly for my usecase:
* Heavily layouted One-Page content, that can be changed easily without having to adjust my OF App
* Simple GUI functionality (communication with the OF App, which has the power to do the advanced stuff)

My focus is not on perfect code but on ease of use and integration. Of course the addon can be adjusted and developed. I am happy if it will be an all-purpose Browser addon one day. Join in, everybody. 
Also: I am not a C++ developer. I am more than happy to learn and improve the code!

### Status

* For now only CPU rendering, GPU ist yet to come. (Please check out [IanByuns work](https://github.com/IanByun/ofxUltralight-byun) for a working GPU solution. Or switch to gpu branch here and help me getting it to work...)
* Basic loading of URLs, Files and raw HTML.
* Mouse klick and scroll wheel implementation.
* Basic keyboard implementation.
* High-level (easy to understand and use) code in your App.
* Communication OF->Ultralight and Ultralight->OF.
* UL log goes to Terminal via OF Logging, same for Console Messages.
* Only windows for now. I will definetely do a Linux Version, though.
* *example-basic-CPU* shows how to use in general.
* *example-communication-GUI* shows how to do communication between Broser (JS) and OF, both directions.


## Installation

* clone this repository to addons folder (or download, unzip, copy and remover the "-master" part in folder name)
* Use project generator, include addon ofxUltralight, create the project. 
* Open it in Visual Studio 2019 (we need Platform Toolset v42 for Ultralight, so no VS 2017, sorry. But you can install them side by side.)
* in case it is not done automatically on opening: retarget solution (right Mouseclick on your solution in solution explorer)
* Set it to x64 (Win32 will fail) and build


I am very happy! It works. Don't expect too much already, but come back from time to time, as it will grow.

oe
