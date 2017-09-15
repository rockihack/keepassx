/*
 *  Copyright (C) 2017 Lennart Glauer <mail@lennart-glauer.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScreenLockPlatformMacImpl.h"

#import <Foundation/NSDistributedNotificationCenter.h>
#import <AppKit/NSWorkspace.h>

#include "screenlock/ScreenLock.h"

@implementation ScreenLockPlatformMacImpl

ScreenLockPlatformMac::ScreenLockPlatformMac()
{
    self = [[ScreenLockPlatformMacImpl alloc] init:this];
}

ScreenLockPlatformMac::~ScreenLockPlatformMac()
{
    [[[NSWorkspace sharedWorkspace] notificationCenter] removeObserver:static_cast<id>(self)];
    [[NSDistributedNotificationCenter defaultCenter] removeObserver:static_cast<id>(self)];
    [static_cast<id>(self) dealloc];
}

void ScreenLockPlatformMac::init(WId window)
{
    Q_UNUSED(window);

    // Lockscreen notification
    [[NSDistributedNotificationCenter defaultCenter] addObserver:static_cast<id>(self)
                                                        selector:@selector(screenLockObserver:)
                                                            name:@"com.apple.screenIsLocked"
                                                          object:nil
                                              suspensionBehavior:NSNotificationSuspensionBehaviorDeliverImmediately];
    // Sleep notification
    [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:static_cast<id>(self)
                                                           selector:@selector(screenLockObserver:)
                                                               name:NSWorkspaceWillSleepNotification
                                                             object:nil];
    // User change notification
    [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:static_cast<id>(self)
                                                           selector:@selector(screenLockObserver:)
                                                               name:NSWorkspaceSessionDidResignActiveNotification
                                                             object:nil];
}

int ScreenLockPlatformMac::platformEventFilter(void* message)
{
    Q_UNUSED(message);
    Q_ASSERT(false);

    return -1;
}

- (id) init:(ScreenLockPlatformMac*) screenlock
{
    self = [super init];

    if (self) {
        _screenlock = screenlock;
    }

    return self;
}

- (void) screenLockObserver:(NSNotification*) notification
{
    Q_UNUSED(notification);

    if (_screenlock != nullptr) {
        Q_EMIT _screenlock->locked();
    }
}

@end
