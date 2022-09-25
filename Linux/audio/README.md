# README

## Overview

Ubuntu uses both `ALSA`, and `PulseAudio` for controlling sound input and output.

`ALSA` tools can be installed by `alsa-utils` which provides the following tools:

- `/usr/bin/aconnect`
- `/usr/bin/alsabat`
- `/usr/bin/alsaloop`
- `/usr/bin/alsamixer`
- `/usr/bin/alsatplg`
- `/usr/bin/alsaucm`
- `/usr/bin/amidi`
- `/usr/bin/amixer`
- `/usr/bin/aplay`
- `/usr/bin/aplaymidi`
- `/usr/bin/arecordmidi`
- `/usr/bin/aseqdump`
- `/usr/bin/aseqnet`
- `/usr/bin/iecset`
- `/usr/bin/speaker-test`
- `/usr/sbin/alsa-info`
- `/usr/sbin/alsabat-test`
- `/usr/sbin/alsactl`

`PulseAudio` tools can be installed by `pulseaudio-utils` which provides the following tools:

- `/usr/bin/pacat`
- `/usr/bin/pacmd`
- `/usr/bin/pactl`
- `/usr/bin/padsp`
- `/usr/bin/pasuspender`
- `/usr/bin/pax11publish`

## How to mute/unmute the speaker using `amixer`

To mute the speaker, one only needs to mute `Master`: `amixer set Master mute`.

However, when `Master` is muted, the associated simple controls are also muted. For example, on my computer, I have a `Speaker`. When I mute `Master`, `Speaker` is muted at the same time.

Unfortunately, I haven't figured out how to find out the relationship between `Master` and the related simple controls (such as `Speaker` in this example). But one method is to use `meld` or `diff` to compare the control contents before and after the change:

- `amixer > ./before.txt`
- Change the settings.
- `amixer > ./after.txt`
- `meld ./before.txt ./after.txt`

Muting the `Master` can mute the associated simple controls simultaneously; however, unmuting the `Master` is not enough: one must unmute all the associated simple controls, too.

## References

- [1] [Advanced Linux Sound Architecture (ALSA) project homepage](https://www.alsa-project.org/main/index.php/Main_Page)
- [2] [PulseAudio](https://www.freedesktop.org/wiki/Software/PulseAudio/)
