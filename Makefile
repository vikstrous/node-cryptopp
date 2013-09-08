install:
	npm install -g node-gyp

all:
	node-gyp configure
	node-gyp build

clean:
	node-gyp clean
