DROP TABLE IF EXISTS CUSTOM_USER;
DROP TABLE IF EXISTS SERVER;

CREATE TABLE CUSTOM_USER(
    id serial PRIMARY KEY,
    username varchar(100) UNIQUE,
    password varchar(255)
);

CREATE TABLE SERVER(
    id serial PRIMARY KEY,
    name varchar(100),
    port int,
    owner int REFERENCES CUSTOM_USER(id),
    online_clients int DEFAULT 0

);
