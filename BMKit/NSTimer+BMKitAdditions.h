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


/** BMKit related additions to the NSTimer class. */
@interface NSTimer (BMKitAdditions)

///------------------------
/// @name Creating a Timer
///------------------------

/** Creates and returns a new NSTimer object and schedules it on the current run loop in the default mode.
 
 After _seconds_ have elapsed, the timer fires, invoking the block _aBlock_ passing the timer as parameter.
 
 @param seconds The number of seconds between firings of the timer. If seconds is less than or equal to `0.0`, this method chooses the non-negative value of `0.1` milliseconds instead.
 @param aBlock The block to invoke when the timer fires. The block is copied by the timer and released when the timer is invalidated.
 @param userInfo The user info for the timer. The object you specify is retained by the timer and released when the timer is invalidated. This parameter may be `nil`.
 @param repeats If `YES`, the timer will repeatedly reschedule itself until invalidated. If `NO`, the timer will be invalidated after it fires.
 @return A new NSTimer object, configured according to the specified parameters.
 @see timerWithTimeInterval:block:userInfo:repeats:
 @see initWithFireDate:interval:block:userInfo:repeats:
 */
+ (NSTimer *)scheduledTimerWithTimeInterval:(NSTimeInterval)seconds block:(BMTimerBlock)aBlock userInfo:(id)userInfo repeats:(BOOL)repeats;

/** Creates and returns a new NSTimer object initialized with the specified block.
 
 You must add the new timer to a run loop, using `addTimer:forMode:`. Then, after seconds seconds have elapsed, the timer fires, invoking the block _aBlock_ passing the timer as parameter. If the timer is configured to repeat, there is no need to subsequently re-add the timer to the run loop.
 
 @param seconds The number of seconds between firings of the timer. If seconds is less than or equal to `0.0`, this method chooses the non-negative value of `0.1` milliseconds instead.
 @param aBlock The block to invoke when the timer fires. The block is copied by the timer and released when the timer is invalidated.
 @param userInfo The user info for the timer. The object you specify is retained by the timer and released when the timer is invalidated. This parameter may be `nil`.
 @param repeats If `YES`, the timer will repeatedly reschedule itself until invalidated. If `NO`, the timer will be invalidated after it fires.
 @return A new NSTimer object, configured according to the specified parameters.
 @see scheduledTimerWithTimeInterval:block:userInfo:repeats:
 @see initWithFireDate:interval:block:userInfo:repeats:
 */
+ (NSTimer *)timerWithTimeInterval:(NSTimeInterval)seconds block:(BMTimerBlock)aBlock userInfo:(id)userInfo repeats:(BOOL)repeats;

/** Initializes a new NSTimer object using the specified block.
 
 You must add the new timer to a run loop, using `addTimer:forMode:`. Upon firing, the timer invokes the block _aBlock_ passing the timer as parameter. If the timer is configured to repeat, there is no need to subsequently re-add the timer to the run loop.
 
 @param date The time at which the timer should first fire.
 @param seconds For a repeating timer, this parameter contains the number of seconds between firings of the timer. If seconds is less than or equal to `0.0`, this method chooses the nonnegative value of `0.1` milliseconds instead.
 @param aBlock The block to invoke when the timer fires. The block is copied by the timer and released when the timer is invalidated.
 @param userInfo Custom user info for the timer. The object you specify is retained by the timer and released when the timer is invalidated. This parameter may be `nil`.
 @param repeats If `YES`, the timer will repeatedly reschedule itself until invalidated. If `NO`, the timer will be invalidated after it fires.
 @return The receiver, initialized such that, when added to a run loop, it will fire at date and then, if _repeats_ is `YES`, every seconds after that.
 @see timerWithTimeInterval:block:userInfo:repeats:
 @see scheduledTimerWithTimeInterval:block:userInfo:repeats:
 */
- (id)initWithFireDate:(NSDate *)date interval:(NSTimeInterval)seconds block:(BMTimerBlock)aBlock userInfo:(id)userInfo repeats:(BOOL)repeats;

@end
