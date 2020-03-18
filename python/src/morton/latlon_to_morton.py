#!/usr/bin/python3

import sys,os,unittest,pymorton

class MyUnitTest(unittest.TestCase):
    def test_1_lonlat2morton(self):
        lat,lon=40.07310025,116.25592587
        morton=pymorton.interleave_latlng(lat,lon)
        print(morton)

        lat2,lon2=pymorton.deinterleave_latlng(morton)

        print(lat2,lon2)

if __name__=='__main__':
    unittest.main()