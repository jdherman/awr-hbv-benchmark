/*
Copyright (C) 2011-2012 Josh Kollat, Jon Herman, Patrick Reed and others.

The HBV Benchmark Problem is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The HBV Benchmark Problem is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the HBV Benchmark Problem.  If not, see <http://www.gnu.org/licenses/>.
*/

import java.io.IOException;

import org.moeaframework.core.Solution;
import org.moeaframework.core.variable.RealVariable;
import org.moeaframework.problem.ExternalProblem;
import org.moeaframework.problem.ProblemException;

public class HBVProblem extends ExternalProblem {
	
	public HBVProblem() throws IOException {
		super("./hbv");
	}

	@Override
	public String getName() {
		return "HBV";
	}

	@Override
	public int getNumberOfVariables() {
		return 14;
	}

	@Override
	public int getNumberOfObjectives() {
		return 4;
	}

	@Override
	public int getNumberOfConstraints() {
		return 0;
	}

	@Override
	public Solution newSolution() {
		Solution solution = new Solution(14, 4, 0);
		solution.setVariable(0, new RealVariable(0.0, 100.0)); //L (mm) 
		solution.setVariable(1, new RealVariable(0.5, 20.0)); //K0 (d)
		solution.setVariable(2, new RealVariable(1.0, 100.0)); //K1 (d)
		solution.setVariable(3, new RealVariable(10.0, 20000.0)); //K2 (d)
		solution.setVariable(4, new RealVariable(0.0, 100.0)); //Perc (mm/d)
		solution.setVariable(5, new RealVariable(0.3, 1.0)); //LP (-)
		solution.setVariable(6, new RealVariable(0.0, 2000.0)); //Fcap (mm)
		solution.setVariable(7, new RealVariable(0.0, 7.0)); //B (-)
		solution.setVariable(8, new RealVariable(24.0, 120.0)); //MaxBas (d)
		solution.setVariable(9, new RealVariable(-3.0, 3.0)); //TT (C)
		solution.setVariable(10, new RealVariable(0.0, 20.0)); //DDF (mm/C*d)
		solution.setVariable(11, new RealVariable(0.0, 1.0)); //CFR (-)
		solution.setVariable(12, new RealVariable(0.0, 0.8)); //CWH (-)
		solution.setVariable(13, new RealVariable(0.0, 7.0)); //TTI (C)
		return solution;
	}

	@Override
	public void evaluate(Solution solution) throws ProblemException {	
		super.evaluate(solution);
	}

}
