# Animatable

An easy-to-use animation library for single values.

### Initialize an animatable value starting from 0.0

```c++
Animatable<float> alpha = 0.0f;
```

### Start an animation.

```c++
alpha.WithAnimation(EaseInOutSine(), 1.0f);
```

### Update an animation

An animation must be updated every frame by calling Update with the given delta time.

```c++
alpha.Update(elapsedTimeSinceLastFrame);
```

------

## Predefined function curves

There are many different curves functions defined in order to get the required feel for the animation.

| Curve name  | Ease-In       | Ease-Out       | Ease-InOut       |
| :---------- | :------------ | :------------- | :--------------- |
| Sine        | EaseInSine    | EaseOutSine    | EaseInOutSine    |
| Cubic       | EaseInCubic   | EaseOutCubic   | EaseInOutCubic   |
| Quint       | EaseInQuint   | EaseOutQuint   | EaseInOutQuint   |
| Circ        | EaseInCirc    | EaseOutCirc    | EaseInOutCirc    |
| Elastic     | EaseInElastic | EaseOutElastic | EaseInOutElastic |
| Quart       | EaseInQuart   | EaseOutQuart   | EaseInOutQuart   |
| Expo        | EaseInExpo    | EaseOutExpo    | EaseInOutExpo    |
| Back        | EaseInBack    | EaseOutBack    | EaseInOutBack    |
| Bounce      | EaseInBounce  | EaseOutBounce  | EaseInOutBounce  |
| Linear      | EaseLinear    | EaseLinear     | EaseLinear       |
| Instant     | Instant       | Instant        | Instant          |
| Never       | Never         | Never          | Never            |
| TimingCurve | TimingCurve   | TimingCurve    | TimingCurve      |



## Using these functions

The function can be modified by passing in a duration in seconds (in this case 3 seconds).

```c++
alpha.WithAnimation(EaseInQuart(3.0f), 1.0f);
```

It is also possible to modify the functions by chaining decorators at the end.

```c++
// Repeating
alpha.WithAnimation(
    EaseInQuart(3.0f)
    	.Speed(2.0f) // Plays the animation twice as fast.
    	.Delay(0.25f) // Starts 0.25 seconds delayed.
    	.RepeatFor(3), // Repeat the animation 3 times
    1.0f
);

// Repeating & Reversing
alpha.WithAnimation(
    EaseInQuart(3.0f)
    	.Speed(2.0f) // Plays the animation twice as fast.
    	.Delay(0.25f) // Starts 0.25 seconds delayed.
    	.RepeatFor(
            3,
            [] { return true; }
        ),
    1.0f
);
```

If the animation should be skipped there are two ways to do that.

```c++
// The animation will be done instantly after calling Update()
alpha.WithAnimation(Instant(), 1.0f);

// There is no need to call Update() after this one.
alpha.WithoutAnimation(1.0f);
```

### Timing Curves

```c++
alpha.WithAnimation(
	TimingCurve(
        3.0f,
        std::vector{ 0.25f, 0.5f, 0.25f, 1.f },
        std::vector{ &ui::ease::InOutBounce, &ui::ease::InOutBack, &ui::ease::Linear }
    ),
    1.0f
);
```



## Using the Value

```c++
NoStroke(); // No border
Fill(255, 0, 0, alpha * 255); // Red tint
Rect(100, 100, 250, 150); // Draw the rectangle
```



------

MIT License

Copyright (c) 2023 FelixCpp

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
