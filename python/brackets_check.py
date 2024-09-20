#python

def is_pair(left, right):
    if (left == '[' and right == ']'):
        return True
    elif (left == '(' and right == ')'):
        return True
    elif (left == '{' and right == '}'):
        return True
    return False

code = str(input())

stack = []
top = -1

last_position = 0

for i in range(len(code)):
    if (code[i] in ['(', '[', '{']):
        if (len(stack) == 0):
            last_position = i + 1
        stack.append(code[i])
        top += 1
    elif (code[i] in [')', ']', '}']):
        if (top != -1 and is_pair(stack[top], code[i])):
            stack.pop()
            top -= 1        
        else:
            print(i + 1)
            break
else:
    if (len(stack) == 0):
        print("Success")
    else:
        print(last_position)
