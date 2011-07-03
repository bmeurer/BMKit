/*-
 * Copyright (c) 2010-2011, Benedikt Meurer <benedikt.meurer@googlemail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#import "BMKitTypes.h"


@interface NSThread (BMKitAdditions)

///---------------------------------------
/// @name Initializing an NSThread Object
///---------------------------------------

/** Returns an `NSThread` object initialized with the given arguments.

 In contrast to the `initWithTarget:selector:object:` instance method of the `NSThread` class, you do not need to set up an autorelease pool for the block in a non garbage-collected environment. This is done automatically.
 
 The block _aBlock_ is copied, and released when the thread finally exits.
 
 @param aBlock The block to execute.
 @return An NSThread object initialized with the given arguments.
 @see detachNewThreadBlock:
 */
- (id)initWithBlock:(BMBlock)aBlock;

///-------------------------
/// @name Starting a Thread
///-------------------------

/** Detaches a new thread and uses the specified block as the thread entry point.
 
 In contrast to the `detachNewThreadSelector:toTarget:withObject:` class method of the `NSThread` class, you do not need to set up an autorelease pool for the block in a non garbage-collected environment. This is done automatically.
 
 The block _aBlock_ is copied, and released when the thread finally exits. The detached thread is exited (using the `exit` class method of the `NSThread` class) as soon as _aBlock_ has completed executing.
 
 If this thread is the first thread detached in the application, this method posts the `NSWillBecomeMultiThreadedNotification` with object `nil` to the default notification center.
 
 @param aBlock The block to execute.
 @see initWithBlock:
 */
+ (void)detachNewThreadBlock:(BMBlock)aBlock;

@end
