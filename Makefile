format:
	forfiles /P src   /M *.cpp /S /C "cmd /C clang-format -i -style=file @path"
	forfiles /P src   /M *.h   /S /C "cmd /C clang-format -i -style=file @path"
	forfiles /P tests /M *.cpp /S /C "cmd /C clang-format -i -style=file @path"
	forfiles /P tests /M *.h   /S /C "cmd /C clang-format -i -style=file @path"
