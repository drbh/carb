import redis
import time
import json

POOL = redis.ConnectionPool(host='127.0.0.1', port=6379, db=0)
my_server = redis.Redis(connection_pool=POOL)

while True:
	res = my_server.rpop("tri")
	if res is not None:
		obj = json.loads(res)
		print(json.dumps(obj, indent=4))
		
