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

#import <UIKit/UIKit.h>

#import "BMKitTypes.h"


typedef void (^BMActionSheetBlock)(UIActionSheet *actionSheet);
typedef void (^BMActionSheetWithButtonIndexBlock)(UIActionSheet *actionSheet, NSInteger buttonIndex);

/** BMKit related additions to the `UIActionSheet` class.
 
 This category allows to use `UIActionSheet` instances without the need to implement the `UIActionSheetDelegate` protocol anywhere. Instead of using a delegate, you can implement the actions using blocks.
 
 @warning It is not possible to mix and match the delegate approach and this blocks based approach. Either use a delegate or use the block based extensions provided by this category.
 */
@interface UIActionSheet (BMKitAdditions) <UIActionSheetDelegate>

///------------------------------
/// @name Creating Action Sheets
///------------------------------

/** Initializes the action sheet using the specified starting parameters.
 
 @param title A string to display in the title area of the action sheet. Pass `nil` if you do not want to display any text in the title area.
 @return A newly initialized action sheet.
 */
- (id)initWithTitle:(NSString *)title;

///---------------------------
/// @name Configuring Buttons
///---------------------------

/** Adds a custom button with a specified code *block* to the action sheet.
 
 @param title The title of the new button.
 @param block The code block to run when the new button is clicked.
 @return The index of the new button. Button indices start at `0` and increase in the order they are added.
 */
- (NSInteger)addButtonWithTitle:(NSString *)title block:(BMActionSheetWithButtonIndexBlock)block;

/** Adds a custom button to the action sheet and sets it as cancel button.
 
 @param title The title of the new button.
 @return The index of the new button. Button indices start at `0` and increase in the order they are added.
 */
- (NSInteger)addCancelButtonWithTitle:(NSString *)title;

/** Adds a custom button with a specified code *block* to the action sheet and sets it as cancel button.
 
 @param title The title of the new button.
 @param block The code block to run when the new button is clicked.
 @return The index of the new button. Button indices start at `0` and increase in the order they are added.
 */
- (NSInteger)addCancelButtonWithTitle:(NSString *)title block:(BMActionSheetWithButtonIndexBlock)block;

/** Adds a custom button to the action sheet and sets it as destructive button.
 
 @param title The title of the new button.
 @return The index of the new button. Button indices start at `0` and increase in the order they are added.
 */
- (NSInteger)addDestructiveButtonWithTitle:(NSString *)title;

/** Adds a custom button with a specified code *block* to the action sheet and sets it as destructive button.
 
 @param title The title of the new button.
 @param block The code block to run when the new button is clicked.
 @return The index of the new button. Button indices start at `0` and increase in the order they are added.
 */
- (NSInteger)addDestructiveButtonWithTitle:(NSString *)title block:(BMActionSheetWithButtonIndexBlock)block;

/** Returns the code block of the button at the specified index.
 
 @param buttonIndex The index of the button. The button indices start at 0.
 @return The code block of the button at the index specified by *buttonIndex* or `nil`.
 */
- (BMActionSheetWithButtonIndexBlock)buttonBlockAtIndex:(NSInteger)buttonIndex;

///-----------------------
/// @name Delegate Blocks
///-----------------------

/** The code block to execute when a button is clicked on the action sheet. */
@property (nonatomic, copy) BMActionSheetWithButtonIndexBlock actionSheetClickedButtonAtIndexBlock;

/** The code block to execute when the action sheet is about to be presented. */
@property (nonatomic, copy) BMActionSheetBlock willPresentActionSheetBlock;

/** The code block to execute when the action sheet is presented. */
@property (nonatomic, copy) BMActionSheetBlock didPresentActionSheetBlock;

/** The code block to execute when the action sheet is about to be dismissed. */
@property (nonatomic, copy) BMActionSheetWithButtonIndexBlock actionSheetWillDismissWithButtonIndexBlock;

/** The code block to execute when the action sheet is dismissed. */
@property (nonatomic, copy) BMActionSheetWithButtonIndexBlock actionSheetDidDismissWithButtonIndexBlock;

@end
