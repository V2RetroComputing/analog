@set OLDPATH=%PATH%
@set PATH=c:\wincupl\WinCupl\Fitters;c:\Wincupl\Shared\;%PATH%

cupl.exe -jxfu c:\wincupl\shared\cupl.dl g16v8 picopal.pld 

@set PATH=%OLDPATH%
