About
-----

BMKit is a collection of useful Objective-C classes and categories for iOS (and Mac) development, that I wrote in the past. I'm currently collecting and cleaning up the various bits and pieces, in order to stuff them into BMKit.

Right now, BMKit is a static library for iOS 4.0 and beyond.


Documentation
-------------

Documentation is done using [AppleDoc](http://github.com/tomaz/appledoc).


License
-------

BMKit is licensed under the [Simplified BSD License](http://en.wikipedia.org/wiki/BSD_license).
See the [License](http://github.com/bmeurer/BMKit/blob/master/LICENSE) file for details.


Installation
------------

* Clone the repository via `git clone git://github.com/bmeurer/BMKit.git`
* In Xcode, add the `BMKit.xcodeproj` to your project or workspace.
* In the build phases of a target, add `libBMKit.a` to the _Target Dependencies_ and _Link Binary with Libraries_.
* In the build settings, change _Other Linker Flags_ to `-ObjC -all_load` and _Header Search Paths_ to `$(BUILT_PRODUCTS_DIR)/../BMKit/**`.
* Include the header file using `#import <BMKit/BMKit.h>`.


Bug Reports
-----------

If you come across any problems, please [create a ticket](http://github.com/bmeurer/BMKit/issues) and we will try to get it fixed as soon as possible.


Contributing
------------

Once you've made your commits:

1. [Fork](http://help.github.com/fork-a-repo/ "Fork a repo") BMKit.
2. Create a topic branch - `git checkout -b my_branch`.
3. Push to your topic branch - `git push origin my_branch`.
4. Create a [Pull Request](http://help.github.com/pull-requests/ "Send pull requests") from your topic branch.
5. That's it!


Author
------

Benedikt Meurer :: benedikt.meurer@googlemail.com :: [@bmeurer](http://twitter.com/bmeurer)


Copyright
---------

Copyright (c) 2004-2011 Benedikt Meurer. See the [License](http://github.com/bmeurer/BMKit/blob/master/LICENSE) file for details.

