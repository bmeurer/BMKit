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

#include <SystemConfiguration/SystemConfiguration.h>

#import <Foundation/Foundation.h>


@protocol BMNetworkReachabilityControllerDelegate;


/** You use a network reachability controller to easily manage a bunch of `SCNetworkReachabilityRef` references.
 
 @see BMNetworkReachabilityControllerDelegate
 */
@interface BMNetworkReachabilityController : NSObject {
@private
    id<BMNetworkReachabilityControllerDelegate> _delegate;
    struct BMNetworkReachabilityEntry          *_entries;
    CFRunLoopRef                                _runLoop;
}

/** The object that is notified whenever a new reachability reference is added to the receiver, an existing reachability reference is removed or the flags of a reachability reference managed by the receiver changed. */
@property (nonatomic, assign) id<BMNetworkReachabilityControllerDelegate> delegate;

/** The OR'ed flags of all reachability references managed by the receiver. This property is KVO-compliant. */
@property (nonatomic, assign, readonly) SCNetworkReachabilityFlags flags;

///-------------------------------
/// @name Managing Reachabilities
///-------------------------------

/** Adds a new reachability reference to the receiver.
 
 @param reachabilty The new reachability reference.
 @return `YES` if the *reachability* was added successfully, `NO` otherwise.
 @see addReachabilityWithAddress:
 @see addReachabilityWithLocalAddress:remoteAddress:
 @see addReachabilityWithName:
 @see removeReachability:
 @see reachabilities
 */
- (BOOL)addReachability:(SCNetworkReachabilityRef)reachability;

/** Creates a new reachability reference to the specified network address and adds it to the receiver.
 
 @param address The address of the desired host.
 @return `YES` if the new reachability was added successfully, `NO` otherwise.
 @see addReachability:
 @see addReachabilityWithLocalAddress:remoteAddress:
 @see addReachabilityWithName:
 @see removeReachability:
 @see reachabilities
 */
- (BOOL)addReachabilityWithAddress:(const struct sockaddr *)address;

/** Creates a new reachability reference to the specified network address pair and adds it to the receiver.
 
 @param localAddress The local address associated with a network connection. If `NULL`, only the *remoteAddress* is of interest.
 @param remoteAddress The remote address associated with a network connection. If `NULL`, only the *localAddress* is of interest.
 @return `YES` if the new reachability was added successfully, `NO` otherwise.
 @see addReachability:
 @see addReachabilityWithAddress:
 @see addReachabilityWithName:
 @see removeReachability:
 @see reachabilities
 */
- (BOOL)addReachabilityWithLocalAddress:(const struct sockaddr *)localAddress
                          remoteAddress:(const struct sockaddr *)remoteAddress;

/** Creates a new reachability reference to the specified network host or node name and adds it to the receiver.

 @param name The node name of the desired host. This *name* is the same as that passed to the `gethostbyname` or `getaddrinfo` functions.
 @return `YES` if the new reachability was added successfully, `NO` otherwise.
 @see addReachability:
 @see addReachabilityWithAddress:
 @see addReachabilityWithLocalAddress:remoteAddress:
 @see removeReachability:
 @see reachabilities
 */
- (BOOL)addReachabilityWithName:(NSString *)name;

/** Removes a reachability reference from the receiver.
 
 @param reachability The reachability reference to remove.
 @return `YES` if *reachability* was removed successfully, `NO` otherwise.
 @see addReachability:
 @see addReachabilityWithAddress:
 @see addReachabilityWithLocalAddress:remoteAddress:
 @see addReachabilityWithName:
 @see reachabilities
 */
- (BOOL)removeReachability:(SCNetworkReachabilityRef)reachability;

/** Returns an array with all reachability references managed by the receiver.
 
 @return An array with all reachability references managed by the receiver.
 @see addReachability:
 @see addReachabilityWithAddress:
 @see addReachabilityWithLocalAddress:remoteAddress:
 @see addReachabilityWithName:
 @see removeReachability:
 */
- (NSArray *)reachabilities;

@end


/** An instance of the BMNetworkReachabilityController class uses methods in this protocol to notify it's delegate that the managed reachability references changed in some way.
 
 @see BMNetworkReachabilityController
 */
@protocol BMNetworkReachabilityControllerDelegate <NSObject>

@optional

/** Notifies the receiver that a new reachability reference was added to the sender.
 
 @param networkReachabilityController The network reachability controller.
 @param reachability The added reachability reference.
 */
- (void)networkReachabilityController:(BMNetworkReachabilityController *)networkReachabilityController
                   didAddReachability:(SCNetworkReachabilityRef)reachability;

/** Notifies the receiver that an existing reachability reference was removed from the sender.
 
 @param networkReachabilityController The network reachability controller.
 @param reachability The removed reachability reference.
 */
- (void)networkReachabilityController:(BMNetworkReachabilityController *)networkReachabilityController
                didRemoveReachability:(SCNetworkReachabilityRef)reachability;

/** Notifies the receiver that the flags of a reachability reference managed by the sender did change.
 
 @param networkReachabilityController The network reachability controller.
 @param reachability The reachability reference whos flags did change.
 @param flags The new flags of *reachability*.
 */
- (void)networkReachabilityController:(BMNetworkReachabilityController *)networkReachabilityController
                didChangeReachability:(SCNetworkReachabilityRef)reachability
                                flags:(SCNetworkReachabilityFlags)flags;

@end

