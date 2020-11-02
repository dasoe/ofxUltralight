# ofxUltralight

openframeworks 0.11.0 addon for ultralight-sdk-1.2-win https://ultralig.ht/ 
Ultralight (c) 2018 Ultralight, Inc. Ultralight is a trademark of Ultralight, Inc.

###About
With the release of Ultralight 1.2 a few days ago, implementation of Ultralight in other Apps has become *much* easier. So I wrote this addon from scratch. Hence it is a very beginning only.
* For now only CPU rendering, GPU ist yet to come.
* Mouse klick implementation. No key or scrollwheel at the moment.
* Only windows for now. I will definetely do a Linux Version, though.


##Installation

* Use project generator to build a project and include ofxUltralight
* Remove the *Additional include directories* (*C/C++* Section in Properties of your project) or you will have compiler errors. You will only need the following Ones:
	- ..\..\..\addons\ofxUltralight\libs
	- ..\..\..\addons\ofxUltralight\libs\include
	- ..\..\..\addons\ofxUltralight\src
	- (and of course %(AdditionalIncludeDirectories))
* Add the following to *Additional Library Directories* (*Linker > General* Section in Properties of your project):
	- ..\..\..\addons\ofxUltralight\libs\lib\
	- (and of course %(AdditionalLibraryDirectories))
* Add the following *Additional Dependencies* (*Linker > Input* Section in Properties of your project:
	- UltralightCore.lib
	- Ultralight.lib
	- WebCore.lib
	- AppCore.lib
	- (and of course %(AdditionalDependencies))
* Copy the contents of folder ofxUltralight\bin\ to the bin folder of your project

See the example for usage.