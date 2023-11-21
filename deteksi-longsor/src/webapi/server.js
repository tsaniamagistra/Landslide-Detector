const Hapi = require('@hapi/hapi');
const routes = require('./routes');
const dbPool = require('./database');

const init = async () => {
  const server = Hapi.server({
    port: 9000,
    host: 'localhost',
  });

  server.decorate('toolkit', 'dbPool', dbPool);

  server.route(routes);

  await server.start();
  console.log(`Server berjalan pada ${server.info.uri}`);
};

init();
