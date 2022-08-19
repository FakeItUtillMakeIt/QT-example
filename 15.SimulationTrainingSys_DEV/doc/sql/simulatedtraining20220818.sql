/*
 Navicat Premium Data Transfer

 Source Server         : database
 Source Server Type    : MySQL
 Source Server Version : 80015
 Source Host           : localhost:3306
 Source Schema         : simulatedtraining

 Target Server Type    : MySQL
 Target Server Version : 80015
 File Encoding         : 65001

 Date: 18/08/2022 12:10:13
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for command_devicestatus_info
-- ----------------------------
DROP TABLE IF EXISTS `command_devicestatus_info`;
CREATE TABLE `command_devicestatus_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `command_id` int(11) NOT NULL COMMENT '外键应用command_info',
  `deviceStatus_id` int(11) NOT NULL COMMENT '外键引用devicestatus_info',
  `createTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录创建时间',
  `lastUpdateTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 21 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of command_devicestatus_info
-- ----------------------------
INSERT INTO `command_devicestatus_info` VALUES (7, 48, 103, '2022-08-16 20:18:13', '2022-08-16 20:18:13');
INSERT INTO `command_devicestatus_info` VALUES (8, 48, 105, '2022-08-16 20:18:17', '2022-08-16 20:18:17');
INSERT INTO `command_devicestatus_info` VALUES (9, 50, 104, '2022-08-16 20:19:42', '2022-08-16 20:19:42');
INSERT INTO `command_devicestatus_info` VALUES (10, 50, 106, '2022-08-16 20:19:49', '2022-08-16 20:19:49');
INSERT INTO `command_devicestatus_info` VALUES (11, 54, 108, '2022-08-17 17:15:49', '2022-08-17 17:15:49');
INSERT INTO `command_devicestatus_info` VALUES (12, 54, 110, '2022-08-17 17:16:26', '2022-08-17 17:16:26');
INSERT INTO `command_devicestatus_info` VALUES (13, 54, 112, '2022-08-17 17:16:36', '2022-08-17 17:16:36');
INSERT INTO `command_devicestatus_info` VALUES (14, 54, 114, '2022-08-17 17:16:53', '2022-08-17 17:16:53');
INSERT INTO `command_devicestatus_info` VALUES (15, 54, 116, '2022-08-17 17:17:26', '2022-08-17 17:17:26');
INSERT INTO `command_devicestatus_info` VALUES (16, 54, 118, '2022-08-17 17:17:35', '2022-08-17 17:17:35');
INSERT INTO `command_devicestatus_info` VALUES (17, 58, 120, '2022-08-17 17:17:54', '2022-08-17 17:17:54');
INSERT INTO `command_devicestatus_info` VALUES (18, 58, 121, '2022-08-17 17:18:03', '2022-08-17 17:18:03');
INSERT INTO `command_devicestatus_info` VALUES (19, 58, 122, '2022-08-17 17:18:20', '2022-08-17 17:18:20');
INSERT INTO `command_devicestatus_info` VALUES (20, 54, 123, '2022-08-17 17:23:26', '2022-08-17 17:23:26');

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
) ENGINE = InnoDB AUTO_INCREMENT = 91 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of command_info
-- ----------------------------
INSERT INTO `command_info` VALUES (47, 1, 0, '箭上配电回令', 46, 2, 21930, '2022-08-16 20:17:23', '2022-08-16 20:17:23');
INSERT INTO `command_info` VALUES (48, 1, 47, '箭上配电指令', 37, 1, 21930, '2022-08-16 20:17:44', '2022-08-16 20:17:44');
INSERT INTO `command_info` VALUES (49, 1, 0, '箭上断电回令', 255, 2, 21930, '2022-08-16 20:19:11', '2022-08-16 20:19:11');
INSERT INTO `command_info` VALUES (50, 1, 49, '箭上断电指令', 0, 1, 21930, '2022-08-16 20:19:29', '2022-08-16 20:19:29');
INSERT INTO `command_info` VALUES (51, 1, 0, '平板车就位回令', 484, 2, 21930, '2022-08-16 20:28:39', '2022-08-16 20:28:39');
INSERT INTO `command_info` VALUES (52, 1, 88, '拆除绑扎', 219, 1, 21930, '2022-08-16 20:29:02', '2022-08-16 20:29:02');
INSERT INTO `command_info` VALUES (53, 1, 0, '控制系统配电回令', 47, 2, 21930, '2022-08-16 20:29:53', '2022-08-16 20:29:53');
INSERT INTO `command_info` VALUES (54, 1, 53, '控制系统配电指令', 36, 1, 21930, '2022-08-16 20:30:07', '2022-08-16 20:30:07');
INSERT INTO `command_info` VALUES (55, 1, 0, '转恒压并网回令', 254, 2, 21930, '2022-08-16 20:30:42', '2022-08-16 20:30:42');
INSERT INTO `command_info` VALUES (56, 1, 55, '转恒压并网指令', 1, 1, 21930, '2022-08-16 20:31:04', '2022-08-16 20:31:04');
INSERT INTO `command_info` VALUES (57, 1, 0, '火工品阻值测量回令', 485, 2, 21930, '2022-08-16 20:31:30', '2022-08-16 20:31:30');
INSERT INTO `command_info` VALUES (58, 1, 57, '火工品阻值测量指令', 218, 1, 21930, '2022-08-16 20:31:49', '2022-08-16 20:31:49');
INSERT INTO `command_info` VALUES (59, 1, 0, '综控机上电异常回令', 59, 3, 21930, '2022-08-16 20:44:46', '2022-08-16 20:44:46');
INSERT INTO `command_info` VALUES (60, 1, 59, '综控机上电异常指令', 60, 3, 21930, '2022-08-16 20:44:46', '2022-08-16 20:44:46');
INSERT INTO `command_info` VALUES (61, 1, 0, '支腿检查', 44, 1, 21930, '2022-08-16 20:52:00', '2022-08-16 20:52:00');
INSERT INTO `command_info` VALUES (62, 1, 0, '集装箱配电', 35, 1, 21930, '2022-08-16 20:52:14', '2022-08-16 20:52:14');
INSERT INTO `command_info` VALUES (63, 1, 0, '调平支腿门', 256, 1, 21930, '2022-08-16 20:52:35', '2022-08-16 20:52:35');
INSERT INTO `command_info` VALUES (64, 1, 47, '整车调平', 257, 1, 21930, '2022-08-16 20:52:52', '2022-08-16 20:52:52');
INSERT INTO `command_info` VALUES (65, 1, 0, '连接脱插', 6, 1, 21930, '2022-08-16 20:53:29', '2022-08-16 20:53:29');
INSERT INTO `command_info` VALUES (66, 1, 0, '地面状态检查', 2, 1, 21930, '2022-08-16 20:53:40', '2022-08-16 20:53:40');
INSERT INTO `command_info` VALUES (67, 1, 0, '配电设备开机', 378, 1, 21930, '2022-08-16 20:54:00', '2022-08-16 20:54:00');
INSERT INTO `command_info` VALUES (68, 1, 0, '前端交换机开机', 17, 1, 21930, '2022-08-16 20:54:19', '2022-08-16 20:54:19');
INSERT INTO `command_info` VALUES (69, 1, 0, '平板车离场', 5, 1, 21930, '2022-08-16 21:00:15', '2022-08-16 21:00:15');
INSERT INTO `command_info` VALUES (70, 1, 0, '箱体展开', 251, 1, 21930, '2022-08-16 21:00:27', '2022-08-16 21:00:27');
INSERT INTO `command_info` VALUES (71, 1, 0, '地面测发设备展开', 40, 1, 21930, '2022-08-16 21:00:43', '2022-08-16 21:00:43');
INSERT INTO `command_info` VALUES (72, 1, 0, '卫星测试', 11, 1, 21930, '2022-08-16 21:00:55', '2022-08-16 21:00:55');
INSERT INTO `command_info` VALUES (73, 1, 0, '拆除卫星测试电缆', 8, 1, 21930, '2022-08-16 21:01:17', '2022-08-16 21:01:17');
INSERT INTO `command_info` VALUES (74, 1, 0, '火箭起竖', 9, 1, 21930, '2022-08-16 21:01:30', '2022-08-16 21:01:30');
INSERT INTO `command_info` VALUES (75, 1, 90, '关闭箱体', 7, 1, 21930, '2022-08-16 21:01:47', '2022-08-16 21:01:47');
INSERT INTO `command_info` VALUES (76, 1, 0, '打开集装箱支腿', 12, 1, 21930, '2022-08-17 11:35:43', '2022-08-17 11:35:43');
INSERT INTO `command_info` VALUES (77, 1, 0, '控制系统上电指令异常回令', 77, 3, 21930, '2022-08-17 17:48:27', '2022-08-17 17:48:27');
INSERT INTO `command_info` VALUES (78, 1, 77, '控制系统上电指令异常指令', 78, 3, 21930, '2022-08-17 17:48:27', '2022-08-17 17:48:27');
INSERT INTO `command_info` VALUES (79, 1, 0, '数据服务器开机', 487, 1, 21930, '2022-08-18 10:35:12', '2022-08-18 10:35:12');
INSERT INTO `command_info` VALUES (80, 1, 0, '前端计算机开机', 480, 1, 21930, '2022-08-18 10:40:08', '2022-08-18 10:40:08');
INSERT INTO `command_info` VALUES (81, 1, 0, '遥测站开机', 30, 1, 21930, '2022-08-18 10:40:26', '2022-08-18 10:40:26');
INSERT INTO `command_info` VALUES (82, 1, 0, '外安综测仪开机', 174, 1, 21930, '2022-08-18 10:46:51', '2022-08-18 10:46:51');
INSERT INTO `command_info` VALUES (83, 1, 0, '主控计算机开机', 3, 1, 21930, '2022-08-18 10:47:05', '2022-08-18 10:47:05');
INSERT INTO `command_info` VALUES (84, 1, 0, '启动主控软件', 4, 1, 21930, '2022-08-18 10:47:24', '2022-08-18 10:47:24');
INSERT INTO `command_info` VALUES (85, 1, 0, '瞄准设备开机', 481, 1, 21930, '2022-08-18 10:47:45', '2022-08-18 10:47:45');
INSERT INTO `command_info` VALUES (86, 1, 0, '运行实时监测软件', 31, 1, 21930, '2022-08-18 10:48:07', '2022-08-18 10:48:07');
INSERT INTO `command_info` VALUES (88, 1, 0, '拆除绑扎回令', 242, 2, 21930, '2022-08-18 11:07:22', '2022-08-18 11:07:22');
INSERT INTO `command_info` VALUES (89, 1, 51, '平板车就位', 16, 1, 21930, '2022-08-18 11:19:42', '2022-08-18 11:19:42');
INSERT INTO `command_info` VALUES (90, 1, 0, '关闭箱体回令', 32, 2, 21930, '2022-08-18 11:25:15', '2022-08-18 11:25:15');

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
) ENGINE = InnoDB AUTO_INCREMENT = 5 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

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
) ENGINE = InnoDB AUTO_INCREMENT = 51 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of device_info
-- ----------------------------
INSERT INTO `device_info` VALUES (40, 1, '综控机', 1, '2022-08-16 20:11:57', '2022-08-16 20:11:57');
INSERT INTO `device_info` VALUES (41, 1, '主电池', 1, '2022-08-16 20:15:35', '2022-08-16 20:15:35');
INSERT INTO `device_info` VALUES (42, 1, '惯组', 0, '2022-08-17 16:35:28', '2022-08-17 16:35:28');
INSERT INTO `device_info` VALUES (43, 1, '陀螺', 1, '2022-08-17 16:35:55', '2022-08-17 16:35:55');
INSERT INTO `device_info` VALUES (44, 1, 'GPS', 1, '2022-08-17 16:36:02', '2022-08-17 16:36:02');
INSERT INTO `device_info` VALUES (45, 1, '飞控模块', 1, '2022-08-17 16:36:15', '2022-08-17 16:36:15');
INSERT INTO `device_info` VALUES (46, 1, '40机', 1, '2022-08-17 16:36:32', '2022-08-17 16:36:32');
INSERT INTO `device_info` VALUES (47, 1, '时序模块', 1, '2022-08-17 16:36:47', '2022-08-17 16:36:47');
INSERT INTO `device_info` VALUES (48, 1, '一级伺服电池', 1, '2022-08-17 16:37:02', '2022-08-17 16:37:02');
INSERT INTO `device_info` VALUES (49, 1, '二级伺服电池', 1, '2022-08-17 16:37:09', '2022-08-17 16:37:09');
INSERT INTO `device_info` VALUES (50, 1, '一级发动机', 1, '2022-08-17 16:37:17', '2022-08-17 16:37:17');

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
) ENGINE = InnoDB AUTO_INCREMENT = 220 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of device_param_info
-- ----------------------------
INSERT INTO `device_param_info` VALUES (200, 40, 117, '2022-08-16 20:15:16', '2022-08-16 20:15:16');
INSERT INTO `device_param_info` VALUES (201, 40, 118, '2022-08-16 20:15:21', '2022-08-16 20:15:21');
INSERT INTO `device_param_info` VALUES (202, 41, 115, '2022-08-16 20:16:03', '2022-08-16 20:16:03');
INSERT INTO `device_param_info` VALUES (203, 41, 116, '2022-08-16 20:16:07', '2022-08-16 20:16:07');
INSERT INTO `device_param_info` VALUES (204, 42, 119, '2022-08-17 17:10:14', '2022-08-17 17:10:14');
INSERT INTO `device_param_info` VALUES (205, 42, 120, '2022-08-17 17:10:19', '2022-08-17 17:10:19');
INSERT INTO `device_param_info` VALUES (206, 43, 121, '2022-08-17 17:10:33', '2022-08-17 17:10:33');
INSERT INTO `device_param_info` VALUES (207, 43, 122, '2022-08-17 17:10:39', '2022-08-17 17:10:39');
INSERT INTO `device_param_info` VALUES (208, 44, 123, '2022-08-17 17:10:45', '2022-08-17 17:10:45');
INSERT INTO `device_param_info` VALUES (209, 44, 124, '2022-08-17 17:10:49', '2022-08-17 17:10:49');
INSERT INTO `device_param_info` VALUES (210, 45, 126, '2022-08-17 17:10:58', '2022-08-17 17:10:58');
INSERT INTO `device_param_info` VALUES (211, 45, 125, '2022-08-17 17:11:07', '2022-08-17 17:11:07');
INSERT INTO `device_param_info` VALUES (212, 46, 127, '2022-08-17 17:11:14', '2022-08-17 17:11:14');
INSERT INTO `device_param_info` VALUES (213, 46, 128, '2022-08-17 17:11:22', '2022-08-17 17:11:22');
INSERT INTO `device_param_info` VALUES (214, 47, 129, '2022-08-17 17:11:29', '2022-08-17 17:11:29');
INSERT INTO `device_param_info` VALUES (215, 47, 130, '2022-08-17 17:11:36', '2022-08-17 17:11:36');
INSERT INTO `device_param_info` VALUES (216, 48, 131, '2022-08-17 17:11:49', '2022-08-17 17:11:49');
INSERT INTO `device_param_info` VALUES (217, 49, 132, '2022-08-17 17:11:54', '2022-08-17 17:11:54');
INSERT INTO `device_param_info` VALUES (218, 50, 133, '2022-08-17 17:12:01', '2022-08-17 17:12:01');
INSERT INTO `device_param_info` VALUES (219, 50, 134, '2022-08-17 17:12:06', '2022-08-17 17:12:06');

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
) ENGINE = InnoDB AUTO_INCREMENT = 124 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of device_status_info
-- ----------------------------
INSERT INTO `device_status_info` VALUES (103, 40, 1, 'linglong/zongkongji_power_on.txt', '2022-08-16 20:14:54', '2022-08-16 20:14:54');
INSERT INTO `device_status_info` VALUES (104, 40, 2, 'linglong/zongkongji_power_off.txt', '2022-08-16 20:15:08', '2022-08-16 20:15:08');
INSERT INTO `device_status_info` VALUES (105, 41, 1, 'linglong/zhudianchi--jianshangpeidian_power_on.txt', '2022-08-16 20:15:45', '2022-08-16 20:15:45');
INSERT INTO `device_status_info` VALUES (106, 41, 2, 'linglong/zhudianchi_power_off1.txt', '2022-08-16 20:15:57', '2022-08-16 20:15:57');
INSERT INTO `device_status_info` VALUES (108, 42, 1, 'linglong/guanzhul_power_on.txt', '2022-08-17 17:04:24', '2022-08-17 17:04:24');
INSERT INTO `device_status_info` VALUES (109, 42, 2, 'linglong/guanzhul_power_power_off.txt', '2022-08-17 17:04:35', '2022-08-17 17:04:35');
INSERT INTO `device_status_info` VALUES (110, 43, 1, 'linglong/tuoluo_power_on.txt', '2022-08-17 17:04:52', '2022-08-17 17:04:52');
INSERT INTO `device_status_info` VALUES (111, 43, 2, 'linglong/tuoluo_power_off.txt', '2022-08-17 17:05:01', '2022-08-17 17:05:01');
INSERT INTO `device_status_info` VALUES (112, 44, 1, 'linglong/GPS_power_on.txt', '2022-08-17 17:05:11', '2022-08-17 17:05:11');
INSERT INTO `device_status_info` VALUES (113, 44, 2, 'linglong/GPS_power_off.txt', '2022-08-17 17:05:22', '2022-08-17 17:05:22');
INSERT INTO `device_status_info` VALUES (114, 45, 1, 'linglong/feikongmokuai_power_on.txt', '2022-08-17 17:05:31', '2022-08-17 17:05:31');
INSERT INTO `device_status_info` VALUES (115, 45, 2, 'linglong/feikongmokuai_power_off.txt', '2022-08-17 17:05:38', '2022-08-17 17:05:38');
INSERT INTO `device_status_info` VALUES (116, 46, 1, 'linglong/40_power_on.txt', '2022-08-17 17:05:48', '2022-08-17 17:05:48');
INSERT INTO `device_status_info` VALUES (117, 46, 2, 'linglong/40_power_off.txt', '2022-08-17 17:05:56', '2022-08-17 17:05:56');
INSERT INTO `device_status_info` VALUES (118, 47, 1, 'linglong/shixumokuai_power_on.txt', '2022-08-17 17:06:10', '2022-08-17 17:06:10');
INSERT INTO `device_status_info` VALUES (119, 47, 2, 'linglong/shixumokuai_power_off.txt', '2022-08-17 17:06:18', '2022-08-17 17:06:18');
INSERT INTO `device_status_info` VALUES (120, 48, 4, 'linglong/yijisifudianchi_power_on.txt', '2022-08-17 17:06:54', '2022-08-17 17:06:54');
INSERT INTO `device_status_info` VALUES (121, 49, 4, 'linglong/erjisifudianchi_power_on .txt', '2022-08-17 17:07:07', '2022-08-17 17:07:07');
INSERT INTO `device_status_info` VALUES (122, 50, 4, 'linglong/yijifadongji_power_off.txt', '2022-08-17 17:09:27', '2022-08-17 17:09:27');
INSERT INTO `device_status_info` VALUES (123, 41, 12, 'linglong/zhudianchi--kongzhixitongpeidian_power_on.txt', '2022-08-17 17:21:38', '2022-08-17 17:21:38');

-- ----------------------------
-- Table structure for fault_command_info
-- ----------------------------
DROP TABLE IF EXISTS `fault_command_info`;
CREATE TABLE `fault_command_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '系统生存期内唯一',
  `rocketID` int(255) NULL DEFAULT NULL COMMENT '火箭型号id',
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '指令故障名称',
  `fault_type` int(255) NULL DEFAULT NULL COMMENT '故障类型 1：控制；2：测量；3：动力',
  `fault_command_id` int(255) NULL DEFAULT NULL COMMENT '引用command_info.id',
  `response_command_id` int(255) NULL DEFAULT NULL COMMENT '引用command_info.id',
  `createTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间',
  `lastUpdateTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 12 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of fault_command_info
-- ----------------------------
INSERT INTO `fault_command_info` VALUES (11, 1, '控制系统上电指令异常', 1, 78, 54, '2022-08-17 17:48:27', '2022-08-17 17:48:27');

-- ----------------------------
-- Table structure for fault_param_info
-- ----------------------------
DROP TABLE IF EXISTS `fault_param_info`;
CREATE TABLE `fault_param_info`  (
  `id` int(255) NOT NULL AUTO_INCREMENT COMMENT '系统生存期内唯一',
  `rocketID` int(255) NULL DEFAULT NULL COMMENT '火箭型号id',
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '参数故障名称',
  `fault_type` int(255) NULL DEFAULT NULL COMMENT '故障类型1：控制；2：测量；3：动力',
  `command_id` int(255) NULL DEFAULT NULL COMMENT '引用command_info.id',
  `device_param_info_id` int(255) NULL DEFAULT NULL COMMENT '引用device_param_info.id',
  `createTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间',
  `lastUpdateTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 48 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of fault_param_info
-- ----------------------------
INSERT INTO `fault_param_info` VALUES (46, 1, '综控机上电异常', 1, 60, 200, '2022-08-16 20:44:46', '2022-08-16 20:44:46');
INSERT INTO `fault_param_info` VALUES (47, 1, '综控机上电异常', 1, 60, 201, '2022-08-16 20:44:46', '2022-08-16 20:44:46');

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
) ENGINE = InnoDB AUTO_INCREMENT = 113 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of main_flow_info
-- ----------------------------
INSERT INTO `main_flow_info` VALUES (105, 1, '发射平台准备', '1', '', '', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `main_flow_info` VALUES (106, 1, '地面测发设备准备', '2', '', '', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `main_flow_info` VALUES (107, 1, '箭上配电', '3', '', '', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `main_flow_info` VALUES (108, 1, '单机自检', '4', '', '', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `main_flow_info` VALUES (109, 1, '外安测试', '5', '', '', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `main_flow_info` VALUES (110, 1, '瞄准', '6', '', '', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `main_flow_info` VALUES (111, 1, '转电', '7', '', '', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `main_flow_info` VALUES (112, 1, '点火', '8', '', '', '2022-08-18 11:19:57', '2022-08-18 11:19:57');

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
) ENGINE = InnoDB AUTO_INCREMENT = 135 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of parameter_info
-- ----------------------------
INSERT INTO `parameter_info` VALUES (115, '主电池电压', 1, '伏特', '2022-08-16 20:08:22', '2022-08-16 20:08:22');
INSERT INTO `parameter_info` VALUES (116, '主电池电流', 1, '安倍', '2022-08-16 20:09:26', '2022-08-16 20:09:26');
INSERT INTO `parameter_info` VALUES (117, '综控机电压', 1, '伏特', '2022-08-16 20:10:33', '2022-08-16 20:10:33');
INSERT INTO `parameter_info` VALUES (118, '综控机电流', 1, '安倍', '2022-08-16 20:11:41', '2022-08-16 20:11:41');
INSERT INTO `parameter_info` VALUES (119, '惯组电压', 1, '伏特', '2022-08-17 16:20:57', '2022-08-17 16:20:57');
INSERT INTO `parameter_info` VALUES (120, '惯组电流', 1, '伏特', '2022-08-17 16:21:04', '2022-08-17 16:21:04');
INSERT INTO `parameter_info` VALUES (121, '陀螺电流', 1, '安倍', '2022-08-17 16:21:14', '2022-08-17 16:21:14');
INSERT INTO `parameter_info` VALUES (122, '陀螺电压', 1, '伏特', '2022-08-17 16:21:21', '2022-08-17 16:21:21');
INSERT INTO `parameter_info` VALUES (123, 'GPS电压', 1, '伏特', '2022-08-17 16:21:35', '2022-08-17 16:21:35');
INSERT INTO `parameter_info` VALUES (124, 'GPS电流', 1, '伏特', '2022-08-17 16:21:41', '2022-08-17 16:21:41');
INSERT INTO `parameter_info` VALUES (125, '飞控模块电流', 1, '安倍', '2022-08-17 16:21:52', '2022-08-17 16:21:52');
INSERT INTO `parameter_info` VALUES (126, '飞控模块电压', 1, '伏特', '2022-08-17 16:21:59', '2022-08-17 16:21:59');
INSERT INTO `parameter_info` VALUES (127, '40机电压', 1, '伏特', '2022-08-17 16:25:04', '2022-08-17 16:25:04');
INSERT INTO `parameter_info` VALUES (128, '40机电流', 1, '安倍', '2022-08-17 16:25:11', '2022-08-17 16:25:11');
INSERT INTO `parameter_info` VALUES (129, '时序模块电流', 1, '安倍', '2022-08-17 16:25:19', '2022-08-17 16:25:19');
INSERT INTO `parameter_info` VALUES (130, '时序模块电压', 1, '伏特', '2022-08-17 16:25:24', '2022-08-17 16:25:24');
INSERT INTO `parameter_info` VALUES (131, '一级伺服电池激活', 1, '伏特', '2022-08-17 16:32:05', '2022-08-17 16:32:05');
INSERT INTO `parameter_info` VALUES (132, '二级伺服电池激活', 1, '伏特', '2022-08-17 16:32:14', '2022-08-17 16:32:14');
INSERT INTO `parameter_info` VALUES (133, '一级发动机点火1', 1, '伏特', '2022-08-17 16:32:41', '2022-08-17 16:32:41');
INSERT INTO `parameter_info` VALUES (134, '一级发动机自毁', 1, '伏特', '2022-08-17 16:32:53', '2022-08-17 16:32:53');

-- ----------------------------
-- Table structure for rockect_param_info
-- ----------------------------
DROP TABLE IF EXISTS `rockect_param_info`;
CREATE TABLE `rockect_param_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `rocket_data_id` int(11) NULL DEFAULT NULL COMMENT '箭上数据返回指令ID,外键引用rocket_data_info.id',
  `device_parameter_id` int(255) NOT NULL COMMENT '外键引用device_param_info.id',
  `index` int(11) NULL DEFAULT NULL COMMENT '索引顺序，从1开始计数',
  `length` int(11) NULL DEFAULT NULL COMMENT '参数字节长度',
  `resultType` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '参数类型包括：unsigned char、unsigned short、unsigned int、char、short、int、float',
  `createTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录创建时间',
  `lastUpdateTime` datetime(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 34 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of rockect_param_info
-- ----------------------------
INSERT INTO `rockect_param_info` VALUES (14, 3, 200, 1, 1, '0', '2022-08-16 20:21:40', '2022-08-16 20:21:40');
INSERT INTO `rockect_param_info` VALUES (15, 3, 201, 2, 1, '0', '2022-08-16 20:21:45', '2022-08-16 20:21:45');
INSERT INTO `rockect_param_info` VALUES (16, 3, 202, 3, 1, '0', '2022-08-16 20:21:48', '2022-08-16 20:21:48');
INSERT INTO `rockect_param_info` VALUES (17, 3, 203, 4, 1, '0', '2022-08-16 20:21:52', '2022-08-16 20:21:52');
INSERT INTO `rockect_param_info` VALUES (18, 3, 204, 5, 1, '0', '2022-08-17 17:12:34', '2022-08-17 17:12:34');
INSERT INTO `rockect_param_info` VALUES (19, 3, 205, 6, 1, '0', '2022-08-17 17:12:39', '2022-08-17 17:12:39');
INSERT INTO `rockect_param_info` VALUES (20, 3, 206, 7, 1, '0', '2022-08-17 17:12:45', '2022-08-17 17:12:45');
INSERT INTO `rockect_param_info` VALUES (21, 3, 207, 8, 1, '0', '2022-08-17 17:12:50', '2022-08-17 17:12:50');
INSERT INTO `rockect_param_info` VALUES (22, 3, 208, 9, 1, '0', '2022-08-17 17:12:54', '2022-08-17 17:12:54');
INSERT INTO `rockect_param_info` VALUES (23, 3, 209, 10, 1, '0', '2022-08-17 17:13:00', '2022-08-17 17:13:00');
INSERT INTO `rockect_param_info` VALUES (24, 3, 210, 11, 1, '0', '2022-08-17 17:13:06', '2022-08-17 17:13:06');
INSERT INTO `rockect_param_info` VALUES (25, 3, 211, 12, 1, '0', '2022-08-17 17:13:11', '2022-08-17 17:13:11');
INSERT INTO `rockect_param_info` VALUES (26, 3, 212, 13, 1, '0', '2022-08-17 17:13:17', '2022-08-17 17:13:17');
INSERT INTO `rockect_param_info` VALUES (27, 3, 213, 14, 1, '0', '2022-08-17 17:13:22', '2022-08-17 17:13:22');
INSERT INTO `rockect_param_info` VALUES (28, 3, 214, 15, 1, '0', '2022-08-17 17:13:27', '2022-08-17 17:13:27');
INSERT INTO `rockect_param_info` VALUES (29, 3, 215, 16, 1, '0', '2022-08-17 17:13:32', '2022-08-17 17:13:32');
INSERT INTO `rockect_param_info` VALUES (30, 3, 216, 17, 1, '0', '2022-08-17 17:13:39', '2022-08-17 17:13:39');
INSERT INTO `rockect_param_info` VALUES (31, 3, 217, 18, 1, '0', '2022-08-17 17:13:43', '2022-08-17 17:13:43');
INSERT INTO `rockect_param_info` VALUES (32, 3, 218, 19, 1, '0', '2022-08-17 17:13:48', '2022-08-17 17:13:48');
INSERT INTO `rockect_param_info` VALUES (33, 3, 219, 20, 1, '0', '2022-08-17 17:13:52', '2022-08-17 17:13:52');

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
) ENGINE = InnoDB AUTO_INCREMENT = 4 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of rocket_data_info
-- ----------------------------
INSERT INTO `rocket_data_info` VALUES (3, 1, '测试协议', 2, 0, '2022-08-16 20:20:55', '2022-08-16 20:20:55');

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
) ENGINE = InnoDB AUTO_INCREMENT = 5 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of rocket_info
-- ----------------------------
INSERT INTO `rocket_info` VALUES (1, '灵龙', '0', '2022-07-22 23:44:25', '2022-07-22 23:44:25');

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
) ENGINE = InnoDB AUTO_INCREMENT = 13 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

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
INSERT INTO `status_info` VALUES (9, '初始化', '2022-08-03 14:31:38', '2022-08-03 14:31:38');
INSERT INTO `status_info` VALUES (10, '点火', '2022-08-17 17:07:43', '2022-08-17 17:07:43');
INSERT INTO `status_info` VALUES (11, '自毁', '2022-08-17 17:07:51', '2022-08-17 17:07:51');
INSERT INTO `status_info` VALUES (12, '控制系统上电', '2022-08-17 17:21:23', '2022-08-17 17:21:23');

-- ----------------------------
-- Table structure for sub_flow_info
-- ----------------------------
DROP TABLE IF EXISTS `sub_flow_info`;
CREATE TABLE `sub_flow_info`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `main_id` int(11) NOT NULL COMMENT '主流程ID，外键引用main_flow_info.id',
  `command_id` int(11) NULL DEFAULT NULL COMMENT '测发指令，外键引用command_info.id',
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '子流程名称',
  `backInfo` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '回令信息',
  `remark` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '备注',
  `createTime` datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录创建时间',
  `lastUpdateTime` datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 285 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of sub_flow_info
-- ----------------------------
INSERT INTO `sub_flow_info` VALUES (256, 105, 89, '平板车就位', '已就位', '以射向线为基准，参考停车参考线，调整集装箱发射平台位置，使得集装箱中线与射向线重合，同时保证车头停与停止线上，保证发射台架中线与参考点误差不超过20cm', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (257, 105, 52, '拆除绑扎', '已拆除绑扎', '', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (258, 105, 61, '支腿检查', '支腿性能状态正常', '', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (259, 105, 62, '集装箱配电', '集装箱已配电', '', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (260, 105, 76, '打开集装箱支腿', '支腿已打开', '', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (261, 105, 64, '整车调平', '整车已调平', '', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (262, 105, 69, '平板车离场', '板车已离场', '调平支腿座旋出到位，调平支腿伸出至自卸状态', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (263, 105, 70, '箱体展开', '箱体已展开', '', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (264, 105, 71, '地面测发设备展开', '设备已展开', '瞄准、地面电源、前置测试终端等设备展开，地线连接、光缆连接', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (265, 105, 72, '卫星测试', '卫星测试正常', '', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (266, 105, 73, '拆除卫星测试电缆', '卫星测试电缆已拆除', '', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (267, 105, 74, '火箭起竖', '火箭已起竖', '', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (268, 105, 75, '关闭箱体', '箱体已关闭', '', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (269, 106, 65, '连接脱插', '脱插连接好', '', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (270, 106, 66, '地面状态检查', '地面状态检查完毕', '', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (271, 106, 67, '配电设备开机', '配电设备开机好', '配电设备开机，设为远控', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (272, 106, 68, '前端交换机开机', '交换机已开机', '', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (273, 106, 79, '数据服务器开机', '数据服务器开机好', '数据服务器开机，运行处理、存储软件', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (274, 106, 80, '前端计算机开机', '前端计算机开机好', '前端计算机开机，设置为远控模式', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (275, 106, 81, '遥测站开机', '遥测站开机好', '遥测地面站开机，启动网络发送', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (276, 106, 82, '外安综测仪开机', '外安综测仪开机好', '外安综测仪开机，准备系统测试', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (277, 106, 83, '主控计算机开机', '主控开机好', '启动判读计算机，运行实时监测软件', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (278, 106, 84, '启动主控软件', '主控软件启动好', '', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (279, 106, 85, '瞄准设备开机', '瞄准设备已开机', '打开瞄准计算机、经纬仪供电开关。', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (280, 106, 86, '运行实时监测软件', '实时监测软件启动好', '启动判读计算机，运行实时监测软件', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (281, 107, 48, '箭上配电指令', '箭上已配电', '判读主电池电压电流、综控机电压电流', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (282, 107, 54, '控制系统配电指令', '控制系统已上电', '惯组、陀螺、电磁阀、舵机配电。判读惯组、陀螺、GPS、飞控模块、时序模块等电压电流', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (283, 107, 56, '转恒压并网指令', '供电模式转换好', '转地面供电', '2022-08-18 11:19:57', '2022-08-18 11:19:57');
INSERT INTO `sub_flow_info` VALUES (284, 107, 58, '火工品阻值测量指令', '阻值测量正常', '判读箭上42路火工品阻值测量结果，均为绿灯', '2022-08-18 11:19:57', '2022-08-18 11:19:57');

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
