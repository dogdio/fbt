#!/bin/bash
PASS=`spring encodepassword password`
PASS2=`echo $PASS | sed s/^.bcrypt.//`

#create table account_data
#(
#  id serial primary key,
#  name text NOT NULL,
#  pass text NOT NULL,
#  role character varying(12) NOT NULL,
#  enabled BOOLEAN NOT NULL,
#
#  lang character varying(4),
#  item_sort_order integer,
#  progress_sort_order integer,
#
#  unique(name)
#);

# 'id' は空にして自動採番とする(Webアプリではnullとしている)
echo "insert into account_data(name, pass, role, enabled, lang, item_sort_order, progress_sort_order) values('admin', '$PASS2', 'ROLE_ADMIN', 't', 'jp', 0, 0);" > init.sql

psql -d demodb -U postgres -f init.sql

rm -f init.sql

