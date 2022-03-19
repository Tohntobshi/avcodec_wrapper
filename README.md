# A tiny wrapper for avcodec library

- reduces usage of avcodec library to one method call to encode or decode video

- encodes BGR24 raw image data to MPEG-4 packet suitable for fransfering over network
and decodes it back

- can be used as is or as a reference how to use avcodec library


ffmpeg libraries should be installed systemwise, on Ubuntu it can be done this way:
```
sudo apt install ffmpeg
```

`example.cpp` shows how to use it in conjunction with opencv images (which by default are BGR24 encoded arrays of pixels), for `example.cpp` opencv should also be installed, for Ubuntu it can be done like this:

```
sudo apt install libopencv-dev
```
