## DS(i)nput - Make the DS(i) be a wireless controller for Linux

This project makes use of a server running on Linux that receives buttons from
the Nintendo DS(i) via UDP and simulates it as a Xbox 360 Controller.

> **NOTE**: This project has only been tested on a Nintendo DS(i)

### Building

##### Building Server with meson (Linux only)

On Linux, the controller is simulated using uinput (requires kernel 2.6.20+).

```
$ cd server
$ meson setup build/ && ninja -j $(nproc) -C build/
```


#### Client (with devkitARM)

For the DS(i) client, it is required to have [devkitPro](https://devkitpro.org/)
and [dsiwifi](https://github.com/shinyquagsire23/dsiwifi/) installed and
properly configured.

```
$ cd client-nds
$ make
```

### Usage and Configuration

#### Server

By default, when started with not command-line arguments, the server binds
itself at 0.0.0.0:1032. For different bind address and port, use:

```
$ dsinput-server [address] [port]
```

#### Client

Copy the generate .nds file anywhere on your SD Card and launch it with either
Unlaunch or Twilight Menu++ (nds-bootstrap). For configuration there needs to be
a valid configuration file at `/data/settings/dsinput.conf`, a example is
provided at [client-nds/dsinput.conf](client-nds/dsinput.conf).

### Licensing

The project is licensed under the MIT License, found in [`LICENSE`](/LICENSE) at
the root of the repository.