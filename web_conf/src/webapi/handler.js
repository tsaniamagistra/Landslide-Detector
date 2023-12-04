const dbPool = require('./database');

const insertLogHandler = async (request, h) => {
  const { moist, vibr, level } = request.payload;

  if (moist === undefined || vibr === undefined || level === undefined) {
    const response = h.response({
      status: 'fail',
      message: 'Gagal menyimpan log. Data kelembaban dan getaran diperlukan.',
    });
    response.code(400);
    return response;
  }

  try {
    const currentTime = new Date().toISOString();

    const [result] = await dbPool.query(
      'INSERT INTO log (log_time, soil_moisture, vibration, level) VALUES (CONVERT_TZ(?, "+00:00", "+07:00"), ?, ?, ?)',
      [currentTime, moist, vibr, level]
    );

    if (result.affectedRows > 0) {
      const response = h.response({
        status: 'success',
        message: 'Log berhasil disimpan',
      });
      response.code(201);
      return response;
    }
  } catch (error) {
    console.error('Error inserting log:', error);
  }

  const response = h.response({
    status: 'fail',
    message: 'Gagal menyimpan log ke database',
  });
  response.code(500);
  return response;
};

const viewLogHandler = async (request, h) => {
  try {
    const [rows] = await dbPool.query(
      'SELECT * FROM log ORDER BY log_time DESC LIMIT 10'
    );
    
    return {
      status: 'success',
      data: {
        logs: rows,
      },
    };
  } catch (error) {
    console.error('Error fetching logs:', error);
    const response = h.response({
      status: 'fail',
      message: 'Gagal mengambil data log dari database',
    });
    response.code(500);
    return response;
  }
};

const checkConnectionHandler = (request, h) => {
  return 'Hello World!';
};

module.exports = { insertLogHandler, viewLogHandler, checkConnectionHandler };
