
#   # os模块
#
#   这个模块包含普遍的操作系统功能. 如果你希望你的程序能够与平台无关的话, 这个模块
#   是尤为重要的. 即它允许一个程序在编写后不需要任何改动, 也不会发生任何问题, 就可
#   以在Linux和Windows下运行. 一个例子就是使用os.sep可以取代操作系统特定的路径分割
#   符.
#
#
#   下面列出了一些在os模块中比较有用的部分. 它们中的大多数都简单明了.
#
#       * os.name字符串指示你正在使用的平台. 比如对于Windows, 它是'nt', 而对于
#         Linux/Unix用户, 它是'posix'.
#       * os.getcwd()函数得到当前工作目录, 即当前Python脚本工作的目录路径.
#       * os.getenv()和os.putenv()函数分别用来读取和设置环境变量.
#       * os.listdir()返回指定目录下的所有文件和目录名.
#       * os.remove()函数用来删除一个文件.
#       * os.system()函数用来运行shell命令.
#       * os.linesep字符串给出当前平台使用的行终止符. 例如, Windows使用'\r\n',
#         Linux使用'\n'而Mac使用'\r'.
#       * os.path.split()函数返回一个路径的目录名和文件名.
#             >>> os.path.split('/home/swaroop/byte/code/poem.txt')
#             ('/home/swaroop/byte/code', 'poem.txt')
#       * os.path.isfile()和os.path.isdir()函数分别检验给出的路径是一个文件还是目
#         录.
#       * os.path.existe()函数用来检验给出的路径是否真地存在.
#
#   你可以利用Python标准文档去探索更多有关这些函数和变量的详细知识. 你也可以使用
#   help(sys)等等.
