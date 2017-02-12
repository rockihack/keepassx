/*
 *  Copyright (C) 2016 Lennart Glauer <mail@lennart-glauer.de>
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

#include "UInput.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>

#include <QDebug>

// File descriptor
#define INVALID_FD -1
#define IS_OPEN(fd) ((fd) >= 0)

// UInput device
#define DEVICE_NAME "KeePassX Autotype"
#define DEVICE_VENDOR 1
#define DEVICE_PRODUCT 1
#define DEVICE_VERSION 1

UInput::UInput()
    : m_fd(INVALID_FD)
{
}

UInput::~UInput()
{
    if (IS_OPEN(m_fd)) {
        destroyDevice(m_fd);
    }
}

//
// Initialize uinput device
//
bool UInput::init()
{
    if (IS_OPEN(m_fd)) {
       return true;
    }

    m_fd = createDevice();

    return IS_OPEN(m_fd);
}

//
// Send key to uinput device
//
bool UInput::sendKey(const uint32_t keycode, const KeyState state) const
{
    struct input_event event;
    memset(&event, 0, sizeof(event));

    event.type = EV_KEY;
    event.code = keycode;
    event.value = state;
    if (!writeEvent(&event)) {
        return false;
    }

    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    if (!writeEvent(&event)) {
        return false;
    }

    return true;
}

//
// Send event to uinput device
//
bool UInput::writeEvent(const struct input_event* event) const
{
    return IS_OPEN(m_fd) && write(m_fd, event, sizeof(*event)) >= 0;
}

//
// Create virtual keyboard device
//
int UInput::createDevice()
{
    const int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd < 0) {
        qWarning("Open device /dev/uinput failed!");
        return INVALID_FD;
    }

    // Enable key events
    if (ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0) {
        close(fd);
        return INVALID_FD;
    }
    // Enable keycodes
    for (int i = 0; i < 256; ++i) {
        if (ioctl(fd, UI_SET_KEYBIT, i) < 0) {
            close(fd);
            return INVALID_FD;
        }
    }

    // Create virtual device
    struct uinput_user_dev virtualDevice;
    memset(&virtualDevice, 0, sizeof(virtualDevice));
    snprintf(virtualDevice.name, UINPUT_MAX_NAME_SIZE, DEVICE_NAME);
    virtualDevice.id.bustype = BUS_VIRTUAL;
    virtualDevice.id.vendor = DEVICE_VENDOR;
    virtualDevice.id.product = DEVICE_PRODUCT;
    virtualDevice.id.version = DEVICE_VERSION;

    if (write(fd, &virtualDevice, sizeof(virtualDevice)) < 0) {
        close(fd);
        return INVALID_FD;
    }
    if (ioctl(fd, UI_DEV_CREATE) < 0) {
        close(fd);
        return INVALID_FD;
    }

    return fd;
}

//
// Destroy virtual device
//
void UInput::destroyDevice(const int fd)
{
    ioctl(fd, UI_DEV_DESTROY);
    close(fd);
}
