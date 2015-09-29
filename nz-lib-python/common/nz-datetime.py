# -*- coding: utf-8 -*-


'common function of date and time'


from datetime import datetime


def get_cur_time(strTimeFormat):
    'example: get_cur_time("%Y%m%d")'
    curTime = datetime.now()
    return curTime.strftime(strTimeFormat)


def is_valid_date(strDate, strDateFormat="%Y%m%d"):
    'example: is_valid_date("2015-09-08", "%Y-%m-%d")'
    try:
        datetime.strptime(strDate, strDateFormat)
        return True
    except:
        return False


if __name__ == "__main__":
    print(get_cur_time("%Y%m%d"))
    print(is_valid_date("20150902"))
