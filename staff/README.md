# For Hackathon Organizers
This section contains the guide for setting up, testing, and resetting the devices for the next Hackathon. They contain all the necessary files to perform these jobs along with a checklist to make it easier to ensure nothing is missed.

## Table of Contents
+ [Setup](README.md#setup)
+ [Testing](README.md#testing)
+ [Resetting](README.md#resetting)

## Setup

## Testing

## Resetting
The following sections contain the files and steps to return the robots back to their Hackathon ready states. Ideally, they should be performed within a couple of weeks after the end of the event.

### DOFBOT-Pi
The microSD card should be wiped and restored back to it's original configuration. Sometimes the students accidently remove things, change locations of some files, etc... This ensures that the software is the same for every student, every term.

[System Image](https://drive.google.com/drive/folders/1jbWoSmyiCACFyX2TvOGYwWqMqQHhws0l) - use something like [Belena Etcher](https://etcher.balena.io/) or plain ol' Linux `dd`:

> $ dd if=Dofbot_Pi5_20240429.img of=/dev/sdX bs=4M status=progress