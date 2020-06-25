package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"

	"github.com/gookit/color"
)

var consoleMutex = &sync.Mutex{}

func colorCodeToString(code color.Color) string {
	switch code {
	case color.FgBlue:
		return "Blue"
	case color.FgGreen:
		return "Green"
	case color.FgYellow:
		return "Yellow"
	case color.FgRed:
		return "Red"
	case color.FgCyan:
		return "Cyan"
	default:
		return "Unknown"
	}
}

func coloredConsoleMessage(col color.Color, message string) {
	consoleMutex.Lock()
	color.Set(col)
	fmt.Printf(message)
	color.Reset()
	consoleMutex.Unlock()
}

func recursiveThing(number int, color color.Color) {
	coloredConsoleMessage(color,
		fmt.Sprintf("%s thread, number is %d\n", colorCodeToString(color), number))
	time.Sleep(time.Duration(rand.Intn(9)+1) * time.Second)

	if number == 0 {
		coloredConsoleMessage(color,
			fmt.Sprintf("%s thread complete!\n", colorCodeToString(color)))
	} else {
		recursiveThing(number-1, color)
	}
}

func main() {
	colors := []color.Color{color.FgBlue,
		color.FgGreen,
		color.FgYellow,
		color.FgRed,
		color.FgCyan}

	var waitGroup sync.WaitGroup

	for _, col := range colors {
		waitGroup.Add(1)
		go func(c color.Color) {
			recursiveThing(10, c)
			waitGroup.Done()
		}(col)
	}

	waitGroup.Wait()
	fmt.Printf("All threads complete!")
}
