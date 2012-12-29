//
//  ViewController.m
//  Tetris
//
//  Created by erlz nuo on 12-12-23.
//  Copyright (c) 2012年 erlz nuo. All rights reserved.
//

#import <assert.h>
#import <pthread.h>
#import "ViewController.h"
#import "OpenGLView.h"
#import "Tetris.h"


@interface ViewController ()
@end


@implementation ViewController

static OpenGLView *sTTRenderView;

static void *renderThread(void *arg);


- (void)viewDidLoad
{
    [super viewDidLoad];
    OpenGLView **ppGLView = &sTTRenderView;

    *ppGLView = [[OpenGLView alloc] initWithFrame:self.view.bounds];
    (*ppGLView).autoresizingMask = 0xFFFFFFFF;
    if ( [[UIScreen mainScreen] respondsToSelector:@selector(scale)] )
        (*ppGLView).contentScaleFactor = [[UIScreen mainScreen] scale];
    [_insertFlagLabel.superview insertSubview:*ppGLView belowSubview:_insertFlagLabel];


//    int ret = [self.renderView initOpenGL];
//    sTTRenderView = self.renderView;
//    assert(ret);


    [[UIApplication sharedApplication] setStatusBarHidden:YES];
//    [self.renderView render];
//    [self.renderView renderRectInIosCoorWithX1:0 withY1:0 withX2:300 withY2:460];

//    [self.renderView OVDisplayClear];
//    [self.renderView OVDisplaySetColorWithR:1.0 withG:0 withB:0 withA:0];
//    [self.renderView OVDisplayDrawRectWithX1:1 withY1:1 withX2:300 withY2:460];
////    [self.renderView OVDisplayFillRectWithX1:0 withY1:0 withX2:300 withY2:460];
//    for (int i = 0; i < 10000; i++) {
//        [self.renderView OVDisplayFillRectWithX:50 withY:89 withW:60 withH:40];
//    }
//    [self.renderView OVDisplayShow];

    pthread_create(&mRenderTid, NULL, renderThread, NULL);


    self.tetrisTimer = [NSTimer scheduledTimerWithTimeInterval:1.0/5
                                                        target:self
                                                      selector:@selector(tetrisTimerAction)
                                                      userInfo:nil
                                                       repeats:YES];
}

- (void)viewDidUnload
{
//    [self.renderView unInitOpenGL];    self.renderView = nil;


    [self.tetrisTimer invalidate];
    self.tetrisTimer = nil;

    [super viewDidUnload];
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (void)dealloc
{
//    [self.renderView unInitOpenGL];    [_renderView release];


    if ( self.tetrisTimer ) {
        [self.tetrisTimer invalidate];
        [_tetrisTimer release];
    }

    [super dealloc];
}



- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
//    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
    return YES;
}


- (void)tetrisTimerAction
{
    actionPerformed();
}

static void *renderThread(void *arg)
{
    assert(sTTRenderView != NULL);

    int i, j;
    int ttw = kTetrisBlockCellW;
    int tth = kTetrisBlockCellH;

    Tetrisblok();
    int ret = [sTTRenderView initOpenGL];
    assert(ret);
    [sTTRenderView OVDisplaySetColorWithR:1.0 withG:0 withB:0 withA:0];

    while ( 1 ) {
        [sTTRenderView OVDisplayClear];

        // 画当前方块
        for (j = 0; j < kShapeW * kShapeH; j++) {
            if ( activatingShapeFlag(j) ) {
                //            g.fillRect((j % kShapeH + x + 1) * 10, (j / kShapeH + y) * 10, 10, 10);
                [sTTRenderView OVDisplayFillRectWithX:(j % kShapeH + x + 1) * ttw
                                                withY:(j / kShapeH + y) * tth
                                                withW:ttw
                                                withH:tth];
            }
        }
        // 画已经固定的方块
        for (j = 0; j < kWallH; j++) {
            for (i = 0; i < kWallW; i++) {
                if (map[i][j] == 1) {
                    //                g.fillRect(i * 10, j * 10, 10, 10);
                    [sTTRenderView OVDisplayFillRectWithX:i*ttw withY:j*tth withW:ttw withH:tth];
                } else if (map[i][j] == 2) {
                    //                g.drawRect(i * 10, j * 10, 10, 10);
                    [sTTRenderView OVDisplayDrawRectWithX:i*ttw withY:j*tth withW:ttw withH:tth];
                }
            }
        }

        [sTTRenderView OVDisplayShow];

        usleep(10*1000);
    }

    [sTTRenderView unInitOpenGL];
    sTTRenderView = nil;

    return NULL;
}


@end
