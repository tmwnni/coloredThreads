using System;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

class Program
{
	// Console lock
	public static object _consoleLock = new object();
	// Random
	public static Random _random = new Random();
	
	// Recursive thing!
	public static void RecursiveThing(int number, ConsoleColor color)
	{
		// Check number
		if(number >= 0)
		{
			// Print to screen
			lock(_consoleLock)
			{
				Console.ForegroundColor = color;
				Console.WriteLine($"{color} thread, number is {number}.");
				Console.ForegroundColor = ConsoleColor.Gray;
			}

			// Wait random time
			Thread.Sleep(_random.Next(1, 10)*1000);

			// Call RecursiveThing
			RecursiveThing(number - 1, color);
		}
	}

	// Async wrapper for RecursiveThing()
	public static Task RecursiveThingAsync(int number, ConsoleColor color)
	{
		return Task.Run(() => RecursiveThing(number, color));
	}

	// Main thread
	public static void Main()
	{		
		// Setup colors
		ConsoleColor[] colors =
		{
			ConsoleColor.Green,
			ConsoleColor.Blue,
			ConsoleColor.Yellow,
			ConsoleColor.Red,
			ConsoleColor.Cyan
		};

		// Create threads
		var threads =
			from color
			in colors
			select RecursiveThingAsync(10, color);

		// Kick off threads and wait for completion
		Task.WaitAll(threads.ToArray());

		// All done!
		Console.WriteLine("All threads complete!");
	}
}
