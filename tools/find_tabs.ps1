$tools = Split-Path -Parent $MyInvocation.MyCommand.Path
. $tools\util.ps1

Get-SourceFiles | Select-String -Pattern "\t" -List
