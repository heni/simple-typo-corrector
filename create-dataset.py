#!/usr/bin/env python2.7
from __future__ import print_function
import os
import re
import urllib2
from bs4 import BeautifulSoup

SourceLinks = frozenset( ln.strip() for ln in """
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1040.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1050.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1100.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1190.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1200.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1210.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1240.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1260.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1270.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1280.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1290.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1300.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1310.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1320.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1330.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1340.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1360.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1360-1.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1370.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1380.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1390.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1400.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1430.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1440.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1450.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_1480.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_0880.shtml
http://az.lib.ru/t/tolstoj_lew_nikolaewich/text_0960.shtml
""".split() if ln.strip())


def ExtractCharset(ContentType, default="latin1"):
    sRes = re.search("charset=([\w-]+)", ContentType)
    return sRes.group(1) if sRes else default


def CreateText(link, dest_file):
    with open(dest_file, "w") as dest_prn:
        docReader = urllib2.urlopen(link)
        charset = ExtractCharset(docReader.headers["content-type"], "utf-8")
        print("proccess link {0} to {1} charset={2}".format(link, dest_file, charset))
        print(BeautifulSoup(docReader.read().decode(charset)).text.encode("utf-8"), file=dest_prn)


if __name__ == "__main__":
    DataDir = "dataset"
    for i, link in enumerate(SourceLinks):
        CreateText(link, dest_file=os.path.join(DataDir, "paper-{0:03d}.txt".format(i+1)))
