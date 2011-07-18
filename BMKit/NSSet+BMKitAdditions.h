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


/** BMKit related additions to the `NSSet` class. */
@interface NSSet (BMKitAdditions)

///-----------------------------
/// @name Accessing Set Members
///-----------------------------

/** Evaluates a given predicate block against each object in the receiving set and returns a new set containing the objects for which the predicate block returns true.
 
 @param predicateBlock A predicate block.
 @return A new set containing the objects in the receiving set for which *predicateBlock* returns true.
 */
- (NSSet *)filteredSetUsingPredicateBlock:(BMPredicateBlock)predicateBlock;

/** Performs a block on each object in the set.
 
 This method raises `NSInvalidArgumentException` if *aBlock* is `nil`.
 
 @param aBlock A block to invoke with the objects in the array. The block must not have the side effect of modifying the receiving array.
 */
- (void)makeObjectsPerformBlock:(BMTargetBlock)aBlock;

@end
