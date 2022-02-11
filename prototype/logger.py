
import os # Fck windows!

class LogType():
    debug = 32
    info = 64
    warning = 128
    error = 256
    critical = 512
    __none = 1024

class colors():
    def gray(): os.system("tput setaf 8")
    def white(): os.system("tput setaf 15")
    def yellow(): os.system("tput setaf 11")
    def cyan(): os.system("tput setaf 14")
    def red(): os.system("tput setaf 9")
    def reset(): os.system("tput sgr0")


__current_log_type = LogType.warning
__inited = False

def __init():
    global __inited

    
    
    __inited = True
    return

def __log_pretty(message: str, log_type: LogType):
    if (log_type == LogType.debug):
        colors.gray()
        print("[debug] ", end= '')
    elif (log_type == LogType.info):
        colors.white()
        print("[info] ", end= '')
    elif (log_type == LogType.warning):
        colors.yellow()
        print("[warning] ", end= '')
    elif (log_type == LogType.error):
        colors.red()
        print("[error] ", end= '')
    elif (log_type == LogType.critical):
        colors.cyan()
        print("[critical] ", end= '')
    print("::", message)
    colors.reset()
    return
    
def log_print(message: str,
              log_type: LogType = LogType.warning):
    if (log_type >= __current_log_type):
        __log_pretty(message, log_type)
    return
    
def log_set_type(log_type: LogType = LogType.warning):
    global __current_log_type
    __current_log_type = log_type
    return

def log_disable():
    __current_log_type = LogType.__none
    return
