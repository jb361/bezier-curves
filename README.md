# Bézier Curves
This application was submitted as coursework in the final year of my undergraduate degree. A piecewise (composite) Bézier curve is constructed from a variable number of cubic Bézier curves, each adjoined with C1 continuity and rendered as polylines by taking forward differences. One-off point and tangent computations evaluate the cubic curve equation in monomial form using Horner’s scheme (with precomputed coefficients).

You can manipulate each curve by dragging control points with the mouse or add a new curve to the spline (using de Casteljau’s algorithm to split the curve with the greatest arc length, whilst maintaining visual tangent continuity). Objects locked to the spline take into account transfer of kinetic to potential energy as they travel – i.e. so that an object’s velocity decreases as it climbs an ascent. Three camera types are implemented: a free camera for editing the track, a 1st person camera with a view from a ship’s cockpit in the direction of the tangent to the curve and a 3rd person camera that tracks a ship using Newton and Hooke's laws.

## Screenshot
![Screenshot](https://user-images.githubusercontent.com/1145329/216070230-0155e4e4-a791-4d46-98d4-b00d29d51094.png)

## System Requirements
A PC with Windows XP or above and OpenGL 1.3 support.

## Controls
In any camera mode:
  - Space bar to toggle camera mode (Free, 1st Person, 3rd Person)
  - Esc or 'Q' to quit the application

In free camera mode (default):
  - Left click and drag to move a control point
  - Right click and drag to rotate the camera view about the y-axis
  - 'C' to add a curve to the track; 'E' or 'R' to add or remove a ship
  - Up/down arrows to increment/decrement the curve resolution

## Credits
Ship model by [Psionic3D](https://psionic3d.newgrounds.com/).
