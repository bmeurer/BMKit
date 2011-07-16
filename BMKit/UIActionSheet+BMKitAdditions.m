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

#import "NSObject+BMKitAdditions.h"
#import "UIActionSheet+BMKitAdditions.h"


@implementation UIActionSheet (BMKitAdditions)

static const char *const BMActionSheetBlockDictionaryKey = "BMActionSheetBlockDictionaryKey";
static NSString *const BMActionSheetClickedButtonAtIndexKey = @"BMActionSheetClickedButtonAtIndex";
static NSString *const BMActionSheetWillPresentActionSheetKey = @"BMActionSheetWillPresentActionSheet";
static NSString *const BMActionSheetDidPresentActionSheetKey = @"BMActionSheetDidPresentActionSheet";
static NSString *const BMActionSheetWillDismissWithButtonIndexKey = @"BMActionSheetWillDismissWithButtonIndex";
static NSString *const BMActionSheetDidDismissWithButtonIndexKey = @"BMActionSheetDidDismissWithButtonIndex";


- (id)BM_blockForKey:(id)key
{
    return key ? [[[[self associatedObjectForKey:BMActionSheetBlockDictionaryKey] objectForKey:key] retain] autorelease] : nil;
}


- (void)BM_setBlock:(id)block
             forKey:(id)key
{
    if (key) {
        NSMutableDictionary *blockDictionary = [self associatedObjectForKey:BMActionSheetBlockDictionaryKey];
        if (!blockDictionary) {
            blockDictionary = [NSMutableDictionary dictionary];
            [self setAssociatedObject:blockDictionary forKey:BMActionSheetBlockDictionaryKey];
        }
        block = [block copy];
        if (block) {
            if (!self.delegate) {
                self.delegate = self;
            }
            NSAssert(self.delegate == self, @"Blocks cannot be used with UIActionSheet unless the delegate is self.");
            [blockDictionary setObject:block forKey:key];
        }
        else {
            [blockDictionary removeObjectForKey:key];
        }
        [block release];
    }
}


#pragma mark -
#pragma mark Creating Action Sheets


- (id)initWithTitle:(NSString *)title
{
    return [self initWithTitle:title
                      delegate:nil
             cancelButtonTitle:nil
        destructiveButtonTitle:nil
             otherButtonTitles:nil];
}


#pragma mark -
#pragma mark Configuring Buttons


- (NSInteger)addButtonWithTitle:(NSString *)title
                          block:(BMActionSheetWithButtonIndexBlock)block
{
    NSInteger buttonIndex = [self addButtonWithTitle:title];
    [self BM_setBlock:block forKey:[NSNumber numberWithInteger:buttonIndex]];
    return buttonIndex;
}


- (NSInteger)addCancelButtonWithTitle:(NSString *)title
{
    return [self addCancelButtonWithTitle:title block:nil];
}


- (NSInteger)addCancelButtonWithTitle:(NSString *)title
                                block:(BMActionSheetWithButtonIndexBlock)block
{
    NSInteger buttonIndex = [self addButtonWithTitle:title block:block];
    self.cancelButtonIndex = buttonIndex;
    return buttonIndex;
}


- (NSInteger)addDestructiveButtonWithTitle:(NSString *)title
{
    return [self addDestructiveButtonWithTitle:title block:nil];
}


- (NSInteger)addDestructiveButtonWithTitle:(NSString *)title
                                     block:(BMActionSheetWithButtonIndexBlock)block
{
    NSInteger buttonIndex = [self addButtonWithTitle:title block:block];
    self.destructiveButtonIndex = buttonIndex;
    return buttonIndex;
}


- (BMActionSheetWithButtonIndexBlock)buttonBlockAtIndex:(NSInteger)buttonIndex
{
    return [self BM_blockForKey:[NSNumber numberWithInteger:buttonIndex]];
}


#pragma mark -
#pragma mark Delegate Blocks


- (BMActionSheetWithButtonIndexBlock)actionSheetClickedButtonAtIndexBlock
{
    return [self BM_blockForKey:BMActionSheetClickedButtonAtIndexKey];
}


