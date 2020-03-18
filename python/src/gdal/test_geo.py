#!/usr/bin/python3

import sys, os, copy, unittest
from osgeo import ogr, osr, gdal
import numpy as np
from matplotlib import pyplot as plt


def plot_pl(pl):
    assert isinstance(pl, ogr.Geometry) and pl.GetGeometryType() == ogr.wkbLineString

    x = [pt[0] for pt in pl.GetPoints()]
    y = [pt[1] for pt in pl.GetPoints()]

    plt.plot(x, y)
    plt.axis([min(x) - 1, max(x) + 1, min(y) - 1, max(y) + 1])
    plt.show()

def plot_pg(pg):
    assert isinstance(pg, ogr.Geometry) and pg.GetGeometryType() == ogr.wkbPolygon
    rg=pg.GetGeometryRef(0)
    x=[pt[0] for pt in rg.GetPoints()]
    y=[pt[1] for pt in rg.GetPoints()]

    plt.plot(x, y)
    plt.axis([min(x) - 1, max(x) + 1, min(y) - 1, max(y) + 1])
    plt.show()

def plot_mpg(mpg):
    assert isinstance(mpg, ogr.Geometry) and mpg.GetGeometryType() == ogr.wkbMultiPolygon
    xs,ys=[],[]
    for i in range(mpg.GetGeometryCount()):
        pg=mpg.GetGeometryRef(i)
        rg=pg.GetGeometryRef(0)
        x=[pt[0] for pt in rg.GetPoints()]
        y=[pt[1] for pt in rg.GetPoints()]
        plt.plot(x, y)
        xs.extend(x)
        ys.extend(y)
    
    plt.axis([min(xs) - 1, max(xs) + 1, min(ys) - 1, max(ys) + 1])
    plt.show()


class MyTestCase(unittest.TestCase):

    def test1GeoSpatial(self):
        pt1 = ogr.Geometry(ogr.wkbPoint)
        pt1.AddPoint_2D(10, 20)

        pt2 = ogr.Geometry(ogr.wkbPoint)
        pt2.AddPoint_2D(20, 20)

        self.assertEqual(pt1.Distance(pt2), 10)

    def test2EmptyGeoWkt(self):
        geo1 = ogr.Geometry(ogr.wkbPoint)
        print("wkbPoint %s" % geo1.ExportToWkt())

        geo2 = ogr.Geometry(ogr.wkbPolygon)
        print("wkbPolygon %s" % geo2.ExportToWkt())

        geo3 = ogr.Geometry(ogr.wkbNDR)
        print("wkbNDR %s" % geo3.ExportToWkt())

        geo4 = ogr.Geometry(ogr.wkbGeometryCollection)
        print("wkbGeometryCollection %s" % geo4.ExportToWkt())

        geo5 = ogr.CreateGeometryFromWkt('GEOMETRYCOLLECTION EMPTY')

    def test3GeoZM(self):
        geo1 = ogr.Geometry(ogr.wkbPointM)
        geo1.AddPointM(10, 10, 1.23)
        s = geo1.ExportToWkt()
        geo2 = ogr.CreateGeometryFromWkt(s)

        self.assertEqual(geo1.GetM(), 1.23)
        self.assertEqual(geo2.GetM(), 0)

    def test4GeoBuffer(self):
        lr = ogr.Geometry(ogr.wkbLinearRing)
        lr.AddPoint_2D(10, 10)
        lr.AddPoint_2D(10, 20)
        lr.AddPoint_2D(20, 20)
        lr.AddPoint_2D(20, 10)
        lr.AddPoint_2D(10, 10)
        pg = ogr.Geometry(ogr.wkbPolygon)
        pg.AddGeometry(lr)
        # plot_geo(pg)

        pg_buffer = pg.Buffer(10)
        # plot_geo(pg_buffer)

        pg2 = pg_buffer.Buffer(-10)
        # plot_geo(pg2)

    def test5GeoUnion(self):
        lr = ogr.Geometry(ogr.wkbLinearRing)
        lr.AddPoint_2D(10, 12)
        lr.AddPoint_2D(12, 10)
        lr.AddPoint_2D(32, 30)
        lr.AddPoint_2D(30, 32)
        lr.AddPoint_2D(10, 12)
        pg1 = ogr.Geometry(ogr.wkbPolygon)
        pg1.AddGeometry(lr)

        lr = ogr.Geometry(ogr.wkbLinearRing)
        lr.AddPoint_2D(30, 10)
        lr.AddPoint_2D(32, 12)
        lr.AddPoint_2D(12, 32)
        lr.AddPoint_2D(10, 30)
        lr.AddPoint_2D(30, 10)
        pg2 = ogr.Geometry(ogr.wkbPolygon)
        pg2.AddGeometry(lr)

        lr=ogr.Geometry(ogr.wkbLinearRing)
        lr.AddPoint_2D(10, 25)
        lr.AddPoint_2D(32, 25)
        lr.AddPoint_2D(32, 27)
        lr.AddPoint_2D(10, 27)
        lr.AddPoint_2D(10, 25)
        pg3=ogr.Geometry(ogr.wkbPolygon)
        pg3.AddGeometry(lr)

        inter = pg1.Union(pg2)
        inter=inter.Union(pg3)
        inter_buffer=inter.Buffer(3)
        inter2=inter_buffer.Buffer(-3)

        # plot_geo([[pg1,pg2,pg3],[inter,inter_buffer,inter2]],2,3)

    def test6PlineWithHole(self):
        lr1 = ogr.Geometry(ogr.wkbLinearRing)
        lr1.AddPoint_2D(10, 10)
        lr1.AddPoint_2D(10, 20)
        lr1.AddPoint_2D(20, 20)
        lr1.AddPoint_2D(20, 10)
        lr1.AddPoint_2D(10, 10)
        pg1=ogr.Geometry(ogr.wkbPolygon)
        pg1.AddGeometry(lr1)

        pg2=pg1.Buffer(-2)
        pg2.AddGeometry(lr1)
        # print(pg2.ExportToWkt())
        # plot_geo([pg1,pg2],1,2)
    
    def test7MpgCascadeUnion(self):
        mpg=ogr.Geometry(ogr.wkbMultiPolygon)
        pgs=[]
        for i in range(-2,2):
            rg=ogr.Geometry(ogr.wkbLinearRing)
            rg.AddPoint_2D(2+i,2+i)
            rg.AddPoint_2D(4+i,2+i)
            rg.AddPoint_2D(4+i,4+i)
            rg.AddPoint_2D(2+i,4+i)
            rg.AddPoint_2D(2+i,2+i)
            pg=ogr.Geometry(ogr.wkbPolygon)
            pg.AddGeometry(rg)
            mpg.AddGeometry(pg)
            pgs.append(pg)
        rg=ogr.Geometry(ogr.wkbLinearRing)
        rg.AddPoint_2D(10,10)
        rg.AddPoint_2D(14,10)
        rg.AddPoint_2D(14,14)
        rg.AddPoint_2D(10,14)
        rg.AddPoint_2D(10,10)
        pg=ogr.Geometry(ogr.wkbPolygon)
        pg.AddGeometry(rg)
        mpg.AddGeometry(pg)
        plot_mpg(mpg)

        union=mpg.UnionCascaded()
        print(union.ExportToWkt())
        # plot_pg(union)



if __name__ == '__main__':
    unittest.main()
