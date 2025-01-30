Copyright (C) 2022, VadRov, all right reserved / www.youtube.com/@VadRov / www.dzen.ru/vadrov

# The project demonstrates the work of a file manager using the FATFS library

Free distribution is allowed without commercial use.
For commercial use, the consent of the author is required.
Will be distributed "as is", that is, use at your own risk.
The author does not provide any guarantees.

# Main features of the file manager:
- Displaying the manager in a window, the parameters (coordinate of the upper left corner, width and height of the window, font, color) of which are specified by the user;
- Abbreviation of file and directory names (if they do not fit in the window) like the Total Commander manager;
- No restrictions on the number of files and subdirectories in a directory;
- Remembering the cursor position in the parent directory and restoring this position when returning to it from a nested subdirectory;
- Displaying icons (badges) of files and directories;
- Support for nested directories with a nesting level limited by the size of the microcontroller's RAM.
- Any interface design through the use of color schemes.
- Return of the selected file name and path to it.
- Possibility to connect any option of manager control selection: buttons, encoder, touchscreen without making changes to the code of the file manager procedure. To do this, it is enough to transfer values ​​(button keys) from external encoder or touchscreen polling procedures through the KEYB_key_buff key buffer of the keyboard module, i.e. emulate pressing non-existent buttons. The KEYB_key_buff buffer can be filled in the interrupt from the encoder or touchscreen.

# The project is based on the author's libraries:
- driver for working with SD-card via SPI using DMA (https://www.youtube.com/watch?v=z99bLhlnukM)
- driver for working with displays via SPI using DMA (https://www.youtube.com/watch?v=8tIJ16riJqo)
- module for working with buttons with a bounce filter, auto-repeat and input buffering (https://www.youtube.com/watch?v=e-w5HS75neg)
- procedures for working with strings.

The video tells about the project:
 [![Watch the video](https://img.youtube.com/vi/4c_PwnasQvs/maxresdefault.jpg)](https://youtu.be/4c_PwnasQvs)

Author: **VadRov**

Contacts: [Youtube](https://www.youtube.com/@VadRov) [Dzen](https://dzen.ru/vadrov) [VK](https://vk.com/vadrov) [Telegram](https://t.me/vadrov_channel)\
Donate: [donate.yoomoney](https://yoomoney.ru/to/4100117522443917)
