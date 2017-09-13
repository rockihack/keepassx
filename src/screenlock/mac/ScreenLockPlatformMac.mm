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
#include "screenlock/ScreenLock.h"

#import <Foundation/NSDistributedNotificationCenter.h>
#import <AppKit/NSWorkspace.h>

@implementation ScreenLockPlatformMacImpl

ScreenLockPlatformMac::ScreenLockPlatformMac()
{
    self = [[ScreenLockPlatformMacImpl alloc] init];
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

ScreenLockPlatformMac::~ScreenLockPlatformMac()
{
    [[[NSWorkspace sharedWorkspace] notificationCenter] removeObserver:static_cast<id>(self)];
    [[NSDistributedNotificationCenter defaultCenter] removeObserver:static_cast<id>(self)];
    [static_cast<id>(self) dealloc];
}

- (void) setScreenLock:(ScreenLock*) screenlock
{
    self.screenlock = screenlock;
}

- (void) screenLockObserver:(NSNotification*) notification
{
    Q_UNUSED(notification);

    ScreenLock* screenlock = self.screenlock;
    if (screenlock != nullptr) {
        Q_EMIT screenlock->locked();
    }
}

void ScreenLockPlatformMac::init(ScreenLock* const screenlock)
{
    [static_cast<id>(self) setScreenlock:screenlock];
}

@end
