InlineHook.dll文件放在E:\盘下
保护进程和结束保护必须先运行C:\Windows\SysWOW64\Taskmgr.exe(win10-64位，32位直接运行任务管理器)
 win7运行需要修改代码DialogA.cpp下的 HWND hwnd = ::FindWindow(NULL, L"任务管理器");（获取任务管理器进程ID方法，修改成win7的任务管理器的名称）

	
