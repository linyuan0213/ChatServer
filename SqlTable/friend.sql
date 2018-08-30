CREATE TABLE `friends` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `fname` varchar(20) DEFAULT NULL,
  `friendname` varchar(20) DEFAULT NULL,
  `ispassed` bit(1) NOT NULL DEFAULT b'0',
  PRIMARY KEY (`id`),
  KEY `fk_fname` (`fname`),
  KEY `fk_friend_name` (`friendname`),
  CONSTRAINT `fk_fname` FOREIGN KEY (`fname`) REFERENCES `users` (`username`),
  CONSTRAINT `fk_friend_name` FOREIGN KEY (`friendname`) REFERENCES `users` (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8
