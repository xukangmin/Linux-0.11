#!/bin/sh
gcc share.h consumer.c -o consumer
gcc share.h producer.c -o producer
