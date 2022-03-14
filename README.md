# NoMoreLeeches
TLDR: it's [LeechBlock NG](https://addons.mozilla.org/en-US/firefox/addon/leechblock-ng/), but for desktop apps. Check out the tutorial [here.](#tutorial)  

Long explanation:  
NoMoreLeeches is a relatively simple tool to help improve your productivity by blocking apps and webpages you probably don't want to be using anyway. I originally made it as a tool to help with my own executive dysfunction, but I'm open-sourcing and maintaining it under the GPLv3 in the hopes that others will find it useful too. NML is built with C++ and FLTK and works on Windows and Linux. (on Linux, Wayland has a few problems. See [Wayland notes](#notes-on-wayland-support) for more details)  

## Features
- 'Lockdown mode' that prevents the user from changing block settings and ignores window close events
- Color themes!
- Delay blocking (keep this window focused for XX seconds to see this window)
- Regular blocking (keep this window focused if you like staring at the 'Application Blocked' window)
- Very small footprint (uses under 3MB of memory and under 1MB of disk space)

## Tutorial
i'll work on this later

## Notes on Wayland support
Wayland doesn't support normal desktop apps getting info on other active apps for security reasons, and I doubt that'll change any time soon, so apps running natively on Wayland won't be detected. You'll have to force said apps to run on XWayland for them to be detected by NML. This can be done through environment variables for most toolkits (`GDK_BACKEND=x11` on GTK, `QT_QPA_PLATFORM=xcb` on Qt).
