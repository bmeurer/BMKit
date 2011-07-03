/*-
 * Copyright (c) 2011, Benedikt Meurer <benedikt.meurer@googlemail.com>
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


/** BMKit related additions to the NSObject class. */
@interface NSObject (BMKitAdditions)

///------------------------------
/// @name Associative References
///------------------------------

typedef enum _BMAssociationPolicy {
    BMAssociationAssignPolicy,
    BMAssociationAtomicRetainPolicy,
    BMAssociationAtomicCopyPolicy,
    BMAssociationNonatomicRetainPolicy,
    BMAssociationNonatomicCopyPolicy
} BMAssociationPolicy;

typedef const void *BMAssociationKey;

/** Returns the object associated with a specified key.
 
 @param aKey An association key, an arbitrary pointer.
 @return The object associated with `aKey`, or `nil` if no object is associated with the key.
 @see setAssociatedObject:forKey:
 @see setAssociatedObject:forKey:policy:
 @see removeAssociatedObjects
 */
- (id)associatedObjectForKey:(BMAssociationKey)aKey;

/** Associates an object with a specified key on the target.
 
 The effect of sending this message is similar to sending the setAssociatedObject:forKey:policy: message, passing `BMAssociationNonatomicRetainPolicy` as `policy`.
 
 @param anObject The object to associate with the key. Pass `nil` to clear the association for `aKey`.
 @param aKey An association key, an arbitrary pointer.
 @see associatedObjectForKey:
 @see setAssociatedObject:forKey:policy:
 @see removeAssociatedObjects
 */
- (void)setAssociatedObject:(id)anObject forKey:(BMAssociationKey)aKey;

/** Associates an object with a specified key on the target using a specified association policy.

 Right now the following association policies are supported:
 - `BMAssociationAssignPolicy` Specifies a weak reference to the associated object.
 - `BMAssociationAtomicRetainPolicy` Specifies a strong reference to the associated object, and that the association is made atomically.
 - `BMAssociationAtomicCopyPolicy` Specifies that the associated object is copied, and that the association is made atomically.
 - `BMAssociationNonatomicRetainPolicy` Specifies a strong reference to the associated object, and that the association is not made atomically.
 - `BMAssociationNonatomicCopyPolicy` Specifies that the associated object is copied, and that the association is not made atomically.
 
 @param anObject The object to associate with the key. Pass `nil` to clear the association for `aKey`.
 @param aKey An association key, an arbitrary pointer.
 @param policy The policy for associating `anObject` with `aKey` on the target.
 @see associatedObjectForKey:
 @see setAssociatedObject:forKey:
 @see removeAssociatedObjects
 */
- (void)setAssociatedObject:(id)anObject forKey:(BMAssociationKey)aKey policy:(BMAssociationPolicy)policy;

/** Removes all associated objects of the target.
 
 @see associatedObjectForKey:
 @see setAssociatedObject:forKey:
 @see setAssociatedObject:forKey:policy:
 */
- (void)removeAssociatedObjects;

///-------------------------
/// @name Performing Blocks
///-------------------------

/** Invokes a block on the current thread using the default mode after a delay.
 
 This method takes a copy of `aBlock` and schedules it for invocation after `delay` seconds using the `performSelector:withObject:afterDelay:` instance method of the NSObject class.
 
 This method retains the receiver and keeps a heap-allocated copy of the `aBlock` parameter until after the block is performed.
 
 @param aBlock The block to invoke.
 @param delay The minimum time before which the message is sent. Specifying a delay of 0 does not necessarily cause the block to be performed immediately. The block is still queued on the thread's run loop and performed as soon as possible.
 @return A copy of the `aBlock`, which can be used to cancel the perform request using the cancelPreviousPerformRequestsWithTarget:block: class method.
 @see performBlock:afterDelay:inModes:
 @see cancelPreviousPerformRequestsWithTarget:block:
 */
- (BMTargetBlock)performBlock:(BMTargetBlock)aBlock afterDelay:(NSTimeInterval)delay;

/** Invokes a block on the current thread using the specified modes after a delay.
 
 This method takes a copy of `aBlock` and schedules it for invocation in the specified `modes` after `delay` seconds using the `performSelector:withObject:afterDelay:inModes:` instance method of the NSObject class.
 
 This method retains the receiver and keeps a heap-allocated copy of the `aBlock` parameter until after the block is performed.
 
 @param aBlock The block to invoke.
 @param delay The minimum time before which the message is sent. Specifying a delay of 0 does not necessarily cause the block to be performed immediately. The block is still queued on the thread's run loop and performed as soon as possible.
 @param modes An array of strings that identify the modes to associate with the timer that performs the selector. This array must contain at least one string. If you specify `nil` or an empty array for this parameter, this method returns without performing the specified block.
 @return A copy of the `aBlock`, which can be used to cancel the perform request using the cancelPreviousPerformRequestsWithTarget:block: class method.
 @see performBlock:afterDelay:
 @see cancelPreviousPerformRequestsWithTarget:block:
 */
- (BMTargetBlock)performBlock:(BMTargetBlock)aBlock afterDelay:(NSTimeInterval)delay inModes:(NSArray *)modes;

/** Invokes a block on the receiver on the main thread using the default mode.
 
 This method schedules a block for invocation on the main thread in the default mode using the `performSelectorOnMainThread:withObject:waitUntilDone:` instance method of the NSObject class.
 
 This method retains the receiver and the `aBlock` parameter (or a heap-allocated of it) until after the block is performed.
 
 @param aBlock The block to invoke.
 @param wait A Boolean that specifies whether the current thread blocks until after the specified block is performed on the receiver on the main thread. Specify `YES` to block this thread; otherwise, specify `NO` to have this method return immediately. If the current thread is also the main thread, and you specify `YES` for this parameter, the block is performed immediately.
 @see performBlockOnMainThread:waitUntilDone:modes:
 @see performBlock:onThread:waitUntilDone:
 */
