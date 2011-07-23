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

#include "BMObjectUtilities.h"

#import "BMNetworkReachabilityController.h"


struct BMNetworkReachabilityEntry
{
    struct BMNetworkReachabilityEntry *next;
    SCNetworkReachabilityRef           reachability;
    SCNetworkReachabilityFlags         flags;
};


@interface BMNetworkReachabilityController (BMKitInternals)

- (void)BM_didChangeReachability:(SCNetworkReachabilityRef)reachability
                           flags:(SCNetworkReachabilityFlags)flags;

@end


@implementation BMNetworkReachabilityController

@synthesize delegate = _delegate;


- (id)init
{
    self = [super init];
    if (self) {
        _runLoop = CFRunLoopGetCurrent();
        if (!_runLoop) {
            [self release];
            return nil;
        }
        CFRetain(_runLoop);
    }
    return self;
}


- (void)dealloc
{
    for (struct BMNetworkReachabilityEntry *entry, **entryp = &_entries; (entry = *entryp); ) {
        SCNetworkReachabilityRef reachability = entry->reachability;
        SCNetworkReachabilityUnscheduleFromRunLoop(reachability, _runLoop, kCFRunLoopCommonModes);
        SCNetworkReachabilitySetCallback(reachability, NULL, NULL);
        CFRelease(reachability);
        *entryp = entry->next;
        free(entry);
    }
    if (_runLoop) CFRelease(_runLoop), _runLoop = NULL;
    [super dealloc];
}


#pragma mark -
#pragma mark Properties


+ (BOOL)automaticallyNotifiesObserversForKey:(NSString *)key
{
    BOOL automaticallyNotifiesObservers;
    if ([key isEqualToString:@"flags"]) {
        automaticallyNotifiesObservers = NO;
    }
    else {
        automaticallyNotifiesObservers = [super automaticallyNotifiesObserversForKey:key];
    }
    return automaticallyNotifiesObservers;
}


- (SCNetworkReachabilityFlags)flags
{
    SCNetworkReachabilityFlags flags = 0;
    for (struct BMNetworkReachabilityEntry *entry = _entries; entry; entry = entry->next) {
        flags |= entry->flags;
    }
    return flags;
}


#pragma mark -
#pragma mark Managing Reachabilities


- (void)BM_didChangeReachability:(SCNetworkReachabilityRef)reachability
                           flags:(SCNetworkReachabilityFlags)flags
{
    // Maintain an extra reference on self
    [self retain];
    
    // Check if we actually have an entry for the reachability
    struct BMNetworkReachabilityEntry *entry;
    for (entry = _entries; entry && entry->reachability != reachability; entry = entry->next)
        ;
    if (entry) {
        // Maintain an extra reference on reachability
        CFRetain(reachability);
        
        // Update the entry flags (our flags property is derived from the entry's flags)
        [self willChangeValueForKey:@"flags"];
        entry->flags = flags;
        [self didChangeValueForKey:@"flags"];
        
        // The above calls back into user code, so check
        // if we still have an entry for the reachability
        for (entry = _entries; entry && entry->reachability != reachability; entry = entry->next)
            ;
        if (entry) {
            // Tell the delegate about the change
            if ([_delegate respondsToSelector:@selector(networkReachabilityController:didChangeReachability:flags:)]) {
                [_delegate networkReachabilityController:self didChangeReachability:reachability flags:flags];
            }
        }
        
        // Release extra reference on reachability
        CFRelease(reachability);
    }
    
    // Release extra reference on self
    [self release];
}


static const void *BMNetworkReachabilityContextRetainNoop(const void *context)
{
    return context;
}


static void BMNetworkReachabilityContextReleaseNoop(const void *context)
{
    (void)context;
}


static void BMNetworkReachabilityContextCallBack(SCNetworkReachabilityRef reachability, SCNetworkReachabilityFlags flags, void *context)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    [(BMNetworkReachabilityController *)context BM_didChangeReachability:reachability flags:flags];
    [pool drain];
}


