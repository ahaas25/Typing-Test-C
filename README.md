# Typing-Test-C (WIP)

![image](https://user-images.githubusercontent.com/94150901/198175436-75b9f576-1b13-498b-b858-cbb1c9e7b7b6.png)

# Summary

* Typing Test Written in C Programming Language
* Tests User WPM in a variety of different modes.
* Timed Mode: 5, 10, 15, 30, and 60s
* Word Mode: 10, 25, 50, and 100 Words
* Focuses on User's Typing Style and Provides Insights
* Similar to my Typing-Teacher project, however this is more focused on analyzing user data, rather than UI.
* Uses ncurses library

# Objectives
* Clean Code
* Memory Management
* Intuitive UI
* Learning C

# How To Compile

* Download source files
* Install ncurses (If not already installed)
* Compile Using ```"gcc gcc typing_test.c -lncurses"```
* OR: Run "make" using Makefile

# Credits
Word list from https://gist.github.com/deekayen/4148741

# Development Status

Target for first release is the end of November. (I'm a full time student so I work on this project when I can). The goal of the first release is to have a fully functional typing test. This means parsing user input for accuracy and WPM. The analysis of user keystrokes will be added in the second release.

Current status of project:

Code:
* Reading of words.txt *implemented*
* Random word picking *implemented*
* Different test lengths *implemented*
* Parsing user input to be implemented
* Saving of user data to be implemented

UI:
* Main menu *implemented*
* Typing prompt in progress. (Currently shows generated prompt and user info and input parsing not added yet)
* Settings menu to be implemented
