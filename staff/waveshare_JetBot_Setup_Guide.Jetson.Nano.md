# Waveshare JetBot Setup Guide (Jetson Nano)

This guide walks through setting up a Jetson Nano (eMMC version) with the Waveshare JetBot kit, including storage expansion, headless VNC access, CUDA configuration, and Docker deployment.

## Prerequisites
* **Hardware:** Jetson Nano, Waveshare JetBot Kit (Chassis, Expansion Board, Camera, Wi-Fi Card), MicroSD Card (32GB+ recommended), 12V Power Supply.
* **Peripherals:** Monitor (HDMI/DP), Keyboard, Mouse, Ethernet Cable.
* **OS:** JetPack 4.6.x (pre-flashed on eMMC).

---

## 1. Initial System Setup
1.  **Connect Peripherals:** Attach the keyboard, monitor, mouse, and ethernet cable to the Nano.
2.  **Boot & Login:**
    * **Username:** `nvidia`
    * **Password:** `nvidia`
3.  **Set Hostname:**
    Open a terminal and set a unique hostname (replace `XX` with your designated number):
    ```bash
    sudo nano /etc/hostname
    ```
    *Change the text to `pccjetXX`, save (`Ctrl+O`, `Enter`), and exit (`Ctrl+X`).*

---

## 2. Storage Expansion (SD Card Setup)
Since the eMMC storage is limited, we will mount the SD card to `/data` for Docker and project files.

1.  **Mount Verification:**
    Insert your formatted SD card. Verify it is recognized:
    ```bash
    df -h
    ```
    *Look for `/dev/mmcblk1p1`. If absent, ensure the Device Tree is updated to enable the SD slot.*

2.  **Get UUID:**
    Run the following command and copy the **UUID string** of the SD card to your clipboard:
    ```bash
    blkid
    ```

3.  **Prepare Directory & fstab:**
    ```bash
    sudo umount /dev/mmcblk1p1
    sudo mkdir -p /data
    sudo nano /etc/fstab
    ```

4.  **Configure Auto-Mount:**
    Append the following line to the end of `/etc/fstab` (Replace `<string>` with your actual UUID):
    ```text
    UUID=<string>   /data   ext4    defaults,nofail     0   2
    ```

5.  **Verify Mount:**
    ```bash
    sudo mount -a
    df -h
    ```
    *Ensure the SD card is now mounted on `/data`.*

---

## 3. Hardware Assembly & Network (including Camera)
1.  **Shutdown:**
    ```bash
    sudo poweroff
    ```
2.  **Install Hardware:**
    * Disconnect power.
    * Install the **Wi-Fi adapter** in the M.2 slot (under the module).
    * Connect the **Camera** to the CSI port (ensure **silver pins face the heatsink**).
3.  **Boot & Connect:**
    * Power on the Jetson.
    * Use the desktop interface to configure and connect to Wi-Fi.

---

## 4. Headless Configuration (VNC)
Set up the `vino` VNC server and configure the display to run without a monitor attached.

1.  **Install & Enable VNC (Vino):**
    Run the following commands to install the server and ensure it starts on login:
    ```bash
    sudo apt update
    sudo apt install vino
    mkdir -p ~/.config/autostart
    cp /usr/share/applications/vino-server.desktop ~/.config/autostart
    ```

2.  **Configure VNC Settings:**
    Disable prompts and encryption (to allow standard VNC clients to connect) and set the password to `nvidia`:
    ```bash
    gsettings set org.gnome.Vino prompt-enabled false
    gsettings set org.gnome.Vino require-encryption false
    gsettings set org.gnome.Vino authentication-methods "['vnc']"
    gsettings set org.gnome.Vino vnc-password $(echo -n 'nvidia'|base64)
    ```

3.  **Enable Automatic Login:**
    *The VNC server only starts after a user logs in.*
    * Go to **System Settings** -> **User Accounts**.
    * Unlock (top right) and toggle **Automatic Login** to **ON**.

