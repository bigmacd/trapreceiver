Dim objShell


If Wscript.Arguments.Count = 1 Then
  set objShell = Wscript.CreateObject("Wscript.Shell")
  If Wscript.Arguments(0) = "start" Then
        objShell.Run "net start TrapRcvr", WindowStyle_Hidden
  Else
	objShell.Run "net stop TrapRcvr", WindowStyle_Hidden
  End If
End If