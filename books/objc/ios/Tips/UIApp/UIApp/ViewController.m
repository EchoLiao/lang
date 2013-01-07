//
//  ViewController.m
//  UIApp
//
//  Created by erlz nuo on 13-1-7.
//  Copyright (c) 2013年 erlz nuo. All rights reserved.
//

#import "ViewController.h"

#define kDocsPath [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"]


@interface ViewController ()
@end


@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}



#pragma mark -

// iPhone 程序启动互相调用, 打开其他程序
// http://www.cnblogs.com/chen1987lei/archive/2012/04/17/2453834.html

- (IBAction)openBrowser:(id)sender
{
//    itms:// 可以打开MobileStore.app
//    audio-player-event:// 可以打开iPod
//    audio-player-event://?uicmd=show-purchased-playlist 可以打开iPod播放列表
//    video-player-event:// 可以打开iPod中的视频


    // app store
//    NSURL *appStoreUrl = [NSURL URLWithString:@"http://phobos.apple.com/WebObjects/MZStore.woa/wa/viewSoftware?id=291586600&amp;mt=8"];
//    [[UIApplication sharedApplication] openURL:appStoreUrl];

    // message
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"sms:55555"]];

    // phone
//    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"tel://8004664411"]];

    // mail
//    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"mailto://info@iphonedevelopertips.com"]];

    // google map
//    NSString *searchQuery = @"the postion I want to know";
//    searchQuery = [searchQuery stringByAddingPercentEscapesUsingEncoding: NSUTF8StringEncoding];
//    NSString *urlString = [NSString stringWithFormat:@"http://maps.google.com/maps?q=%@", searchQuery];
//    [[UIApplication sharedApplication] openURL:[NSURL URLWithString: urlString]];

    // browser
//    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"http://www.google.com/"]];
}

- (IBAction)openAnotherApp:(id)sender
{
    NSString *video = [NSString stringWithFormat:@"VPlayer://%@/like.mp4", kDocsPath];
//    NSString *video = @"VPlayer://http://v.youku.com/v_show/id_XNDk3ODAzMzEy.html";
//    NSString *video = @"VPlayer://assets-library://asset/asset.MOV?id=128AACF1-96E3-4024-85F0-12686DBD54AA&ext=MOV";

    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:video]];

    NSLog(@"NAL &&& ... %@", video);
}


@end
