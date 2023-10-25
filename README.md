# Typing-Test-C


<p align="center">
  <img src="https://user-images.githubusercontent.com/94150901/204944905-326723d6-cc86-4823-8f43-d7c078fc8f89.png">
</p>

<p align="center">
  <img src="https://user-images.githubusercontent.com/94150901/204944987-a2fcd11a-13b5-49a8-831a-5c143454bf93.png">
</p>

<p align="center">
  <img src="https://user-images.githubusercontent.com/94150901/204943113-6448515a-ff00-4cc8-81fd-262572be6c6a.png">
</p>

# Summary

* Typing Test Written in C Programming Language
* Tests User WPM in a variety of different modes.
* Timed Mode: 5, 10, 15, 30, and 60s
* Word Mode: 10, 25, 50, and 100 Words
* Focuses on User's Typing Style and Provides Insights
* Similar to my Typing-Teacher project, however this is more focused on analyzing user data, rather than UI, which was the focus of that project.
* Uses ncurses library

# Objectives
* Clean Code
* Memory Management
* Intuitive UI
* Learning C

# How To Compile

* Download source files
* Install ncurses (If not already installed)
  * On Debian Systems: ```sudo apt install libncurses5-dev libncursesw5-dev```
* Compile Using ```gcc typing_test.c utilities.c -lncurses```
* OR: Run ```make``` using Makefile

# How To Run
After obtaining your compiled Typing-Test-C program, simply run the program from your terminal. Keep in mind this program requires a ```words.txt``` file in the same directory as itself to generate word prompts.

Note: This program has only been tested in a color-supporting terminal. If you experience unexpected behavior, ensure your terminal supports color and text highlighting.

# Credits
Word list from https://gist.github.com/deekayen/4148741

# Development Status

First release goal:
Target for first release is the end of November. (I'm a full time student so I work on this project when I can). The goal of the first release is to have a fully functional typing test. This means parsing user input for accuracy and WPM. The analysis of user keystrokes will be added in the second release.

First release goal reached!
Find it [here](https://github.com/ahaas25/Typing-Test-C/releases/tag/milestone)

Second release goal:
The second release goal for this project is to have a much more refined project. I hope to have all modes implemented, the UI to automatically wrap the console size, and user keystroke analysis if time permits. I'm hoping to have this release out by early January. (It's finals season so I won't have much time to work on this project until late December)

Second release goal reached! Due to time constraints, not all features were implemented in this release.
Find it [here](https://github.com/ahaas25/Typing-Test-C/releases/tag/milestone2)

Current status of project:

Code:
* Reading of words.txt *implemented*
* Random word picking *implemented*
* Different test lengths *implemented*
* Timed Tests *implemented*
* Test results screen *implemeted*
* Saving user stats *implemented*
* ~~Per-key stats~~ May be added in a feature release

UI:
* Main menu *implemented*
* Typing prompt *implemented*
* Stats menu *implemented*
* Settings menu *implemented*
* Themes