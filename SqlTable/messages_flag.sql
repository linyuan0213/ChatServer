CREATE TABLE `messages_flg` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `message_id` int(10) unsigned NOT NULL,
  `isreceived` BOOL DEFAULT FALSE,
  PRIMARY KEY (`id`),
  KEY `fk_message_id` (`message_id`),
  CONSTRAINT `fk_message_id` FOREIGN KEY (`message_id`) REFERENCES `messages` (`id`)
) ENGINE=InnoDB CHARSET=utf8

