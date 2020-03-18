#!/usr/bin/python3

import sys,os,unittest
from type import SearchRequest_pb2,Test_pb2
import chardet
from google.protobuf import json_format 
import json
class MyTestCase(unittest.TestCase):
    def test1NormalPB(self):
        request = SearchRequest_pb2.SearchRequest()
        request.page_number=100
        request.corpus=1
        request.ips.extend([1,2,3])
        print(request)

    def test2RecursivePB(self):
        response=SearchRequest_pb2.ResponseMsg()
        response.msg="resp"
        # response.center=SearchRequest_pb2.Pt()
        # response.pts.extend([SearchRequest_pb2.Pt(x=3,y=4) for i in range(3)])
        # for i in range(3):
        #     pt=SearchRequest_pb2.Pt()
        #     pt.x,pt.y=2*i,2*i+1
        #     response.pts.append(pt)
        # # for i in range(3):
        # #     response.pts.append(SearchRequest_pb2.Pt(3,4))
        # for i in range(3):
        #     response.pts.add(x=3,y=4)
        #     # pt.x,pt.y=3*i,3*i+1
        response.pts.extend([SearchRequest_pb2.Pt(x=3*i,y=4*i) for i in range(3)])
        print(response)

    def test3BufCharacter(self):
        request = SearchRequest_pb2.SearchRequest()
        request.page_number=100
        request.corpus=1
        request.ips.extend([1,2,3])
        request.query='123'
        print(request)
        buf=request.SerializeToString()
        encode=chardet.detect(buf)
        print(encode['encoding'])
        print(encode)

    def test4BufRead(self):
        request = SearchRequest_pb2.SearchRequest()
        request.page_number=100
        request.corpus=1
        request.ips.extend([1,2,3])
        request.query='123'

        req2=SearchRequest_pb2.SearchRequest()
        req2.ParseFromString(request.SerializeToString())
        print(req2)

        self.assertEqual(req2.page_number,100)
        print(type(req2.ips))
        # self.assertTrue(isinstance(req2.ips,list))

    def test5serialize_array(self):
        request=SearchRequest_pb2.SearchRequest()
        request.page_number=100
        request.corpus=1
        request.ips.extend([1,2,3])
        request.query='123'

        # array=request.SerializeToArray()
        # print(array)
    
    def test6oneof(self):
        person=Test_pb2.Person()
        person.name='cai'
        person.hgt=10
        person.wgt=20
        print(person.HasField('hgt'))
        print(person.HasField('wgt'))
        print(person)
        
        # js=json_format.MessageToJson(person)
        # j=json.loads(js)
        # print(j['wgt'])

        per_dict=json_format.MessageToDict(person)
        print(per_dict)

if __name__=='__main__':
    unittest.main()