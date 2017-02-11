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

#ifndef UINPUT_H_
#define UINPUT_H_

#include <stdint.h>

class UInput
{
public:
    enum KeyState {
        Release     = 0,
        Keypress    = 1,
        Autorepeat  = 2
    };

    UInput();
    virtual ~UInput();

    bool init();
    bool sendKey(const uint32_t keycode, const KeyState state) const;

private:
    int m_fd;

    static int createDevice();
    static void destroyDevice(int fd);

    bool writeEvent(const struct input_event* event) const;
};

#endif  // UINPUT_H_
