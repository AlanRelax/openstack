#!/usr/bin/env python

import MySQLdb
import sys
import re
import os

HOST = %HOST_IP%	# %HOST_IP% need to be replace by actual ldap server's IP

def login(user,pwd):
    import ldap
    con = ldap.initialize(HOST)
    try:
        s = con.simple_bind_s("anjuke\\"+user,pwd)
        if s:
            add_user(user,pwd)
        else:
            print "error"
    except ldap.INVALID_CREDENTIALS:
        print "your username or password is incorecct"
    except ldap.LDAPError, e:
       
        if type(e.message) == dict and e.message.has_key("desc"):
            print e.message["desc"]
        else:
           
            print e

def add_user(user,pw):
    os.system("sudo /data/stack/nova/bin/nova-manage service list")	# This command is for test, if it is ready to deploy, it will be repalce with keystone command


def check_name(user,pw):
    conn = MySQLdb.connect("localhost","openstack","0Penstack","keystone")
    cursor = conn.cursor()
    cursor.execute("""SELECT * FROM users""")
    while (1):
        row = cursor.fetchone()
        if row != None:
            if user == row[1]:
                print "Exits"
                break
        else:
            login(user,pw)
            break
    cursor.close()
    conn.close()

def ProcessCommandFlags(args):
  """
  Parse command line flags per specified usage, pick off key, value pairs
  All flags of type "--key=value" will be processed as __flags[key] = value,
                    "--option" will be processed as __flags[option] = option
  """

  flags = {}
  rkeyval = '--(?P<key>\S*)[=](?P<value>\S*)' # --key=val
  roption = '--(?P<option>\S*)'               # --key
  r = '(' + rkeyval + ')|(' + roption + ')'
  rc = re.compile(r)
  for a in args:
    try:
      rcg = rc.search(a).groupdict()
      if rcg.has_key('key'):
        flags[rcg['key']] = rcg['value']
      if rcg.has_key('option'):
        flags[rcg['option']] = rcg['option']
    except AttributeError:
      return None
  return flags
#end def ProcessCommandFlags


if __name__ == "__main__":
    flags = ProcessCommandFlags(sys.argv[1:])
    
    if not flags or not flags.has_key('username') or not flags.has_key('password'):
        print "error: please input username and password"
    else:
        user = flags["username"]
        pwd = flags["password"]
        if user and pwd:
#            login(user,pwd)
            check_name(user,pwd)
        else:
            print "error: please input username and password"
