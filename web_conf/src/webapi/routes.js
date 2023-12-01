const { insertLogHandler, viewLogHandler, checkConnectionHandler } = require('./handler');

module.exports = [
  {
    method: 'POST',
    path: '/insert-log',
    handler: insertLogHandler
  },
  {
    method: 'GET',
    path: '/view-log',
    handler: viewLogHandler
  },
  {
    method: 'GET',
    path: '/check-connection',
    handler: checkConnectionHandler,
  },
];
