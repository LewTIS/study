# coding: utf-8
import asyncio
from aiohttp import web

@asyncio.coroutine
def index(request):
    return web.Response(body=b'Hello aiohttp!',content_type='text/html')

@asyncio.coroutine
def user(request):
    name = request.match_info['name']
    body = 'Hello %s' % name
    return web.Response(body=body.encode('utf-8'),content_type='text/html')
    pass

@asyncio.coroutine
def init(loop):

    app = web.Application(loop=loop)
    
    app.router.add_route('GET', '/', index)
    app.router.add_route('GET', '/user/{name}', user)
    
    server = yield from loop.create_server(app.make_handler(),'127.0.0.1',9999)
    print('Server is running at http:/127.0.0.1:9999 ...')

loop = asyncio.get_event_loop()
loop.run_until_complete(init(loop)) 
loop.run_forever()