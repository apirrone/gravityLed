# gravityLed

![There is supposed to be a cool gif here](https://media.giphy.com/media/8AdqYCYtrNTIgP7AE1/200w_d.gif)

Tiny "physics engine" for displaying balls bouncing around walls and each others on a led matrix (or other use).

It's very simple to use : 

```c++
#include "gravityLed.hpp"

gravityLed g;
[...]
while(true){
	[...]
	g.step();
	bool** occupancyMatrix = g.getMatrix();
	// do stuff with matrix
	[...]
}
```
## Make the demo: 

	$ make

### Demo Dependencies :

- ncurses (but just if you want to display the matrix in a terminal)
