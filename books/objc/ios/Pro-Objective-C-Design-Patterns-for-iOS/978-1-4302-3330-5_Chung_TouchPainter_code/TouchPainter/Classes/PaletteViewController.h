//
//  PaletteViewController.h
//  TouchPainter
//
//  Created by Carlo Chung on 10/18/10.
//  Copyright 2010 Carlo Chung. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CommandBarButton.h"
#import "CommandSlider.h"
#import "SetStrokeColorCommand.h"
#import "SetStrokeSizeCommand.h"

// MARK-Adapter: PaletteViewController 充当"适配器". [(P94)]
@interface PaletteViewController : UIViewController
#ifdef Use_Delegate_Implement_Adapter
                                   <SetStrokeColorCommandDelegate, 
                                    SetStrokeSizeCommandDelegate>
#endif
{
	@private
	IBOutlet CommandSlider *redSlider_;
	IBOutlet CommandSlider *greenSlider_;
	IBOutlet CommandSlider *blueSlider_;
	IBOutlet CommandSlider *sizeSlider_;
	IBOutlet UIView *paletteView_;
}

// slider event handler
- (IBAction) onCommandSliderValueChanged:(CommandSlider *)slider;

@end
