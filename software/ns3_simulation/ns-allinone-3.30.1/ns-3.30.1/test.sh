i=2
sum=0
s="+1.000000000+1.000000000+1.000000000+1.001927036+1.010000000+1.011476012+1.020000000+1.021476012+1.030000000+1.031476012+1.040000000+1.041476012+1.050000000+1.051476012+1.060000000+1.061476012+1.070000000+1.071476012+1.080000000+1.080992012+1.090000000+1.090992012+1.100000000+1.100992012+1.110000000+1.110992012+1.120000000+1.120992012+1.130000000+1.130992012+1.140000000+1.140992012+1.150000000+1.150992012+1.160000000+1.160992012+1.170000000+1.170992012+1.180000000+1.180748012+1.190000000+1.190748012+1.200000000+1.200748012+1.210000000+1.210748012+1.220000000+1.220748012+1.230000000+1.230748012+1.240000000+1.240748012+1.250000000+1.250748013+1.260000000+1.260748013+1.270000000+1.270748013+1.280000000+1.280508013+1.290000000+1.290508013+1.300000000+1.300508013+1.310000000+1.310508013+1.320000000+1.320508013+1.330000000+1.330508013+1.340000000+1.340508013+1.350000000+1.350508013+1.360000000+1.360508013+1.370000000+1.370508013+1.380000000+1.380384013+1.390000000+1.390384013+1.400000000+1.400384013+1.410000000+1.410384013+1.420000000+1.420384014+1.430000000+1.430384014+1.440000000+1.440384014+1.450000000+1.450384014+1.460000000+1.460384014+1.470000000+1.470384014+1.480000000+1.480264014+1.490000000+1.490264014+1.500000000+1.500264014+1.510000000+1.510264014+1.520000000+1.520264014+1.530000000+1.530264014+1.540000000+1.540264014+1.550000000+1.550264014+1.560000000+1.560264014+1.570000000+1.570264014+1.580000000+1.580204014+1.590000000+1.590204014+1.600000000+1.600204014+1.610000000+1.610204014+1.620000000+1.620204014+1.630000000+1.630204014+1.640000000+1.640204014+1.650000000+1.650204014+1.660000000+1.660204014+1.670000000+1.670204013+1.680000000+1.680184013+1.690000000+1.690184013+1.700000000+1.700184013+1.710000000+1.710184013+1.720000000+1.720184013+1.730000000+1.730184013+1.740000000+1.740184013+1.750000000+1.750184013+1.760000000+1.760184012+1.770000000+1.770184012+1.780000000+1.780184012+1.790000000+1.790184012+1.800000000+1.800184012+1.810000000+1.810184012+1.820000000+1.820184012+1.830000000+1.830184012+1.840000000+1.840184012+1.850000000+1.850184011+1.860000000+1.860184011+1.870000000+1.870184011+1.880000000+1.880184011+1.890000000+1.890184011+1.900000000+1.900184011+1.910000000+1.910184012+1.920000000+1.920184012+1.930000000+1.930184012+1.940000000+1.940184012+1.950000000+1.950184012+1.960000000+1.960184012+1.970000000+1.970184012+1.980000000+1.980184012"
#while [ $i -le 201 ]
while [ $s ]
do
    start=$(cut -d + -f $i <<< $s)
    end=$(cut -d + -f $((i+1)) <<< $s)
    runtime=$(bc -l <<<"${end}-${start}")
    sum=$(bc -l <<<"${sum}+${runtime}")
    # echo "start in ms is: $start"
    # echo "end in ms is: $end"
    # echo "runtime in ms is: $runtime"
    # echo "Link Latency2 in ms is: $sum"
    i=$(( $i + 2 ))
done
    sum=$(bc -l <<<"${sum}*1000")
  echo "Link Latency2 in ms is: $sum"