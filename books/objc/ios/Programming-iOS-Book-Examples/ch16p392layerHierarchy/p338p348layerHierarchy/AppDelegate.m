

#import "AppDelegate.h"
#import <QuartzCore/QuartzCore.h>

@implementation AppDelegate

@synthesize window = _window;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    self.window.rootViewController = [UIViewController new];
    UIView* v = self.window.rootViewController.view;
    
    // figure 14-1 with layers instead of views
    
    CALayer* lay1 = [[CALayer alloc] init];
    lay1.frame = CGRectMake(113, 111, 132, 194);
    lay1.backgroundColor = [[UIColor colorWithRed:1 green:0 blue:0 alpha:1] CGColor];
    [v.layer addSublayer:lay1];
    CALayer* lay2 = [[CALayer alloc] init];
    lay2.backgroundColor = [[UIColor colorWithRed:0 green:1 blue:0 alpha:1] CGColor];
    lay2.frame = CGRectMake(0, 16, 62, 54);
    [lay1 addSublayer:lay2]; // lay2.frame 的位置参考系是 lay1.frame
    CALayer* lay3 = [[CALayer alloc] init];
    lay3.backgroundColor = [[UIColor colorWithRed:0 green:0 blue:1 alpha:1] CGColor];
    lay3.frame = CGRectMake(43, 197, 160, 230);
    [v.layer addSublayer:lay3];
    lay3.zPosition = -1.0;

    
    
    
    // Override point for customization after application launch.
    self.window.backgroundColor = [UIColor whiteColor];
    [self.window makeKeyAndVisible];
    return YES;
}

@end
