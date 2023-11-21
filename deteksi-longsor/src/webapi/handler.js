const insertLogHandler = async (request, h) => {
  const { hum, vibr } = request.payload;

  if (hum === undefined || vibr === undefined) {
    const response = h.response({
      status: 'fail',
      message: 'Gagal menyimpan log. Data kelembaban dan getaran diperlukan.',
    });
    response.code(400);
    return response;
  }

  try {
    const currentTime = new Date().toISOString();

    const [result] = await request.server.app.dbPool.query(
      'INSERT INTO log_table (hum, vibr, log_time) VALUES (?, ?, CONVERT_TZ(?, "+00:00", "+07:00"))',
      [hum, vibr, currentTime]
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
    const [rows] = await request.server.app.dbPool.query('SELECT * FROM log_table');
    
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

module.exports = { insertLogHandler, viewLogHandler };
