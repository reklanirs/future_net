# -*- coding: UTF-8 -*-
from __future__ import division
import os
import subprocess2
import time
import sys, traceback


def check(path):
    data = []
    with open(os.path.join(path,u'myresult.csv'),"r") as f:
        line = f.readlines()[0].strip()
        if line == 'NA':
            return -1
        data = [int(x) for x in line.split("|")]

    edge = {}
    with open(os.path.join(path,u'topo.csv'),"r") as f:
        for l in f.readlines():

            tmp = [int(x) for x in l.strip().split(",")]
            # print tmp
            edge[tmp[0]] = tmp

    st,ed,must=0,0,[]
    with open(os.path.join(path,u'demand.csv'),"r") as f:
        line = f.readlines()[0].strip().split(",")
        st = int(line[0])
        ed = int(line[1])
        must = [int(x) for x in line[2].split("|")]
    road = [st]
    p = st
    sum = 0
    # print edge.keys()
    for eid in data:
        if eid not in edge.keys():
            print "unknow edge id ", eid
            return -1
        if edge[eid][1] != p:
            print "edge ", eid," start with ",edge[eid][1],"but now at ",p
            return -1
        sum += edge[eid][3]
        p = edge[eid][2]
        road.append(p)
        
    print [x for x in road if x in must]
    
    
    sroad = set(road)
    if len(sroad) != len(road):
        print "dupicate in road",road
        retu -1
    for p in must:
        if p not in sroad:
            print p,"is not in road",road
            return -1
    if road[-1] != ed:
        print "end with ", road[-1]," not ",ed
        return -1
    return sum

def main():
    cnt=0
    success = 0
    score = 0.0
    TIMEOUT = 10
    FNULL = open(os.devnull, 'w')

    for dirName, subdirList, fileList in os.walk(u"../test-case/"):

        try:
            if 'topo.csv' in fileList:
                cnt += 1
                path = os.path.abspath(dirName)
                print('enter test-case: %s' % dirName)
                cmd = ['./bin/Release/future.exe',
                    os.path.join(path,'topo.csv'),
                    os.path.join(path,'demand.csv'),
                    os.path.join(path,'myresult.csv'),
                ]
                sample = filter(lambda x: u'权值' in x, fileList)

                if sample:
                    # print sample[0].encode('UTF-8')
                    sample = int(sample[0].split(u"_")[1])
                    # print sample
                else:
                    sample = -1
                if sample == 100000:
                    sample = -1
                print "sample is ", sample
                code = subprocess2.Popen(cmd).waitOrTerminate(TIMEOUT)
                # code = subprocess2.Popen(cmd).waitUpTo(TIMEOUT)

                if code['returnCode'] != 0:
                    print "exit code not 0 but ",code['returnCode']
                    if sample == -1:
                        print "no solution"
                        success += 1
                        score += 1
                else:
                    if os.path.exists(os.path.join(path,'myresult.csv')):
                        sum = check(path)
                    if sum != -1:

                        print "success find ",sum," best may be ", sample
                        success += 1
                        score += sample/sum if sample != -1 else 1

                print success,"/",cnt, "success"
                print "-"*20
                # break
        except Exception,e:
            print "error!!!!"
            traceback.print_exc(file=sys.stdout)


    print "final score", score/cnt
if __name__ == '__main__':
    print "start"
    main()
