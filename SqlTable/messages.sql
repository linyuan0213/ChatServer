CREATE TABLE messages(
	id INT UNSIGNED NOT NULL AUTO_INCREMENT,
	classify INT NOT NULL,
	sender VARCHAR(20),
	receiver VARCHAR(20),
	message TEXT,
	filename VARCHAR(50),
	PRIMARY KEY(id),
	CONSTRAINT fk_sender FOREIGN KEY(sender) REFERENCES users(username),
	CONSTRAINT fk_receiver FOREIGN KEY(receiver) REFERENCES users(username)
)engine=InnoDB charset=utf8;
