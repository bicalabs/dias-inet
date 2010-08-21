
DROP DATABASE IF EXISTS dias_objdb;

CREATE DATABASE dias_objdb;

USE dias_objdb;

CREATE TABLE groups (
  group_id	BIGINT AUTO_INCREMENT PRIMARY KEY NOT NULL,
  name		CHAR(64) NOT NULL
);

CREATE TABLE users (
  user_id	BIGINT AUTO_INCREMENT PRIMARY KEY NOT NULL,
  title		CHAR(64) NOT NULL,
  name		CHAR(64) NOT NULL,
  surname	CHAR(64) NOT NULL,
  pathronimyc	CHAR(64),
  nicname	CHAR(64) NOT NULL,
  passwd	CHAR(64) NOT NULL,
  group_ptr	BIGINT NOT NULL REFERENCES groups.group_id ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE usr_grp (
  user_ptr	BIGINT NOT NULL REFERENCES users.user_id ON UPDATE CASCADE ON DELETE CASCADE,
  group_ptr	BIGINT NOT NULL REFERENCES groups.group_id ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE objects (
  path		CHAR(255) PRIMARY KEY NOT NULL,
  user_ptr	BIGINT NOT NULL REFERENCES users.user_id ON UPDATE CASCADE ON DELETE CASCADE,
  type		INT(8) NOT NULL,
  perms		INT(8) NOT NULL DEFAULT 0777,
  attr		INT(8) NOT NULL,
  created	TIMESTAMP,
  accessed	TIMESTAMP,
  modified	TIMESTAMP
);

CREATE TABLE relations (
  parent_ptr	CHAR(255) NOT NULL REFERENCES objects.path,
  child_ptr	CHAR(255) NOT NULL REFERENCES objects.path
);
