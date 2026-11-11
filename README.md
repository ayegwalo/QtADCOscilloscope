# Qt ADC Oscilloscope

A four-channel oscilloscope simulation built with **C++**, **Qt 6**, and
**CMake**. The project demonstrates a modular ADC software design,
configurable signal generation, digital quantisation, and real-time
waveform display.

> **Project status:** This version uses a simulated ADC. It does not yet
> acquire data from a physical ADC device.

## Features

-   Four independently configurable channels.
-   Selectable signal types:
    -   Null (zero signal)
    -   Sine
    -   Square
    -   Sawtooth
-   Configurable ADC resolution (8--16 bits).
-   Configurable sampling interval in microseconds.
-   Simulated analogue-to-digital conversion.
-   Waveform display with a grid and separate channel colours.
-   Start and Stop controls.
-   ADC interface separated from the GUI.

## Architecture

The application is organised into three main parts:

``` text
QtADCOscilloscope/
├── ADC/
│   ├── CADCInterface.h
│   ├── CADCPlugin.h
│   └── CADCPlugin.cpp
├── Display/
│   ├── CWaveformWidget.h
│   └── CWaveformWidget.cpp
├── main.cpp
├── mainwindow.h
├── mainwindow.cpp
├── mainwindow.ui
├── CMakeLists.txt
└── .gitignore
```

### Main components

-   **MainWindow** --- reads the user controls, configures the ADC
    component, requests samples, and passes them to the display.
-   **CADCInterface** --- defines the operations that an ADC
    implementation must provide.
-   **CADCPlugin** --- implements the interface and simulates four ADC
    channels.
-   **CWaveformWidget** --- paints the channel waveforms and
    oscilloscope grid.

The current `CADCPlugin` is a C++ class instance linked into the
application. It is not yet a dynamically loaded Qt plugin.

## Requirements

-   C++ compiler with C++17 support
-   Qt 6.5 or later
-   CMake 3.19 or later
-   Qt Creator (recommended, optional)

## Build

### Using Qt Creator

1.  Open the project's `CMakeLists.txt` in Qt Creator.
2.  Select a Qt 6 kit for your operating system.
3.  Configure the project.
4.  Build the project.
5.  Run `QtADCOscilloscope`.

### Using the command line

From the project root:

``` bash
cmake -S . -B build
cmake --build build
```

Run the generated executable from the build directory appropriate to
your platform and generator.

## Using the application

1.  Choose a signal type for each channel.
2.  Set the ADC resolution between 8 and 16 bits.
3.  Set the sampling interval in microseconds.
4.  Press **Start** to display the generated samples.
5.  Press **Stop** to pause waveform updates.

Each channel has its own signal type, resolution, and sampling interval.

## How the simulation works

The plugin generates a signal for each channel, samples it according to
the configured interval, and converts each sample into an integer ADC
code.

For an unsigned (N)-bit ADC, the simulated code range is:

\[ 0 `\leq `{=tex}D `\leq 2`{=tex}\^N - 1 \]

The GUI maps the digital codes back to a normalised display range for
plotting. This is a software simulation, not a measurement of a physical
input voltage.

## Current limitations

-   No physical ADC hardware is connected.
-   Signal generation is simulated in software.
-   The current implementation uses a timer-driven GUI update loop.
-   Hardware-specific acquisition, buffering, and synchronisation have
    not yet been implemented.
-   The current ADC class is statically linked rather than dynamically
    loaded as a Qt plugin.

## Planned improvements

-   Connect a physical ADC device.
-   Add a hardware-specific acquisition implementation behind
    `CADCInterface`.
-   Improve sampling synchronisation and buffering.
-   Add voltage scaling and configurable input ranges.
-   Add trigger controls and time-base settings.
-   Add data export and measurement tools.
-   Consider dynamic plugin loading if required by the target
    architecture.

## Contributing

Suggestions, bug reports, and improvements are welcome. Please open an
issue to discuss substantial changes before submitting a pull request.

## License

No license has been selected yet. Unless a license is added, the default
copyright restrictions apply.
