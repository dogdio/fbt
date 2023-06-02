#!/bin/sh
psql -d demodb -U postgres -f $1
