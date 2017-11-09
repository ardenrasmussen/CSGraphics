# Wireframe

## Display

$(x,y,z)$->$(x_1,y_1)$
$x_1 = \\frac{x}{z}$
$y_1 = \\frac{y}{z}$
$(x_1,y_1)$->$(x_2,y_2)$
$x_2 = \\frac{C_x}{\\tan{half}}\\cdot\\frac{x}{z}$
$y_2 = \\frac{C_y}{\\tan{half}}\\cdot\\frac{y}{z}$
Display at $(x_2,y_2)$

## Matrix

Matrix is $4x4$.

### Translate

[1, 0, 0, x][0, 1, 0, y]
[0, 0, 1, z][0, 0, 0, 1]

### Scale

[x, 0, 0, 1][0, y, 0, 1]
[0, 0, z, 1][0, 0, 0, 1]

### Rotate

#### X

[1, 0, 0, 0]
[0, c, -s, 0]
[0, s, c, 0]
[0, 0, 0, 1]

#### Y

[c, 0, s, 0]
[0, 1, 0, 0]
[-s, 0, c, 0]
[0, 0, 0, 1]

#### Z

[c, -s, 0, 0]
[s, c, 0, 0]
[0, 0, 1, 0]
[0, 0, 0, 1]

[a b c]   [x1 x2 x3 ...]
[d e f] * [y1 y2 y3 ...]
[h i j]   [1  1  1  ...]

x1 = a*x1 + b*y1 + c
y1 = d*x1 + e*y2 + f



# Light #

$test$

$[0,1] <- intensity = ambient + diffuse + specular$
ambient = general background radiation ($0.2$)
diffuse = Interaction between the light and the polys ($0.5*\{\vec{n}\cdot\vec{l} or 0 if \vec{n}\cdot\vec{l} < 0\}$)
specula = Where is the eye ($(1-0.2-0.5)*\{\vec{e}\cdot\vec{r} or 0 if \vec{e}\cdot\vec{r} < 0\}$)

