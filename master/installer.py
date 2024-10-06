from os import system

cmd = (
    f"""nuitka --standalone --onefile --remove-output --report=report.xml --windows-disable-console --mingw64 --windows-icon-from-ico={r'./asset/favicon.ico'} --show-progress --show-memory --enable-plugin=pyside6 --enable-plugin=numpy --output-dir=dist .\main.py"""
)
print(cmd)
if int(system(cmd)) == 0:
    print("构建成功")
else:
    print("构建失败")
