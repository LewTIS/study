
def _private_1(name):
    return f'hello,{name}'

def _private_2(name):
    return f'hi,{name}'

def greet(name):
    if len(name)>3:
        return _private_1(name)
    else:
        return _private_2(name)

