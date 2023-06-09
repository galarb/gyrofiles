1 REM GWBASIC program to control MCP23S17 I/O expander inside PC Emulator
10 CLS
20 REM
30 REM check for I/O available
40 IF INP(&HE0) AND 1 = 0 THEN
50   PRINT "Extended I/O not available on this board!"
60   END
70 END IF
80 REM
90 REM Setup Port A and B as Outputs
100 OUT &HE1, &HFF
110 OUT &HE2, &HFF
120 REM
130 PRINT "on/off Port A and B"
140 FOR i = 1 TO 4
150   OUT &HE5, 255
160   OUT &HE6, 255
170   FOR d = 1 TO 200: NEXT
180   OUT &HE5, 0
190   OUT &HE6, 0
200   FOR d = 1 TO 200: NEXT
210 NEXT
220 REM
230 PRINT "on/off single GPIO A0 to B2"
240 FOR i = 1 TO 4
250   FOR gpio = 0 TO 10
260     OUT &HE7, gpio
270     OUT &HE9, 0
280     OUT &HE7, gpio + 1
290     OUT &HE9, 1
300     FOR d = 1 TO 50: NEXT
310   NEXT
320   FOR gpio = 10 TO 0 STEP -1
330     OUT &HE7, gpio
340     OUT &HE9, 0
350     IF gpio > 0 THEN: OUT &HE7, gpio - 1
360     OUT &HE9, 1
370     FOR d = 1 TO 50: NEXT
380   NEXT
390 NEXT
400 REM
410 PRINT "Check for B3 to B7 as Inputs..."
420 PRINT "Press any key to stop"
430 OUT &HE2, &H7: REM setup GPIO B3 to B7 as inputs (bits 3..7 = 0)
440 WHILE LEN(INKEY$) = 0
450   FOR gpio = 11 TO 15
460     GOSUB 1000: REM checkGPIO (gpio)
470   NEXT
480 WEND
490 END
500 REM
1000 REM SUB checkGPIO (gpio)
1010 OUT &HE7, gpio
1020 IF INP(&HE9) = 1 THEN
1030   PRINT "pressed "; gpio; "...";
1040   WHILE INP(&HE9): WEND
1050   PRINT "released"
1060 END IF
1070 RETURN

