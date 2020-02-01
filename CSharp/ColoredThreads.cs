/*
	This program uses the Task pattern rather than threads
*/
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
		if (number < 0)
		{
			ColoredConsoleMesssage($"{color} thread complete!", color);
			return;
		}

		// Print to screen
		ColoredConsoleMesssage($"{color} thread, number is {number}.", color);

		// Wait random time
		Thread.Sleep(_random.Next(1, 10) * 1000);

		// Call RecursiveThing
		RecursiveThing(number - 1, color);
	}

	// Async wrapper for RecursiveThing()
	public static Task RecursiveThingAsync(int number, ConsoleColor color) =>
		Task.Run(() => RecursiveThing(number, color));

	// Lock the console and write the message with desired color
	public static void ColoredConsoleMesssage(string message, ConsoleColor color)
	{
		lock(_consoleLock)
		{
			Console.ForegroundColor = color;
			Console.WriteLine(message);
			Console.ForegroundColor = ConsoleColor.Gray;
		}
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
