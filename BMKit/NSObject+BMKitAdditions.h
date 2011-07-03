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

#import <Foundation/Foundation.h>


typedef void (^BMTargetBlock)(id aTarget);

/** Additions to the NSObject class. */
@interface NSObject (BMKitAdditions)

///-----------------------------
/// @name Associative References
///-----------------------------

typedef enum _BMAssociationPolicy {
    BMAssociationAssignPolicy,
    BMAssociationAtomicRetainPolicy,
    BMAssociationAtomicCopyPolicy,
    BMAssociationNonatomicRetainPolicy,
    BMAssociationNonatomicCopyPolicy
} BMAssociationPolicy;

typedef const void *BMAssociationKey;

- (id)associatedObjectForKey:(BMAssociationKey)aKey;
- (void)setAssociatedObject:(id)anObject forKey:(BMAssociationKey)aKey;
- (void)setAssociatedObject:(id)anObject forKey:(BMAssociationKey)aKey policy:(BMAssociationPolicy)policy;
- (void)removeAssociatedObjects;

///------------------------
/// @name Performing Blocks
///------------------------

- (BMTargetBlock)performBlock:(BMTargetBlock)aBlock afterDelay:(NSTimeInterval)delay;
- (BMTargetBlock)performBlock:(BMTargetBlock)aBlock afterDelay:(NSTimeInterval)delay inModes:(NSArray *)modes;
- (void)performBlockOnMainThread:(BMTargetBlock)aBlock waitUntilDone:(BOOL)wait;
- (void)performBlockOnMainThread:(BMTargetBlock)aBlock waitUntilDone:(BOOL)wait modes:(NSArray *)modes;
- (void)performBlock:(BMTargetBlock)aBlock onThread:(NSThread *)aThread waitUntilDone:(BOOL)wait;
- (void)performBlock:(BMTargetBlock)aBlock onThread:(NSThread *)aThread waitUntilDone:(BOOL)wait modes:(NSArray *)modes;
- (void)performBlockInBackground:(BMTargetBlock)aBlock;
+ (void)cancelPreviousPerformRequestsWithTarget:(id)aTarget block:(BMTargetBlock)aBlock;

@end
