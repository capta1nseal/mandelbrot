# mandelbrot
Just a little hobby project in fractal rendering.
Rendered on the CPU. A long-term goal would be to move compute to the GPU.

## Installation
- Requires an installation of SDL3 on your include path, or provide your own and link in Makefile.
- Run `make` from project root to build the project, `make test` to build and instantly run. The flag `-j<n>` can be used to set the number of threads to use for the build, where `<n>` is the number of threads.

### Usage
- Run the executable.
- Window can be resized, toggle fullscreen with F11.
- Move around with WASD keys.
- Zoom in/out with up/down arrow keys.
- Increase/decrease animation speed with right/left arrow keys.
- Zoom in and centre on click by left-clicking.
    - Resizing or zooming may result in needing to wait a moment until enough iterations are recalculated to be able to see anything.
- Keys 1-4 select a shading function. works instantly and doesn't need any recalculations.

#### Status
- Draws mandelbrot set.
- Configurable shading with smooth colouring.
- Navigation with keyboard and mouse.
- A bit slow since it's rendered on CPU.

##### Planned
- Performance optimisation:
    - Render with openGL shaders on the GPU instead of CPU pixel manipulation.
- Smooth user experience:
    - Initial low-quality passes for snappy navigation.
    - GUI for changing between colouring algorithms, zooming, etc.
