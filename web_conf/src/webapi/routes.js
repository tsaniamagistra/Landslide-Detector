const {
  insertLogHandler,
  viewLogHandler,
  checkConnectionHandler,
  viewBahayaHandler,
  viewSiagaHandler, } = require('./handler');

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
      path: '/view-bahaya',
      handler: viewBahayaHandler
    },
    {
      method: 'GET',
      path: '/view-siaga',
      handler: viewSiagaHandler
    },
    {
      method: 'GET',
      path: '/check-connection',
      handler: checkConnectionHandler,
    },
  ];
  