# mandelbrot
Just a little hobby project in fractal rendering.
Features a full mandelbrot set and julia set renderer with some nice graphics.
Rendered on the CPU. A long-term goal would be to move compute to the GPU.

## Installation
- Requires an installation of SDL3 on your include path, or provide your own and link in Makefile.
- Run `make` from project root to build the project, `make test` to build and instantly run. The flag `-j<n>` can be used to set the number of threads to use for the build, where `<n>` is the number of threads.

### Usage
- Run the executable.
- Window can be resized, toggle fullscreen with F11.
- Move around with WASD keys.
- Zoom in/out with up/down arrow keys.
- Toggle between mandelbrot and julia sets with J.
- Increase/decrease animation speed with right/left arrow keys.
- Zoom in and centre on click by left-clicking.
    - Resizing or zooming may result in needing to wait a moment until enough iterations are recalculated to be able to see anything.
- Keys 1-4 select a shading function. works instantly and doesn't need any recalculations.

#### Status
- Draws the mandelbrot set.
- Can be toggled to the correspending julia set by pressing a key.
  - Switching to the julia set after moving within a mandelbrot set will change the value of c.
  - Switching back to the mandelbrot set after moving within a julia set will change the initial value of z.
- Configurable shading with smooth colouring.
- Navigation with keyboard and mouse.
- A bit slow since it's rendered on CPU.

#### Maths
Here are the equations and paramaters for the mandelbrot and julia sets:
(x and y correspond to pixels' coordinates in the complex plane)
- `mandelbrot: z_n+1 = (z_n)^2 + c { z_0 = a + bi, c = x + yi } (escape when |z_n| > 2)`
- `julia:      z_n+1 = (z_n)^2 + c { z_0 = x + yi, c = a + bi } (escape when |z_n| > 2)`
- the center of the screen is initially -0.5 + 0i
- initially a + bi = 0 + 0i
- Switching to the julia set:
  - Sets `a` and `b` to the `x` and `y` of the center of the screen.
  - Sets the `x` and `y` of the center of the screen to the mandelbrot set's `a` and `b`.
- Switching to the mandelbrot set:
  - Sets `a` and `b` to the `x` and `y` of the center of the screen.
  - Sets the `x` and `y` of the center of the screen to the julia set's `a` and `b`.

##### Planned
- Performance optimisation:
    - Render with openGL shaders on the GPU instead of CPU pixel manipulation.
- Smooth user experience:
    - Initial low-quality passes for snappy navigation.
    - GUI for changing between colouring algorithms, zooming, etc.
