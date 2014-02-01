//
//  BITMainViewController.m
//  AKController
//
//  Created by Keith Sheppard on 12/31/13.
//  Copyright (c) 2013 BoldBits. All rights reserved.
//

#import "BITMainViewController.h"

#define MAX_MOTOR_WIRE_VAL 254

@interface BITMainViewController ()

@property NSOutputStream* outStream;
@property NSInputStream* inStream;

- (void)driveMotorLeftValue:(float)leftVal rightValue:(float)rightVal;

@end

@implementation BITMainViewController

- (void)driveMotorLeftValue:(float)leftVal rightValue:(float)rightVal
{
    int halfMax = MAX_MOTOR_WIRE_VAL / 2;
    leftVal = roundf((leftVal + 1.0f) * halfMax);
    rightVal = roundf((rightVal + 1.0f) * halfMax);
    
    uint8_t cmd[2] = {(uint8_t)leftVal, (uint8_t)rightVal};
    uint8_t* cmdPtr = cmd;
    
    int len = 2;
    while(len) {
        long currBytesWritten = [self.outStream write:cmdPtr maxLength:len];
        if(currBytesWritten == -1) {
            [NSException raise:@"Failed to write" format:@"Failed to write"];
        } else {
            len -= currBytesWritten;
            cmdPtr += currBytesWritten;
        }
    }
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
    CFReadStreamRef readStream;
    CFWriteStreamRef writeStream;
    CFStreamCreatePairWithSocketToHost(NULL, (CFStringRef)@"1.2.3.4", 2000, &readStream, &writeStream);
    self.inStream = (__bridge_transfer NSInputStream *)readStream;
    self.outStream = (__bridge_transfer NSOutputStream *)writeStream;
    
    [self.inStream setDelegate:self];
    [self.outStream setDelegate:self];
    
    [self.inStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [self.outStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [self.inStream open];
    [self.outStream open];
    
    // sending 0, 0 to the motors is an initialization signal that tells AK
    // that it should start listening to the data stream
    [self driveMotorLeftValue:0.0f rightValue:0.0f];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(IBAction)forwardPressed:(id)sender{
    NSLog(@"forward");
    [self driveMotorLeftValue:0.75f rightValue:0.75f];
}

-(IBAction)leftPressed:(id)sender{
    NSLog(@"left");
    [self driveMotorLeftValue:0.2f rightValue:-0.2f];
}

-(IBAction)rightPressed:(id)sender{
    NSLog(@"right");
    [self driveMotorLeftValue:-0.2f rightValue:0.2f];
}

-(IBAction)backPressed:(id)sender{
    NSLog(@"back");
    [self driveMotorLeftValue:-0.5f rightValue:-0.5f];
}

-(IBAction)controlButtonReleased:(id)sender{
    NSLog(@"stop");
    [self driveMotorLeftValue:0.0f rightValue:0.0f];
}

- (void)stream:(NSStream *)stream handleEvent:(NSStreamEvent)eventCode {
    switch(eventCode) {

        case NSStreamEventOpenCompleted:
            NSLog(@"Stream opened");
            break;

        case NSStreamEventHasBytesAvailable:
            NSLog(@"Bytes Available!");
            uint8_t buf[1024];
            long len = [self.inStream read:buf maxLength:1024];
            NSLog(@"length = %d", (int)len);
            
            if(len > 0)
            {
                NSMutableData* data=[[NSMutableData alloc] initWithLength:0];
                
                [data appendBytes: (const void *)buf length:len];
                
                NSString* string = [[NSString alloc] initWithData:data encoding:NSASCIIStringEncoding];
                NSLog(@"Server said- %@", string);
            }
            break;
            
        case NSStreamEventErrorOccurred:
            NSLog(@"Can not connect to the host!");
            break;
            
        case NSStreamEventEndEncountered:
            NSLog(@"End Encountered");
            break;
            
        case NSStreamEventHasSpaceAvailable:
            NSLog(@"Space Availible.");
            break;
            
        case NSStreamEventNone:
            NSLog(@"None event");
            break;
    }
}

- (NSUInteger) supportedInterfaceOrientations {
    // Return a bitmask of supported orientations. If you need more,
    // use bitwise or (see the commented return).
    return UIInterfaceOrientationMaskPortrait;
    // return UIInterfaceOrientationMaskPortrait | UIInterfaceOrientationMaskPortraitUpsideDown;
}

- (UIInterfaceOrientation) preferredInterfaceOrientationForPresentation {
    // Return the orientation you'd prefer - this is what it launches to. The
    // user can still rotate. You don't have to implement this method, in which
    // case it launches in the current orientation
    return UIDeviceOrientationPortrait;
}

- (IBAction)speedXYUpdate:(UIButton *)button forEvent:(UIEvent *)event {
    NSLog(@"Speed update");
    NSSet *touches = [event touchesForView:button];
    
    for (UITouch* touch in touches) {
        float xNorm = ([touch locationInView:button].x / button.frame.size.width) * 2.0f - 1.0f;
        float yNorm = -(([touch locationInView:button].y / button.frame.size.height) * 2.0f - 1.0f);
        
        NSLog(@"x %f, y %f", xNorm, yNorm);
        
        float leftMotor = yNorm - (xNorm / 10.0f);
        if(leftMotor > 1.0f) leftMotor = 1.0f;
        if(leftMotor < -1.0f) leftMotor = -1.0f;
        float rightMotor = yNorm + (xNorm / 10.0f);
        if(rightMotor > 1.0f) rightMotor = 1.0f;
        if(rightMotor < -1.0f) rightMotor = -1.0f;
        
        [self driveMotorLeftValue:leftMotor rightValue:rightMotor];
    }
}

- (IBAction)buttonWasTouched:(UIButton *)button forEvent:(UIEvent *)event {
    NSSet *touches = [event touchesForView:button];
    NSLog(@"button:%@ touches:%@", button, touches);
}

@end
