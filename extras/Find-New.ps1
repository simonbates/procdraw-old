$tools = Split-Path -Parent $MyInvocation.MyCommand.Path
. $tools\util.ps1

Get-SourceFiles $pwd "*.c","*.cpp","*.h" | Select-String -Pattern "\Wnew\s"
