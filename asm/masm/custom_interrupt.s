; LB3 from ETU Computer Organization Course

EOFLine EQU '$'

AStack SEGMENT STACK
    DW 512 DUP(?)
AStack ENDS

DATA SEGMENT
    keep_cs         DW 0       ; Хранение сегмента прерываний
    keep_ip         DW 0       ; Хранение смещения вектора прерываний
    current_ticks   DW 0       ; Текущее количество тиков
    previous_ticks  DW 9       ; Предыдущее количество тиков
    remaining_count DW 5       ; Оставшееся количество выводов
    
    message DB 'Assembler is the best! $', 0Ah, 0Dh, EOFLine
DATA ENDS

CODE SEGMENT
    ASSUME CS:CODE, DS:DATA, SS:AStack

; Процедура вывода сообщения на экран
PrintMessage PROC FAR
    mov AH, 09h                 ; Записываем в AH номер функции вывода строки
    mov DX, OFFSET message      ; Загружаем в DX отступ message
    int 21h                     ; Выводи строку в stdout
    ret                         ; Возвращаемся из функции
PrintMessage ENDP

Interrupt PROC FAR
    ; Сохранение изменяемых регистров
    push AX
    
; ================|
; Вариант 9       |
; Шифр задания: a |
; ================|
; Прерывание: 1Ch - прерывание системного таймера
; Задание: Выполнить вывод на экран заданного
;          количества (3-5) одинаковых сообщений
;          задержка между которыми возрастает в 2
;          раза, начиная от 1 секунды.
; ================|

    dec current_ticks            ; Уменьшаем current_ticks на единицу
    mov AX, current_ticks        ; Записываем current_ticks в AX
    cmp AX, 0                    ; Сравниваем AX с нулём

    jge return                   ; Завершаем выполнение если прошло заданное количество тиков

print_message:
    call PrintMessage            ; Выводим сообщение

    dec remaining_count          ; Умньшаем remaining_count на единицу 
    
; Новое количество тиков равно previous_ticks * 2
    mov AX, previous_ticks       ; Записываем в AX предыдущее число тиков
    shl AX, 1                    ; Удваиваем это значение
    mov current_ticks, AX        ; Записываем новое значение в current_ticks
    mov previous_ticks, AX       ; Записываем это значение в previous_ticks

return:
    mov AL, 20h            ; Разрешение обработки прерываний с более низкими уровнями,
    out 20h, AL            ; чем только что обработанное
    
    ; Восстановление регистров из стека
    pop AX
    
    iret                   ; Возврат из прерывания
Interrupt ENDP


Main PROC FAR             ; Начало главной процедуры

; Инициализация программы
    push DS               ; Сохранение адреса начала PSP в стеке
    sub AX, AX            ; Обнуление регистра AX
    push AX               ; Загрузка нулевого кода завершения в стек
    mov AX, DATA          ; /Загрузка сегментного
    mov DS, AX            ; \регистра данных

; Сохранение адреса заменяемого прерывания в переменные keep_ip и keep_cs
    mov AH, 35h           ; Функция получения вектора прерываний
    mov AL, 1Ch           ; Номер заменяемого вектора
    int 21h               ; Вызываем функцию сохранения сегмента в ES, а смещения в BX
    mov keep_ip, BX       ; Сохранение значения BX в keep_ip
    mov keep_cs, ES       ; Сохранение значения ES в keep_cs

; Замена вектора прерывания на новый
    ; Сохраняем DS, так как его значение будет изменено прерыванием 25h
    push DS                   ; Сохраняем значение DS в стеке
    mov DX, OFFSET Interrupt   ; Загружаем в DX смещение процедуры Interrupt
    mov AX, SEG Interrupt      ; Сохраняем адрес сегмента процедуры в AX

    ; Записываем в DS адрес сегмента, где расположен Interrupt
    mov DS, AX                ; Записываем адрес сегмента процедуры в DS
    
    ; 25h:
    ; Принимает DS:DX - новый обработчик прерывания и AL - номер прерывания
    mov AH, 25h               ; Записываем функцию установки вектора в AH
    mov AL, 1Ch               ; Записываем номер заменяемого вектора в AL
    
    int 21h                   ; Меняем прерывание по адресу 1Ch на Int
    pop DS                    ; Восстанавливаем DS

; Вызов пользовательского прерывания
    mov AH, 1Ch               ; Записываем номер прерывания в AH
print_loop:
    int 21h                   ; Вызываем пользовательское прерывание
    cmp remaining_count, 0    ; Сравниваем remaining_count с 0
    jne print_loop            ; Проверяем, что ещё нужно выводить сообщения

; Восстановление оргинального вектора прерывания
    cli                   ; Сброс флага прерываниия IF (запрещение прерываний)
    push DS               ; Сохранение DS в стеке
    mov DX, keep_ip       ; Запись в DX keep_ip
    mov AX, keep_cs       ; Запись в AX keep_cs
    mov DS, AX            ; Запись AX в DS
    mov AH, 25h           ; Запись функции установки вектора
    mov AL, 1Ch           ; Запись номера заменяемого вектора
    int 21h               ; Вызов функции из AH
    pop DS                ; Восстановление значения DS
    sti                   ; Установка флага прерываний IF (разрешение прерываний)
 
    ret                   ; Возврат из программы
Main ENDP

CODE ENDS                 ; Конец сегмента кода

END Main                  ; Указание на то, что программу нужно начать с Main
