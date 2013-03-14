import java.io.IOException;
import java.io.File;

import org.moeaframework.core.NondominatedPopulation;
import org.moeaframework.core.Problem;
import org.moeaframework.core.Solution;
import org.moeaframework.problem.ProblemProvider;
import org.moeaframework.core.PopulationIO;


public class HBVProblemProvider extends ProblemProvider {

	@Override
	public Problem getProblem(String name) {
		if (name.equals("HBV")) {
			try {
				return new HBVProblem();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		return null;
	}

	@Override
	public NondominatedPopulation getReferenceSet(String name) {
    		return null;
	}

}
