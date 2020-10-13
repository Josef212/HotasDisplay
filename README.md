# HotasDisplay

This is a simple attempt to display info on the Logitech G X52 Pro HOTAS display. The basic idea is to offer a simple system with two different parts:
- DirectOutputWrapper: Is a class to deal with the display.
- IDisplayDataProvider: Is a basic interface that provides the info to display. An implementation must be provided to the DirectOutputWrapper that will request data to it.

The project is still WIP and provide a basic implementation that needs a lot of improvement. 

By Josef21296 