4.  **Configure Dummy Screen (Headless Mode):**
    To use VNC without a monitor connected, you must force a screen resolution in X11.
    
    Edit the configuration file:
    ```bash
    sudo nano /etc/X11/xorg.conf
    ```
    
    Append the following block to the end of the file:
    ```text
    Section "Screen"
       Identifier    "Default Screen"
       Monitor       "Configured Monitor"
       Device        "Tegra0"
       SubSection "Display"
           Depth    24
           Virtual 1280 1024 # Modify the resolution by editing these values
       EndSubSection
    EndSection
    ```

5.  **Apply Changes:**
    ```bash
    sudo reboot
    ```
    *After rebooting, you can connect from your PC using a VNC client (like Remmina) to the Jetson's IP address.*

---

## 5. CUDA & GPU Configuration
Set up the environment variables to access NVIDIA libraries.

1.  **Update Bash Profile:**
    ```bash
    echo 'export PATH=/usr/local/cuda-10.2/bin:$PATH' >> ~/.bashrc
    echo 'export LD_LIBRARY_PATH=/usr/local/cuda-10.2/lib64:$LD_LIBRARY_PATH' >> ~/.bashrc
    source ~/.bashrc
    ```

2.  **Verify Installation:**
    ```bash
    nvcc --version
    sudo tegrastats
    ```
    *(Check for CPU/GPU voltages and temperatures).*

3.  **Run CUDA Sample:**
    Compile and run the device query sample to confirm GPU accessibility.
    ```bash
    cd /usr/local/cuda-10.2/samples/1_Utilities/deviceQuery
    sudo make
    ./deviceQuery
    ```

4.  **Install Jetson Stats (jtop):**
    ```bash
    sudo apt update
    sudo apt install python3-pip cmake
    sudo pip3 install jetson-stats
    sudo jtop
    ```
    *(Press `q` to exit jtop).*

5.  **Test Camera:**
    Verify the camera stream on your monitor or VNC:
    ```bash
    gst-launch-1.0 nvarguscamerasrc ! nvegltransform ! nveglglessink
    ```

---

## 6. Docker Configuration
We must move Docker data to the SD card and prevent race conditions on boot.

1.  **Prepare Project Directory:**
    ```bash
    sudo mkdir -p /data/projects
    cd /data/projects
    ```

2.  **Relocate Docker Data:**
    ```bash
    sudo systemctl stop docker
    sudo mkdir -p /data/docker-data
    sudo rsync -axPS /var/lib/docker/ /data/docker-data
    ```

3.  **Configure Docker Daemon:**
    Edit the config file:
    ```bash
    sudo nano /etc/docker/daemon.json
    ```
    Ensure the file looks like this (pay attention to the comma `,` after the runtimes block):
    ```json
    {
        "runtimes": {
            "nvidia": {
                "path": "nvidia-container-runtime",
                "runtimeArgs": []
            }
        },
        "data-root": "/data/docker-data",
        "default-runtime": "nvidia"
    }
    ```

4.  **Fix Boot Race Condition:**
    Ensure systemd waits for `/data` to mount before starting Docker.
    ```bash
    sudo mkdir -p /etc/systemd/system/docker.service.d
    sudo nano /etc/systemd/system/docker.service.d/mount-dependency.conf
    ```
    Paste the following:
    ```ini
    [Unit]
    Requires=data.mount
    After=data.mount
    ```

5.  **Apply Changes:**
    ```bash
    sudo systemctl daemon-reload
    sudo systemctl start docker
    ```

---

## 7. Install JetBot Software
1.  **Clone Repository:**
    ```bash
    cd /data/projects
    sudo git clone https://github.com/waveshare/jetbot.git
    sudo chown -R nvidia:nvidia /data/projects/jetbot
    ```

2.  **Select Branch:**
    Check your branch and switch to `jetbot_0.4.2` if necessary:
    ```bash
    cd jetbot
    git branch
    # If on master:
    git checkout jetbot_0.4.2
    ```