- (BOOL)addReachability:(SCNetworkReachabilityRef)reachability
{
    if (!reachability) {
        return NO;
    }
    struct BMNetworkReachabilityEntry *entry;
    for (entry = _entries; entry; entry = entry->next) {
        if (entry->reachability == reachability) {
            return NO;
        }
    }
    entry = (struct BMNetworkReachabilityEntry *)malloc(sizeof(struct BMNetworkReachabilityEntry));
    if (!entry) {
        return NO;
    }
    if (!SCNetworkReachabilityScheduleWithRunLoop(reachability, _runLoop, kCFRunLoopCommonModes)) {
        free(entry);
        return NO;
    }
    SCNetworkReachabilityContext context = {
        .version = 0,
        .info = self,
        .retain = BMNetworkReachabilityContextRetainNoop,
        .release = BMNetworkReachabilityContextReleaseNoop,
        .copyDescription = BMObjectCopyDescription
    };
    if (!SCNetworkReachabilitySetCallback(reachability, BMNetworkReachabilityContextCallBack, &context)) {
        SCNetworkReachabilityUnscheduleFromRunLoop(reachability, _runLoop, kCFRunLoopCommonModes);
        free(entry);
        return NO;
    }
    
    // Hook up the entry
    entry->reachability = CFRetain(reachability);
    if (!SCNetworkReachabilityGetFlags(reachability, &entry->flags)) entry->flags = 0;
    entry->next = _entries;
    _entries = entry;
    
    // Maintain references...
    [self retain];
    CFRetain(reachability);

    // Tell the delegate about the new reachability
    if ([_delegate respondsToSelector:@selector(networkReachabilityController:didAddReachability:)]) {
        [_delegate networkReachabilityController:self didAddReachability:reachability];
    }
    
    // Check if we still have the given reachability and notify the delegate about the change
    for (entry = _entries; entry; entry = entry->next) {
        if (entry->reachability == reachability) {
            [self BM_didChangeReachability:reachability flags:entry->flags];
            break;
        }
    }
    
    // Release extra references
    CFRelease(reachability);
    [self release];
    
    return YES;
}


- (BOOL)addReachabilityWithAddress:(const struct sockaddr *)address
{
    BOOL succeed = NO;
    SCNetworkReachabilityRef reachability = SCNetworkReachabilityCreateWithAddress(CFGetAllocator(_runLoop), address);
    if (reachability) {
        succeed = [self addReachability:reachability];
        CFRelease(reachability);
    }
    return succeed;
}


- (BOOL)addReachabilityWithLocalAddress:(const struct sockaddr *)localAddress
                          remoteAddress:(const struct sockaddr *)remoteAddress
{
    BOOL succeed = NO;
    SCNetworkReachabilityRef reachability = SCNetworkReachabilityCreateWithAddressPair(CFGetAllocator(_runLoop), localAddress, remoteAddress);
    if (reachability) {
        succeed = [self addReachability:reachability];
        CFRelease(reachability);
    }
    return succeed;
}


- (BOOL)addReachabilityWithName:(NSString *)name
{
    BOOL succeed = NO;
    const char *UTF8Name = [name cStringUsingEncoding:NSUTF8StringEncoding];
    if (UTF8Name) {
        SCNetworkReachabilityRef reachability = SCNetworkReachabilityCreateWithName(CFGetAllocator(_runLoop), UTF8Name);
        if (reachability) {
            succeed = [self addReachability:reachability];
            CFRelease(reachability);
        }
    }
    return succeed;
}


- (BOOL)removeReachability:(SCNetworkReachabilityRef)reachability
{
    for (struct BMNetworkReachabilityEntry *entry, **entryp = &_entries; (entry = *entryp); entryp = &entry->next) {
        if (entry->reachability == reachability) {
            // Maintain extra references (we are calling back to user code)
            [self retain];
            CFRetain(reachability);

            // Prepare KVO notification on flags
            [self willChangeValueForKey:@"flags"];
            
            // Check if the entry for the reachability is
            // still around, and if so, remove it...
            for (entryp = &_entries; (entry = *entryp); entryp = &entry->next) {
                if (entry->reachability == reachability) {
                    // Remove the entry from the list
                    *entryp = entry->next;
                    free(entry);
                    
                    // Unschedule and release the reachability the reachability
                    SCNetworkReachabilityUnscheduleFromRunLoop(reachability, _runLoop, kCFRunLoopCommonModes);
                    SCNetworkReachabilitySetCallback(reachability, NULL, NULL);
                    CFRelease(reachability);
                    
                    // Notify the delegate
                    if ([_delegate respondsToSelector:@selector(networkReachabilityController:didRemoveReachability:)]) {
                        [_delegate networkReachabilityController:self didRemoveReachability:reachability];
                    }
                    break;
                }
            }
            
            // Send KVO notification on flags
            [self didChangeValueForKey:@"flags"];
            
            // Cleanup extra references
            CFRelease(reachability);
            [self release];
            return YES;
        }
    }
    return NO;
}


- (NSArray *)reachabilities
{
    unsigned numberOfReachabilities = 0;
    struct BMNetworkReachabilityEntry *entry;
    for (entry = _entries; entry; entry = entry->next) {
        ++numberOfReachabilities;
    }
    CFArrayRef arrayOfReachabilities = NULL;
    SCNetworkReachabilityRef *reachabilities = calloc(numberOfReachabilities, sizeof(SCNetworkReachabilityRef)), *rp;
    if (reachabilities) {
        for (entry = _entries, rp = reachabilities; entry; entry = entry->next, ++rp) {
            assert(rp >= reachabilities);
            assert(rp < reachabilities + numberOfReachabilities);
            *rp = entry->reachability;
        }
        arrayOfReachabilities = CFArrayCreate(kCFAllocatorDefault, (const void **)reachabilities, numberOfReachabilities, &kCFTypeArrayCallBacks);
        free(reachabilities);
    }
    return [(NSArray *)arrayOfReachabilities autorelease];
}


@end
