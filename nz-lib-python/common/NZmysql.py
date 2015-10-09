# -*- coding: utf-8 -*-


'common function of mysql'


__author__ = 'naturezhang'


# import NZcommon
# import sys

# print(str(NZcommon.get_cur_file_dir()))
# sys.path.append(str(NZcommon.get_cur_file_dir()) + '/../external-packages')

import pymysql.cursors

     
def execute_sql(strSql, strDB, strHost, strUser, strPassword):
    connection = pymysql.connect(host = strHost,
                                 user = strUser,
                                 password = strPassword,
                                 db = strDB,
                                 charset='utf8mb4',
                                 cursorclass=pymysql.cursors.DictCursor)
    try:
        with connection.cursor() as cursor:
            # Read a single record
            cursor.execute(strSql)
            # 查询一条记录
            # result = cursor.fetchone()
            # 查询多条记录
            # result = cursor.fetchall()
            # print(result)
            return cursor.fetchall()
    finally:
        connection.close()
        
    
if __name__ == "__main__":
    pass

