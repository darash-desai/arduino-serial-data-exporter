# Arduino Serial Data Exporter

The Arduino Serial Data Exporter library was created in order to streamline data exported via USB serial. Manually printing via `Serial.print()` and `Serial.println()` can quickly bloat code, particularly when trying to make outputs on the Arduino Serial Monitor readable or when formatting and printing for the Arduino Serial Plotter.

The `arduino-serial-data-exporter` library creates an export buffer to which data can be added during each run loop. This data is collected in the buffer until it is finally exported. The library currently support exporting in two data formats:
- `exportJSON()` exports data as a flat JSON object. This can be readily parsed and visualized using the [`arduino-serial-data-viewer`](https://github.com/lyvewave/arduino-serial-data-viewer) web tool
- `exportToPlotter()` exports data in a format that is compatible with the Arduino IDE Serial Plotter

## Installation
To install this library, download a ZIP archive of this source by clicking on the green `Code` button at the top of this page and then on `Download ZIP`. Save this to a readily accessible location on your local computer.

In your Arduino IDE, use the menu bar to navigate to `Sketch` -> `Include Library` -> `Add .ZIP Library...`. In the dialog, navigate to and select the ZIP file you downloaded. That's it!

## How to use
To use the serial exporter, first add the following line at the top of your sketch:

`#include "SerialDataExporter.h"`

You can now create a `SerialDataExporter` object to handle exporting data. Here's a quick example:

```
int bufferSizes[] = {100, 3, 3};
SerialDataExporter exporter = SerialDataExporter(Serial, bufferSizes);
```

This creates an exporter that is capable of exporting a maximum of 3 `int` variables and 3 `double` variables using the Arduino Serial stream. <strong>Important:</strong> don't forget to initialize serial communication in your `setup()` function! The exporter above also uses a `char[]` buffer with a length of 100 to store labels for each variable. Variables and associated labels can the be added to the exporter as follows:

```
int intVariable = 10;
int doubleVariable = 20.5;

exporter.add("my_int_variable", intVariable);
exporter.add("my_double_variable", doubleVariable);
exporter.exportJSON();
```

These two lines add two different variables to the exporter and then export them in JSON format through the Serial stream. The first line adds `intVariable` to the exporter with an associated label of `my_int_variable`, which is 15 characters long. The second line adds `doubleVariable` with a label of `my_double_variable`, which is 18 characters long. To support this, our exporter must be initialized with a `char[]` buffer length of at least `(15 + 1) + (18 + 1) = 35`. The additional character for each label is added to account for the null terminating character `\0`. Since our exporter was initialized with a length of `100` in the code above, this is fine. If either the `char[]` buffer or individual variable buffers are full when attempting to add another variable, the variable will not be added.

The output from `exporter.exportJSON()` from the code snippet above would be:

 `{"my_int_variable":10,"my_double_variable":20.5}`

Currently, only `int` and `double` data types are supported for export. For more details regarding how to create a `SerialDataExporter` object, refer to the comments in the header file.

Here's a complete example for an Arduino sketch:

```
#include "Arduino.h"
#include "SerialDataExporter.h"

int bufferSizes[] = {100, 3, 3};
SerialDataExporter exporter = SerialDataExporter(Serial, bufferSizes);

void setup() {
  Serial.begin(9600);   // Initialize the Serial stream with a baud rate of 9600
}

int intVariable = 10;
double doubleVariable = 20.5;

void loop() {
  exporter.add("my_int_variable", intVariable);
  exporter.add("my_double_variable", doubleVariable);
  exporter.exportJSON();

  intVariable++;
  doubleVariable++;

  delay(500);
}
```

The code above exports `intVariable` and `doubleVariable` via Serial every 500 ms, incrementing both variables each time. Note that as soon as `exportJSON()` is called, all of the internal buffers for the exporter are cleared. The output from the code would be:

```
{"my_int_variable":10,"my_double_variable":20.5}
{"my_int_variable":11,"my_double_variable":21.5}
{"my_int_variable":12,"my_double_variable":22.5}
{"my_int_variable":13,"my_double_variable":23.5}
...
```
