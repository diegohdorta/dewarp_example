# Dewarp Overview

Dewarping refers to the process of correcting an image, to reverse the effects
of geometric distortions caused by the camera lens found in fisheye or 360-Degree.

## Simple Dewarping Example on i.MX 8M Plus

1. Build, install and export the Yocto toolchain by reading the [Yocto User Guide][yocto_doc].
```console
$ source /opt/fsl-imx-internal-xwayland/<version>/environment-setup-aarch64-poky-linux 
```

2. Clone this repo and compile the example:
```console
$ git clone https://github.com/diegohdorta/dewarp_example && cd dewarp_example
$ make
```
3. Copy the media folder to the board:
```console
$ scp -r media/ root@<ip_adress>:/home/root
```
4. Run the application in the board:
```console
# cd /home/root/media
# ./run
```

Output:
<p align="center">
  <img src="https://raw.githubusercontent.com/diegohdorta/dewarp_example/master/media/example.gif" height="191" width="176">
</p>

[yocto_doc]: https://www.nxp.com/docs/en/user-guide/IMX_YOCTO_PROJECT_USERS_GUIDE.pdf
