import java.io.File;
import java.io.IOException;
import java.io.PrintStream;
import java.io.File;
import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.OptionBuilder;
import org.apache.commons.cli.Options;
import org.moeaframework.util.CommandLineUtility;
import org.moeaframework.util.pbs.PBSJob;
import org.moeaframework.util.pbs.PBSSubmitter;

import org.moeaframework.util.io.RedirectStream;

/**
 * Translates individual {@link PBSJob}s to valid PBS scripts and submits the
 * scripts using {@code qsub}.
 */
public class CustomSubmitter {

	/**
	 * The queue to which the jobs are submitted. Set to {@code null} to use the
	 * default PBS queue.
	 */
	private final String queue;

	/**
	 * The directory where output logs are written. Set to {@code null} to use
	 * the default PBS behavior.
	 */
	private final File outputDirectory;

	/**
	 * The directory where error logs are written. Set to {@code null} to use
	 * the default PBS behavior.
	 */
	private final File errorDirectory;

	/**
	 * Store the new line character to prevent repetitive calls to
	 * {@code System.getProperty("line.separator")}.
	 */
	private static String newLine = System.getProperty("line.separator");

	/**
	 * Class constructor for a PBS submitter with the specified queue, output
	 * directory, and error directory. The output and error directories may be
	 * {@code null} to use the default PBS behavior for saving output and error
	 * logs.
	 * 
	 * @param queue the queue to which the jobs are submitted; {@code null} to
	 *        use the default PBS queue
	 * @param outputDirectory the directory where output logs are written;
	 *        {@code null} to use the default PBS behavior
	 * @param errorDirectory the directory where error logs are written;
	 *        {@code null} to use the default PBS behavior
	 * @throws IOException if an I/O error occurred
	 */
	public CustomSubmitter(String queue, File outputDirectory, File errorDirectory)
			throws IOException {
		super();
		this.queue = queue;
		this.outputDirectory = outputDirectory;
		this.errorDirectory = errorDirectory;

		// ensure output and error folders exist
		if ((outputDirectory != null) && !outputDirectory.exists()) {
			if (!outputDirectory.mkdirs()) {
				throw new IOException("unable to create output directory");
			}
		}

		if ((errorDirectory != null) && !errorDirectory.exists()) {
			if (!errorDirectory.mkdirs()) {
				throw new IOException("unable to create output directory");
			}
		}
	}

	/**
	 * Translates the specified PBS job to a valid PBS script which can be
	 * submitted to the system's {@code qsub} program.
	 * 
	 * @param job the PBS job
	 * @return the PBS script sued to execute the specified PBS job
	 */
	protected String toPBSScript(PBSJob job) {
		StringBuilder sb = new StringBuilder();

		sb.append("#PBS -N ").append(job.getName()).append(newLine);
		sb.append("#PBS -l nodes=").append(job.getNodes()).append(":nehalem:ppn=1").append(newLine);
		sb.append("#PBS -l walltime=").append(job.getWalltime()).append(
				":00:00").append(newLine);

		if (outputDirectory != null) {
			sb.append("#PBS -o ").append(
					new File(outputDirectory, job.getName()).getPath()).append(
					newLine);
		}

		if (errorDirectory != null) {
			sb.append("#PBS -e ").append(
					new File(errorDirectory, job.getName()).getPath()).append(
					newLine);
		}

		if (queue != null) {
			sb.append("#PBS -q ").append(queue).append(newLine);
		}

		for (String command : job.getCommands()) {
			sb.append(command).append(newLine);
		}

		return sb.toString();
	}

	/**
	 * Submits the specified PBS job to the PBS queue using the {@code qsub}
	 * program.
	 * 
	 * @param job the PBS job to be submitted
	 * @throws IOException if an error occurred while invoking {@code qsub}
	 */
	public void submit(PBSJob job) throws IOException {
		Process process = new ProcessBuilder("qsub").start();

		RedirectStream.redirect(process.getInputStream(), System.out);
		RedirectStream.redirect(process.getErrorStream(), System.err);

		String script = toPBSScript(job);
		System.out.println(script);

		PrintStream ps = new PrintStream(process.getOutputStream());
		ps.print(script);
		ps.close();

		try {
			int exitStatus = process.waitFor();

			if (exitStatus != 0) {
				throw new IOException("qsub terminated with exit status "
						+ exitStatus);
			}
		} catch (InterruptedException e) {
			Thread.currentThread().interrupt();
		}
	}

}
