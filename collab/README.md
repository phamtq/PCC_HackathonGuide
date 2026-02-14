# Tools for Collaboration

Ideas are a dime a dozen. Everyone has them, but not everyone has the ability to bring them to life. And you certainly won't be able to do it on your own. In addition to there not being enough robotic equipment for everyone to work by themselves, you just don't have enough time. You're going to feel tired and frustrated and that time you spend recovering can be made more useful by letting other people work on other parts of the project.

So to increase your chances of bringing that project to fruition, you're going to need some tools to organize everything and keep everyone on the same page. Many of these tools are used as part of professional projects in jobs you'll encounter when you graduate. 

# Table of Contents

+ [Project Management Software](README.md#project-management-software)
+ [Communication Tools](README.md#communication-tools)
+ [Central Repository](README.md#central-repository)

## Project Management Software
You can use everything from a simple shared TODO list like [Google Keep](https://keep.google.com), or ones specifically designed for larger projects like:

+ [Trello](https://trello.com/)
+ [Notion](https://www.notion.com/)
+ [Asana](https://asana.com). 

They are all free or have a trial period that will work for your 1 day project. 

If you're unfamilar with any of these tools, I suggest writing the tasks down on paper and then transfering it later to these pieces of software as you get more familar with it. 

## Communication Tools
While you're going to be next to each other working on this project, some of you might want to work away from the group in a quiter spot. Having a way to communicate with each other will help you stay in the loop while letting you get things done. It's also a good way to transfer small files to each other like a PDF of something you found.

You can use things like [Discord](https://discord.com/), [Slack](https://slack.com/), or even starting a group chat over iMessage, Android Messages, or What'sApp.

## Central Repository
You're going to want to a way to manage the code you and your teammates are going to write. After all, if you and a teammate are working on the same source code file, how do you ensure that both your edits are available when it comes time to run it?

The solution to this issue is called a version control software system. The most popular one these days is the software known as `git`. Created by the same guy that created Linux, it a very handy tool used throughout the industry to work collaboratively on software.

Here's now you get started. First, download the software to your computer. Here the various ways to do that depending on which operating system you're running.

### Windows
Download the installer for Windows [here](https://git-scm.com/install/windows) and install it. Use the defaults during the installation except for your preferred editor like `vim` or `VS Code`. When that's done you can launch it by double-click on the `Git Bash` icon on your desktop.

To test to make sure it installed correctly, type:

> $ git --version

### macOS
To run it on macOS, you're going to first need to install a program called `homebrew` to make the process much easier. Open the `Terminal` and go to the [official Homebrew website](https://brew.sh/). Follow the instructions on the page. Then run this command in the `Terminal`:

> $ brew install git

To test to make sure it installed correctly, type:

> $ git --version

### Linux
Arguably the easiest method that but one that'll vary depending on which package manager you have for your particular distro. For example, to install it on Arch Linux:

> $ sudo pacman -S git

For Fedora:

>$ sudo dnf install git

For Ubuntu:

> $ sudo apt install git

To test to make sure it installed correctly, type:

> $ git --version

# Git Basics
