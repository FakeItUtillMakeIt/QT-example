/*
 Navicat Premium Data Transfer

 Source Server         : localhost
 Source Server Type    : MySQL
 Source Server Version : 80015
 Source Host           : localhost:3306
 Source Schema         : simulatedtraining

 Target Server Type    : MySQL
 Target Server Version : 80015
 File Encoding         : 65001

 Date: 15/07/2022 09:59:15
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

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
  `prefix` int(11) NULL DEFAULT NULL COMMENT '指令前缀，例如0x55AA',
  `createTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录创建时间',
  `lastUpdateTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

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
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for device_info
-- ----------------------------
DROP TABLE IF EXISTS `device_info`;
CREATE TABLE `device_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `rocket_id` int(11) NULL DEFAULT NULL COMMENT '外键引用rocket_info.id',
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '设备名称',
  `isVirtual` int(11) NULL DEFAULT 1 COMMENT '0表示真实设备，1表示虚拟设备',
  `createTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录创建时间',
  `lastUpdateTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

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
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

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
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for parameter_info
-- ----------------------------
DROP TABLE IF EXISTS `parameter_info`;
CREATE TABLE `parameter_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '参数名称',
  `type` int(255) NOT NULL COMMENT '参数类型：1表示连续量，2表示开关量',
  `unit` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '单位',
  `createTime` datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录创建时间',
  `lastUpdateTime` datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

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
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

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
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

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
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

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
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

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
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

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
) ENGINE = InnoDB AUTO_INCREMENT = 4 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of user_info
-- ----------------------------
INSERT INTO `user_info` VALUES (1, 'pengbo', '123', '2022-07-14 11:35:50', '2022-07-14 11:35:50');
INSERT INTO `user_info` VALUES (2, 'lijin', '123', '2022-07-14 11:36:06', '2022-07-14 11:36:06');
INSERT INTO `user_info` VALUES (3, 'yangwenhan', '123', '2022-07-14 11:36:27', '2022-07-14 11:36:27');
INSERT INTO `user_info` VALUES (4, 'yangluguo', '123', '2022-07-14 11:36:40', '2022-07-14 11:36:40');
INSERT INTO `user_info` VALUES (5, 'xuxiaojian', '123', '2022-07-14 11:36:56', '2022-07-14 11:36:56');

SET FOREIGN_KEY_CHECKS = 1;
