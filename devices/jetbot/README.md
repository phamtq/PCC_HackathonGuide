# Hackathon Project

This is a recollection of what I (Dima) and Jess did for our project during the Feb 20, 2026 PCC Hackathon.

[View this document's Markdown source code](index.md)

## Preface

We worked on a Jetbot robot and successfully implemented my idea of making its wheels controlled remotely
using a phone through its orientation sensors, similar to mobile racing games. Additionally, we managed
to pass a video feed from its camera to the screen of the same phone.

## Devices

Our setup involved the following devices:

* Wi-Fi router

  Used to enable peer-to-peer LAN access that PCC's Wi-Fi networks heavily interfere with; all of the
  following devices were connected to its network.

* Jetbot robot

  Device with wheels and a camera running on a vendor-provided Linux image with an SSH server and a
  Docker service.

* Jess's personal phone

  Used for remote control and camera display through 2 Android applications.

* Jess's personal laptop

  Used for typing commands on her phone and the robot through SSH

### Jetbot

The robot had a preinstalled Docker container of Jupyter, a server exposing remote web UI access to
the container with a Python environment that has the dependencies needed to interact with the robot's API.
Since I preferred executing our program from the shell, I did not use (or learn how to use) the web UI for
anything other than its text editor. I did not find any other way to get the Jetbot Python dependencies to
import other than by running the Python program from within the Jupyter container. To do this, I:

1. SSH into the robot: `ssh nvidia@192.168.1.5` (password `nvidia`; the IP address was displayed on the robot)
2. Start a shell within the container: `docker exec -it 8c62cb02916a /bin/bash`
   (the hexadecimal ID was found with `docker ps`)
3. Go into our program's directory (likely unnecessary: `cd jetbot/working_notebooks/basic_motion/`)
4. Invoke the Python script: `python3 <SCRIPT>.py`

Streaming video from the robot worked without involving Jupyter:

1. SSH into the robot: `ssh nvidia@192.168.1.5`
2. Run the video streaming command: `gst-launch-1.0 ... | nc ...` (more on the command below)

### Phone

The phone was used for remote control and for video playback. Remote control commands were sent using Termux,
a terminal emulator application. Video playback was done using mpv-android, an application bundling the mpv
media player for Android. We downloaded both from F-Droid. The Google Play version of Termux did not support
the sensor API.

In hindsight, using a webpage for both remote control and video playback would have likely been a simpler
cross-platform solution, given that there exists a web API for accessing sensor data. Additionally, it would
have enabled access from outside the local network through an HTTPS reverse proxy. We managed to get remote
control (w/a/s/d with no video) through a webpage to work in the evening with not enough time to properly
demonstrate it; to do this, I made the webpage connect to a websocket-to-TCP proxy on my laptop, and the
proxy connected to the robot to send wheel speed commands in a very similar way to how control from Termux worked.

### Laptop

We used the laptop to write code and run commands through SSH on the robot and the phone. With the code and
the commands prepared beforehand (or perhaps by connecting a keyboard to the phone), it would not be necessary,
as it is possible to SSH into the robot from a phone.

#### SSH Port Forwarding

Additionally, we used it for SSH port forwarding to resolve a strange networking issue. For some reason,
when connected to the lab Wi-Fi network, the phone was unable to make connections to or ping any other
device on the network, despite there being no issues connecting *to* or pinging the phone itself from
other devices. This was not a problem for connecting to the phone using SSH since the phone is on the
receiving end, and not a problem for remote control, since it is the robot that makes the connection to
the phone to receive the data stream. However, for video playback, I wanted the phone to initiate a
connection (because I did not realize how trivial it would be to make mpv-android receive data on an
inbound connection). It did not work, so to get around this, I used SSH port forwarding: mpv-android
would connect to localhost where SSH is listening, and SSH would forward it through the tunnel, out the
laptop and to the robot. Because the SSH tunnel to the phone is initiated by the laptop, that got us
around the connectivity issue. The SSH flag we used when connecting to the phone was:
`-R 1235:192.168.1.5:1235`. When the phone (namely, the mpv-android application) would connect to
`127.0.0.1:1235`, the laptop would open a connection on its behalf to `192.168.1.5:1235` (the address of
the robot and the port of the video stream socket). As for why that strange issue was taking place, my
hunch is that it could have to do with the lab network lacking an Internet connection, so maybe the phone
was resorting to routing 192.168.1.0/24 traffic through the inactive mobile carrier connection, refusing
to route it at all or something along those lines; I cannot think of anything else that would explain it.

## Programs

On the robot:

* (In Docker) our Python program that connects to the phone, receives orientation data and controls the
  wheels (`termux-sensor-control.py`; runs in the Jupyter Docker container)
* Video streaming command: `gst-launch-1.0 ... | nc ...`
  * `gst-launch-1.0` accesses the camera and dumps an H.264 video stream to standard out
  * `nc` (netcat) waits for an incoming connection and forwards all data from standard in to that network socket

On the phone:

* (In Termux) orientation data streaming command: `termux-sensor ... | nc ...`
  * `termux-sensor` taps into the phone's orientation sensor and outputs JSON objects containing angular values
    several times a second to standard out
  * `nc` does the same job as on the robot
* mpv-android, which connects to the video stream socket and displays the video on the phone screen
* (In Termux) SSH server (sshd) that allowed for port forwarding and for typing commands for the phone on
  a laptop keyboard.

## Main Python Program

The program simply opens a network connection, reads incoming JSON objects and uses the angular values to
set the speed of both wheels. We made it so that when the phone is at a 45 degree tilt, the robot is not
moving; tilting forward relative to that moves forward and vice versa, tilting left/right steers. The
`angle_to_speed_function` makes it so that around the point where robot is not moving, there is a zone
where tilt corresponds to really gentle movement. Tilting past it, the sensitivity quickly increases.

```python
import json
import socket
import math
from jetbot import Robot

PHONE_IP = "192.168.1.15"
PORT = 1234

robot = Robot()

def handle_orientation_update(orientation):
    [yaw, pitch, roll] = orientation["Orientation Sensor"].get("values")

    forward_speed = angle_to_speed_function(-roll + 45)
    # positive difference means left > right
    wheel_speed_difference = angle_to_speed_function(-pitch)

    speed_left = forward_speed + wheel_speed_difference / 2
    speed_right = forward_speed - wheel_speed_difference / 2
    print(f"yaw={yaw:.1f}, pitch={pitch:.1f}, roll={roll:.1f}, speed_left={speed_left:.3f}, speed_right={speed_right:.3f}")

    robot.set_motors(speed_left, speed_right)

def angle_to_speed_function(angle, cap=45):
    if angle < -cap:
        angle = -cap
    if angle > cap:
        angle = cap
    x = angle * 0.035
    return 0.5 * (x - math.atan(x))

def start_network_control():
    try:
        print(f"connecting to {PHONE_IP}:{PORT}...")
        with socket.create_connection((PHONE_IP, PORT)) as s:
            print("connected!")
            while True:
                data = s.recv(8192)
                json_text = data.decode(errors="replace")
                try:
                    json_obj = json.loads(json_text)
                    handle_orientation_update(json_obj)
                except json.JSONDecodeError:
                    print(f"invalid JSON \"{json_text}\"")
                if not data:
                    print("connection ended")
                    break
    except ConnectionRefusedError:
        print("connection refused")
    except OSError as e:
        print(f"socket error: {e}")

start_network_control()
```

We saved the program at `/jetbot/working_notebooks/basic_motion/termux-sensor-control.py` in the Jupyter
Docker container and ran it with `python3 termux-sensor-control.py`

## Termux

### Sensor API

In order to use the sensor API, Termux had to be downloaded from F-Droid and not from Google Play (the APK
release would probably work too). Additionally, the Termux:API package (available on F-Droid) was needed to
use the API. Lastly, to tap into Termux APIs the `termux-api` package was needed that can be installed with
`pkg i termux-api`. It provides the `termux-sensor` program that outputs sensor readings to standard out.

We ran the `termux-sensor` program with the following command: `termux-sensor -s Orientation\ sensor`.
Output frequency can be adjusted with the `-d <MILLISECONDS INTERVAL>` flag.

Importantly, when the `termux-sensor` program quits abruptly, such as when it is being piped into `nc` and
the socket is closed by the other side, it fails to clean up. This causes the next invocation of
`termux-sensor` to hang without reading any values. This can be resolved by starting `termux-sensor` and
terminating it with SIGINT (Ctrl+C) right away; it cleans up, and the next time it works correctly. To do
this automatically, it can be started with the `timeout` program that quits it automatically after 0.1s:
`timeout 0.1 termux-sensor -s Orientation\ sensor`.

The full command we used was:
`termux-sensor -s Orientation\ sensor | nc -vnl 1234; timeout 0.1 termux-sensor -s Orientation\ sensor`.
It sends the orientation data stream to a client connecting on port 1234 and cleanly quits `termux-sensor` after
the connection ends. To automatically restart it after the connection closes, it can also be put in a loop:
`while true; do termux-sensor -s Orientation\ sensor | nc -vnl 1234; timeout 0.1 termux-sensor -s Orientation\ sensor; done`.

### SSHD

As previously mentioned, we connected to the phone using SSH. It can be set up as follows.

#### Server package
Install with `pkg i openssh`.

#### Authentication
Password-based authentication is not an option (at least on unrooted Android) because there is no way
for an application to set its user password. To use key-based authentication, copy your public key
into `~/.ssh/authorized_keys`.

* If your desktop/laptop does not have an identity keypair (in `~/.ssh/id_*`), go into `~/.ssh` (if missing,
  `mkdir ~/.ssh` to create directory) and generate the keypair with `ssh-keygen -t ecdsa`. It will create
  `id_ecdsa` and `id_ecdsa.pub`.
* Copy the public key (`id_ecdsa.pub`) to the phone. The easiest way to do so I found is by running an HTTP
  server with one command on the desktop/laptop which will expose access to the files from the directory where
  you run it: `python3 -m http.server`. It defaults to port 8000. On the phone, run
  `curl -O http://<COMPUTER'S IP ADDRESS>:8000/id_ecdsa.pub`.
* Add the public key to the authorized keys file. It does not initially exist, so you can just rename the
  downloaded file: `mv id_ecdsa.pub authorized_keys`.

#### User
Android maintains a separate Linux user for each application, including Termux. To find out the username of
Termux, use the `whoami` command. It will look like `u0_a123`. When connecting, you have to specify the
username of Termux.

#### Running the server
`sshd` runs the server in the background. To run in foreground, you can use `sshd -D`. Termux's default SSHD
port is 8022, so you have to pass `-p 8022` when connecting on the command line (binding on ports below 1024
is impossible on unrooted Android).

#### Connecting
If your Termux user is `u0_a123`, your phone's IP address is 192.168.1.15, and your private key is stored at
`~/.ssh/id_ecdsa`, you can connect with this command:

`ssh -i ~/.ssh/id_ecdsa -p 8022 u0_a123@192.168.1.15`

You can simplify this by creating a config in `~/.ssh/config` with the following lines:
```config
Host myphone
    HostName 192.168.1.15
    User u0_a123
    Port 8022
    IdentityFile ~/.ssh/id_ecdsa
```
After you do so, you can connect using simply `ssh myphone`.

## Video Streaming

The video streaming setup was extremely straightforward: one program read data from the camera and sent it over
a network socket, another read data from that socket and played it.

### Robot side

The program used for reading the camera is `gst-launch-1.0`. We ran it with the following command:

`gst-launch-1.0 -v nvarguscamerasrc ! "video/x-raw(memory:NVMM), width=1280, height=720, format=NV12, framerate=30/1" ! nvv4l2h264enc maxperf-enable=1 ! h264parse ! filesink location=/dev/stdout | nc -vnl 1235`

The `| nc -vnl 1235` part reads standard in (written to by `gst-launch-1.0`) and sends it over a TCP socket to
the first client that connects to the robot on port 1235.

### Phone side

The application used for playing the video stream is mpv-android. We used the "Open URL" button on the main
screen and supplied the following URL: `tcp://127.0.0.1:1235`. As described above, we played video from localhost
(utilizing SSH port forwarding) because of the inability to connect to other devices from the phone directly. If
this was not an issue, we could have foregone SSH port forwarding and connected directly to the robot with
`tcp://192.168.1.5:1235`. Another workaround would be to use mpv's feature to play video from an incoming
connection, like so: `tcp://0.0.0.0:1235?listen=1`. This makes mpv listen on port 1235 (on all interfaces)
and play the received data; the part at the end of the `gst-launch-1.0` command would then need to look like so:
`| nc -v 192.168.1.15 1235` (where 192.168.1.15 is the phone's address).

#### Latency reduction

Because mpv buffers played data, the playback can be noticeably behind. The delay could be decreased by
increasing the playback rate until mpv catches up and then reducing it back to normal (by pressing the "1.00X"
button). From my experience, mpv falls behind again if it catches up too much and tries to play data that's
not there yet, so you have to be careful about resetting it back to 1.00 on time.

## Summary

The Python program inside Docker connected to the phone, where Termux had a `termux-sensor` command piping
orientation sensor readings into netcat, and controlled the wheels based on received values.

The `gst-launch-1.0` command on the robot was piping a video stream into netcat. mpv-android on the phone
connected to it and displayed the video.

## P.S.

Thanks so much to Jess, Quinn, and Ioan for such an awesome time! :D ❤️
