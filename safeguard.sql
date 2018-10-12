/*
 Navicat MySQL Data Transfer

 Source Server         : Mysql
 Source Server Type    : MySQL
 Source Server Version : 50625
 Source Host           : localhost:3306
 Source Schema         : safeguard

 Target Server Type    : MySQL
 Target Server Version : 50625
 File Encoding         : 65001

 Date: 12/10/2018 20:19:15
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for md5
-- ----------------------------
DROP TABLE IF EXISTS `md5`;
CREATE TABLE `md5`  (
  `md5value` varchar(100) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;

-- ----------------------------
-- Records of md5
-- ----------------------------
INSERT INTO `md5` VALUES ('6cf7a10834a486a4d23a81f2526c3646');
INSERT INTO `md5` VALUES ('baaca87fe5ac99e0f1442b54e03056f4');
INSERT INTO `md5` VALUES ('d41d8cd98f00b204e9800998ecf8427e');

-- ----------------------------
-- Table structure for process
-- ----------------------------
DROP TABLE IF EXISTS `process`;
CREATE TABLE `process`  (
  `processname` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;

-- ----------------------------
-- Records of process
-- ----------------------------
INSERT INTO `process` VALUES ('QQ.exe');
INSERT INTO `process` VALUES ('MicrosoftEdge.exe');
INSERT INTO `process` VALUES ('notepad++.exe');

SET FOREIGN_KEY_CHECKS = 1;