---

## 8. Launch & Run
1.  **Start the JetBot Container:**
    Run this command to download and start the environment. It sets the container to restart automatically on boot.
    ```bash
    sudo docker run --runtime nvidia -it -d \
      --privileged \
      --network host \
      --name jetbot_jupyter \
      --restart always \
      -v /data/projects:/workspace \
      -v /tmp/argus_socket:/tmp/argus_socket \
      -v /etc/enctune.conf:/etc/enctune.conf \
      -v /etc/timezone:/etc/timezone:ro \
      -v /etc/localtime:/etc/localtime:ro \
      --device /dev/i2c-1 \
      --device /dev/i2c-0 \
      --device /dev/video0 \
      jetbot/jetbot:jupyter-0.4.3-32.5.0 \
      jupyter lab --ip=0.0.0.0 --allow-root --NotebookApp.token=jetbot
    ```

2.  **Verify Access:**
    Open a browser and navigate to: `http://pccjetXX:8888`.
    You should see the Jupyter Lab interface.

---

## 9. Mobile Operation
1.  **Shutdown Desktop Mode:**
    ```bash
    sudo poweroff
    ```
2.  **Switch Power:**
    * Remove the **5V Barrel Jack**.
    * Plug the **12V Waveshare Power Supply** into the expansion board to charge the batteries.
    * **Note:** Wait for the indicator light to turn Green (fully charged).
3.  **Boot on Battery:**
    * Remove the power supply.
    * Flip the **Slider Switch** on the Waveshare board to **ON**.
4.  **Connect & Test:**
    * The Jetson will boot and connect to Wi-Fi automatically.
    * On your PC, go to `http://pccjetXX:8888`. Use password jetbot
    * Open the **Basic Motion** notebook and run the cells to test motor movement.
 
 ---
 
 ## 10. OLED Display Setup (Headless Mode)

This JetBot is equipped with a 0.91" OLED display (SSD1306) at I2C address `0x3c`. We use a custom service to cycle the **IP Address** and **Hostname** on the screen at boot, making it easy to connect without a monitor.

### 1. Install System Dependencies
The `Pillow` library requires several system-level image libraries to compile correctly on the Jetson.

```bash
sudo apt-get update
sudo apt-get install libjpeg8-dev zlib1g-dev libfreetype6-dev liblcms2-dev libwebp-dev ttf-dejavu -y
```

### 2. Install Python Libraries
Install the driver for the display and the image manipulation library.

```bash
sudo pip3 install Adafruit-SSD1306 Pillow
```

### 3. Install the Display Script
1.  Download the `display_ip.py` script from the project repository:
    * **Download Link:** [Google Drive: JetBot Scripts](https://drive.google.com/drive/folders/1OhGdTPZ-96nvbjz_5eoPSsCIAn7K_7fi)
2.  Place the file in your scripts directory:
    * **Path:** `/data/projects/jetbot/scripts/display_ip.py`

### 4. Configure Auto-Start (Systemd)
To ensure the display starts automatically on boot and restarts if it crashes, we use a systemd service.

1.  Create the service file:
    ```bash
    sudo nano /etc/systemd/system/display-ip.service
    ```

2.  Paste the following configuration (ensure the `ExecStart` path matches your file location):
    ```ini
    [Unit]
    Description=OLED Display IP Service
    After=multi-user.target

    [Service]
    Type=simple
    # Ensure this points to where you saved the script
    ExecStart=/usr/bin/python3 /data/projects/jetbot/scripts/display_ip.py
    User=nvidia
    Restart=always
    RestartSec=5

    [Install]
    WantedBy=multi-user.target
    ```

3.  Enable and start the service:
    ```bash
    sudo systemctl daemon-reload
    sudo systemctl enable display-ip.service
    sudo systemctl start display-ip.service
    ```

### Verification
Reboot the Jetson. After approximately 30 seconds, the OLED screen should light up and begin cycling between the Hostname and the IP Address.