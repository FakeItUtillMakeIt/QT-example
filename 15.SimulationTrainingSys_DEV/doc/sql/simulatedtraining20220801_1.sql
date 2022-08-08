/*
 Navicat Premium Data Transfer

 Source Server         : localhost_3306
 Source Server Type    : MySQL
 Source Server Version : 80015
 Source Host           : localhost:3306
 Source Schema         : simulatedtraining

 Target Server Type    : MySQL
 Target Server Version : 80015
 File Encoding         : 65001

 Date: 01/08/2022 19:04:13
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for command_device_info
-- ----------------------------
DROP TABLE IF EXISTS `command_devicestatus_info`;
CREATE TABLE `command_devicestatus_info`  (
  `id` int(11) NOT NULL,
  `command_id` int(11) NOT NULL COMMENT '外键应用command_info',
  `deviceStatus_id` int(11) NOT NULL COMMENT '外键引用devicestatus_info',
  `createTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录创建时间',
  `lastUpdateTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of command_device_info
-- ----------------------------
INSERT INTO `command_devicestatus_info` VALUES (1, 1, 1, '2022-08-01 19:04:02', '2022-08-01 19:04:02');

-- ----------------------------
-- Table structure for command_info
-- ----------------------------
DROP TABLE IF EXISTS `command_info`;
CREATE TABLE `command_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `rocket_id` int(11) NOT NULL COMMENT '外键引用rocket_info.id',
  `back_id` int(11) NOT NULL DEFAULT 0 COMMENT '测发回令, 默认为0,外键引用command_info.id',
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '指令名称',
  `code` int(11) NOT NULL COMMENT '指令编码',
  `type` int(11) NOT NULL DEFAULT 1 COMMENT '指令类型：1表示测发指令、2表示测发回令、3表示故障指令',
  `prefix` int(11) NULL DEFAULT 43605 COMMENT '指令前缀，例如0x55AA',
  `createTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录创建时间',
  `lastUpdateTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 13 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of command_info
-- ----------------------------
INSERT INTO `command_info` VALUES (1, 2, 3, '综控机上电', 1, 1, 43605, '2022-07-23 16:49:14', '2022-07-23 16:49:14');
INSERT INTO `command_info` VALUES (2, 2, 3, '伺服上电', 2, 1, 43605, '2022-07-23 16:50:35', '2022-07-23 16:50:35');
INSERT INTO `command_info` VALUES (3, 2, 0, '测发回令', 3, 2, 47957, '2022-07-23 18:12:55', '2022-07-23 18:12:55');
INSERT INTO `command_info` VALUES (8, 2, 0, '故障指令1', 4, 3, 43605, '2022-07-27 11:26:37', '2022-07-27 11:26:37');
INSERT INTO `command_info` VALUES (9, 2, 0, '故障指令2', 9, 3, 43605, '2022-07-27 11:28:07', '2022-07-27 11:28:07');
INSERT INTO `command_info` VALUES (10, 2, 0, '故障指令3', 10, 3, 43605, '2022-07-27 11:28:49', '2022-07-27 11:28:49');
INSERT INTO `command_info` VALUES (11, 2, 0, '故障指令4', 11, 3, 43605, '2022-07-27 11:43:31', '2022-07-27 11:43:31');
INSERT INTO `command_info` VALUES (12, 2, 0, '故障指令5', 12, 3, 43605, '2022-07-27 11:43:47', '2022-07-27 11:43:47');

-- ----------------------------
-- Table structure for command_param_info
-- ----------------------------
DROP TABLE IF EXISTS `command_param_info`;
CREATE TABLE `command_param_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `command_id` int(11) NULL DEFAULT NULL COMMENT '测发指令ID,外键引用command_info.id',
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '参数名称',
  `code` int(255) NULL DEFAULT NULL COMMENT '参数编码,例如0x01',
  `index` int(11) NULL DEFAULT NULL COMMENT '索引顺序，从1开始计数',
  `length` int(11) NULL DEFAULT NULL COMMENT '参数字节长度',
  `resultType` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '参数类型包括：unsigned char、unsigned short、unsigned int、char、short、int、float',
  `defaultValue` float(255, 0) NULL DEFAULT NULL COMMENT '默认值',
  `createTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录创建时间',
  `lastUpdateTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 2 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of command_param_info
-- ----------------------------
INSERT INTO `command_param_info` VALUES (1, 3, '执行情况', 1, 1, 1, '1', 1, '2022-07-23 18:14:01', '2022-07-23 18:14:01');

-- ----------------------------
-- Table structure for compgraphic
-- ----------------------------
DROP TABLE IF EXISTS `compgraphic`;
CREATE TABLE `compgraphic`  (
  `componnetName` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `posX` double(255, 0) NULL DEFAULT NULL,
  `posY` double(255, 0) NULL DEFAULT NULL,
  `bindParam` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  PRIMARY KEY (`componnetName`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of compgraphic
-- ----------------------------
INSERT INTO `compgraphic` VALUES ('button', 123, 140, '0');
INSERT INTO `compgraphic` VALUES ('label', 105, 72, '1');
INSERT INTO `compgraphic` VALUES ('plot', 97, 168, '');

-- ----------------------------
-- Table structure for device_info
-- ----------------------------
DROP TABLE IF EXISTS `device_info`;
CREATE TABLE `device_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `rocket_id` int(11) NULL DEFAULT 2 COMMENT '外键引用rocket_info.id',
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '设备名称',
  `isVirtual` int(11) NULL DEFAULT 1 COMMENT '0表示真实设备，1表示虚拟设备',
  `createTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录创建时间',
  `lastUpdateTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 31 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of device_info
-- ----------------------------
INSERT INTO `device_info` VALUES (4, 2, '惯组', 1, '2022-07-27 19:43:44', '2022-07-27 19:43:44');
INSERT INTO `device_info` VALUES (5, 2, '二级伺服系统', 1, '2022-07-27 20:05:13', '2022-07-27 20:05:13');
INSERT INTO `device_info` VALUES (6, 2, '一级伺服电池', 1, '2022-07-27 20:09:36', '2022-07-27 20:09:36');
INSERT INTO `device_info` VALUES (7, 2, '二级伺服电池', 1, '2022-07-27 20:15:22', '2022-07-27 20:15:22');
INSERT INTO `device_info` VALUES (8, 2, '传感器', 1, '2022-07-27 20:18:33', '2022-07-27 20:18:33');
INSERT INTO `device_info` VALUES (9, 2, '射频图像', 1, '2022-07-27 20:23:34', '2022-07-27 20:23:34');
INSERT INTO `device_info` VALUES (10, 2, '姿控电磁阀', 1, '2022-07-27 20:26:17', '2022-07-27 20:26:17');
INSERT INTO `device_info` VALUES (11, 2, '综控机', 1, '2022-07-27 20:28:49', '2022-07-27 20:28:49');
INSERT INTO `device_info` VALUES (12, 2, '姿控动力系统', 1, '2022-07-27 20:35:45', '2022-07-27 20:35:45');
INSERT INTO `device_info` VALUES (13, 2, '姿控发动机', 1, '2022-07-27 20:42:54', '2022-07-27 20:42:54');
INSERT INTO `device_info` VALUES (14, 2, '动力一级发动机', 1, '2022-07-27 20:46:02', '2022-07-27 20:46:02');
INSERT INTO `device_info` VALUES (15, 2, '动力二级发动机', 1, '2022-07-27 20:52:03', '2022-07-27 20:52:03');
INSERT INTO `device_info` VALUES (16, 2, '动力三级发动机', 1, '2022-07-27 20:58:13', '2022-07-27 20:58:13');
INSERT INTO `device_info` VALUES (17, 2, '动力四级发动机', 1, '2022-07-27 21:01:10', '2022-07-27 21:01:10');
INSERT INTO `device_info` VALUES (18, 2, '一级火工品阻值', 1, '2022-07-27 21:04:38', '2022-07-27 21:04:38');
INSERT INTO `device_info` VALUES (19, 2, '二级火工品阻值', 1, '2022-07-27 21:10:11', '2022-07-27 21:10:11');
INSERT INTO `device_info` VALUES (20, 2, '三级火工品阻值', 1, '2022-07-27 21:13:03', '2022-07-27 21:13:03');
INSERT INTO `device_info` VALUES (21, 2, '星箭火工品阻值', 1, '2022-07-27 21:16:08', '2022-07-27 21:16:08');
INSERT INTO `device_info` VALUES (22, 2, '整流罩火工品阻值', 1, '2022-07-27 21:22:02', '2022-07-27 21:22:02');
INSERT INTO `device_info` VALUES (23, 2, '控制舱', 1, '2022-07-28 10:23:36', '2022-07-28 10:23:36');
INSERT INTO `device_info` VALUES (24, 2, '卫星', 1, '2022-07-28 10:29:14', '2022-07-28 10:29:14');
INSERT INTO `device_info` VALUES (25, 2, '整流罩', 1, '2022-07-28 10:31:49', '2022-07-28 10:31:49');
INSERT INTO `device_info` VALUES (26, 2, '测量一级发动机', 1, '2022-07-28 10:57:12', '2022-07-28 10:57:12');
INSERT INTO `device_info` VALUES (27, 2, '测量二级发动机', 1, '2022-07-28 10:57:29', '2022-07-28 10:57:29');
INSERT INTO `device_info` VALUES (28, 2, '测量三级发动机', 1, '2022-07-28 10:57:36', '2022-07-28 10:57:36');
INSERT INTO `device_info` VALUES (29, 2, '\r\n测量四级发动机', 1, '2022-07-28 10:57:45', '2022-07-28 10:57:45');
INSERT INTO `device_info` VALUES (30, 2, '遥测', 1, '2022-07-28 11:26:29', '2022-07-28 11:26:29');

-- ----------------------------
-- Table structure for device_param_info
-- ----------------------------
DROP TABLE IF EXISTS `device_param_info`;
CREATE TABLE `device_param_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `device_id` int(11) NOT NULL COMMENT '外键引用device_info.id',
  `parameter_id` int(11) NOT NULL COMMENT '外键引用parameter_info.id',
  `createTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录创建时间',
  `lastUpdateTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 196 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of device_param_info
-- ----------------------------
INSERT INTO `device_param_info` VALUES (1, 4, 7, '2022-07-27 19:49:52', '2022-07-27 19:49:52');
INSERT INTO `device_param_info` VALUES (2, 4, 8, '2022-07-27 19:50:05', '2022-07-27 19:50:05');
INSERT INTO `device_param_info` VALUES (3, 4, 9, '2022-07-27 19:50:13', '2022-07-27 19:50:13');
INSERT INTO `device_param_info` VALUES (4, 4, 10, '2022-07-27 19:50:22', '2022-07-27 19:50:22');
INSERT INTO `device_param_info` VALUES (5, 5, 11, '2022-07-27 20:06:46', '2022-07-27 20:06:46');
INSERT INTO `device_param_info` VALUES (6, 6, 12, '2022-07-27 20:11:13', '2022-07-27 20:11:13');
INSERT INTO `device_param_info` VALUES (7, 7, 13, '2022-07-27 20:16:23', '2022-07-27 20:16:23');
INSERT INTO `device_param_info` VALUES (8, 8, 14, '2022-07-27 20:20:43', '2022-07-27 20:20:43');
INSERT INTO `device_param_info` VALUES (9, 8, 15, '2022-07-27 20:20:54', '2022-07-27 20:20:54');
INSERT INTO `device_param_info` VALUES (10, 8, 16, '2022-07-27 20:21:00', '2022-07-27 20:21:00');
INSERT INTO `device_param_info` VALUES (11, 8, 17, '2022-07-27 20:21:10', '2022-07-27 20:21:10');
INSERT INTO `device_param_info` VALUES (12, 8, 18, '2022-07-27 20:21:21', '2022-07-27 20:21:21');
INSERT INTO `device_param_info` VALUES (13, 9, 19, '2022-07-27 20:24:43', '2022-07-27 20:24:43');
INSERT INTO `device_param_info` VALUES (14, 9, 20, '2022-07-27 20:24:51', '2022-07-27 20:24:51');
INSERT INTO `device_param_info` VALUES (15, 10, 21, '2022-07-27 20:27:16', '2022-07-27 20:27:16');
INSERT INTO `device_param_info` VALUES (16, 10, 22, '2022-07-27 20:27:21', '2022-07-27 20:27:21');
INSERT INTO `device_param_info` VALUES (17, 11, 23, '2022-07-27 20:32:24', '2022-07-27 20:32:24');
INSERT INTO `device_param_info` VALUES (18, 11, 24, '2022-07-27 20:32:41', '2022-07-27 20:32:41');
INSERT INTO `device_param_info` VALUES (19, 11, 25, '2022-07-27 20:32:52', '2022-07-27 20:32:52');
INSERT INTO `device_param_info` VALUES (20, 11, 26, '2022-07-27 20:32:57', '2022-07-27 20:32:57');
INSERT INTO `device_param_info` VALUES (21, 11, 27, '2022-07-27 20:33:02', '2022-07-27 20:33:02');
INSERT INTO `device_param_info` VALUES (22, 11, 28, '2022-07-27 20:33:09', '2022-07-27 20:33:09');
INSERT INTO `device_param_info` VALUES (23, 11, 29, '2022-07-27 20:33:16', '2022-07-27 20:33:16');
INSERT INTO `device_param_info` VALUES (24, 11, 30, '2022-07-27 20:33:22', '2022-07-27 20:33:22');
INSERT INTO `device_param_info` VALUES (25, 11, 31, '2022-07-27 20:33:27', '2022-07-27 20:33:27');
INSERT INTO `device_param_info` VALUES (26, 11, 32, '2022-07-27 20:33:33', '2022-07-27 20:33:33');
INSERT INTO `device_param_info` VALUES (27, 11, 33, '2022-07-27 20:33:41', '2022-07-27 20:33:41');
INSERT INTO `device_param_info` VALUES (28, 11, 34, '2022-07-27 20:33:47', '2022-07-27 20:33:47');
INSERT INTO `device_param_info` VALUES (29, 12, 35, '2022-07-27 20:38:44', '2022-07-27 20:38:44');
INSERT INTO `device_param_info` VALUES (30, 12, 36, '2022-07-27 20:38:50', '2022-07-27 20:38:50');
INSERT INTO `device_param_info` VALUES (31, 12, 37, '2022-07-27 20:38:55', '2022-07-27 20:38:55');
INSERT INTO `device_param_info` VALUES (32, 13, 38, '2022-07-27 20:44:22', '2022-07-27 20:44:22');
INSERT INTO `device_param_info` VALUES (33, 13, 39, '2022-07-27 20:44:29', '2022-07-27 20:44:29');
INSERT INTO `device_param_info` VALUES (34, 13, 40, '2022-07-27 20:44:37', '2022-07-27 20:44:37');
INSERT INTO `device_param_info` VALUES (35, 14, 41, '2022-07-27 20:49:19', '2022-07-27 20:49:19');
INSERT INTO `device_param_info` VALUES (36, 14, 42, '2022-07-27 20:49:29', '2022-07-27 20:49:29');
INSERT INTO `device_param_info` VALUES (37, 14, 43, '2022-07-27 20:49:36', '2022-07-27 20:49:36');
INSERT INTO `device_param_info` VALUES (38, 14, 44, '2022-07-27 20:49:41', '2022-07-27 20:49:41');
INSERT INTO `device_param_info` VALUES (39, 14, 45, '2022-07-27 20:49:48', '2022-07-27 20:49:48');
INSERT INTO `device_param_info` VALUES (40, 14, 46, '2022-07-27 20:49:54', '2022-07-27 20:49:54');
INSERT INTO `device_param_info` VALUES (41, 14, 47, '2022-07-27 20:50:00', '2022-07-27 20:50:00');
INSERT INTO `device_param_info` VALUES (42, 14, 48, '2022-07-27 20:50:10', '2022-07-27 20:50:10');
INSERT INTO `device_param_info` VALUES (43, 14, 49, '2022-07-27 20:50:19', '2022-07-27 20:50:19');
INSERT INTO `device_param_info` VALUES (44, 15, 41, '2022-07-27 20:55:49', '2022-07-27 20:55:49');
INSERT INTO `device_param_info` VALUES (45, 15, 42, '2022-07-27 20:55:55', '2022-07-27 20:55:55');
INSERT INTO `device_param_info` VALUES (46, 15, 43, '2022-07-27 20:56:01', '2022-07-27 20:56:01');
INSERT INTO `device_param_info` VALUES (47, 15, 44, '2022-07-27 20:56:06', '2022-07-27 20:56:06');
INSERT INTO `device_param_info` VALUES (48, 15, 45, '2022-07-27 20:56:12', '2022-07-27 20:56:12');
INSERT INTO `device_param_info` VALUES (49, 15, 46, '2022-07-27 20:56:19', '2022-07-27 20:56:19');
INSERT INTO `device_param_info` VALUES (50, 15, 47, '2022-07-27 20:56:26', '2022-07-27 20:56:26');
INSERT INTO `device_param_info` VALUES (51, 15, 48, '2022-07-27 20:56:36', '2022-07-27 20:56:36');
INSERT INTO `device_param_info` VALUES (52, 15, 49, '2022-07-27 20:56:52', '2022-07-27 20:56:52');
INSERT INTO `device_param_info` VALUES (53, 16, 41, '2022-07-27 20:58:45', '2022-07-27 20:58:45');
INSERT INTO `device_param_info` VALUES (54, 16, 42, '2022-07-27 20:58:50', '2022-07-27 20:58:50');
INSERT INTO `device_param_info` VALUES (55, 16, 43, '2022-07-27 20:58:57', '2022-07-27 20:58:57');
INSERT INTO `device_param_info` VALUES (56, 16, 44, '2022-07-27 20:59:03', '2022-07-27 20:59:03');
INSERT INTO `device_param_info` VALUES (57, 16, 45, '2022-07-27 20:59:09', '2022-07-27 20:59:09');
INSERT INTO `device_param_info` VALUES (58, 16, 46, '2022-07-27 20:59:15', '2022-07-27 20:59:15');
INSERT INTO `device_param_info` VALUES (59, 16, 47, '2022-07-27 20:59:23', '2022-07-27 20:59:23');
INSERT INTO `device_param_info` VALUES (60, 16, 48, '2022-07-27 20:59:31', '2022-07-27 20:59:31');
INSERT INTO `device_param_info` VALUES (61, 16, 49, '2022-07-27 20:59:38', '2022-07-27 20:59:38');
INSERT INTO `device_param_info` VALUES (62, 17, 41, '2022-07-27 21:01:22', '2022-07-27 21:01:22');
INSERT INTO `device_param_info` VALUES (63, 17, 42, '2022-07-27 21:01:41', '2022-07-27 21:01:41');
INSERT INTO `device_param_info` VALUES (64, 17, 43, '2022-07-27 21:01:48', '2022-07-27 21:01:48');
INSERT INTO `device_param_info` VALUES (65, 17, 44, '2022-07-27 21:02:01', '2022-07-27 21:02:01');
INSERT INTO `device_param_info` VALUES (66, 17, 45, '2022-07-27 21:02:05', '2022-07-27 21:02:05');
INSERT INTO `device_param_info` VALUES (67, 17, 46, '2022-07-27 21:02:11', '2022-07-27 21:02:11');
INSERT INTO `device_param_info` VALUES (68, 17, 47, '2022-07-27 21:02:16', '2022-07-27 21:02:16');
INSERT INTO `device_param_info` VALUES (69, 17, 48, '2022-07-27 21:02:24', '2022-07-27 21:02:24');
INSERT INTO `device_param_info` VALUES (70, 17, 49, '2022-07-27 21:02:31', '2022-07-27 21:02:31');
INSERT INTO `device_param_info` VALUES (71, 18, 52, '2022-07-27 21:07:07', '2022-07-27 21:07:07');
INSERT INTO `device_param_info` VALUES (72, 18, 53, '2022-07-27 21:07:15', '2022-07-27 21:07:15');
INSERT INTO `device_param_info` VALUES (73, 18, 54, '2022-07-27 21:07:26', '2022-07-27 21:07:26');
INSERT INTO `device_param_info` VALUES (74, 18, 55, '2022-07-27 21:07:32', '2022-07-27 21:07:32');
INSERT INTO `device_param_info` VALUES (75, 18, 56, '2022-07-27 21:07:50', '2022-07-27 21:07:50');
INSERT INTO `device_param_info` VALUES (76, 18, 57, '2022-07-27 21:07:55', '2022-07-27 21:07:55');
INSERT INTO `device_param_info` VALUES (77, 19, 58, '2022-07-27 21:11:27', '2022-07-27 21:11:27');
INSERT INTO `device_param_info` VALUES (78, 19, 59, '2022-07-27 21:11:35', '2022-07-27 21:11:35');
INSERT INTO `device_param_info` VALUES (79, 19, 60, '2022-07-27 21:11:40', '2022-07-27 21:11:40');
INSERT INTO `device_param_info` VALUES (80, 19, 61, '2022-07-27 21:11:46', '2022-07-27 21:11:46');
INSERT INTO `device_param_info` VALUES (81, 19, 62, '2022-07-27 21:11:52', '2022-07-27 21:11:52');
INSERT INTO `device_param_info` VALUES (82, 19, 63, '2022-07-27 21:11:58', '2022-07-27 21:11:58');
INSERT INTO `device_param_info` VALUES (83, 20, 64, '2022-07-27 21:14:19', '2022-07-27 21:14:19');
INSERT INTO `device_param_info` VALUES (84, 20, 65, '2022-07-27 21:14:30', '2022-07-27 21:14:30');
INSERT INTO `device_param_info` VALUES (85, 20, 66, '2022-07-27 21:14:48', '2022-07-27 21:14:48');
INSERT INTO `device_param_info` VALUES (86, 20, 67, '2022-07-27 21:14:58', '2022-07-27 21:14:58');
INSERT INTO `device_param_info` VALUES (87, 20, 68, '2022-07-27 21:15:03', '2022-07-27 21:15:03');
INSERT INTO `device_param_info` VALUES (88, 20, 69, '2022-07-27 21:15:09', '2022-07-27 21:15:09');
INSERT INTO `device_param_info` VALUES (89, 21, 70, '2022-07-27 21:19:22', '2022-07-27 21:19:22');
INSERT INTO `device_param_info` VALUES (90, 21, 71, '2022-07-27 21:19:34', '2022-07-27 21:19:34');
INSERT INTO `device_param_info` VALUES (91, 21, 72, '2022-07-27 21:19:40', '2022-07-27 21:19:40');
INSERT INTO `device_param_info` VALUES (92, 21, 73, '2022-07-27 21:19:46', '2022-07-27 21:19:46');
INSERT INTO `device_param_info` VALUES (93, 21, 74, '2022-07-27 21:19:52', '2022-07-27 21:19:52');
INSERT INTO `device_param_info` VALUES (94, 21, 75, '2022-07-27 21:19:58', '2022-07-27 21:19:58');
INSERT INTO `device_param_info` VALUES (95, 21, 76, '2022-07-27 21:20:05', '2022-07-27 21:20:05');
INSERT INTO `device_param_info` VALUES (96, 21, 77, '2022-07-27 21:20:11', '2022-07-27 21:20:11');
INSERT INTO `device_param_info` VALUES (97, 21, 78, '2022-07-27 21:20:15', '2022-07-27 21:20:15');
INSERT INTO `device_param_info` VALUES (98, 21, 79, '2022-07-27 21:20:20', '2022-07-27 21:20:20');
INSERT INTO `device_param_info` VALUES (99, 21, 80, '2022-07-27 21:20:25', '2022-07-27 21:20:25');
INSERT INTO `device_param_info` VALUES (100, 21, 81, '2022-07-27 21:20:32', '2022-07-27 21:20:32');
INSERT INTO `device_param_info` VALUES (101, 21, 82, '2022-07-27 21:20:36', '2022-07-27 21:20:36');
INSERT INTO `device_param_info` VALUES (102, 21, 83, '2022-07-27 21:20:42', '2022-07-27 21:20:42');
INSERT INTO `device_param_info` VALUES (103, 21, 84, '2022-07-27 21:20:47', '2022-07-27 21:20:47');
INSERT INTO `device_param_info` VALUES (104, 22, 85, '2022-07-27 21:23:12', '2022-07-27 21:23:12');
INSERT INTO `device_param_info` VALUES (105, 22, 86, '2022-07-27 21:23:18', '2022-07-27 21:23:18');
INSERT INTO `device_param_info` VALUES (106, 22, 87, '2022-07-27 21:23:22', '2022-07-27 21:23:22');
INSERT INTO `device_param_info` VALUES (107, 22, 88, '2022-07-27 21:23:26', '2022-07-27 21:23:26');
INSERT INTO `device_param_info` VALUES (109, 23, 89, '2022-07-28 10:27:01', '2022-07-28 10:27:01');
INSERT INTO `device_param_info` VALUES (110, 23, 90, '2022-07-28 10:27:07', '2022-07-28 10:27:07');
INSERT INTO `device_param_info` VALUES (111, 23, 91, '2022-07-28 10:27:17', '2022-07-28 10:27:17');
INSERT INTO `device_param_info` VALUES (112, 23, 92, '2022-07-28 10:27:30', '2022-07-28 10:27:30');
INSERT INTO `device_param_info` VALUES (113, 24, 93, '2022-07-28 10:30:19', '2022-07-28 10:30:19');
INSERT INTO `device_param_info` VALUES (114, 24, 94, '2022-07-28 10:30:26', '2022-07-28 10:30:26');
INSERT INTO `device_param_info` VALUES (115, 24, 95, '2022-07-28 10:30:31', '2022-07-28 10:30:31');
INSERT INTO `device_param_info` VALUES (116, 25, 96, '2022-07-28 10:33:23', '2022-07-28 10:33:23');
INSERT INTO `device_param_info` VALUES (117, 25, 97, '2022-07-28 10:33:27', '2022-07-28 10:33:27');
INSERT INTO `device_param_info` VALUES (118, 25, 98, '2022-07-28 10:33:32', '2022-07-28 10:33:32');
INSERT INTO `device_param_info` VALUES (119, 25, 99, '2022-07-28 10:33:37', '2022-07-28 10:33:37');
INSERT INTO `device_param_info` VALUES (120, 25, 100, '2022-07-28 10:33:46', '2022-07-28 10:33:46');
INSERT INTO `device_param_info` VALUES (121, 25, 101, '2022-07-28 10:33:55', '2022-07-28 10:33:55');
INSERT INTO `device_param_info` VALUES (122, 26, 41, '2022-07-28 10:59:51', '2022-07-28 10:59:51');
INSERT INTO `device_param_info` VALUES (123, 26, 42, '2022-07-28 11:00:07', '2022-07-28 11:00:07');
INSERT INTO `device_param_info` VALUES (124, 26, 43, '2022-07-28 11:00:11', '2022-07-28 11:00:11');
INSERT INTO `device_param_info` VALUES (125, 26, 44, '2022-07-28 11:00:17', '2022-07-28 11:00:17');
INSERT INTO `device_param_info` VALUES (126, 26, 45, '2022-07-28 11:00:22', '2022-07-28 11:00:22');
INSERT INTO `device_param_info` VALUES (127, 26, 46, '2022-07-28 11:00:28', '2022-07-28 11:00:28');
INSERT INTO `device_param_info` VALUES (128, 26, 47, '2022-07-28 11:00:38', '2022-07-28 11:00:38');
INSERT INTO `device_param_info` VALUES (129, 26, 48, '2022-07-28 11:00:42', '2022-07-28 11:00:42');
INSERT INTO `device_param_info` VALUES (130, 26, 49, '2022-07-28 11:00:48', '2022-07-28 11:00:48');
INSERT INTO `device_param_info` VALUES (131, 26, 102, '2022-07-28 11:07:44', '2022-07-28 11:07:44');
INSERT INTO `device_param_info` VALUES (132, 26, 103, '2022-07-28 11:07:50', '2022-07-28 11:07:50');
INSERT INTO `device_param_info` VALUES (142, 26, 104, '2022-07-28 11:14:37', '2022-07-28 11:14:37');
INSERT INTO `device_param_info` VALUES (143, 14, 102, '2022-07-28 11:14:42', '2022-07-28 11:14:42');
INSERT INTO `device_param_info` VALUES (144, 14, 103, '2022-07-28 11:15:06', '2022-07-28 11:15:06');
INSERT INTO `device_param_info` VALUES (145, 14, 105, '2022-07-28 11:15:10', '2022-07-28 11:15:10');
INSERT INTO `device_param_info` VALUES (146, 15, 102, '2022-07-28 11:15:14', '2022-07-28 11:15:14');
INSERT INTO `device_param_info` VALUES (147, 15, 103, '2022-07-28 11:15:19', '2022-07-28 11:15:19');
INSERT INTO `device_param_info` VALUES (148, 15, 104, '2022-07-28 11:15:24', '2022-07-28 11:15:24');
INSERT INTO `device_param_info` VALUES (149, 16, 102, '2022-07-28 11:15:30', '2022-07-28 11:15:30');
INSERT INTO `device_param_info` VALUES (150, 16, 103, '2022-07-28 11:15:34', '2022-07-28 11:15:34');
INSERT INTO `device_param_info` VALUES (151, 16, 104, '2022-07-28 11:15:40', '2022-07-28 11:15:40');
INSERT INTO `device_param_info` VALUES (152, 17, 102, '2022-07-28 11:15:44', '2022-07-28 11:15:44');
INSERT INTO `device_param_info` VALUES (153, 17, 103, '2022-07-28 11:15:48', '2022-07-28 11:15:48');
INSERT INTO `device_param_info` VALUES (154, 17, 104, '2022-07-28 11:16:01', '2022-07-28 11:16:01');
INSERT INTO `device_param_info` VALUES (155, 27, 41, '2022-07-28 11:16:10', '2022-07-28 11:16:10');
INSERT INTO `device_param_info` VALUES (156, 27, 42, '2022-07-28 11:16:14', '2022-07-28 11:16:14');
INSERT INTO `device_param_info` VALUES (157, 27, 43, '2022-07-28 11:16:19', '2022-07-28 11:16:19');
INSERT INTO `device_param_info` VALUES (158, 27, 44, '2022-07-28 11:16:25', '2022-07-28 11:16:25');
INSERT INTO `device_param_info` VALUES (159, 27, 45, '2022-07-28 11:16:31', '2022-07-28 11:16:31');
INSERT INTO `device_param_info` VALUES (160, 27, 46, '2022-07-28 11:16:36', '2022-07-28 11:16:36');
INSERT INTO `device_param_info` VALUES (161, 27, 47, '2022-07-28 11:16:40', '2022-07-28 11:16:40');
INSERT INTO `device_param_info` VALUES (162, 27, 48, '2022-07-28 11:16:46', '2022-07-28 11:16:46');
INSERT INTO `device_param_info` VALUES (163, 27, 49, '2022-07-28 11:16:53', '2022-07-28 11:16:53');
INSERT INTO `device_param_info` VALUES (164, 27, 102, '2022-07-28 11:17:01', '2022-07-28 11:17:01');
INSERT INTO `device_param_info` VALUES (165, 27, 103, '2022-07-28 11:17:07', '2022-07-28 11:17:07');
INSERT INTO `device_param_info` VALUES (166, 27, 104, '2022-07-28 11:17:14', '2022-07-28 11:17:14');
INSERT INTO `device_param_info` VALUES (167, 28, 41, '2022-07-28 11:17:27', '2022-07-28 11:17:27');
INSERT INTO `device_param_info` VALUES (168, 28, 42, '2022-07-28 11:17:33', '2022-07-28 11:17:33');
INSERT INTO `device_param_info` VALUES (169, 28, 43, '2022-07-28 11:17:38', '2022-07-28 11:17:38');
INSERT INTO `device_param_info` VALUES (170, 28, 44, '2022-07-28 11:17:43', '2022-07-28 11:17:43');
INSERT INTO `device_param_info` VALUES (171, 28, 45, '2022-07-28 11:17:48', '2022-07-28 11:17:48');
INSERT INTO `device_param_info` VALUES (172, 28, 46, '2022-07-28 11:17:52', '2022-07-28 11:17:52');
INSERT INTO `device_param_info` VALUES (173, 28, 47, '2022-07-28 11:17:57', '2022-07-28 11:17:57');
INSERT INTO `device_param_info` VALUES (174, 28, 48, '2022-07-28 11:18:01', '2022-07-28 11:18:01');
INSERT INTO `device_param_info` VALUES (175, 28, 49, '2022-07-28 11:18:09', '2022-07-28 11:18:09');
INSERT INTO `device_param_info` VALUES (176, 28, 102, '2022-07-28 11:18:20', '2022-07-28 11:18:20');
INSERT INTO `device_param_info` VALUES (177, 28, 103, '2022-07-28 11:18:31', '2022-07-28 11:18:31');
INSERT INTO `device_param_info` VALUES (178, 28, 104, '2022-07-28 11:18:36', '2022-07-28 11:18:36');
INSERT INTO `device_param_info` VALUES (179, 29, 41, '2022-07-28 11:19:11', '2022-07-28 11:19:11');
INSERT INTO `device_param_info` VALUES (180, 29, 42, '2022-07-28 11:19:15', '2022-07-28 11:19:15');
INSERT INTO `device_param_info` VALUES (181, 29, 43, '2022-07-28 11:19:20', '2022-07-28 11:19:20');
INSERT INTO `device_param_info` VALUES (182, 29, 44, '2022-07-28 11:19:26', '2022-07-28 11:19:26');
INSERT INTO `device_param_info` VALUES (183, 29, 45, '2022-07-28 11:19:30', '2022-07-28 11:19:30');
INSERT INTO `device_param_info` VALUES (184, 29, 46, '2022-07-28 11:19:35', '2022-07-28 11:19:35');
INSERT INTO `device_param_info` VALUES (185, 29, 47, '2022-07-28 11:19:41', '2022-07-28 11:19:41');
INSERT INTO `device_param_info` VALUES (186, 29, 48, '2022-07-28 11:19:46', '2022-07-28 11:19:46');
INSERT INTO `device_param_info` VALUES (187, 29, 49, '2022-07-28 11:19:51', '2022-07-28 11:19:51');
INSERT INTO `device_param_info` VALUES (188, 29, 102, '2022-07-28 11:19:56', '2022-07-28 11:19:56');
INSERT INTO `device_param_info` VALUES (189, 29, 103, '2022-07-28 11:20:01', '2022-07-28 11:20:01');
INSERT INTO `device_param_info` VALUES (190, 29, 104, '2022-07-28 11:20:05', '2022-07-28 11:20:05');
INSERT INTO `device_param_info` VALUES (191, 30, 105, '2022-07-28 11:34:17', '2022-07-28 11:34:17');
INSERT INTO `device_param_info` VALUES (192, 30, 106, '2022-07-28 11:34:21', '2022-07-28 11:34:21');
INSERT INTO `device_param_info` VALUES (193, 30, 107, '2022-07-28 11:34:26', '2022-07-28 11:34:26');
INSERT INTO `device_param_info` VALUES (194, 30, 108, '2022-07-28 11:34:36', '2022-07-28 11:34:36');
INSERT INTO `device_param_info` VALUES (195, 30, 109, '2022-07-28 11:34:41', '2022-07-28 11:34:41');

-- ----------------------------
-- Table structure for device_status_info
-- ----------------------------
DROP TABLE IF EXISTS `device_status_info`;
CREATE TABLE `device_status_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `device_id` int(11) NOT NULL COMMENT '外键引用device_info.id',
  `status_id` int(11) NOT NULL COMMENT '外键引用status_info.id',
  `dataPath` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '实时值本地文件相对路径',
  `createTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录创建时间',
  `lastUpdateTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 99 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of device_status_info
-- ----------------------------
INSERT INTO `device_status_info` VALUES (1, 4, 1, 'imu_power_on.txt', '2022-07-27 19:52:47', '2022-07-27 19:52:47');
INSERT INTO `device_status_info` VALUES (2, 4, 2, 'imu_power_off.txt', '2022-07-27 19:53:26', '2022-07-27 19:53:26');
INSERT INTO `device_status_info` VALUES (3, 4, 3, 'imu_power_off.txt', '2022-07-27 19:53:47', '2022-07-27 19:53:47');
INSERT INTO `device_status_info` VALUES (4, 5, 1, 'second_servo_power_on.txt', '2022-07-27 20:08:07', '2022-07-27 20:08:07');
INSERT INTO `device_status_info` VALUES (5, 5, 2, 'second_servo_power_off.txt', '2022-07-27 20:08:30', '2022-07-27 20:08:30');
INSERT INTO `device_status_info` VALUES (6, 5, 3, 'second_servo_power_off.txt', '2022-07-27 20:08:42', '2022-07-27 20:08:42');
INSERT INTO `device_status_info` VALUES (7, 6, 4, 'first_servo_activate.txt', '2022-07-27 20:12:28', '2022-07-27 20:12:28');
INSERT INTO `device_status_info` VALUES (8, 6, 2, 'first_servo_zero.txt', '2022-07-27 20:13:38', '2022-07-27 20:13:38');
INSERT INTO `device_status_info` VALUES (9, 6, 3, 'first_servo_zero.txt', '2022-07-27 20:14:03', '2022-07-27 20:14:03');
INSERT INTO `device_status_info` VALUES (10, 7, 4, 'second_servo_activate.txt', '2022-07-27 20:17:24', '2022-07-27 20:17:24');
INSERT INTO `device_status_info` VALUES (11, 7, 2, 'second_servo_zero.txt', '2022-07-27 20:17:58', '2022-07-27 20:17:58');
INSERT INTO `device_status_info` VALUES (12, 7, 3, 'second_servo_zero.txt', '2022-07-27 20:18:12', '2022-07-27 20:18:12');
INSERT INTO `device_status_info` VALUES (13, 8, 1, 'sensor_power_on.txt', '2022-07-27 20:22:11', '2022-07-27 20:22:11');
INSERT INTO `device_status_info` VALUES (14, 8, 2, 'sensor_power_off.txt', '2022-07-27 20:22:31', '2022-07-27 20:22:31');
INSERT INTO `device_status_info` VALUES (15, 8, 3, 'sensor_power_off.txt', '2022-07-27 20:22:39', '2022-07-27 20:22:39');
INSERT INTO `device_status_info` VALUES (16, 9, 1, 'radio_power_on.txt', '2022-07-27 20:25:29', '2022-07-27 20:25:29');
INSERT INTO `device_status_info` VALUES (17, 9, 2, 'radio_power_off.txt', '2022-07-27 20:25:35', '2022-07-27 20:25:35');
INSERT INTO `device_status_info` VALUES (18, 9, 3, 'radio_power_off.txt', '2022-07-27 20:25:41', '2022-07-27 20:25:41');
INSERT INTO `device_status_info` VALUES (19, 10, 4, 'attitude_control_power_on.txt', '2022-07-27 20:27:56', '2022-07-27 20:27:56');
INSERT INTO `device_status_info` VALUES (20, 10, 2, 'attitude_control_power_off.txt', '2022-07-27 20:28:04', '2022-07-27 20:28:04');
INSERT INTO `device_status_info` VALUES (21, 10, 3, 'attitude_control_power_off.txt', '2022-07-27 20:28:23', '2022-07-27 20:28:23');
INSERT INTO `device_status_info` VALUES (22, 11, 1, 'initator_activate.txt', '2022-07-27 20:34:09', '2022-07-27 20:34:09');
INSERT INTO `device_status_info` VALUES (23, 11, 2, 'initator_nonactivate.txt', '2022-07-27 20:34:13', '2022-07-27 20:34:13');
INSERT INTO `device_status_info` VALUES (24, 11, 3, 'initator_nonactivate.txt', '2022-07-27 20:34:17', '2022-07-27 20:34:17');
INSERT INTO `device_status_info` VALUES (25, 12, 5, 'attitude_control_system_static.txt', '2022-07-27 20:41:27', '2022-07-27 20:41:27');
INSERT INTO `device_status_info` VALUES (26, 12, 6, 'attitude_control_system_fire.txt', '2022-07-27 20:41:33', '2022-07-27 20:41:33');
INSERT INTO `device_status_info` VALUES (27, 12, 7, 'attitude_control_system_zero.txt', '2022-07-27 20:41:40', '2022-07-27 20:41:40');
INSERT INTO `device_status_info` VALUES (28, 12, 3, 'attitude_control_system_zero.txt', '2022-07-27 20:42:34', '2022-07-27 20:42:34');
INSERT INTO `device_status_info` VALUES (29, 13, 5, 'attitude_engine_static.txt', '2022-07-27 20:44:51', '2022-07-27 20:44:51');
INSERT INTO `device_status_info` VALUES (30, 13, 6, 'attitude_engine_fire.txt', '2022-07-27 20:44:57', '2022-07-27 20:44:57');
INSERT INTO `device_status_info` VALUES (31, 13, 7, 'attitude_engine_zero.txt', '2022-07-27 20:45:03', '2022-07-27 20:45:03');
INSERT INTO `device_status_info` VALUES (32, 13, 3, 'attitude_engine_zero.txt', '2022-07-27 20:45:50', '2022-07-27 20:45:50');
INSERT INTO `device_status_info` VALUES (33, 14, 5, 'power_first_engine_static.txt', '2022-07-27 20:51:01', '2022-07-27 20:51:01');
INSERT INTO `device_status_info` VALUES (34, 14, 6, 'power_first_engine_fire.txt', '2022-07-27 20:51:07', '2022-07-27 20:51:07');
INSERT INTO `device_status_info` VALUES (35, 14, 7, 'power_first_engine_zero.txt', '2022-07-27 20:51:12', '2022-07-27 20:51:12');
INSERT INTO `device_status_info` VALUES (36, 14, 3, 'power_first_engine_zero.txt', '2022-07-27 20:51:37', '2022-07-27 20:51:37');
INSERT INTO `device_status_info` VALUES (37, 15, 5, 'power_second_engine_static.txt', '2022-07-27 20:57:18', '2022-07-27 20:57:18');
INSERT INTO `device_status_info` VALUES (38, 15, 6, 'power_second_engine_fire.txt', '2022-07-27 20:57:24', '2022-07-27 20:57:24');
INSERT INTO `device_status_info` VALUES (39, 15, 7, 'power_second_engine_zero.txt', '2022-07-27 20:57:29', '2022-07-27 20:57:29');
INSERT INTO `device_status_info` VALUES (40, 15, 3, 'power_second_engine_zero.txt', '2022-07-27 20:57:37', '2022-07-27 20:57:37');
INSERT INTO `device_status_info` VALUES (41, 16, 5, 'power_third_engine_static.txt', '2022-07-27 21:00:07', '2022-07-27 21:00:07');
INSERT INTO `device_status_info` VALUES (42, 16, 6, 'power_third_engine_fire.txt', '2022-07-27 21:00:11', '2022-07-27 21:00:11');
INSERT INTO `device_status_info` VALUES (43, 16, 7, 'power_third_engine_zero.txt', '2022-07-27 21:00:18', '2022-07-27 21:00:18');
INSERT INTO `device_status_info` VALUES (44, 16, 3, 'power_third_engine_zero.txt', '2022-07-27 21:00:38', '2022-07-27 21:00:38');
INSERT INTO `device_status_info` VALUES (45, 17, 5, 'power_fourth_engine_static.txt', '2022-07-27 21:02:40', '2022-07-27 21:02:40');
INSERT INTO `device_status_info` VALUES (46, 17, 6, 'power_fourth_engine_fire.txt', '2022-07-27 21:02:45', '2022-07-27 21:02:45');
INSERT INTO `device_status_info` VALUES (47, 17, 7, 'power_fourth_engine_zero.txt', '2022-07-27 21:02:50', '2022-07-27 21:02:50');
INSERT INTO `device_status_info` VALUES (48, 17, 3, 'power_fourth_engine_zero.txt', '2022-07-27 21:02:54', '2022-07-27 21:02:54');
INSERT INTO `device_status_info` VALUES (49, 18, 5, 'iniator_sep1_static.txt', '2022-07-27 21:08:36', '2022-07-27 21:08:36');
INSERT INTO `device_status_info` VALUES (50, 18, 7, 'iniator_sep1_zero.txt', '2022-07-27 21:08:44', '2022-07-27 21:08:44');
INSERT INTO `device_status_info` VALUES (51, 18, 3, 'iniator_sep1_zero.txt', '2022-07-27 21:08:55', '2022-07-27 21:08:55');
INSERT INTO `device_status_info` VALUES (52, 19, 5, 'iniator_sep2_static.txt', '2022-07-27 21:12:25', '2022-07-27 21:12:25');
INSERT INTO `device_status_info` VALUES (53, 19, 7, 'iniator_sep2_zero.txt', '2022-07-27 21:12:30', '2022-07-27 21:12:30');
INSERT INTO `device_status_info` VALUES (54, 19, 3, 'iniator_sep2_zero.txt', '2022-07-27 21:12:35', '2022-07-27 21:12:35');
INSERT INTO `device_status_info` VALUES (55, 20, 5, 'iniator_sep3_static.txt', '2022-07-27 21:15:22', '2022-07-27 21:15:22');
INSERT INTO `device_status_info` VALUES (56, 20, 7, 'iniator_sep3_zero.txt', '2022-07-27 21:15:27', '2022-07-27 21:15:27');
INSERT INTO `device_status_info` VALUES (57, 20, 3, 'iniator_sep3_zero.txt', '2022-07-27 21:15:32', '2022-07-27 21:15:32');
INSERT INTO `device_status_info` VALUES (58, 21, 5, 'iniator_sepxj_static.txt', '2022-07-27 21:21:32', '2022-07-27 21:21:32');
INSERT INTO `device_status_info` VALUES (59, 21, 7, 'iniator_sepxj_zero.txt', '2022-07-27 21:21:36', '2022-07-27 21:21:36');
INSERT INTO `device_status_info` VALUES (60, 21, 3, 'iniator_sepxj_zero.txt', '2022-07-27 21:21:41', '2022-07-27 21:21:41');
INSERT INTO `device_status_info` VALUES (61, 22, 5, 'iniator_sepzlz_static.txt', '2022-07-27 21:24:11', '2022-07-27 21:24:11');
INSERT INTO `device_status_info` VALUES (62, 22, 7, 'iniator_sepzlz_static.txt', '2022-07-27 21:24:17', '2022-07-27 21:24:17');
INSERT INTO `device_status_info` VALUES (63, 22, 3, 'iniator_sepzlz_static.txt', '2022-07-27 21:24:27', '2022-07-27 21:24:27');
INSERT INTO `device_status_info` VALUES (64, 23, 5, 'control_storehouse_static.txt', '2022-07-28 10:28:07', '2022-07-28 10:28:07');
INSERT INTO `device_status_info` VALUES (65, 23, 6, 'control_storehouse_fire.txt', '2022-07-28 10:28:14', '2022-07-28 10:28:14');
INSERT INTO `device_status_info` VALUES (66, 23, 7, 'control_storehouse_zero.txt', '2022-07-28 10:28:27', '2022-07-28 10:28:27');
INSERT INTO `device_status_info` VALUES (67, 23, 3, 'control_storehouse_zero.txt', '2022-07-28 10:28:35', '2022-07-28 10:28:35');
INSERT INTO `device_status_info` VALUES (68, 24, 5, 'satellite_static.txt', '2022-07-28 10:31:03', '2022-07-28 10:31:03');
INSERT INTO `device_status_info` VALUES (69, 24, 6, 'satellite_fire.txt', '2022-07-28 10:31:07', '2022-07-28 10:31:07');
INSERT INTO `device_status_info` VALUES (70, 24, 7, 'satellite_zero.txt\"', '2022-07-28 10:31:11', '2022-07-28 10:31:11');
INSERT INTO `device_status_info` VALUES (71, 24, 3, 'satellite_zero.txt\"', '2022-07-28 10:31:17', '2022-07-28 10:31:17');
INSERT INTO `device_status_info` VALUES (72, 25, 5, 'fairing_static.txt', '2022-07-28 10:34:44', '2022-07-28 10:34:44');
INSERT INTO `device_status_info` VALUES (73, 25, 8, 'fairing_fire_befor.txt', '2022-07-28 10:34:55', '2022-07-28 10:34:55');
INSERT INTO `device_status_info` VALUES (74, 25, 6, 'fairing_fire.txt', '2022-07-28 10:35:03', '2022-07-28 10:35:03');
INSERT INTO `device_status_info` VALUES (75, 25, 7, 'fairing_zero.txt', '2022-07-28 10:35:12', '2022-07-28 10:35:12');
INSERT INTO `device_status_info` VALUES (76, 25, 3, 'fairing_zero.txt', '2022-07-28 10:35:17', '2022-07-28 10:35:17');
INSERT INTO `device_status_info` VALUES (77, 26, 5, 'measure_first_engine_static.txt', '2022-07-28 11:10:35', '2022-07-28 11:10:35');
INSERT INTO `device_status_info` VALUES (78, 26, 6, 'measure_first_engine_fire.txt', '2022-07-28 11:10:40', '2022-07-28 11:10:40');
INSERT INTO `device_status_info` VALUES (79, 26, 7, 'measure_first_engine_zero.txt', '2022-07-28 11:10:45', '2022-07-28 11:10:45');
INSERT INTO `device_status_info` VALUES (80, 26, 3, 'measure_first_engine_zero.txt', '2022-07-28 11:10:50', '2022-07-28 11:10:50');
INSERT INTO `device_status_info` VALUES (81, 27, 5, 'measure_second_engine_static.txt', '2022-07-28 11:21:21', '2022-07-28 11:21:21');
INSERT INTO `device_status_info` VALUES (82, 27, 8, 'measure_second_engine_fire_before.txt', '2022-07-28 11:21:26', '2022-07-28 11:21:26');
INSERT INTO `device_status_info` VALUES (83, 27, 6, 'measure_second_engine_fire.txt', '2022-07-28 11:21:30', '2022-07-28 11:21:30');
INSERT INTO `device_status_info` VALUES (84, 27, 7, 'measure_second_engine_zero.txt', '2022-07-28 11:21:36', '2022-07-28 11:21:36');
INSERT INTO `device_status_info` VALUES (85, 27, 3, 'measure_second_engine_zero.txt', '2022-07-28 11:22:36', '2022-07-28 11:22:36');
INSERT INTO `device_status_info` VALUES (86, 28, 5, 'measure_third_engine_static.txt', '2022-07-28 11:22:50', '2022-07-28 11:22:50');
INSERT INTO `device_status_info` VALUES (87, 28, 8, 'measure_third_engine_fire_before.txt', '2022-07-28 11:22:54', '2022-07-28 11:22:54');
INSERT INTO `device_status_info` VALUES (88, 28, 6, 'measure_third_engine_fire.txt', '2022-07-28 11:22:58', '2022-07-28 11:22:58');
INSERT INTO `device_status_info` VALUES (89, 28, 7, 'measure_third_engine_zero.txt', '2022-07-28 11:23:03', '2022-07-28 11:23:03');
INSERT INTO `device_status_info` VALUES (90, 28, 3, 'measure_third_engine_zero.txt', '2022-07-28 11:23:07', '2022-07-28 11:23:07');
INSERT INTO `device_status_info` VALUES (91, 29, 5, 'measure_fourth_engine_static.txt', '2022-07-28 11:24:24', '2022-07-28 11:24:24');
INSERT INTO `device_status_info` VALUES (92, 29, 8, 'measure_fourth_engine_fire_before.txt', '2022-07-28 11:24:32', '2022-07-28 11:24:32');
INSERT INTO `device_status_info` VALUES (93, 29, 6, 'measure_fourth_engine_fire.txt', '2022-07-28 11:24:36', '2022-07-28 11:24:36');
INSERT INTO `device_status_info` VALUES (94, 29, 7, 'measure_fourth_engine_zero.txt', '2022-07-28 11:24:47', '2022-07-28 11:24:47');
INSERT INTO `device_status_info` VALUES (95, 29, 3, 'measure_fourth_engine_zero.txt', '2022-07-28 11:24:53', '2022-07-28 11:24:53');
INSERT INTO `device_status_info` VALUES (96, 30, 1, 'telemetring_power_on.txt', '2022-07-28 11:28:41', '2022-07-28 11:28:41');
INSERT INTO `device_status_info` VALUES (97, 30, 2, 'telemetring_power_off.txt', '2022-07-28 11:28:45', '2022-07-28 11:28:45');
INSERT INTO `device_status_info` VALUES (98, 30, 3, 'telemetring_power_off.txt', '2022-07-28 11:28:50', '2022-07-28 11:28:50');

-- ----------------------------
-- Table structure for fault_command_info
-- ----------------------------
DROP TABLE IF EXISTS `fault_command_info`;
CREATE TABLE `fault_command_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '系统生存期内唯一',
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '指令故障名称',
  `fault_type` int(255) NULL DEFAULT NULL COMMENT '故障类型 1：控制；2：测量；3：动力',
  `fault_command_id` int(255) NULL DEFAULT NULL COMMENT '引用command_info.id',
  `response_command_id` int(255) NULL DEFAULT NULL COMMENT '引用command_info.id',
  `createTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间',
  `lastUpdateTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 9 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of fault_command_info
-- ----------------------------
INSERT INTO `fault_command_info` VALUES (7, '综控机上电指令异常', 1, 8, 2, '2022-07-27 14:05:14', '2022-07-27 14:05:14');
INSERT INTO `fault_command_info` VALUES (8, '伺服上电指令异常', 1, 9, 3, '2022-07-27 14:05:37', '2022-07-27 14:05:37');

-- ----------------------------
-- Table structure for fault_param_info
-- ----------------------------
DROP TABLE IF EXISTS `fault_param_info`;
CREATE TABLE `fault_param_info`  (
  `id` int(255) NOT NULL AUTO_INCREMENT COMMENT '系统生存期内唯一',
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '参数故障名称',
  `fault_type` int(255) NULL DEFAULT NULL COMMENT '故障类型1：控制；2：测量；3：动力',
  `command_id` int(255) NULL DEFAULT NULL COMMENT '引用command_info.id',
  `device_param_info_id` int(255) NULL DEFAULT NULL COMMENT '引用device_param_info.id',
  `createTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间',
  `lastUpdateTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 30 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of fault_param_info
-- ----------------------------
INSERT INTO `fault_param_info` VALUES (4, '箭上无法加电', 1, 8, 1, '2022-07-27 13:55:32', '2022-07-27 13:55:32');
INSERT INTO `fault_param_info` VALUES (5, '箭上无法加电', 1, 8, 2, '2022-07-27 13:56:19', '2022-07-27 13:56:19');
INSERT INTO `fault_param_info` VALUES (6, '箭上设备供电异常', 1, 9, 2, '2022-07-27 13:56:44', '2022-07-27 13:56:44');
INSERT INTO `fault_param_info` VALUES (7, '箭上设备自检异常', 1, 10, 2, '2022-07-27 13:57:01', '2022-07-27 13:57:01');
INSERT INTO `fault_param_info` VALUES (8, '火工品阻值异常', 1, 11, 3, '2022-07-27 13:57:17', '2022-07-27 13:57:17');
INSERT INTO `fault_param_info` VALUES (9, '陀螺惯组数据异常', 1, 12, 4, '2022-07-27 13:57:36', '2022-07-27 13:57:36');
INSERT INTO `fault_param_info` VALUES (10, '伺服系统零位异常', 1, 12, 5, '2022-07-27 13:57:51', '2022-07-27 13:57:51');
INSERT INTO `fault_param_info` VALUES (11, '伺服电池激活异常', 1, 11, 6, '2022-07-27 13:58:10', '2022-07-27 13:58:10');
INSERT INTO `fault_param_info` VALUES (12, '主电池供电异常', 1, 9, 5, '2022-07-27 13:58:30', '2022-07-27 13:58:30');
INSERT INTO `fault_param_info` VALUES (13, '点火失败', 1, 9, 6, '2022-07-27 13:58:47', '2022-07-27 13:58:47');
INSERT INTO `fault_param_info` VALUES (14, '网络通讯异常（地面）', 1, 10, 3, '2022-07-27 13:59:07', '2022-07-27 13:59:07');
INSERT INTO `fault_param_info` VALUES (15, 'GPS定位异常', 2, 8, 4, '2022-07-27 13:59:36', '2022-07-27 13:59:36');
INSERT INTO `fault_param_info` VALUES (16, '遥测信号异常', 2, 9, 3, '2022-07-27 13:59:54', '2022-07-27 13:59:54');
INSERT INTO `fault_param_info` VALUES (17, '遥测信号异常', 2, 9, 1, '2022-07-27 14:00:07', '2022-07-27 14:00:07');
INSERT INTO `fault_param_info` VALUES (18, '一级传感器异常', 2, 11, 5, '2022-07-27 14:00:19', '2022-07-27 14:00:19');
INSERT INTO `fault_param_info` VALUES (19, '二级传感器异常', 2, 12, 6, '2022-07-27 14:00:30', '2022-07-27 14:00:30');
INSERT INTO `fault_param_info` VALUES (20, '三级传感器异常', 2, 10, 2, '2022-07-27 14:00:54', '2022-07-27 14:00:54');
INSERT INTO `fault_param_info` VALUES (21, '四级传感器异常', 2, 10, 5, '2022-07-27 14:01:23', '2022-07-27 14:01:23');
INSERT INTO `fault_param_info` VALUES (22, '一级发动机前封头压力异常', 3, 8, 1, '2022-07-27 14:01:41', '2022-07-27 14:01:41');
INSERT INTO `fault_param_info` VALUES (23, '二级发动机前封头压力异常', 3, 9, 2, '2022-07-27 14:01:53', '2022-07-27 14:01:53');
INSERT INTO `fault_param_info` VALUES (24, '三级发动机前封头压力异常', 3, 10, 3, '2022-07-27 14:02:07', '2022-07-27 14:02:07');
INSERT INTO `fault_param_info` VALUES (25, '四级发动机前封头压力异常', 3, 11, 4, '2022-07-27 14:02:18', '2022-07-27 14:02:18');
INSERT INTO `fault_param_info` VALUES (26, '姿控气瓶压力异常', 3, 12, 5, '2022-07-27 14:02:32', '2022-07-27 14:02:32');
INSERT INTO `fault_param_info` VALUES (27, '姿控电磁阀阻值测量异常', 3, 12, 6, '2022-07-27 14:02:55', '2022-07-27 14:02:55');
INSERT INTO `fault_param_info` VALUES (28, '火工品阻值异常', 3, 10, 2, '2022-07-27 14:03:09', '2022-07-27 14:03:09');
INSERT INTO `fault_param_info` VALUES (29, '火工品阻值异常', 3, 10, 3, '2022-07-27 14:03:20', '2022-07-27 14:03:20');

-- ----------------------------
-- Table structure for main_flow_info
-- ----------------------------
DROP TABLE IF EXISTS `main_flow_info`;
CREATE TABLE `main_flow_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `rocket_id` int(11) NOT NULL COMMENT '外键引用rocket_info.id',
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '主流程名称',
  `index` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '主流程顺序号',
  `backInfo` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '回令信息',
  `remark` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '备注',
  `createTime` datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录创建时间',
  `lastUpdateTime` datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 8 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of main_flow_info
-- ----------------------------
INSERT INTO `main_flow_info` VALUES (3, 1, '人员就位', '1', '各岗位报告就位情况', '/', '2022-07-28 11:36:24', '2022-07-28 11:36:24');
INSERT INTO `main_flow_info` VALUES (4, 1, '控制箭上加电', '2', '箭上控制已配电', '报告配电电压值', '2022-07-28 11:36:24', '2022-07-28 11:36:24');
INSERT INTO `main_flow_info` VALUES (5, 1, '测量箭上加电', '3', '箭上测量已配电', '报告配电电压值', '2022-07-28 11:36:24', '2022-07-28 11:36:24');
INSERT INTO `main_flow_info` VALUES (6, 1, '惯组加电', '4', '惯组已配电', '报告配电电压值', '2022-07-28 11:36:25', '2022-07-28 11:36:25');
INSERT INTO `main_flow_info` VALUES (7, 1, '瞄准供电', '5', '瞄准已配电', '报告配电电压值', '2022-07-28 11:36:25', '2022-07-28 11:36:25');

-- ----------------------------
-- Table structure for parameter_info
-- ----------------------------
DROP TABLE IF EXISTS `parameter_info`;
CREATE TABLE `parameter_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '参数名称',
  `type` int(255) NOT NULL DEFAULT 1 COMMENT '参数类型：1表示连续量，2表示开关量',
  `unit` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '单位',
  `createTime` datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录创建时间',
  `lastUpdateTime` datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 110 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of parameter_info
-- ----------------------------
INSERT INTO `parameter_info` VALUES (7, '惯组陀螺电压', 1, 'V', '2022-07-27 19:47:19', '2022-07-27 19:47:19');
INSERT INTO `parameter_info` VALUES (8, '惯组陀螺电流', 1, 'A', '2022-07-27 19:48:09', '2022-07-27 19:48:09');
INSERT INTO `parameter_info` VALUES (9, 'DASL接口电压', 1, 'V', '2022-07-27 19:48:34', '2022-07-27 19:48:34');
INSERT INTO `parameter_info` VALUES (10, 'DASL接口电流', 1, 'A', '2022-07-27 19:49:04', '2022-07-27 19:49:04');
INSERT INTO `parameter_info` VALUES (11, '二级伺服控制+28v', 1, 'V', '2022-07-27 20:06:25', '2022-07-27 20:06:25');
INSERT INTO `parameter_info` VALUES (12, '一级伺服激活电压', 1, 'V', '2022-07-27 20:10:58', '2022-07-27 20:10:58');
INSERT INTO `parameter_info` VALUES (13, '二级伺服激活电压', 1, 'V', '2022-07-27 20:16:09', '2022-07-27 20:16:09');
INSERT INTO `parameter_info` VALUES (14, '传感器电压5V', 1, 'V', '2022-07-27 20:19:14', '2022-07-27 20:19:14');
INSERT INTO `parameter_info` VALUES (15, '传感器电压+15V', 1, 'V', '2022-07-27 20:19:28', '2022-07-27 20:19:28');
INSERT INTO `parameter_info` VALUES (16, '传感器电压-15V', 1, 'V', '2022-07-27 20:19:44', '2022-07-27 20:19:44');
INSERT INTO `parameter_info` VALUES (17, '伺服系统电压', 1, 'V', '2022-07-27 20:19:58', '2022-07-27 20:19:58');
INSERT INTO `parameter_info` VALUES (18, '伺服系统电流', 1, 'A', '2022-07-27 20:20:13', '2022-07-27 20:20:13');
INSERT INTO `parameter_info` VALUES (19, '射频图像电压', 1, 'V', '2022-07-27 20:24:02', '2022-07-27 20:24:02');
INSERT INTO `parameter_info` VALUES (20, '射频图像电流', 1, 'A', '2022-07-27 20:24:20', '2022-07-27 20:24:20');
INSERT INTO `parameter_info` VALUES (21, '电流', 1, 'A', '2022-07-27 20:26:43', '2022-07-27 20:26:43');
INSERT INTO `parameter_info` VALUES (22, '电压', 1, 'V', '2022-07-27 20:27:02', '2022-07-27 20:27:02');
INSERT INTO `parameter_info` VALUES (23, '火工品母线电压', 1, 'V', '2022-07-27 20:29:41', '2022-07-27 20:29:41');
INSERT INTO `parameter_info` VALUES (24, '火工品母线电流', 1, 'A', '2022-07-27 20:30:04', '2022-07-27 20:30:04');
INSERT INTO `parameter_info` VALUES (25, '主电池电压', 1, 'V', '2022-07-27 20:30:16', '2022-07-27 20:30:16');
INSERT INTO `parameter_info` VALUES (26, '主电池电流', 1, 'A', '2022-07-27 20:30:27', '2022-07-27 20:30:27');
INSERT INTO `parameter_info` VALUES (27, '箭载计算机电压', 1, 'V', '2022-07-27 20:30:38', '2022-07-27 20:30:38');
INSERT INTO `parameter_info` VALUES (28, '箭载计算机电流', 1, 'A', '2022-07-27 20:30:47', '2022-07-27 20:30:47');
INSERT INTO `parameter_info` VALUES (29, '射频基带模块电压', 1, 'V', '2022-07-27 20:30:58', '2022-07-27 20:30:58');
INSERT INTO `parameter_info` VALUES (30, '射频基带模块电流', 1, 'A', '2022-07-27 20:31:07', '2022-07-27 20:31:07');
INSERT INTO `parameter_info` VALUES (31, '飞控模块电压', 1, 'V', '2022-07-27 20:31:19', '2022-07-27 20:31:19');
INSERT INTO `parameter_info` VALUES (32, '飞控模块电流', 1, 'A', '2022-07-27 20:31:30', '2022-07-27 20:31:30');
INSERT INTO `parameter_info` VALUES (33, '两器供电15V', 1, 'V', '2022-07-27 20:31:40', '2022-07-27 20:31:40');
INSERT INTO `parameter_info` VALUES (34, '两器供电5V', 1, 'V', '2022-07-27 20:31:53', '2022-07-27 20:31:53');
INSERT INTO `parameter_info` VALUES (35, '气瓶压力', 1, 'P', '2022-07-27 20:37:25', '2022-07-27 20:37:25');
INSERT INTO `parameter_info` VALUES (36, '减压阀出口压力', 1, 'P', '2022-07-27 20:37:42', '2022-07-27 20:37:42');
INSERT INTO `parameter_info` VALUES (37, '推力室压力', 1, 'P', '2022-07-27 20:38:03', '2022-07-27 20:38:03');
INSERT INTO `parameter_info` VALUES (38, '姿控气瓶压力1', 1, 'P', '2022-07-27 20:43:40', '2022-07-27 20:43:40');
INSERT INTO `parameter_info` VALUES (39, '姿控气瓶压力2', 1, 'p', '2022-07-27 20:43:56', '2022-07-27 20:43:56');
INSERT INTO `parameter_info` VALUES (40, '减压阀压力', 1, 'P', '2022-07-27 20:44:09', '2022-07-27 20:44:09');
INSERT INTO `parameter_info` VALUES (41, '尾段温度', 1, '℃', '2022-07-27 20:46:39', '2022-07-27 20:46:39');
INSERT INTO `parameter_info` VALUES (42, '尾段轴向振动', 1, 'N', '2022-07-27 20:47:40', '2022-07-27 20:47:40');
INSERT INTO `parameter_info` VALUES (43, '尾段法向振动', 1, 'N', '2022-07-27 20:47:51', '2022-07-27 20:47:51');
INSERT INTO `parameter_info` VALUES (44, '尾段热流', 1, 'N', '2022-07-27 20:48:02', '2022-07-27 20:48:02');
INSERT INTO `parameter_info` VALUES (45, '前封头压力', 1, 'P', '2022-07-27 20:48:11', '2022-07-27 20:48:11');
INSERT INTO `parameter_info` VALUES (46, '喉部外壁温度', 1, '℃', '2022-07-27 20:48:22', '2022-07-27 20:48:22');
INSERT INTO `parameter_info` VALUES (47, '后封头压力', 1, 'P', '2022-07-27 20:48:34', '2022-07-27 20:48:34');
INSERT INTO `parameter_info` VALUES (48, '后封头温度', 1, '℃', '2022-07-27 20:48:51', '2022-07-27 20:48:51');
INSERT INTO `parameter_info` VALUES (49, '燃烧室压力', 1, 'P', '2022-07-27 20:49:06', '2022-07-27 20:49:06');
INSERT INTO `parameter_info` VALUES (52, '一级点火1', 1, 'N', '2022-07-27 21:05:10', '2022-07-27 21:05:10');
INSERT INTO `parameter_info` VALUES (53, '一级点火2', 1, 'N', '2022-07-27 21:05:24', '2022-07-27 21:05:24');
INSERT INTO `parameter_info` VALUES (54, '一级自毁1', 1, 'N', '2022-07-27 21:05:34', '2022-07-27 21:05:34');
INSERT INTO `parameter_info` VALUES (55, '一级自毁2', 1, 'N', '2022-07-27 21:05:44', '2022-07-27 21:05:44');
INSERT INTO `parameter_info` VALUES (56, '一二级分离1', 1, 'N', '2022-07-27 21:05:55', '2022-07-27 21:05:55');
INSERT INTO `parameter_info` VALUES (57, '一二级分离2', 1, 'N', '2022-07-27 21:06:05', '2022-07-27 21:06:05');
INSERT INTO `parameter_info` VALUES (58, '二级点火1', 1, 'N', '2022-07-27 21:10:34', '2022-07-27 21:10:34');
INSERT INTO `parameter_info` VALUES (59, '二级点火2', 1, 'N', '2022-07-27 21:10:37', '2022-07-27 21:10:37');
INSERT INTO `parameter_info` VALUES (60, '二级自毁1', 1, 'N', '2022-07-27 21:10:42', '2022-07-27 21:10:42');
INSERT INTO `parameter_info` VALUES (61, '二级自毁2', 1, 'N', '2022-07-27 21:10:49', '2022-07-27 21:10:49');
INSERT INTO `parameter_info` VALUES (62, '二三级分离1', 1, 'N', '2022-07-27 21:10:55', '2022-07-27 21:10:55');
INSERT INTO `parameter_info` VALUES (63, '二三级分离2', 1, 'N', '2022-07-27 21:11:01', '2022-07-27 21:11:01');
INSERT INTO `parameter_info` VALUES (64, '三级点火1', 1, 'N', '2022-07-27 21:13:32', '2022-07-27 21:13:32');
INSERT INTO `parameter_info` VALUES (65, '三级点火2', 1, 'N', '2022-07-27 21:13:38', '2022-07-27 21:13:38');
INSERT INTO `parameter_info` VALUES (66, '三级自毁1', 1, 'N', '2022-07-27 21:13:49', '2022-07-27 21:13:49');
INSERT INTO `parameter_info` VALUES (67, '三级自毁2', 1, 'N', '2022-07-27 21:13:55', '2022-07-27 21:13:55');
INSERT INTO `parameter_info` VALUES (68, '三四级分离1', 1, 'N', '2022-07-27 21:14:00', '2022-07-27 21:14:00');
INSERT INTO `parameter_info` VALUES (69, '三四级分离2', 1, 'N', '2022-07-27 21:14:03', '2022-07-27 21:14:03');
INSERT INTO `parameter_info` VALUES (70, '四级点火1', 1, 'N', '2022-07-27 21:16:42', '2022-07-27 21:16:42');
INSERT INTO `parameter_info` VALUES (71, '四级点火2', 1, 'N', '2022-07-27 21:16:48', '2022-07-27 21:16:48');
INSERT INTO `parameter_info` VALUES (72, '四级自毁1', 1, 'N', '2022-07-27 21:16:54', '2022-07-27 21:16:54');
INSERT INTO `parameter_info` VALUES (73, '四级自毁2', 1, 'N', '2022-07-27 21:17:00', '2022-07-27 21:17:00');
INSERT INTO `parameter_info` VALUES (74, '星箭分离1', 1, 'N', '2022-07-27 21:17:09', '2022-07-27 21:17:09');
INSERT INTO `parameter_info` VALUES (75, '星箭分离2', 1, 'N', '2022-07-27 21:17:16', '2022-07-27 21:17:16');
INSERT INTO `parameter_info` VALUES (76, '姿控电磁阀1', 1, 'N', '2022-07-27 21:17:26', '2022-07-27 21:17:26');
INSERT INTO `parameter_info` VALUES (77, '姿控电磁阀2', 1, 'N', '2022-07-27 21:17:29', '2022-07-27 21:17:29');
INSERT INTO `parameter_info` VALUES (78, '姿控电磁阀3', 1, 'N', '2022-07-27 21:17:42', '2022-07-27 21:17:42');
INSERT INTO `parameter_info` VALUES (79, '姿控电磁阀4', 1, 'N', '2022-07-27 21:17:54', '2022-07-27 21:17:54');
INSERT INTO `parameter_info` VALUES (80, '姿控电磁阀5', 1, 'N', '2022-07-27 21:17:57', '2022-07-27 21:17:57');
INSERT INTO `parameter_info` VALUES (81, '姿控电磁阀6', 1, 'N', '2022-07-27 21:18:06', '2022-07-27 21:18:06');
INSERT INTO `parameter_info` VALUES (82, '姿控电磁阀7', 1, 'N', '2022-07-27 21:18:20', '2022-07-27 21:18:20');
INSERT INTO `parameter_info` VALUES (83, '姿控电磁阀8', 1, 'N', '2022-07-27 21:18:31', '2022-07-27 21:18:31');
INSERT INTO `parameter_info` VALUES (84, '姿控电爆阀', 1, 'N', '2022-07-27 21:18:38', '2022-07-27 21:18:38');
INSERT INTO `parameter_info` VALUES (85, '整流罩分离(纵向)1', 1, 'N', '2022-07-27 21:22:29', '2022-07-27 21:22:29');
INSERT INTO `parameter_info` VALUES (86, '整流罩分离(纵向)2', 1, 'N', '2022-07-27 21:22:35', '2022-07-27 21:22:35');
INSERT INTO `parameter_info` VALUES (87, '整流罩分离(横向)1', 1, 'N', '2022-07-27 21:22:43', '2022-07-27 21:22:43');
INSERT INTO `parameter_info` VALUES (88, '整流罩分离(横向)2', 1, 'N', '2022-07-27 21:22:50', '2022-07-27 21:22:50');
INSERT INTO `parameter_info` VALUES (89, '压力', 1, 'P', '2022-07-28 10:25:23', '2022-07-28 10:25:23');
INSERT INTO `parameter_info` VALUES (90, '环境温度', 1, '℃', '2022-07-28 10:25:33', '2022-07-28 10:25:33');
INSERT INTO `parameter_info` VALUES (91, '轴向振动', 1, 'N', '2022-07-28 10:25:42', '2022-07-28 10:25:42');
INSERT INTO `parameter_info` VALUES (92, '法向振动', 1, 'N', '2022-07-28 10:25:48', '2022-07-28 10:25:48');
INSERT INTO `parameter_info` VALUES (93, '支架轴向振动', 1, 'N', '2022-07-28 10:29:43', '2022-07-28 10:29:43');
INSERT INTO `parameter_info` VALUES (94, '支架法向振动', 1, 'N', '2022-07-28 10:29:52', '2022-07-28 10:29:52');
INSERT INTO `parameter_info` VALUES (95, '支架横向振动', 1, 'N', '2022-07-28 10:29:58', '2022-07-28 10:29:58');
INSERT INTO `parameter_info` VALUES (96, '锥段振动', 1, 'N', '2022-07-28 10:32:33', '2022-07-28 10:32:33');
INSERT INTO `parameter_info` VALUES (97, '柱段振动', 1, 'N', '2022-07-28 10:32:40', '2022-07-28 10:32:40');
INSERT INTO `parameter_info` VALUES (98, '锥段热流', 1, 'N', '2022-07-28 10:32:46', '2022-07-28 10:32:46');
INSERT INTO `parameter_info` VALUES (99, '锥段冲击', 1, 'N', '2022-07-28 10:32:52', '2022-07-28 10:32:52');
INSERT INTO `parameter_info` VALUES (100, '柱段冲击', 1, 'N', '2022-07-28 10:33:01', '2022-07-28 10:33:01');
INSERT INTO `parameter_info` VALUES (101, '柱段热流', 1, 'N', '2022-07-28 10:33:10', '2022-07-28 10:33:10');
INSERT INTO `parameter_info` VALUES (102, '伺服温度1', 1, '℃', '2022-07-28 11:06:41', '2022-07-28 11:06:41');
INSERT INTO `parameter_info` VALUES (103, '伺服温度2', 1, '℃', '2022-07-28 11:06:45', '2022-07-28 11:06:45');
INSERT INTO `parameter_info` VALUES (104, '尾段压力', 1, 'P', '2022-07-28 11:13:48', '2022-07-28 11:13:48');
INSERT INTO `parameter_info` VALUES (105, '20机功率遥测', 1, 'N', '2022-07-28 11:27:19', '2022-07-28 11:27:19');
INSERT INTO `parameter_info` VALUES (106, '20机灵敏度遥测', 1, 'N', '2022-07-28 11:27:22', '2022-07-28 11:27:22');
INSERT INTO `parameter_info` VALUES (107, '40机视放遥测', 1, 'N', '2022-07-28 11:27:30', '2022-07-28 11:27:30');
INSERT INTO `parameter_info` VALUES (108, '40机指令遥测', 1, 'N', '2022-07-28 11:27:37', '2022-07-28 11:27:37');
INSERT INTO `parameter_info` VALUES (109, '信号强度', 1, 'N', '2022-07-28 11:27:40', '2022-07-28 11:27:40');

-- ----------------------------
-- Table structure for rockect_param_info
-- ----------------------------
DROP TABLE IF EXISTS `rockect_param_info`;
CREATE TABLE `rockect_param_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `rocket_data_id` int(11) NULL DEFAULT NULL COMMENT '箭上数据返回指令ID,外键引用rocket_data_info.id',
  `parameter_id` int(255) NOT NULL COMMENT '外键引用parameter_info.id',
  `index` int(11) NULL DEFAULT NULL COMMENT '索引顺序，从1开始计数',
  `length` int(11) NULL DEFAULT NULL COMMENT '参数字节长度',
  `resultType` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '参数类型包括：unsigned char、unsigned short、unsigned int、char、short、int、float',
  `createTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录创建时间',
  `lastUpdateTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for rocket_data_info
-- ----------------------------
DROP TABLE IF EXISTS `rocket_data_info`;
CREATE TABLE `rocket_data_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `rocket_id` int(11) NOT NULL COMMENT '外键引用rocket_info.id',
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '指令名称',
  `code` int(255) NULL DEFAULT NULL COMMENT '指令编码',
  `prefix` int(255) NULL DEFAULT NULL COMMENT '指令前缀，例如0x55AA',
  `createTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录创建时间',
  `lastUpdateTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for rocket_info
-- ----------------------------
DROP TABLE IF EXISTS `rocket_info`;
CREATE TABLE `rocket_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '型号名称',
  `code` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '火箭型号',
  `createTime` datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录创建时间',
  `lastUpdateTime` datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 3 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of rocket_info
-- ----------------------------
INSERT INTO `rocket_info` VALUES (1, '灵龙', 'Ling', '2022-07-22 23:44:25', '2022-07-22 23:44:25');
INSERT INTO `rocket_info` VALUES (2, '长征11', 'CZ11', '2022-07-22 23:44:41', '2022-07-22 23:44:41');

-- ----------------------------
-- Table structure for scene_info
-- ----------------------------
DROP TABLE IF EXISTS `scene_info`;
CREATE TABLE `scene_info`  (
  `sceneID` int(255) NOT NULL,
  `sceneName` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`sceneID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for secne1
-- ----------------------------
DROP TABLE IF EXISTS `secne1`;
CREATE TABLE `secne1`  (
  `objID` int(11) NOT NULL AUTO_INCREMENT,
  `objType` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `objBindParam` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '',
  `objPosX` double(255, 1) NOT NULL,
  `objPosY` double(255, 1) NOT NULL,
  PRIMARY KEY (`objID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for status_info
-- ----------------------------
DROP TABLE IF EXISTS `status_info`;
CREATE TABLE `status_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '设备状态名称',
  `createTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录创建时间',
  `lastUpdateTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 9 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of status_info
-- ----------------------------
INSERT INTO `status_info` VALUES (1, '配电', '2022-07-27 19:51:14', '2022-07-27 19:51:14');
INSERT INTO `status_info` VALUES (2, '断电', '2022-07-27 19:51:22', '2022-07-27 19:51:22');
INSERT INTO `status_info` VALUES (3, '故障', '2022-07-27 19:51:34', '2022-07-27 19:51:34');
INSERT INTO `status_info` VALUES (4, '激活', '2022-07-27 20:11:41', '2022-07-27 20:11:41');
INSERT INTO `status_info` VALUES (5, 'static', '2022-07-27 20:40:08', '2022-07-27 20:40:08');
INSERT INTO `status_info` VALUES (6, 'fire', '2022-07-27 20:40:17', '2022-07-27 20:40:17');
INSERT INTO `status_info` VALUES (7, 'zero', '2022-07-27 20:40:25', '2022-07-27 20:40:25');
INSERT INTO `status_info` VALUES (8, 'fire_befor', '2022-07-28 10:34:22', '2022-07-28 10:34:22');

-- ----------------------------
-- Table structure for sub_flow_info
-- ----------------------------
DROP TABLE IF EXISTS `sub_flow_info`;
CREATE TABLE `sub_flow_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `main_id` int(11) NOT NULL COMMENT '主流程ID，外键引用main_flow_info.id',
  `command_id` int(11) NULL DEFAULT NULL COMMENT '测发指令，外键引用command_info.id',
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '子流程名称',
  `remark` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '备注',
  `createTime` datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录创建时间',
  `lastUpdateTime` datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 11 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of sub_flow_info
-- ----------------------------
INSERT INTO `sub_flow_info` VALUES (3, 3, 4, '各岗位人员就绪', '/', '2022-07-28 11:36:24', '2022-07-28 11:36:24');
INSERT INTO `sub_flow_info` VALUES (4, 4, 1, '综控机上电', '报告配电电压值', '2022-07-28 11:36:24', '2022-07-28 11:36:24');
INSERT INTO `sub_flow_info` VALUES (5, 4, 2, '伺服上电', '报告配电电压值', '2022-07-28 11:36:24', '2022-07-28 11:36:24');
INSERT INTO `sub_flow_info` VALUES (6, 5, 5, '传感器配电', '报告配电电压值', '2022-07-28 11:36:24', '2022-07-28 11:36:24');
INSERT INTO `sub_flow_info` VALUES (7, 5, 6, '射频图像配电', '报告配电电压值', '2022-07-28 11:36:25', '2022-07-28 11:36:25');
INSERT INTO `sub_flow_info` VALUES (8, 5, 7, '遥测站配电', '报告配电电压值', '2022-07-28 11:36:25', '2022-07-28 11:36:25');
INSERT INTO `sub_flow_info` VALUES (9, 6, 8, '惯组配电', '报告配电电压值', '2022-07-28 11:36:25', '2022-07-28 11:36:25');
INSERT INTO `sub_flow_info` VALUES (10, 7, 9, '瞄准供电', '报告配电电压值', '2022-07-28 11:36:25', '2022-07-28 11:36:25');

-- ----------------------------
-- Table structure for switch_show_info
-- ----------------------------
DROP TABLE IF EXISTS `switch_show_info`;
CREATE TABLE `switch_show_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `param_id` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '外键parameter_info.id',
  `switch_valueid` int(11) NULL DEFAULT NULL COMMENT '外键switch_value.id',
  `status_value` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '状态索引值',
  `createTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录创建时间',
  `lastUpdateTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for switch_value
-- ----------------------------
DROP TABLE IF EXISTS `switch_value`;
CREATE TABLE `switch_value`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '状态值名称',
  `createTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录创建时间',
  `lastUpdateTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for user_info
-- ----------------------------
DROP TABLE IF EXISTS `user_info`;
CREATE TABLE `user_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userName` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `password` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `createTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP,
  `lastUpdateTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 6 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of user_info
-- ----------------------------
INSERT INTO `user_info` VALUES (1, 'pengbo', '123', '2022-07-14 11:35:50', '2022-07-14 11:35:50');
INSERT INTO `user_info` VALUES (2, 'lijin', '123', '2022-07-14 11:36:06', '2022-07-14 11:36:06');
INSERT INTO `user_info` VALUES (3, 'yangwenhan', '123', '2022-07-14 11:36:27', '2022-07-14 11:36:27');
INSERT INTO `user_info` VALUES (4, 'yangluguo', '123', '2022-07-14 11:36:40', '2022-07-14 11:36:40');
INSERT INTO `user_info` VALUES (5, 'xuxiaojian', '123', '2022-07-14 11:36:56', '2022-07-14 11:36:56');

SET FOREIGN_KEY_CHECKS = 1;
