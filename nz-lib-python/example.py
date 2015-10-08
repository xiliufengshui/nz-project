# -*- coding: utf-8 -*-


'just for test'


__author__ = 'naturezhang'


# 增加路径
import sys
sys.path.append('/data/home/naturezhang/nz-project/nz-lib-python/common')
sys.path.append('/data/home/naturezhang/nz-project/nz-lib-python/external-packages')

# 加载使用模块
import NZmysql


if __name__ == '__main__':
    NZmysql.execute_sql('select * from users', 'test', '10.240.130.213', 'root', 'test')
    
