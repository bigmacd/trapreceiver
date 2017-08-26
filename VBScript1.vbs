' VBScript source code
' Store the arguments in a variable:
Set objArgs = Wscript.Arguments

' Display all command-line arguments
For Each strArg in objArgs
  WScript.Echo strArg
Next

' Display the first 3 command-line arguments
For I = 0 to objArgs.Count
  Wscript.Echo objArgs(I)
Next