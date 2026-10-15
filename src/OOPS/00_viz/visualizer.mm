#import <Cocoa/Cocoa.h>
#include "visualizer.h"

namespace viz {
static std::vector<DescribeFn> gObjects;
static NSInteger gFocused = 0;
}

@interface VizView : NSView
@end

@implementation VizView
- (void)drawRect:(NSRect)dirtyRect {
    [[NSColor colorWithCalibratedRed:0.10 green:0.12 blue:0.16 alpha:1.0] setFill];
    NSRectFill(dirtyRect);

    NSDictionary *titleAttrs = @{
        NSFontAttributeName: [NSFont boldSystemFontOfSize:14],
        NSForegroundColorAttributeName: [NSColor whiteColor],
    };
    NSDictionary *labelAttrs = @{
        NSFontAttributeName: [NSFont systemFontOfSize:11],
        NSForegroundColorAttributeName: [NSColor colorWithCalibratedWhite:0.85 alpha:1.0],
    };
    NSDictionary *hintAttrs = @{
        NSFontAttributeName: [NSFont systemFontOfSize:11],
        NSForegroundColorAttributeName: [NSColor colorWithCalibratedWhite:0.6 alpha:1.0],
    };
    NSColor *accents[] = {
        [NSColor colorWithCalibratedRed:0.95 green:0.35 blue:0.30 alpha:1.0],
        [NSColor colorWithCalibratedRed:0.30 green:0.70 blue:0.95 alpha:1.0],
        [NSColor colorWithCalibratedRed:0.60 green:0.90 blue:0.40 alpha:1.0],
        [NSColor colorWithCalibratedRed:0.95 green:0.75 blue:0.30 alpha:1.0],
    };

    CGFloat width = self.bounds.size.width;
    CGFloat top = self.bounds.size.height - 20;
    CGFloat cardW = width - 40;
    CGFloat y = top;

    viz::Snapshot focusedSnap;

    for (size_t i = 0; i < viz::gObjects.size(); ++i) {
        viz::Snapshot snap = viz::gObjects[i]();
        if ((NSInteger)i == viz::gFocused) focusedSnap = snap;

        CGFloat cardH = 40 + 22 * (CGFloat)snap._bars.size() + 16 * (CGFloat)snap._lines.size();
        NSRect card = NSMakeRect(20, y - cardH, cardW, cardH);

        [[NSColor colorWithCalibratedWhite:0.18 alpha:1.0] setFill];
        NSBezierPath *cardPath = [NSBezierPath bezierPathWithRoundedRect:card xRadius:8 yRadius:8];
        [cardPath fill];

        if ((NSInteger)i == viz::gFocused) {
            [[NSColor colorWithCalibratedWhite:0.95 alpha:1.0] setStroke];
            [cardPath setLineWidth:1.5];
            [cardPath stroke];
        }

        [accents[i % 4] setFill];
        NSRectFill(NSMakeRect(card.origin.x, card.origin.y, 4, card.size.height));

        NSString *title = [NSString stringWithUTF8String:snap._title.c_str()];
        [title drawAtPoint:NSMakePoint(card.origin.x + 14, NSMaxY(card) - 24) withAttributes:titleAttrs];

        CGFloat rowY = NSMaxY(card) - 46;
        for (const viz::Bar &b : snap._bars) {
            NSString *lbl = [NSString stringWithUTF8String:b.label.c_str()];
            [lbl drawAtPoint:NSMakePoint(card.origin.x + 14, rowY) withAttributes:labelAttrs];

            NSRect track = NSMakeRect(card.origin.x + 90, rowY + 2, cardW - 220, 12);
            [[NSColor colorWithCalibratedWhite:0.28 alpha:1.0] setFill];
            NSBezierPath *tp = [NSBezierPath bezierPathWithRoundedRect:track xRadius:3 yRadius:3];
            [tp fill];

            double frac = b.value < 0 ? 0 : (b.value > 1 ? 1 : b.value);
            NSRect fill = NSMakeRect(track.origin.x, track.origin.y, track.size.width * frac, track.size.height);
            [accents[i % 4] setFill];
            NSBezierPath *fp = [NSBezierPath bezierPathWithRoundedRect:fill xRadius:3 yRadius:3];
            [fp fill];

            NSString *disp = [NSString stringWithUTF8String:b.display.c_str()];
            [disp drawAtPoint:NSMakePoint(NSMaxX(track) + 10, rowY) withAttributes:labelAttrs];
            rowY -= 22;
        }
        for (const std::string &line : snap._lines) {
            NSString *ns = [NSString stringWithUTF8String:line.c_str()];
            [ns drawAtPoint:NSMakePoint(card.origin.x + 14, rowY) withAttributes:labelAttrs];
            rowY -= 16;
        }

        y -= cardH + 10;
    }

    NSMutableString *hint = [NSMutableString stringWithString:@"[Tab] switch focus"];
    for (const viz::Action &a : focusedSnap._actions) {
        [hint appendFormat:@"   [%c] %s", a.key, a.hint.c_str()];
    }
    [hint drawAtPoint:NSMakePoint(20, 10) withAttributes:hintAttrs];
}

