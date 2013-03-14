import java.io.File;
import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.OptionBuilder;
import org.apache.commons.cli.Options;
import org.moeaframework.util.CommandLineUtility;
import org.moeaframework.util.pbs.PBSJob;
import org.moeaframework.util.pbs.PBSSubmitter;

public class ExampleJobFactory extends CommandLineUtility {

	private ExampleJobFactory() {
		super();
	}

	public static void main(String[] args) {
		new ExampleJobFactory().start(args);
	}

	@SuppressWarnings("static-access")
	@Override
	public Options getOptions() {
		Options options = super.getOptions();

		options.addOption(OptionBuilder.hasArgs().withArgName("value")
				.withDescription("seed(s) to run").create("seed"));
		
		options.addOption(OptionBuilder.hasArgs().withArgName("value")
				.withDescription("algorithm(s) to run").create("algorithm"));
		
		options.addOption(OptionBuilder.hasArgs().withArgName("value")
				.withDescription("problem(s) to run").create("problem"));
		
		return options;
	}

	@Override
	public void run(CommandLine commandLine) throws Exception {
		//these are the default settings
		String[] seeds = new String[] { 
				"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", 
				"10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
				"20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
				"30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
				"40", "41", "42", "43", "44", "45", "46", "47", "48", "49",};
		
		String[] algorithms = new String[] { "NSGAII", "eMOEA", "eNSGAII", 
				"OMOPSO", "IBEA", "GDE3", "MOEAD", "SPEA2" };
		//TODO: add AMALGAM and HypE when supported
		
		String[] problems = new String[] { "HBV" };
		
		//the default settings can be overridden by command line options
		if (commandLine.hasOption("seed")) {
			seeds = commandLine.getOptionValues("seed");
		}
		
		if (commandLine.hasOption("algorithm")) {
			algorithms = commandLine.getOptionValues("algorithm");
		}
		
		if (commandLine.hasOption("problem")) {
			problems = commandLine.getOptionValues("problem");
		}
		
		//submit the jobs to PBS
		CustomSubmitter submitter = new CustomSubmitter(null, new File("output/"),
				new File("error/"));
		
		for (String problem : problems) {
			for (String algorithm : algorithms) {
				for (String seed : seeds) {
				
				  try { //sleep 2sec between submissions
          	Thread.sleep(2000);
          } catch (InterruptedException e) {
          	//do nothing
          }

					StringBuilder sb = new StringBuilder();
					sb.append("java -classpath $CLASSPATH -Xmx256m -server org.moeaframework.analysis.sensitivity.Evaluator");
					sb.append(" --parameterFile ./params/").append(algorithm).append("_Params");
					sb.append(" --input ./params/").append(algorithm).append("_LHS");
					sb.append(" --problem ").append(problem);
					sb.append(" --algorithm ").append(algorithm);
					sb.append(" --seed ").append(seed);
					sb.append(" --output ").append("~/scratch/").append(algorithm).append('_').append(problem).append('_').append(seed);
					sb.append(" --full");
					sb.append(" -x epsilon=0.01,0.025,0.01,0.01");

					PBSJob job = new PBSJob(algorithm + "_" + problem + "_" + seed, 96, 1);
					job.addCommand("cd $PBS_O_WORKDIR");
					job.addCommand("source setup.sh");
					job.addCommand(sb.toString());
				
					submitter.submit(job);
				}
			}
		}
	}

}
