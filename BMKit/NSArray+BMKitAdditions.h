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


/** BMKit related additions to the `NSArray` class. */
@interface NSArray (BMKitAdditions)

///-------------------------------------
/// @name Performing Blocks on Elements
///-------------------------------------

/** Performs the block on each object in the array, starting with the first object and continuing through the array to the last object.
 
 This method raises `NSInvalidArgumentException` if *aBlock* is `nil`.
 
 @param aBlock A block to invoke with the objects in the array. The block must not have the side effect of modifying the receiving array.
 */
- (void)makeObjectsPerformBlock:(BMTargetBlock)aBlock;

///---------------------------
/// @name Deriving New Arrays
///---------------------------

/** Evaluates a given predicate block against each object in the receiving array and returns a new array containing the objects for which the predicate block returns true.
 
 @param aBlock The predicate block against which to evaluate the receiving array’s elements.
 @return A new array containing the objects in the receiving array for which *aBlock* returns true.
 */
- (NSArray *)filteredArrayUsingPredicateBlock:(BMPredicateBlock)aBlock;

/** Invokes the transformator on each object in the receiving array and returns a new array containing the transformed objects.
 
 @param aTransformator The transformator to invoke on the receiving array's elements.
 @return A new array containing the transformed objects in the receiving array.
 */
- (NSArray *)transformedArrayUsingTransformator:(BMTransformator)aTransformator;

@end
