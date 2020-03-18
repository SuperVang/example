#!/usr/bin/python3

import sys, os, unittest
import construct, struct
import numpy as np
from matplotlib import pyplot as plt


class MyTestCase(unittest.TestCase):
    def test1Struct(self):
        format = construct.Struct("width" / construct.Int32sb,
                                  "height" / construct.Int32sb)
        bin = format.build(dict(width=3, height=4))

        par = format.parse(bin)

        assert isinstance(par, construct.Container)
        self.assertEqual(format.sizeof(), 8)

        width = par['width']
        height = par['height']

        self.assertEqual(width, 3)
        self.assertEqual(height, 4)

    def test2RecursiveStruct(self):
        pt_format = construct.Struct("x" / construct.Double, "y" / construct.Double)
        line_format = construct.Struct('s' / pt_format, 'e' / pt_format)

        pt1_bin = pt_format.build(dict(x=1, y=1))
        pt2_bin = pt_format.build(dict(x=2, y=2))
        line_bin = line_format.build(dict(s=dict(x=1, y=1), e=dict(x=2, y=2)))

        pt1 = pt_format.parse(pt1_bin)
        pt2 = pt_format.parse(pt2_bin)
        line = line_format.parse(line_bin)

        self.assertEqual(line['s']['x'], pt1['x'])
        self.assertEqual(line['e']['y'], pt2['y'])

        # // *小段存储
        # // *开始8字节magic,之后8字节为后面buffer size(size不包含当前size本身空间)
        # // *剩余为type(4 bytes) / length(8 bytes) / value(length bytes)
        # // *type: 0
        # // *轨迹线, 固定长度, MapPoint数组, 个数 = length / sizeof(MapPoint), MapPoint数据格式如下:
        # // *struct MapPoint {
        # // * Point center;
        # // *Point left;
        # // *Point right;
        # // *double heading;
        # // *double laneWidth;
        # // *};
        # // *type: 1
        # // *停车位, 变长, 数据格式如下:
        # // *struct ParkingLotData {
        # // * int id;
        # // *Point center;
        # // *Point position;
        # // *Point vertices[4];
        # // *int pointNumOfBacking;
        # // *Point * pointsOfBacking;
        # // *};
        # // *其中Point结构体如下:
        # // *struct Point {
        # // * double x;
        # // *double y;
        # // *double angle;
        # // *};

    def test2LoadParking(self):
        point_format = construct.Struct("x" / construct.Float64l, "y" / construct.Float64l,
                                        "angle" / construct.Float64l)

        print("Point Size : ", point_format.sizeof())

        map_point_format = construct.Struct("center" / point_format, "left" / point_format, "right" / point_format,
                                            "heading" / construct.Float64l, "lanewidth" / construct.Float64l)

        print("Map Point Size : ", map_point_format.sizeof())

        parking_format = construct.Struct("id" / construct.Int32sl,
                                          "center" / point_format,
                                          "position" / point_format,
                                          "vertices" / construct.Array(4, point_format),
                                          "pointNumOfBacking" / construct.Int32sl)

        # print("Map Point Size : ",parking_format.sizeof())

        with open('../../data/parkingLot_20190823.pk', 'rb') as f:
            data = f.read()
            print(len(data))
            magic = data[0:8]
            magic2=construct.CString(encoding='utf-8').parse(data[0:8])
            print(magic2)
            print("Magic Word", magic)
            buffer_size = struct.unpack('<Q', data[8:16])
            buffer_size2=construct.Int64ul.parse(data[8:16])
            print("Buffer Size : ", buffer_size)
            print("Buffer Size2 : ",buffer_size2)
            self.assertEqual(buffer_size[0],buffer_size2)
            val = data[16:]

            tpe = struct.unpack('<I', val[0:4])
            self.assertEqual(construct.Int32sl.parse(val[0:4]),tpe[0])
            print("Type ", tpe)

            length = struct.unpack('<q', val[4:12])
            self.assertEqual(construct.Int64ul.parse(val[4:12]),length[0])
            print("Length ", length)

            traj_val = val[12:length[0] + 12]

            traj_num = int(length[0] / map_point_format.sizeof())

            trajs_format = construct.Struct("trajs" / construct.Array(traj_num, map_point_format))

            traj = trajs_format.parse(traj_val)
            assert isinstance(traj, construct.Container)
            x, y = [], []
            for mp in traj['trajs']:
                x.append(mp['center']['x'])
                y.append(mp['center']['y'])
                # print(mp)
            plt.plot(x, y)
            plt.axis([-100, 100, -100, 100])
            plt.show()

            tpe = struct.unpack('<I', val[length[0] + 12:length[0] + 16])
            print("Type ", tpe)
            par_length = struct.unpack('<q', val[length[0] + 16:length[0] + 24])
            print("Length ", par_length)

            self.assertEqual(length[0] + par_length[0] + 24, buffer_size[0])

            # tmp_val = val[length[0] + 24:length[0] + par_length[0] + 24]
            # tmp_offset = 0
            # while tmp_offset < par_length[0]:

            #     solid_parking_size = parking_format.sizeof()
            #     point_size = point_format.sizeof()

            #     id = struct.unpack('<I', tmp_val[tmp_offset:tmp_offset + 4])
            #     tmp_offset = tmp_offset + 4
            #     for index in range(6):
            #         pt = point_format.parse(tmp_val[tmp_offset:tmp_offset + point_size])
            #         tmp_offset = tmp_offset + point_size
            #     back_num = struct.unpack('<I', tmp_val[tmp_offset:tmp_offset + 4])
            #     tmp_offset = tmp_offset + 4

            #     for index in range(back_num):
            #         pt = point_format.parse(tmp_val[tmp_offset:tmp_offset + point_size])
            #         tmp_offset = tmp_offset + point_size


if __name__ == '__main__':
    unittest.main()
