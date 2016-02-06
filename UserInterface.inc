cseg
; sub-routine to set a value
; user must move value to be changed into accumulator
; move maximum value and minimum value into MaxVal and MinVal respectively
; set bcd_num if value to be changed is a bcd number
set_val:
check_inc:
    jb INC_B, check_dec        ; if the 'increment' button is not pressed skip to decrement button
    Wait_Milli_Seconds(#50)    
    jb INC_B, check_dec  
    jnb INC_B, $            
    cjne a, MaxVal, incr        ; if already at max value, loop back to min value
    mov a, MinVal
    sjmp check_done
incr:
    add a, #0x01
    da a
    ljmp check_done
check_dec:
    jb DEC_B, check_done        ; if the 'decrement' button is not pressed skip to done button
    Wait_Milli_Seconds(#50)    
    jb DEC_B, check_done
    jnb DEC_B, $
    cjne a, MinVal, decr        ; if already at min value, loop back to max value
    mov a, MaxVal
    sjmp check_done
decr:
    add a, #0x99
    da a
    sjmp check_done
check_done:
    mov Temp, a
    mov a, WhichVal
    cjne a, #0x01, check2
    Set_Cursor(1,8)
    sjmp check_button
check2:
    cjne a, #0x02, check3
    Set_Cursor(2,8)
    sjmp check_button
check3:
    cjne a, #0x03, check4
    Set_Cursor(1,14)
    sjmp check_button
check4:
    Set_Cursor(2,14)
    
check_button:
     Display_BCD(Temp)
    mov a, Temp
    jb SET_B, set_val_jump  ; if the 'done' button is not pressed check other buttons again
    Wait_Milli_Seconds(#50)    
    jb SET_B, set_val_jump
    jnb SET_B, $    
    ret        
set_val_jump:
    ljmp set_val

; sub-routine to go through value setting process    
set_Values:        ; soak time
    Set_Cursor(1,7)
    WriteData(#'*')
    mov a, SoakTime
    mov WhichVal, #0x01
    mov MaxVal, #0x16			; should be 165
    mov MinVal, #0x12			; should be 125
    lcall set_val
    mov SoakTmp, a
    Set_Cursor(1,7)
    WriteData(#' ')                    ; clears asterisk
next1:        ; soak temp
    Set_Cursor(2,7)
    WriteData(#'*')
    mov a, SoakTmp
    mov WhichVal, #0x02
    mov MaxVal, #0x12				; should be 120
    mov MinVal, #0x60
    lcall set_val
    mov SoakTime, a
    Set_Cursor(2,7)
    WriteData(#' ')
next2:        ; Reflow temp
    Set_Cursor(1,13)
    WriteData(#'*')
    mov a, ReflowTmp
    mov WhichVal, #0x03
    mov MaxVal,    #0x21			; should be 210
    mov MinVal,    #0x18			; should be 180
    lcall set_val
    mov ReflowTmp, a
    Set_Cursor(1, 13)
    WriteData(#' ')
next3:        ; reflow time
    Set_Cursor(2,13)
    WriteData(#'*')
    mov a, ReflowTime
    mov WhichVal, #0x04
    mov MaxVal,    #0x40
    mov MinVal,    #0x30
    lcall set_val
    mov ReflowTime, a
    Set_Cursor(2,13)
    WriteData(#' ')
    
; check if set or start buttons are pressed - set to continue setting values, start to go back to state0
checking:
    jb SET_B, finish                         ; if the keep setting button is not pressed skip
    Wait_Milli_Seconds(#50)
    jb SET_B, finish  
    jnb SET_B, $
    ljmp set_Values                                ; skip back up to setting soakTmp if button pressed
finish:
    jb STOP, checking                          ; if the done button is not pressed skip
    Wait_Milli_Seconds(#50)
    jb STOP, checking 
    jnb STOP, $        

    Set_Cursor(2,1)                                ; blank out bottom row
    Send_Constant_String(#Blank)        
    ret


