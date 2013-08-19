-- phpMyAdmin SQL Dump
-- version 3.4.2
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Aug 19, 2013 at 01:15 PM
-- Server version: 5.0.51
-- PHP Version: 5.2.5

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `icm`
--

-- --------------------------------------------------------

--
-- Table structure for table `action`
--

CREATE TABLE IF NOT EXISTS `action` (
  `playerid` varchar(255) character set latin1 default NULL,
  `portalid` varchar(255) character set latin1 default NULL,
  `guid` varchar(255) character set latin1 NOT NULL,
  `text` varchar(500) character set latin1 default NULL,
  `toportalid` varchar(255) character set latin1 NOT NULL,
  `action` varchar(255) character set latin1 NOT NULL,
  `actionlevel` varchar(255) character set latin1 NOT NULL,
  `timestamp` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `unixtimestamp` bigint(11) default NULL,
  PRIMARY KEY  (`guid`),
  KEY `toportalid` (`toportalid`),
  KEY `portalid` (`portalid`),
  KEY `playerid` (`playerid`),
  KEY `action` (`action`),
  KEY `timestamp` (`timestamp`),
  KEY `unixtimestamp` (`unixtimestamp`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `lastnotification`
--

CREATE TABLE IF NOT EXISTS `lastnotification` (
  `timestamp` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  PRIMARY KEY  (`timestamp`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `lastupdate`
--

CREATE TABLE IF NOT EXISTS `lastupdate` (
  `timestamp` timestamp NOT NULL default CURRENT_TIMESTAMP,
  UNIQUE KEY `timestamp` (`timestamp`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `player`
--

CREATE TABLE IF NOT EXISTS `player` (
  `guid` varchar(255) character set latin1 NOT NULL,
  `name` varchar(255) character set latin1 default NULL,
  `team` varchar(255) character set latin1 default NULL,
  `email` varchar(255) character set latin1 default NULL,
  `level` int(11) default NULL,
  PRIMARY KEY  (`guid`),
  KEY `team` (`team`),
  KEY `name` (`name`),
  KEY `level` (`level`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `playermonitors`
--

CREATE TABLE IF NOT EXISTS `playermonitors` (
  `guid` int(11) NOT NULL auto_increment,
  `playerid` varchar(255) NOT NULL,
  `monitoredplayerid` varchar(255) NOT NULL,
  `active` char(1) NOT NULL default 'Y',
  PRIMARY KEY  (`guid`),
  UNIQUE KEY `guid` (`guid`),
  KEY `playerid` (`playerid`),
  KEY `monitoredplayerid` (`monitoredplayerid`),
  KEY `active` (`active`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=4 ;

-- --------------------------------------------------------

--
-- Table structure for table `portal`
--

CREATE TABLE IF NOT EXISTS `portal` (
  `guid` varchar(255) character set latin1 NOT NULL,
  `name` varchar(255) character set latin1 default NULL,
  `plain` varchar(8000) character set latin1 default NULL,
  `latE6` int(11) default NULL,
  `lngE6` int(11) default NULL,
  `address` varchar(8000) character set latin1 default NULL,
  `team` varchar(255) character set latin1 default NULL,
  `playerid` varchar(255) collate latin1_general_ci NOT NULL,
  `capturedtime` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `imageurl` varchar(1024) collate latin1_general_ci default NULL,
  `lastupdate` timestamp NULL default NULL,
  PRIMARY KEY  (`guid`),
  KEY `team` (`team`),
  KEY `name` (`name`),
  KEY `playerid` (`playerid`),
  KEY `capturestime` (`capturedtime`),
  KEY `lastupdate` (`lastupdate`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `portalarchive`
--

CREATE TABLE IF NOT EXISTS `portalarchive` (
  `jason` text NOT NULL,
  `timestamp` timestamp NOT NULL default CURRENT_TIMESTAMP,
  KEY `timestamp` (`timestamp`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `portaledge`
--

CREATE TABLE IF NOT EXISTS `portaledge` (
  `startportalid` varchar(255) NOT NULL,
  `endportalid` varchar(255) NOT NULL,
  `guid` varchar(255) NOT NULL,
  PRIMARY KEY  (`guid`),
  KEY `startportalid` (`startportalid`,`endportalid`),
  KEY `endportalid` (`endportalid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `portalfield`
--

CREATE TABLE IF NOT EXISTS `portalfield` (
  `portalAid` varchar(255) NOT NULL,
  `portalBid` varchar(255) NOT NULL,
  `portalCid` varchar(255) NOT NULL,
  `guid` varchar(255) NOT NULL,
  `team` varchar(255) NOT NULL,
  `score` int(11) NOT NULL,
  `playerid` varchar(255) NOT NULL,
  `creationtime` datetime NOT NULL,
  UNIQUE KEY `guid` (`guid`),
  KEY `portalAid` (`portalAid`,`portalBid`,`portalCid`),
  KEY `team` (`team`),
  KEY `playerid` (`playerid`,`creationtime`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `portalmod`
--

CREATE TABLE IF NOT EXISTS `portalmod` (
  `guid` int(255) NOT NULL auto_increment,
  `portalid` varchar(255) NOT NULL,
  `type` varchar(255) NOT NULL,
  `playerid` varchar(255) NOT NULL,
  `display` varchar(255) NOT NULL,
  `rarity` varchar(255) NOT NULL,
  `stats` varchar(255) NOT NULL,
  PRIMARY KEY  (`guid`),
  KEY `portalid` (`portalid`),
  KEY `playerid` (`playerid`),
  KEY `type` (`type`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=14958515 ;

-- --------------------------------------------------------

--
-- Table structure for table `portalmonitors`
--

CREATE TABLE IF NOT EXISTS `portalmonitors` (
  `guid` int(11) NOT NULL auto_increment,
  `playerid` varchar(255) NOT NULL,
  `portalid` varchar(255) NOT NULL,
  `active` char(1) NOT NULL default 'Y',
  UNIQUE KEY `guid` (`guid`),
  UNIQUE KEY `playerid_2` (`playerid`,`portalid`),
  KEY `playerid` (`playerid`),
  KEY `portalid` (`portalid`),
  KEY `active` (`active`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=59 ;

-- --------------------------------------------------------

--
-- Table structure for table `portalresonator`
--

CREATE TABLE IF NOT EXISTS `portalresonator` (
  `slot` int(11) NOT NULL,
  `level` int(11) NOT NULL,
  `guid` varchar(255) NOT NULL,
  `evergytotal` int(11) NOT NULL,
  `distancetoportal` int(11) NOT NULL,
  `playerid` varchar(255) NOT NULL,
  `portalid` varchar(255) NOT NULL,
  UNIQUE KEY `guid` (`guid`),
  KEY `level` (`level`),
  KEY `playerid` (`playerid`),
  KEY `portalid` (`portalid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `temptable`
--

CREATE TABLE IF NOT EXISTS `temptable` (
  `guid` varchar(255) NOT NULL,
  `name` varchar(255) default NULL,
  `plain` varchar(8000) default NULL,
  `latE6` int(11) default NULL,
  `lngE6` int(11) default NULL,
  `address` varchar(8000) default NULL,
  `team` varchar(255) default NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
