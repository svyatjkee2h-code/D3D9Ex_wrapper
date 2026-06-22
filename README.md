This is a simple wrapper of Direct3D9Ex functions on top of Direct3D9 for Windows 2000/XP/2003.
By design it should replace your original d3d9.dll in System32 folder but you have to rename your original dll to d3d9base.dll to allow our wrapper to work.

Use it at your own risk. Nobody is responsible for any damages!

How to use:
1. You need to make sure you have disable Windows System File Checker (SFC).
2. Backup your original d3d9.dll from System32/SysWOW64 folder
3. Place this wrapper d3d9.dll into System32/SysWOW64 using corresponding architectures
4. Place your original d3d9.dll into System32/SysWOW64 being renamed to d3d9base.dll
