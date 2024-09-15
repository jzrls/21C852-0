from os import system

cmd = f"""nuitka --standalone --onefile --remove-output --windows-console-mode=disable --mingw64 --windows-icon-from-ico={r'./asset/favicon.ico'} --show-progress --show-memory --enable-plugin=pyside6 --output-dir=dist .\main.py"""
system(cmd)
