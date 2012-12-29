//
//  ViewController.h
//  Tetris
//
//  Created by erlz nuo on 12-12-23.
//  Copyright (c) 2012年 erlz nuo. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "OpenGLView.h"

#define kTetrisBlockCellW   20
#define kTetrisBlockCellH   20



@interface ViewController : UIViewController
{
    pthread_t           mRenderTid;
}

//@property (strong, nonatomic) IBOutlet OpenGLView *renderView;
@property (strong, nonatomic) IBOutlet UILabel      *insertFlagLabel;

@property (strong, nonatomic) NSTimer *tetrisTimer;

@end