- (void)setActionSheetClickedButtonAtIndexBlock:(BMActionSheetWithButtonIndexBlock)actionSheetClickedButtonAtIndexBlock
{
    [self BM_setBlock:actionSheetClickedButtonAtIndexBlock forKey:BMActionSheetClickedButtonAtIndexKey];
}


- (BMActionSheetBlock)willPresentActionSheetBlock
{
    return [self BM_blockForKey:BMActionSheetWillPresentActionSheetKey];
}


- (void)setWillPresentActionSheetBlock:(BMActionSheetBlock)willPresentActionSheetBlock
{
    [self BM_setBlock:willPresentActionSheetBlock forKey:BMActionSheetWillPresentActionSheetKey];
}


- (BMActionSheetBlock)didPresentActionSheetBlock
{
    return [self BM_blockForKey:BMActionSheetDidPresentActionSheetKey];
}


- (void)setDidPresentActionSheetBlock:(BMActionSheetBlock)didPresentActionSheetBlock
{
    [self BM_setBlock:didPresentActionSheetBlock forKey:BMActionSheetDidPresentActionSheetKey];
}


- (BMActionSheetWithButtonIndexBlock)actionSheetWillDismissWithButtonIndexBlock
{
    return [self BM_blockForKey:BMActionSheetWillDismissWithButtonIndexKey];
}


- (void)setActionSheetWillDismissWithButtonIndexBlock:(BMActionSheetWithButtonIndexBlock)actionSheetWillDismissWithButtonIndexBlock
{
    [self BM_setBlock:actionSheetWillDismissWithButtonIndexBlock forKey:BMActionSheetWillDismissWithButtonIndexKey];
}


- (BMActionSheetWithButtonIndexBlock)actionSheetDidDismissWithButtonIndexBlock
{
    return [self BM_blockForKey:BMActionSheetDidDismissWithButtonIndexKey];
}


- (void)setActionSheetDidDismissWithButtonIndexBlock:(BMActionSheetWithButtonIndexBlock)actionSheetDidDismissWithButtonIndexBlock
{
    [self BM_setBlock:actionSheetDidDismissWithButtonIndexBlock forKey:BMActionSheetDidDismissWithButtonIndexKey];
}


#pragma mark -
#pragma mark UIActionSheetDelegate


- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
    [actionSheet retain];
    BMActionSheetWithButtonIndexBlock actionSheetWithButtonIndexBlock = [actionSheet actionSheetClickedButtonAtIndexBlock];
    if (actionSheetWithButtonIndexBlock) {
        actionSheetWithButtonIndexBlock(actionSheet, buttonIndex);
    }
    actionSheetWithButtonIndexBlock = [actionSheet buttonBlockAtIndex:buttonIndex];
    if (actionSheetWithButtonIndexBlock) {
        actionSheetWithButtonIndexBlock(actionSheet, buttonIndex);
    }
    [actionSheet release];
}


- (void)willPresentActionSheet:(UIActionSheet *)actionSheet
{
    BMActionSheetBlock actionSheetBlock = [actionSheet willPresentActionSheetBlock];
    if (actionSheetBlock) {
        actionSheetBlock(actionSheet);
    }
}


- (void)didPresentActionSheet:(UIActionSheet *)actionSheet
{
    BMActionSheetBlock actionSheetBlock = [actionSheet didPresentActionSheetBlock];
    if (actionSheetBlock) {
        actionSheetBlock(actionSheet);
    }
}


- (void)actionSheet:(UIActionSheet *)actionSheet willDismissWithButtonIndex:(NSInteger)buttonIndex
{
    BMActionSheetWithButtonIndexBlock actionSheetWithButtonIndexBlock = [self actionSheetWillDismissWithButtonIndexBlock];
    if (actionSheetWithButtonIndexBlock) {
        actionSheetWithButtonIndexBlock(actionSheet, buttonIndex);
    }
}


- (void)actionSheet:(UIActionSheet *)actionSheet didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    BMActionSheetWithButtonIndexBlock actionSheetWithButtonIndexBlock = [self actionSheetDidDismissWithButtonIndexBlock];
    if (actionSheetWithButtonIndexBlock) {
        actionSheetWithButtonIndexBlock(actionSheet, buttonIndex);
    }
}


@end
