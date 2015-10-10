# -*- coding: utf-8 -*-


'common function of date and time'


__author__ = 'naturezhang'


from datetime import datetime, timedelta


def get_cur_time(strTimeFormat="%Y%m%d"):
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


def add_one_day(strSrcDate, strDateFormat="%Y%m%d"):
    'example: add_one_day("2015-09-09", "%Y-%m-%d")'
    dateTmp = datetime.strptime(strSrcDate, strDateFormat)
    dateTmp = dateTmp + timedelta(days=1)
    return dateTmp.strftime("%Y%m%d")


def strdate_to_timestamp(strSrcDate, strDateFormat="%Y%m%d"):
    'example: date_to_unixtime("2015-09-09", "%Y-%m-%d")'
    dateTmp = datetime.strptime(strSrcDate, strDateFormat)
    return dateTmp.timestamp()


def timestamp_to_strdate(fTimestamp, strDateFormat):
    'example: timestamp_to_strdate()'
    datetimeTmp = datetime.fromtimestamp(fTimestamp)
    return datetimeTmp.strftime(strDateFormat)


if __name__ == "__main__":
    print(get_cur_time())
    print(get_cur_time("%Y%m%d"))
    print(is_valid_date("20150902"))
