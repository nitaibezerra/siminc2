-- phpMyAdmin SQL Dump
-- version 2.7.0-rc1
-- http://www.phpmyadmin.net
-- 
-- Host: localhost:3306
-- Generation Time: Dec 20, 2005 at 03:51 AM
-- Server version: 5.0.16
-- PHP Version: 5.1.1
-- 
-- Database: `support_tickets2`
-- 

-- --------------------------------------------------------

-- 
-- Table structure for table `tickets_answers`
-- 

CREATE TABLE IF NOT EXISTS `tickets_answers` (
  `id` int(10) unsigned NOT NULL,
  `user_id` int(32) unsigned NOT NULL default '0',
  `ticket_id` int(32) unsigned NOT NULL default '0',
  `body` text NOT NULL,
  `timestamp` int(16) unsigned NOT NULL default '0',
  `rating` int(1) unsigned NOT NULL default '0',
  `subject` varchar(64) binary NOT NULL default '',
  UNIQUE KEY `id` (`id`),
  KEY `user_id` (`user_id`),
  KEY `ticket_id` (`ticket_id`)
) TYPE=MyISAM AUTO_INCREMENT=5 ;


-- --------------------------------------------------------

-- 
-- Table structure for table `tickets_department_users`
-- 

CREATE TABLE IF NOT EXISTS `tickets_department_users` (
  `id` int(16) unsigned NOT NULL,
  `department_id` int(16) unsigned NOT NULL default '0',
  `user_id` int(16) unsigned NOT NULL default '0',
  UNIQUE KEY `id` (`id`),
  KEY `department_id` (`department_id`),
  KEY `user_id` (`user_id`)
) TYPE=MyISAM AUTO_INCREMENT=13 ;

-- 
-- Dumping data for table `tickets_department_users`
-- 

INSERT INTO `tickets_department_users` VALUES (1, 1, 1);

-- --------------------------------------------------------

-- 
-- Table structure for table `tickets_departments`
-- 

CREATE TABLE IF NOT EXISTS `tickets_departments` (
  `id` int(16) NOT NULL,
  `name` varchar(64) binary NOT NULL default '',
  `description` varchar(255) binary NOT NULL default '',
  UNIQUE KEY `id` (`id`)
) TYPE=MyISAM AUTO_INCREMENT=6 ;

-- 
-- Dumping data for table `tickets_departments`
-- 

INSERT INTO `tickets_departments` VALUES (1, 'Administration', '');

-- --------------------------------------------------------

-- 
-- Table structure for table `tickets_history_log`
-- 

CREATE TABLE IF NOT EXISTS `tickets_history_log` (
  `id` int(10) unsigned NOT NULL,
  `message` text NOT NULL,
  `type` varchar(64) binary NOT NULL default '',
  `priority` enum('1','2','3','4','5') NOT NULL default '1',
  `user_id` int(16) unsigned NOT NULL default '0',
  `file` varchar(64) binary NOT NULL default '',
  `line` varchar(64) binary NOT NULL default '',
  `ip` varchar(16) binary NOT NULL default '',
  `referer` varchar(255) binary NOT NULL default '',
  `timestamp` int(12) unsigned NOT NULL default '0',
  UNIQUE KEY `id` (`id`),
  KEY `user_id` (`user_id`)
) TYPE=MyISAM AUTO_INCREMENT=279 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `tickets_notes`
-- 

CREATE TABLE IF NOT EXISTS `tickets_notes` (
  `id` smallint(5) unsigned NOT NULL,
  `user_id` smallint(5) unsigned NOT NULL default '0',
  `title` varchar(50) binary NOT NULL default '',
  `body` text NOT NULL,
  `timestamp` int(32) unsigned default '0',
  `status` enum('0','1') NOT NULL default '1',
  UNIQUE KEY `ID` (`id`),
  KEY `user_id` (`user_id`)
) TYPE=MyISAM AUTO_INCREMENT=1 ;


-- --------------------------------------------------------

-- 
-- Table structure for table `tickets_options`
-- 

CREATE TABLE IF NOT EXISTS `tickets_options` (
  `id` int(10) unsigned NOT NULL,
  `name` varchar(255) binary NOT NULL default '',
  `value` varchar(32) binary NOT NULL default '',
  `changed` int(16) unsigned NOT NULL default '0',
  `type` varchar(16) binary NOT NULL default '',
  PRIMARY KEY  (`id`),
  KEY `changed` (`changed`)
) TYPE=MyISAM AUTO_INCREMENT=8 ;

-- 
-- Dumping data for table `tickets_options`
-- 

INSERT INTO `tickets_options` VALUES (1, 'OPTION_EMAIL_MODS_WHEN_NEW_TICKET', '1', 1123815378, 'boolean');
INSERT INTO `tickets_options` VALUES (2, 'OPTION_EMAIL_CLIENT_WHEN_NEW_ANSWER', '1', 1123815378, 'boolean');
INSERT INTO `tickets_options` VALUES (3, 'OPTION_EMAIL_CLIENT_WHEN_TICKETSTATUS_CHANGES', '1', 1123815378, 'boolean');
INSERT INTO `tickets_options` VALUES (4, 'OPTION_EMAIL_TEMPLATE', '1', 1123815345, 'boolean');
INSERT INTO `tickets_options` VALUES (5, 'OPTION_EMAIL_USER_WHEN_REGISTERED', '1', 1123815345, 'boolean');
INSERT INTO `tickets_options` VALUES (6, 'OPTIONS_NOTES', '1', 1123815484, 'boolean');
INSERT INTO `tickets_options` VALUES (7, 'OPTION_RECENT_TICKETS_DAYS', '7', 1124320692, 'integer');

-- --------------------------------------------------------

-- 
-- Table structure for table `tickets_tickets`
-- 

CREATE TABLE IF NOT EXISTS `tickets_tickets` (
  `id` int(5) unsigned NOT NULL,
  `user_id` int(16) unsigned NOT NULL,
  `subject` varchar(50) binary NOT NULL default '',
  `timestamp` bigint(10) unsigned NOT NULL default '0',
  `status` set('Open','Closed') NOT NULL default 'Open',
  `urgency` set('1','2','3','4') NOT NULL default '1',
  `body` text NOT NULL,
  `department_id` int(16) unsigned default NULL,
  PRIMARY KEY  (`id`),
  KEY `user_id` (`user_id`),
  KEY `department_id` (`department_id`)
) TYPE=MyISAM AUTO_INCREMENT=10 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `users`
-- 

CREATE TABLE IF NOT EXISTS `users` (
  `user_id` int(16) unsigned NOT NULL,
  `username` varchar(16) binary NOT NULL default '',
  `password` varchar(32) binary NOT NULL default '',
  `name` varchar(30) binary NOT NULL,
  `email` varchar(150) binary NOT NULL default '',
  `timestamp` int(16) NOT NULL default '0',
  `admin` enum('Admin','Mod','Client') NOT NULL default 'Mod',
  UNIQUE KEY `ID` (`user_id`)
) TYPE=MyISAM PACK_KEYS=0 AUTO_INCREMENT=6 ;

-- 
-- Dumping data for table `users`
-- 

INSERT INTO `users` VALUES (1, 'administrator', 'password', 'Nicolas Connault', 'nick@connault.com.au', 1135050113, 'Admin');

