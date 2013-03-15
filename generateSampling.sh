# Copyright (C) 2011-2012 Josh Kollat, Jon Herman, Patrick Reed and others.

# The HBV Benchmark Problem is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# The HBV Benchmark Problem is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public License
# along with the HBV Benchmark Problem.  If not, see <http://www.gnu.org/licenses/>.

#!/bin/sh
source setup.sh
MOEAFRAMEWORK_SEED=1337
MOEAFRAMEWORK_SAMPLES=10
java $JAVA_HEAP -classpath $CLASSPATH org.moeaframework.analysis.sensitivity.SampleGenerator -m latin -n $MOEAFRAMEWORK_SAMPLES -p params/NSGAII_Params -o params/NSGAII_LHS -s $MOEAFRAMEWORK_SEED
java $JAVA_HEAP -classpath $CLASSPATH org.moeaframework.analysis.sensitivity.SampleGenerator -m latin -n $MOEAFRAMEWORK_SAMPLES -p params/SPEA2_Params -o params/SPEA2_LHS -s $MOEAFRAMEWORK_SEED
java $JAVA_HEAP -classpath $CLASSPATH org.moeaframework.analysis.sensitivity.SampleGenerator -m latin -n $MOEAFRAMEWORK_SAMPLES -p params/OMOPSO_Params -o params/OMOPSO_LHS -s $MOEAFRAMEWORK_SEED
java $JAVA_HEAP -classpath $CLASSPATH org.moeaframework.analysis.sensitivity.SampleGenerator -m latin -n $MOEAFRAMEWORK_SAMPLES -p params/MOEAD_Params -o params/MOEAD_LHS -s $MOEAFRAMEWORK_SEED
java $JAVA_HEAP -classpath $CLASSPATH org.moeaframework.analysis.sensitivity.SampleGenerator -m latin -n $MOEAFRAMEWORK_SAMPLES -p params/IBEA_Params -o params/IBEA_LHS -s $MOEAFRAMEWORK_SEED
java $JAVA_HEAP -classpath $CLASSPATH org.moeaframework.analysis.sensitivity.SampleGenerator -m latin -n $MOEAFRAMEWORK_SAMPLES -p params/GDE3_Params -o params/GDE3_LHS -s $MOEAFRAMEWORK_SEED
java $JAVA_HEAP -classpath $CLASSPATH org.moeaframework.analysis.sensitivity.SampleGenerator -m latin -n $MOEAFRAMEWORK_SAMPLES -p params/eNSGAII_Params -o params/eNSGAII_LHS -s $MOEAFRAMEWORK_SEED
java $JAVA_HEAP -classpath $CLASSPATH org.moeaframework.analysis.sensitivity.SampleGenerator -m latin -n $MOEAFRAMEWORK_SAMPLES -p params/eMOEA_Params -o params/eMOEA_LHS -s $MOEAFRAMEWORK_SEED
