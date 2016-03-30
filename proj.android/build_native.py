#!/usr/bin/python
# build_native.py
# Build native codes
# 
# Please use cocos console instead


import sys
import os, os.path
import shutil
from optparse import OptionParser

def build(build_mode):


    current_dir = os.path.dirname(os.path.realpath(__file__))
    cocos_root = os.path.join(current_dir, "../cocos2d")

    app_android_root = os.path.join(current_dir, "../")
		
    if build_mode is None:
    	  build_mode = 'debug'
    elif build_mode != 'release':
        build_mode = 'debug'
    
    command = 'cocos compile -p android -s %s -m %s' % (app_android_root, build_mode) 
    if os.system(command) != 0:
        print "renaming JavaOCer.cpp to JavaOCer.mm"
        os.system("mv ../Classes/Util/JavaOCer.cpp ../Classes/Util/JavaOCer.mm")
        print "moving ShareSDK.xml back"
        os.system("mv ShareSDK.xml assets")
        raise Exception("Build dynamic library for project [ " + app_android_root + " ] fails!")

# -------------- main --------------
if __name__ == '__main__':

    print "renaming JavaOCer.mm to JavaOCer.cpp"
    os.system("mv ../Classes/Util/JavaOCer.mm ../Classes/Util/JavaOCer.cpp")
    print "moving ShareSDK.xml to safety, I think"
    os.system("mv assets/ShareSDK.xml ShareSDK.xml")
    
    parser = OptionParser()
    parser.add_option("-n", "--ndk", dest="ndk_build_param", help='it is not used', action="append")
    parser.add_option("-p", "--platform", dest="android_platform", 
    help='it is not used')
    parser.add_option("-b", "--build", dest="build_mode", 
    help='the build mode for java project,debug[default] or release.Get more information,please refer to http://developer.android.com/tools/building/building-cmdline.html')
    (opts, args) = parser.parse_args()
    
    print "Please use cocos console instead.\n"
    build(opts.build_mode)
    
    print "renaming JavaOCer.cpp to JavaOCer.mm"
    os.system("mv ../Classes/Util/JavaOCer.cpp ../Classes/Util/JavaOCer.mm")
    print "moving ShareSDK.xml back"
    os.system("mv ShareSDK.xml assets")

