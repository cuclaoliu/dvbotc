/***************************************************************************
 *   Copyright (C) 2007 by Damien Phillip Morrissey,,,   *
 *   dpm95@uow.edu.au   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
int pilot_set[][176]= {
	{0, 12, 24, 36, 48, 54, 60, 72, 84, 87, 96, 108, 120, 132, 141, 144, 156, 168, 180, 192, 201, 204, 216, 228, 240, 252, 255, 264, 276, 279, 282, 288, 300, 312, 324, 333, 336, 348, 360, 372, 384, 396, 408, 420, 432, 444, 450, 456, 468, 480, 483, 492, 504, 516, 525, 528, 531, 540, 552, 564, 576, 588, 600, 612, 618, 624, 636, 648, 660, 672, 684, 696, 708, 714, 720, 732, 744, 756, 759, 765, 768, 780, 792, 804, 816, 828, 840, 852, 864, 873, 876, 888, 900, 912, 918, 924, 936, 939, 942, 948, 960, 969, 972, 984, 996, 1008, 1020, 1032, 1044, 1050, 1056, 1068, 1080, 1092, 1101, 1104, 1107, 1110, 1116, 1128, 1137, 1140, 1146, 1152, 1164, 1176, 1188, 1200, 1206, 1212, 1224, 1236, 1248, 1260, 1269, 1272, 1284, 1296, 1308, 1320, 1323, 1332, 1344, 1356, 1368, 1377, 1380, 1392, 1404, 1416, 1428, 1440, 1452, 1464, 1476, 1488, 1491, 1500, 1512, 1524, 1536, 1548, 1560, 1572, 1584, 1596, 1608, 1620, 1632, 1644, 1656, 1668, 1680, 1683, 1692, 1704},
	{0, 3, 15, 27, 39, 48, 51, 54, 63, 75, 87, 99, 111, 123, 135, 141, 147, 156, 159, 171, 183, 192, 195, 201, 207, 219, 231, 243, 255, 267, 279, 282, 291, 303, 315, 327, 333, 339, 351, 363, 375, 387, 399, 411, 423, 432, 435, 447, 450, 459, 471, 483, 495, 507, 519, 525, 531, 543, 555, 567, 579, 591, 603, 615, 618, 627, 636, 639, 651, 663, 675, 687, 699, 711, 714, 723, 735, 747, 759, 765, 771, 780, 783, 795, 804, 807, 819, 831, 843, 855, 867, 873, 879, 888, 891, 903, 915, 918, 927, 939, 942, 951, 963, 969, 975, 984, 987, 999, 1011, 1023, 1035, 1047, 1050, 1059, 1071, 1083, 1095, 1101, 1107, 1110, 1119, 1131, 1137, 1140, 1143, 1146, 1155, 1167, 1179, 1191, 1203, 1206, 1215, 1227, 1239, 1251, 1263, 1269, 1275, 1287, 1299, 1311, 1323, 1335, 1347, 1359, 1371, 1377, 1383, 1395, 1407, 1419, 1431, 1443, 1455, 1467, 1479, 1491, 1503, 1515, 1527, 1539, 1551, 1563, 1575, 1587, 1599, 1611, 1623, 1635, 1647, 1659, 1671, 1683, 1695, 1704},
	{0, 6, 18, 30, 42, 48, 54, 66, 78, 87, 90, 102, 114, 126, 138, 141, 150, 156, 162, 174, 186, 192, 198, 201, 210, 222, 234, 246, 255, 258, 270, 279, 282, 294, 306, 318, 330, 333, 342, 354, 366, 378, 390, 402, 414, 426, 432, 438, 450, 462, 474, 483, 486, 498, 510, 522, 525, 531, 534, 546, 558, 570, 582, 594, 606, 618, 630, 636, 642, 654, 666, 678, 690, 702, 714, 726, 738, 750, 759, 762, 765, 774, 780, 786, 798, 804, 810, 822, 834, 846, 858, 870, 873, 882, 888, 894, 906, 918, 930, 939, 942, 954, 966, 969, 978, 984, 990, 1002, 1014, 1026, 1038, 1050, 1062, 1074, 1086, 1098, 1101, 1107, 1110, 1122, 1134, 1137, 1140, 1146, 1158, 1170, 1182, 1194, 1206, 1218, 1230, 1242, 1254, 1266, 1269, 1278, 1290, 1302, 1314, 1323, 1326, 1338, 1350, 1362, 1374, 1377, 1386, 1398, 1410, 1422, 1434, 1446, 1458, 1470, 1482, 1491, 1494, 1506, 1518, 1530, 1542, 1554, 1566, 1578, 1590, 1602, 1614, 1626, 1638, 1650, 1662, 1674, 1683, 1686, 1698, 1704},
	{0, 9, 21, 33, 45, 48, 54, 57, 69, 81, 87, 93, 105, 117, 129, 141, 153, 156, 165, 177, 189, 192, 201, 213, 225, 237, 249, 255, 261, 273, 279, 282, 285, 297, 309, 321, 333, 345, 357, 369, 381, 393, 405, 417, 429, 432, 441, 450, 453, 465, 477, 483, 489, 501, 513, 525, 531, 537, 549, 561, 573, 585, 597, 609, 618, 621, 633, 636, 645, 657, 669, 681, 693, 705, 714, 717, 729, 741, 753, 759, 765, 777, 780, 789, 801, 804, 813, 825, 837, 849, 861, 873, 885, 888, 897, 909, 918, 921, 933, 939, 942, 945, 957, 969, 981, 984, 993, 1005, 1017, 1029, 1041, 1050, 1053, 1065, 1077, 1089, 1101, 1107, 1110, 1113, 1125, 1137, 1140, 1146, 1149, 1161, 1173, 1185, 1197, 1206, 1209, 1221, 1233, 1245, 1257, 1269, 1281, 1293, 1305, 1317, 1323, 1329, 1341, 1353, 1365, 1377, 1389, 1401, 1413, 1425, 1437, 1449, 1461, 1473, 1485, 1491, 1497, 1509, 1521, 1533, 1545, 1557, 1569, 1581, 1593, 1605, 1617, 1629, 1641, 1653, 1665, 1677, 1683, 1689, 1701, 1704}
};

float refsig_w[1705] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1};