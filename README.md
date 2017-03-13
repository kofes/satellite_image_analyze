# Satellite lib

The **Satellite lib** is a C++ library created to manage earth-images from satellites and analize it.

## Math

**Math** - module with the implementation of statistical methods, aimed at analyzing images and identifying characteristic homogeneous areas.

#### Most useful functions

##### getPixelsInLine ( X0, Y0, X1, Y1 )

Returns: ``std::queue<double>``

Computes the Cartesian coordinates of the pixels intersected by the line *(X0, Y0)*-*(X1, Y1)* (The Brezenham algorithm)

##### getPixelsInCircle ( X, Y, R )

Returns: ``std::queue<double>``

Computes the Cartesian coordinates of the pixels intersected by the circle with the center at the point *(X, Y)* and the radius *R* (The Brezenham algorithm)

##### d ( X0, Y0, X1, Y1, H, Image )

Returns: ``double``

Computes the "drift" function of the *Image*'s fragment located between points *(X0, Y0)* and *(X1, Y1)* with "log" *H*

##### cov ( X0, Y0, X1, Y1, H, Image )

Returns: ``double``

Computes the "covariance" function of the *Image*'s fragment located between points *(X0, Y0)* and *(X1, Y1)* with "log" *H*

##### r ( X0, Y0, X1, Y1, H, Image )

Returns: ``double``

Computes the "correlation" function of the *Image*'s fragment located between points *(X0, Y0)* and *(X1, Y1)* with "log" *H*

##### r ( X0, Y0, X1, Y1, H, Image )

Returns: ``double``

Computes the "correlation" function of the *Image*'s fragment located between points *(X0, Y0)* and *(X1, Y1)* with "log" *H*

##### g ( X0, Y0, X1, Y1, H, Image )

Returns: ``double``

Computes the "semivariance" function of the *Image*'s fragment located between points *(X0, Y0)* and *(X1, Y1)* with "log" *H*

##### leastSquares ( Degree, X, Y, {Diff = 1e-2} )

Returns: ``std::vector<double>``

Computes the coefficients of the polynomial function with a degree of *Degree* using the least squares method (for solving the resulting system of linear algebraic equations used Gauss-Seidel method with the error *Diff*)

#### Class Pack

Class **Pack** is class-container, that contain the values of "drift", "covariance" and "semivariance" and method ``calc``:

##### calc ( X0, Y0, X1, Y1, H, Image )

Returns: ``Pack``

Computes the values of "drift", "covariance" and "semivariance" in parralel.

## Image

**Image** - module with the implementation of class-container *Image* with some manipulation methods.

#### Most useful methods

##### width ()

Returns: ``unsigned short``

Get width of *Image*

##### height ()

Returns: ``unsigned short``

Get height of *Image*

##### operator[] ( Index )

Returns: ``const unsigned short*``

Get line with index *Index* from *Image*

##### changeMaxMin ( MinColor, MaxColor )

Returns: ``void``

Set max and min color of *Image*

##### binary ( Border )

Returns: ``void``

Binarizes the *Image*: sets all pixels less than *Border* to 0; unless to 255

##### friend operator< ( ``std::ofstream&`` Output, Image )

Returns: ``std::ofstream&``

Write *Image* to output

## Passport

**Image** - module with the implementation of container classes *passport::Default* and *passport::Proection* used to read and present the passport data of satellite-format images (http://www.satellite.dvo.ru/contentid-14.html)
