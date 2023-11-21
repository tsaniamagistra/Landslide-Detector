const { insertLogHandler, viewLogHandler } = require('./handler');

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
  }
];