- (BOOL)acceptsFirstResponder { return YES; }
@end

@interface VizAppDelegate : NSObject <NSApplicationDelegate>
@property (strong) NSWindow *window;
@property (strong) VizView *view;
@property (strong) NSTimer *timer;
@property (copy) NSString *windowTitle;
@end

@implementation VizAppDelegate
- (void)applicationDidFinishLaunching:(NSNotification *)notification {
    NSRect frame = NSMakeRect(200, 200, 720, 480);
    self.window = [[NSWindow alloc]
        initWithContentRect:frame
        styleMask:(NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable)
        backing:NSBackingStoreBuffered
        defer:NO];
    [self.window setTitle:self.windowTitle];

    self.view = [[VizView alloc] initWithFrame:frame];
    [self.window setContentView:self.view];
    [self.window makeFirstResponder:self.view];
    [self.window makeKeyAndOrderFront:nil];
    [NSApp activateIgnoringOtherApps:YES];

    self.timer = [NSTimer scheduledTimerWithTimeInterval:1.0/30.0
                                                  target:self
                                                selector:@selector(tick:)
                                                userInfo:nil
                                                 repeats:YES];

    NSEvent * (^handler)(NSEvent *) = ^NSEvent *(NSEvent *event) {
        NSString *chars = event.charactersIgnoringModifiers;
        if (chars.length == 0) return event;
        unichar c = [chars characterAtIndex:0];
        if (c == '\t') {
            if (!viz::gObjects.empty())
                viz::gFocused = (viz::gFocused + 1) % (NSInteger)viz::gObjects.size();
            [self.view setNeedsDisplay:YES];
            return nil;
        }
        if (viz::gFocused >= 0 && viz::gFocused < (NSInteger)viz::gObjects.size()) {
            viz::Snapshot snap = viz::gObjects[viz::gFocused]();
            for (const viz::Action &a : snap._actions) {
                if ((unichar)a.key == c || (unichar)tolower(a.key) == tolower(c)) {
                    a.fn();
                    [self.view setNeedsDisplay:YES];
                    return nil;
                }
            }
        }
        return event;
    };
    [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskKeyDown handler:handler];
}
- (void)tick:(NSTimer *)t { [self.view setNeedsDisplay:YES]; }
- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)a { return YES; }
@end

namespace viz {
void show(std::initializer_list<DescribeFn> describes, const std::string &windowTitle) {
    gObjects.assign(describes.begin(), describes.end());
    gFocused = 0;
    @autoreleasepool {
        NSApplication *app = [NSApplication sharedApplication];
        VizAppDelegate *d = [[VizAppDelegate alloc] init];
        d.windowTitle = [NSString stringWithUTF8String:windowTitle.c_str()];
        [app setDelegate:d];
        [app setActivationPolicy:NSApplicationActivationPolicyRegular];
        [app run];
    }
}
}
