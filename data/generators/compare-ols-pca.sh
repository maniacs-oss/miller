#!/bin/sh

for p in \
  -0.975 -0.950 -0.925 -0.900 \
  -0.875 -0.850 -0.825 -0.800 \
  -0.775 -0.750 -0.725 -0.700 \
  -0.675 -0.650 -0.625 -0.600 \
  -0.575 -0.550 -0.525 -0.500 \
  -0.475 -0.450 -0.425 -0.400 \
  -0.375 -0.350 -0.325 -0.300 \
  -0.275 -0.250 -0.225 -0.200 \
  -0.175 -0.150 -0.125 -0.100 \
  -0.075 -0.050 -0.025 -0.000 \
   0.000  0.025  0.050  0.075 \
   0.100  0.125  0.150  0.175 \
   0.200  0.225  0.250  0.275 \
   0.300  0.325  0.350  0.375 \
   0.400  0.425  0.450  0.475 \
   0.500  0.525  0.550  0.575 \
   0.600  0.625  0.650  0.675 \
   0.700  0.725  0.750  0.775 \
   0.800  0.825  0.850  0.875 \
   0.900  0.925  0.950  0.975
do
  for try in 1 2 3 4 5; do
    #echo
    #echo p $p
    #a.out $p|mlr --oxtab stats2 -a linreg-ols,linreg-pca -f x,y
    echo "p=$p,`a.out $p | mlr stats2 -a linreg-ols,r2,linreg-pca -f x,y`"
  done
done