- (void)performBlockOnMainThread:(BMTargetBlock)aBlock waitUntilDone:(BOOL)wait;

/** Invokes a block on the receiver on the main thread using the specified modes.
 
 This method schedules a block for invocation on the main thread in the specified `modes` using the `performSelectorOnMainThread:withObject:waitUntilDone:modes:` instance method of the NSObject class.
 
 This method retains the receiver and the `aBlock` parameter (or a heap-allocated of it) until after the block is performed.
 
 @param aBlock The block to invoke.
 @param wait A Boolean that specifies whether the current thread blocks until after the specified block is performed on the receiver on the main thread. Specify `YES` to block this thread; otherwise, specify `NO` to have this method return immediately. If the current thread is also the main thread, and you specify `YES` for this parameter, the block is performed immediately.
 @param modes An array of strings that identify the modes to associate with the timer that performs the selector. This array must contain at least one string. If you specify `nil` or an empty array for this parameter, this method returns without performing the specified block.
 @see performBlockOnMainThread:waitUntilDone:
 @see performBlock:onThread:waitUntilDone:
 */
- (void)performBlockOnMainThread:(BMTargetBlock)aBlock waitUntilDone:(BOOL)wait modes:(NSArray *)modes;

/** Invokes a block on the receiver on the specified thread using the default mode.
 
 This method schedules a block for invocation on `aThread` in the default mode using the `performSelector:onThread:withObject:waitUntilDone:` instance method of the NSObject class.
 
 This method retains the receiver and the `aBlock` parameter (or a heap-allocated of it) until after the block is performed.
 
 @param aBlock The block to invoke.
 @param aThread The thread on which to execute `aBlock`.
 @param wait A Boolean that specifies whether the current thread blocks until after the specified block is performed on the receiver on the specified thread. Specify `YES` to block this thread; otherwise, specify `NO` to have this method return immediately. If the current thread and target thread are the same, and you specify `YES` for this parameter, the block is performed immediately on the current thread. If you specify `NO`, this method queues the message on the thread's run loop and returns, just like it does for other threads. The current thread must then dequeue and process the message when it has an opportunity to do so.
 @see performBlock:onThread:waitUntilDone:modes:
 @see performBlockOnMainThread:waitUntilDone:
 */
- (void)performBlock:(BMTargetBlock)aBlock onThread:(NSThread *)aThread waitUntilDone:(BOOL)wait;

/** Invokes a block on the receiver on the specified thread using the specified modes.
 
 This method schedules a block for invocation on `aThread` in the specified `modes` using the `performSelector:onThread:withObject:waitUntilDone:modes:` instance method of the NSObject class.
 
 This method retains the receiver and the `aBlock` parameter (or a heap-allocated of it) until after the block is performed.
 
 @param aBlock The block to invoke.
 @param aThread The thread on which to execute `aBlock`.
 @param wait A Boolean that specifies whether the current thread blocks until after the specified block is performed on the receiver on the specified thread. Specify `YES` to block this thread; otherwise, specify `NO` to have this method return immediately. If the current thread and target thread are the same, and you specify `YES` for this parameter, the block is performed immediately on the current thread. If you specify `NO`, this method queues the message on the thread's run loop and returns, just like it does for other threads. The current thread must then dequeue and process the message when it has an opportunity to do so.
 @param modes An array of strings that identify the modes to associate with the timer that performs the selector. This array must contain at least one string. If you specify `nil` or an empty array for this parameter, this method returns without performing the specified block.
 @see performBlock:onThread:waitUntilDone:
 @see performBlockOnMainThread:waitUntilDone:modes:
 */
- (void)performBlock:(BMTargetBlock)aBlock onThread:(NSThread *)aThread waitUntilDone:(BOOL)wait modes:(NSArray *)modes;

/** Invokes a block on the receiver on a new background thread.
 
 This method takes a copy of `aBlock` and schedules it for background execution in a new background using the `performSelectorInBackground:withObject:` instance method of the NSObject class. This method also sets up an autorelease pool for the new background, so in contrast to the `performSelectorInBackground:withObject:` instance method, you do not need to set up an autorelease pool yourself.
 
 This method retains the receiver and keeps a heap-allocated copy of the `aBlock` parameter until after the block is performed.
 
 @param aBlock The block to invoke.
 @see performBlock:onThread:waitUntilDone:
 */
- (void)performBlockInBackground:(BMTargetBlock)aBlock;

/** Cancels perform requests previously registered with performBlock:afterDelay:.
 
 All perform requests are canceled that have the same target as `aTarget` and block as `aBlock`.
 
 @warning *Important:* `aBlock` must refer to the heap-allocated copy of the block, which was passed to performBlock:afterDelay:. If unsure, just pass the value returned from performBlock:afterDelay: as `aBlock`.
 
 @param aTarget The target for requests previously registered with the performBlock:afterDelay: instance method.
 @param aBlock The heap-allocated copy of the block for requests previously registered with the performBlock:afterDelay: instance method.
 @see performBlock:afterDelay:
 @see performBlock:afterDelay:inModes:
 */
+ (void)cancelPreviousPerformRequestsWithTarget:(id)aTarget block:(BMTargetBlock)aBlock;

@end
