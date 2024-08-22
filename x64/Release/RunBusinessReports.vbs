' VBScript for Business Management System Automation

' 실행할 프로그램과 로그 파일 경로
Dim programName, logFilePath
programName = "KPI_Sales_ManagementSys.exe"
logFilePath = "C:\Users\gwangho\source\repos\KPI_Sales_ManagementSys\x64\Release\log.txt"

' FileSystemObject 생성
Set objFSO = CreateObject("Scripting.FileSystemObject")

' 프로그램 실행
Set objShell = CreateObject("WScript.Shell")
result = objShell.Run(programName, 1, True)

' 실행 결과 확인
If result = 0 Then
    WScript.Echo "The program executed successfully."
Else
    WScript.Echo "There was an error during program execution."
End If

' 로그 파일 처리
If objFSO.FileExists(logFilePath) Then
    Set logFile = objFSO.OpenTextFile(logFilePath, 1)
    
    ' 로그 파일 내용 출력
    Do Until logFile.AtEndOfStream
        WScript.Echo logFile.ReadLine
    Loop
    
    logFile.Close
Else
    WScript.Echo "Log file not found."
End If

' 보고서 파일 생성 및 열기
Dim reportFileName
reportFileName = "C:\Users\gwangho\source\repos\KPI_Sales_ManagementSys\KPI_Sales_ManagementSys\report.txt"
Set reportFile = objFSO.CreateTextFile(reportFileName, True)
reportFile.WriteLine "Business Management System Report"
reportFile.WriteLine "Report generated on: " & Now
reportFile.Close

WScript.Echo "Report generated at: " & reportFileName
objShell.Run "notepad.exe " & reportFileName, 1, False
