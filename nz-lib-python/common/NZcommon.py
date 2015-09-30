# -*- coding: utf-8 -*-


'common function'


__author__ = 'naturezhang'


import os
import sys


def get_cur_file_dir():
     '获取脚本当前路径'
     path = sys.path[0]
     #判断为脚本文件还是py2exe编译后的文件，如果是脚本文件，则返回的是脚本的目录，如果是py2exe编译后的文件，则返回的是编译后的文件路径
     if os.path.isdir(path):
         return path
     elif os.path.isfile(path):
         return os.path.dirname(path)
