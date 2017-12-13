$source = "."
$destination = "..\Integra85Firmware.zip"
Add-Type -assembly "System.IO.Compression.FileSystem"
[IO.Compression.ZipFile]::CreateFromDirectory($source, $destination)