HBV Rainfall-Runoff Model
==========================
Multiobjective Optimization Benchmark
--------------------------
Copyright (C) 2012 Patrick Reed
For use with [MOEAFramework](http://www.moeaframework.org)
Licensed under the GNU Lesser General Public License

Citation:
P.M. Reed, D. Hadka, J.D. Herman, J.R. Kasprzyk, J.B. Kollat, Evolutionary multiobjective optimization in water resources: The past, present, and future, Advances in Water Resources, Volume 51, January 2013, Pages 438-456, ISSN 0309-1708, 10.1016/j.advwatres.2012.01.005.
[Link to Paper](http://www.sciencedirect.com/science/article/pii/S0309170812000073)

The HBV model is a lumped conceptual rainfall–runoff model with three primary routines:
* Snow accumulation and melt, represented by a variant of the degree-day snow model
* Soil moisture accounting, with a probability distribution to represent the spatial variability of storage elements
* A response routine, in which the linear outflow from two sub-basins is transformed by a routing parameter. 
In sum, the model contains 14 real-valued decision variables that require calibration.

The MOEAs in this study were used to calibrate the HBV model for the Williams River, West Virginia, United States (USGS Gage 03186500). Calibration was performed using precipitation and streamflow data from the MOPEX dataset over the period 1962–1972 with a one-year warmup period. Four objective functions were used: the Nash–Sutcliffe Efficiency (NSE), which measures the model fit primarily during high-flow periods; the Box-Cox transformed root mean squared error (TRMSE), which accounts for low-flow periods; the runoff coefficient error (ROCE), which accounts for the long-term water balance; and the slope of the flow duration curve error (SFDCE), which measures the ability of the model to match the long-term variability of flows.

Contents:
* HBV_src: C++ source code for the HBV model
* params: MOEA parameters to be sampled for each algorithm
* Java files: to define the HBV problem for MOEAFramework and submit PBS cluster jobs
* generateSampling.sh: Create the Latin Hypercube samples of algorithm parameters
* HBV_Reference.txt: The benchmark reference set from this study

To run: 
0. Make sure you have [MOEAFramework](http://www.moeaframework.org). Download the "all-in-one executable" and put the .jar file in the main directory of this project.

1. Compile the HBV Model
	cd HBV_src && make

2. Move the executable and data file into the main directory
	mv hbv WIL.in ..
	
3. Compile the Java files (repeat for all Java files)
	javac -classpath MOEAFramework-<VERSION>-Executable.jar:. HBVProblem.java
Before you compile ExampleJobFactory.java, you may want to read through the file and see if you want to modify any parts of the setup.

4. Generate MOEA parameters:
	./generateSampling.sh

5. Run `ExampleJobFactory` to submit jobs to a PBS queue. If you read the file ExampleJobFactory.java, you will see which algorithms and random seeds will be submitted by default. Change these as needed.
	java -classpath MOEAFramework-<VERSION>-Executable.jar:. ExampleJobFactory -seed 0
The `-seed` argument is optional. If omitted, 50 seeds will be run.

You can perform a regular (non-PBS) run like this:
	java -classpath MOEAFramework-<VERSION>-Executable.jar:. \
	org.moeaframework.analysis.sensitivity.Evaluator \
	--parameterFile ./path/to/file \
	--input ./path/to/LHS/file \
	--problem HBV \
	--algorithm NSGAII \
	--seed 0 \
	--output ./path/NSGAII_HBV_0.out \ 
	--full \
	-x epsilon=0.01,0.025,0.01,0.01

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.