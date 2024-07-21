DROP TABLE IF EXISTS CONNECTION CASCADE;
DROP TABLE IF EXISTS CUSTOM_USER CASCADE;
DROP TABLE IF EXISTS SERVER CASCADE;

CREATE TABLE CUSTOM_USER(
    id serial PRIMARY KEY,
    username varchar(100) UNIQUE,
    password varchar(255),
    is_active boolean DEFAULT false
);

CREATE TABLE SERVER(
    id serial PRIMARY KEY,
    name varchar(100),
    port int,
    owner int REFERENCES CUSTOM_USER(id)

);

CREATE TABLE CONNECTION(
    id serial PRIMARY KEY,
    user_id int REFERENCES CUSTOM_USER(id),
    server_id int REFERENCES SERVER(id)
);