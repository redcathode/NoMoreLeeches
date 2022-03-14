# Tutorial
Welcome to the NML tutorial! I'll brief over how to download and install NML, then go over some common usages.

## Download & Install
### Windows
Download NoMoreLeeches.exe from [here](https://github.com/asolidtime/spyr/releases/latest).  
This isn't an installer, just a program executable. If you want to 'install' it, move it into any folder, then create a shortcut (right click on the executable, then `Create Shortcut`) and move the created shortcut to your desktop or taskbar.  
  
### Linux
For now, I recommend following the build instructions detailed in the README, then manually creating a .desktop linking to the generated executable and moving it to `.local/share/applications`. You can also grab a binary from GitHub releases, but it's not guaranteed to work on every distro. Work on an AUR package is underway.

## Getting Started
When you first run NoMoreLeeches, you'll be greeted with something like this:  
  
(note: this tutorial is under very active development, i'll have screenshots and gifs in a bit)  
  
There's a lot going on here, so I'm gonna start by showing how to use the most basic functionality: blocking.    
Make sure that NML is 'currently enabled'. Select a window you'd like to try blocking, then switch back to NML.  
  
(screenshot of the active window fields)  
  
You'll notice that the 'active window title' and 'active window class' fields are now populated with a bunch of text related to your app.  
(a bit of info: window title is... the window title. Window class is what your application calls itself, and it'll almost always be the same for a given app, even if the app doesn't always have the same window title every time.)  
If you want to block the app itself regardless of its title, copy-paste the 'window class' field into 'Blocked window classes'.  
If you only want to block the app when its title contains a certain keyword (e.g. 'reddit' to block a web browser if you switch to a tab with 'reddit' in the title), type the keyword you want to block into 'Blocked window wildcards'. This is case-sensitive.  
  
## Lockdown Mode
(todo)
