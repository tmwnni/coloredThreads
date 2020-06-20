(*
    This program uses the F# async pattern rather than threads
*)
open System
open System.Threading

let consoleLock = Object()
let random = Random()

let colorName color =
    match color with
    | ConsoleColor.Green -> "Green"
    | ConsoleColor.Blue -> "Blue"
    | ConsoleColor.Yellow -> "Yellow"
    | ConsoleColor.Red -> "Red"
    | ConsoleColor.Cyan -> "Cyan"
    | _ -> "Unknown"

let coloredConsoleMessage color message =
    lock consoleLock (fun () ->
    Console.ForegroundColor <- color
    printf "%s\n" message
    Console.ForegroundColor <- ConsoleColor.Gray
    )

let rec recursiveThing number color =
    coloredConsoleMessage color (sprintf "%s thread, number is %d" (colorName color) number)
    Thread.Sleep(random.Next(1, 10) * 1000)

    match number with
    | 0 -> coloredConsoleMessage color (sprintf "%s thread complete!" (colorName color))
    | _ -> recursiveThing (number - 1) color

let recursiveThingAsync number color =
    async { recursiveThing number color }

[<EntryPoint>]
let main argv =
    let colors =
        [ConsoleColor.Green;
        ConsoleColor.Blue;
        ConsoleColor.Yellow;
        ConsoleColor.Red;
        ConsoleColor.Cyan]

    colors
        |> List.map (recursiveThingAsync 10)
        |> Async.Parallel
        |> Async.RunSynchronously
        |> ignore

    printf "All threads complete!\n"
    0 // return an integer exit code
