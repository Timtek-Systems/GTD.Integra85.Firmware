[CmdletBinding()]
Param(
    [Parameter(Mandatory=$True,Position=1)]
    [string]$hexFile
    )

$avrdude = "C:\Program Files (x86)\Arduino\hardware\tools\avr\bin\avrdude.exe"
$avrdudeConfig = "C:\Program Files (x86)\Arduino\hardware\tools\avr\etc\avrdude.conf"

$arguments = "-C$avrdudeConfig", "-v", "-patmega328p", "-carduino", "-PCOM3", "-D", "-Uflash:w:$hexFile`:i"

& $avrdude $arguments
