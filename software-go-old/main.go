package main

import (
	"machine"

	"tinygo.org/x/drivers/flash"
	"tinygo.org/x/drivers/gps"
	"tinygo.org/x/tinyfs/littlefs"
)

// nmea0183 protocol
var uart = machine.UART0

// byte number boundaries of littlefs
var (
	flashStart uint32 = 0x300000
	flashSize  uint32 = 0x100000
)

func main() {
	//TODO: use actual pins
	txPin := machine.Pin(1)
	rxPin := machine.Pin(3)

	uart.Configure(machine.UARTConfig{BaudRate: 4800,
		TX: txPin,
		RX: rxPin,
	})

	device := gps.NewUART(uart)
	parser := gps.NewParser()

	for {
		sentence, err := device.NextSentence()
		if err != nil {
			println(err)
		}
		fix, err := parser.Parse(sentence)
		//important elements of this struct:
		//sentence type
		//lat/long obviously
		//time gives us a utc time as time.Time
		if fix.Type == gps.GSA {
			println("got GSA fix with latitude:", fix.Latitude, " and longitude", fix.Longitude)
		}
		println("NMEA0138 sentence:", sentence)
	}
}

func initFS() (*littlefs.FS, error) {
	blockdev := flash.
}
