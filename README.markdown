## About

[BMKit](http://benediktmeurer.de/BMKit) is a collection of [well documented](http://bmeurer.github.com/BMKit) Objective-C classes and categories for iOS (and Mac) development that make life easier by solving common problems in iOS (and Mac) development. Right now BMKit is a static library for iOS 4.0 and beyond.


## Documentation

Documentation is done using [AppleDoc](http://github.com/tomaz/appledoc). The generated API documentation is available [online](http://bmeurer.github.com/BMKit/).


## License

BMKit is licensed under the [Simplified BSD License](http://en.wikipedia.org/wiki/BSD_license).
See the [LICENSE](http://github.com/bmeurer/BMKit/raw/master/LICENSE) file for details.


## Installation

Run the following command to add BMKit as a [Git](http://git-scm.org) submodule to your project. Be sure you have are you in the root folder of your project.

    $ git submodule add git://github.com/bmeurer/BMKit.git Vendor/BMKit

If you don't want to add BMKit as a submodule to your project or if your project is not managed using [Git](http://git-scm.org), then you can run the following commands to download the source code of the most recent revision.

    $ mkdir -p Vendor/BMKit
    $ curl -L http://github.com/bmeurer/BMKit/tarball/master | tar xz --strip 1 -C Vendor/BMKit


### Adding to your Project

* In Xcode, add the `BMKit.xcodeproj` to your project.
* In the _Build Phases_ of a target, add `libBMKit.a` to the _Target Dependencies_ and _Link Binary with Libraries_.
* In the build phases of a target, add the `ImageIO.framework`, `MobileCoreServices.framework` and `SystemConfiguration.framework` frameworks. to the _Link Binary with Libraries_.
* Choose the _Build Settings_ tab. Make sure _All_ is selected in the top left of the bar under the tabs.
* Add `Vendor/BMKit` to the _Header Search Path_ (do not click the _Recursive_ checkbox).
* Add `-all_load -ObjC` to _Other Linker Flags_.


### Usage

To use BMKit, simply add the following line to your source file. I recommend adding this to your prefix to make things easy.

    #import <BMKit/BMKit.h>

You can also import individual files instead of the whole framework by doing something like:

    #import <BMKit/BMNetworkReachabilityController.h>


## Bug Reports

If you come across any problems, please [create a ticket](http://github.com/bmeurer/BMKit/issues) and we will try to get it fixed as soon as possible.


## Contributing

Once you've made your commits:

1. [Fork](http://help.github.com/fork-a-repo/ "Fork a repo") BMKit.
2. Create a topic branch - `git checkout -b my_branch`.
3. Push to your topic branch - `git push origin my_branch`.
4. Create a [Pull Request](http://help.github.com/pull-requests/ "Send pull requests") from your topic branch.
5. That's it!


## Authors

Benedikt Meurer :: benedikt.meurer@googlemail.com :: [@bmeurer](http://twitter.com/bmeurer)


## Copyright

Copyright (c) 2004-2011 Benedikt Meurer. See the [License](http://github.com/bmeurer/BMKit/raw/master/LICENSE) file for details.

