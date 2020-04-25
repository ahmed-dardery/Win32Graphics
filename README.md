# Win32Graphics
This holds the experiments we do in Graphics Course, this will also contain some proofs for the algorithms

The derivations of the formulas are found in #Material directory, the rest of the codebase contains Win32 API calls organizations.
Especially the Win32 class, which is designed as a "copy and reuse" for commonly used calls (Color choice dialog, and double buffer hdc, etc.)

The MenuHandler requires more work to copy and reuse than I would like, but for now it can be used to quickly set up a menu with check options or check group options.

## Supported Shapes
Currently, the algorithms for drawing a line, a circle, and a bezier curve are supported. A naive implementation of circle filling is also implemented.

![Screenshot of the shapes the app supports](/#Images/shapes.png)