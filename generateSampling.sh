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
java $JAVA_HEAP -classpath $CLASSPATH org.moeaframework.analysis.sensitivity.SampleGenerator -m latin -n $MOEAFRAMEWORK_SAMPLES -p params/Borg_Params -o params/Borg_LHS -s $MOEAFRAMEWORK_SEED
