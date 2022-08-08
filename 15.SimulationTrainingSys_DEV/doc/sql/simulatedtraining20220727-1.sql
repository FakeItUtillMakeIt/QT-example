/*
 Navicat Premium Data Transfer

 Source Server         : loacalhost
 Source Server Type    : MySQL
 Source Server Version : 80015
 Source Host           : localhost:3306
 Source Schema         : simulatedtraining

 Target Server Type    : MySQL
 Target Server Version : 80015
 File Encoding         : 65001

 Date: 27/07/2022 14:04:48
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
  `prefix` int(11) NULL DEFAULT 43605 COMMENT '指令前缀，例如0x55AA',
  `createTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录创建时间',
  `lastUpdateTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 4 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of command_info
-- ----------------------------
INSERT INTO `command_info` VALUES (1, 2, 3, '综控机上电', 1, 1, 43605, '2022-07-23 16:49:14', '2022-07-23 16:49:14');
INSERT INTO `command_info` VALUES (2, 2, 3, '伺服上电', 2, 1, 43605, '2022-07-23 16:50:35', '2022-07-23 16:50:35');
INSERT INTO `command_info` VALUES (3, 2, 0, '测发回令', 3, 2, 47957, '2022-07-23 18:12:55', '2022-07-23 18:12:55');

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
) ENGINE = InnoDB AUTO_INCREMENT = 4 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of device_info
-- ----------------------------
INSERT INTO `device_info` VALUES (1, 2, '综控机', 1, '2022-07-23 11:29:29', '2022-07-23 11:29:29');
INSERT INTO `device_info` VALUES (2, 2, '伺服系统', 1, '2022-07-23 11:29:51', '2022-07-23 11:29:51');
INSERT INTO `device_info` VALUES (3, 2, '射频模块', 1, '2022-07-23 11:30:42', '2022-07-23 11:30:42');

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
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of device_param_info
-- ----------------------------
INSERT INTO `device_param_info` VALUES (1, 1, 1, '2022-07-26 23:22:15', '2022-07-26 23:22:15');
INSERT INTO `device_param_info` VALUES (2, 1, 2, '2022-07-26 23:22:23', '2022-07-26 23:22:23');
INSERT INTO `device_param_info` VALUES (3, 1, 3, '2022-07-26 23:22:31', '2022-07-26 23:22:31');
INSERT INTO `device_param_info` VALUES (4, 1, 4, '2022-07-26 23:22:40', '2022-07-26 23:22:40');
INSERT INTO `device_param_info` VALUES (5, 1, 5, '2022-07-26 23:22:47', '2022-07-26 23:22:47');
INSERT INTO `device_param_info` VALUES (6, 1, 6, '2022-07-26 23:22:53', '2022-07-26 23:22:53');
INSERT INTO `device_param_info` VALUES (7, 1, 7, '2022-07-26 23:22:58', '2022-07-26 23:22:58');
INSERT INTO `device_param_info` VALUES (8, 1, 8, '2022-07-26 23:23:02', '2022-07-26 23:23:02');
INSERT INTO `device_param_info` VALUES (9, 1, 9, '2022-07-26 23:23:08', '2022-07-26 23:23:08');
INSERT INTO `device_param_info` VALUES (10, 1, 10, '2022-07-26 23:23:13', '2022-07-26 23:23:13');
INSERT INTO `device_param_info` VALUES (11, 1, 11, '2022-07-26 23:23:19', '2022-07-26 23:23:19');
INSERT INTO `device_param_info` VALUES (12, 1, 12, '2022-07-26 23:23:25', '2022-07-26 23:23:25');

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
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of device_status_info
-- ----------------------------
INSERT INTO `device_status_info` VALUES (1, 1, 1, 'initator_activate.txt', '2022-07-26 23:45:24', '2022-07-26 23:45:24');
INSERT INTO `device_status_info` VALUES (2, 1, 2, 'initator_nonactivate.txt', '2022-07-26 23:45:58', '2022-07-26 23:45:58');
INSERT INTO `device_status_info` VALUES (3, 1, 3, 'initator_nonactivate.txt', '2022-07-26 23:46:06', '2022-07-26 23:46:06');

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
) ENGINE = InnoDB AUTO_INCREMENT = 6 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of fault_command_info
-- ----------------------------
INSERT INTO `fault_command_info` VALUES (1, '指令故障1', 1, 2, 2, '2022-07-23 10:20:40', '2022-07-23 10:20:40');
INSERT INTO `fault_command_info` VALUES (2, '指令故障2', 2, 2, 2, '2022-07-23 10:20:55', '2022-07-23 10:20:55');
INSERT INTO `fault_command_info` VALUES (3, '指令故障3', 3, 2, 2, '2022-07-23 10:21:09', '2022-07-23 10:21:09');
INSERT INTO `fault_command_info` VALUES (6, '指令故障2', 4, 2, 2, '2022-07-25 09:34:38', '2022-07-25 09:34:38');

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
) ENGINE = InnoDB AUTO_INCREMENT = 3 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of fault_param_info
-- ----------------------------
INSERT INTO `fault_param_info` VALUES (1, '故障1', 1, 2, 2, '2022-07-23 10:09:37', '2022-07-23 10:09:37');
INSERT INTO `fault_param_info` VALUES (2, '故障2', 2, 2, 2, '2022-07-23 10:09:10', '2022-07-23 10:09:10');
INSERT INTO `fault_param_info` VALUES (3, '故障3', 3, 2, 2, '2022-07-23 10:11:02', '2022-07-23 10:11:02');

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
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

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
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of parameter_info
-- ----------------------------
INSERT INTO `parameter_info` VALUES (1, '火工品母线电压', 1, 'mV', '2022-07-26 23:17:38', '2022-07-26 23:17:38');
INSERT INTO `parameter_info` VALUES (2, '火工品母线电流', 1, 'mA', '2022-07-26 23:17:51', '2022-07-26 23:17:51');
INSERT INTO `parameter_info` VALUES (3, '主电池电压', 1, 'V', '2022-07-26 23:18:33', '2022-07-26 23:18:33');
INSERT INTO `parameter_info` VALUES (4, '主电池电流', 1, 'A', '2022-07-26 23:18:46', '2022-07-26 23:18:46');
INSERT INTO `parameter_info` VALUES (5, '箭载计算机电压', 1, 'V', '2022-07-26 23:19:05', '2022-07-26 23:19:05');
INSERT INTO `parameter_info` VALUES (6, '箭载计算机电流', 1, 'A', '2022-07-26 23:19:19', '2022-07-26 23:19:19');
INSERT INTO `parameter_info` VALUES (7, '射频基带模块电压', 1, 'V', '2022-07-26 23:19:32', '2022-07-26 23:19:32');
INSERT INTO `parameter_info` VALUES (8, '射频基带模块电流', 1, 'A', '2022-07-26 23:19:47', '2022-07-26 23:19:47');
INSERT INTO `parameter_info` VALUES (9, '飞控模块电压', 1, 'V', '2022-07-26 23:20:00', '2022-07-26 23:20:00');
INSERT INTO `parameter_info` VALUES (10, '飞控模块电流', 1, 'A', '2022-07-26 23:20:18', '2022-07-26 23:20:18');
INSERT INTO `parameter_info` VALUES (11, '两器供电15V', 1, 'V', '2022-07-26 23:20:42', '2022-07-26 23:20:42');
INSERT INTO `parameter_info` VALUES (12, '两器供电5V', 1, 'V', '2022-07-26 23:20:48', '2022-07-26 23:20:48');

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
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

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
-- Records of status_info
-- ----------------------------
INSERT INTO `status_info` VALUES (1, '配电', '2022-07-26 23:42:37', '2022-07-26 23:42:37');
INSERT INTO `status_info` VALUES (2, '断电', '2022-07-26 23:42:41', '2022-07-26 23:42:41');
INSERT INTO `status_info` VALUES (3, '激活', '2022-07-26 23:42:48', '2022-07-26 23:42:48');
INSERT INTO `status_info` VALUES (6, '故障', '2022-07-26 23:43:23', '2022-07-26 23:43:23');

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
