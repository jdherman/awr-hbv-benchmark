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
