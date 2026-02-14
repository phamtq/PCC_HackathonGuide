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

**Windows**

Download the installer for Windows [here](https://git-scm.com/install/windows) and install it. Use the defaults during the installation except for your preferred editor like `vim` or `VS Code`. When that's done you can launch it by double-click on the `Git Bash` icon on your desktop.

To test to make sure it installed correctly, type:

> $ git --version

Skip down to "Git Setup" when finished.

**macOS**

To run it on macOS, you're going to first need to install a program called `homebrew` to make the process much easier. Open the `Terminal` and go to the [official Homebrew website](https://brew.sh/). Follow the instructions on the page. Then run this command in the `Terminal`:

> $ brew install git

To test to make sure it installed correctly, type:

> $ git --version

Skip down to "Git Setup" when finished.

**Linux**

Arguably the easiest method that but one that'll vary depending on which package manager you have for your particular distro. For example, to install it on Arch Linux:

> $ sudo pacman -S git

For Fedora:

>$ sudo dnf install git

For Ubuntu:

> $ sudo apt install git

To test to make sure it installed correctly, type:

> $ git --version

Skip down to "Git Setup" when finished.

**Git Setup**

One of the first things you're going to want to do is set up your installation with your information. Since it's easier to set it up for all your projects rather than go through this process for each of them, we going to make your settings "global":

Type these two commands, replacing the name and email with your information:

```sh
$ git config --global user.name "Maria Perez"
$ git config --global user.email maria.perez@pcc.edu
```

### Online Git Repository
You're going to want to host your project online so that your other teammates will have access to it. There are many services that do it such as [GitHub](https://github.com/), [GitLab](https://about.gitlab.com/), [Bitbucket](https://bitbucket.org/), and [SourceForge](https://sourceforge.net/) but for our purposes, we're going to use, arguably, the most popular one, GitHub.

Have everyone on your team sign up for a GitHub account if they don't already have one by clicking on the `Sign Up` button in the top right corner of the page.

Once that's done, designate one team member as the holder of the project like the Project Manager. Don't worry, you'll all have access to it. That person will create a new repository (*repo*) for the project. To do that click on the `Repositories` tab and then the green "New" button. 

![Create a new repo](images/github.create_screenshot.png)

Next, fill in the settings for your project. Just about everything on here can be changed so don't worry about making any mistakes. A good project name might be `PCC_Hackathon_Winter26`, especially if you plan on doing it every term.

![Fill out settings](images/github.settings_screenshot.png)

Click the green `Create repository` button when done.

Now we're going to give your teammates access to the repo. Click on the `Settings` tab.

![Adding collaborator - Settings tab](images/github.access1_screenshot.png)

Then `Collaborators` on the left hand size menu, and then on `Add people`.

![Adding collaborator - Collaborators](images/github.access2_screenshot.png)

Add your teammates by their GitHub username and click on the green `Add to repository` button when you've added them all. They might need to confirm the addition in the email. 

![Adding collaborator - Collaborators](images/github.access3_screenshot.png)

After that, we're going to create a copy of the project on each of your teammates computers. Click on the green `<> Code` button and then on that icon next to that web address.

![Cloning](images/github.clone_screenshot.png)

Finally, go back to your `Terminal` / `Git Bash` program and go to a folder where you want to keep your project (for example, your Documents folder). Then type `git clone` followed the web address you coped this command.

> $ git clone https://github.com/<some_user>/<project_name>.git

This will make an exact copy on your computer!

### Git Basics

