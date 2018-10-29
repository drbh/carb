# carb


Welcome to C-ARB!

This app is a simple C++ program, two Python I/O scripts and a Redis backbone.

It is the fastest way to take ina any stream of assets with associted convsersion rates between them. Most likely these are currency pairs, or some other asset that holds trasferable value.

Example

Our Assets
```
A: Goat
B: Chicken
C: Giraffe

```

The Conversions Rates
```
1 Goats 	= 2 Chickens
3 Chicken 	= 1 Giraffe
2 Goats 	= 1 Giraffe
```

So now, if I start with Goats (assume there are infinate of each asset to trade with). What is my best series of trades where I end with the same asset I started with but have more!?

Its only one of there options.

```
Hold - Goats
ABCA - Goats for Chickens for Giraffes for Goats
ACBA - Goats fot Giraffes for Chickens for Goats
```

In this case we cane easily deduce that we should go `ABCA - Goats for Chickens for Giraffes for Goats`

Heres the math

```
// A -> B -> C 
// A -> C
(A/C) / ( (A/B) * (B/C) )

// A -> C -> B 
// A -> B
(A/B) / ( (A/C) * (1/(B/C)) )
```

Path 1
```
// A -> B -> C 
2  = (0.5) / ( 0.5 * 0.5 )
```

Path 2
```
// A -> C -> B
.5 = 0.5 / ( 0.5 * 2 )
```

As we can see from the two outputs, we should go ABCA which is `Goats for Chickens for Giraffes for Goats` this way is twice as profitable as going the other way. We prove this by showing that A->B is 2 times bigger than A->C->B, this suggest that we should go the other way.

