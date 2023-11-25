const express = require('express'); 
const app = express();
var mqtt = require('mqtt')
var mysql = require('mysql')
var dt;
var nguong=45;

app.use(express.static("truyen"))
app.set("views engine", "ejs")
app.set("views", "./views")
app.get('/', function (req, res) {
    res.render('web.ejs')
});

var server = require("http").Server(app)
var io = require('socket.io')(server)
var port = 5005;
server.listen(port, function () {
    console.log('http://localhost:5005')
})

// server.listen(5005, '10.21.109.89', () => {
//     console.log('http://10.21.109.89:5005')
// })

var client = mqtt.connect("mqtt://test.mosquitto.org:1883");
// var client = mqtt.connect("mqtt://broker.hivemq.com:1883");
client.on("connect", function () {
    console.log("connected mqtt " + client.connected);
    client.subscribe("temperature-humidity");
});

//--------sql------------------------------
var con = mysql.createConnection({
    host: 'localhost',
    user: 'root',
    password: '',
    database: 'wsh'
});
//----------lay nguong------------------
function LayNguong(){
   con.query("SELECT * FROM nguong", function (err, result, fields) {
    if (err) throw err;
    nguong=result[0].nguong; 
    console.log("Done")
  }); 
}
LayNguong();

//--------------nhan tin-----------------------
client.on('message', function (topic, message, packet) {  // function có chức năng nhận gói tin là bộ đệm (buffer) bên arduino
    console.log("")
    console.log("Topic:")  
    const objData = JSON.parse(message) //Khi nhận dữ liệu từ máy chủ web, dữ liệu luôn là một chuỗi.
    console.log(objData); // JSON.parse() --> trả về đối tượng JS

    var newLight = objData.Light; 

    var n = new Date()
    var month = n.getMonth() + 1
    var Date_and_Time = n.getFullYear() + "-" + month + "-" + n.getDate() + " " + n.getHours() + ":" + n.getMinutes() + ":" + n.getSeconds();
    dt=Date_and_Time
    if(newLight > nguong){
        client.publish("MayBom", '0');
        io.sockets.emit("Trạng thái", "0");
        console.log("OFF")
      }else if(newLight <= nguong){
        client.publish("MayBom", '1');
        io.sockets.emit("Trạng thái", "1");
        console.log("ON"+" "+newLight+" "+nguong)
      }
    
    var sql = "INSERT INTO bang_IoT (do_am, thoi_gian) VALUES ('" + newLight + "', '" + Date_and_Time.toString() + "')"
    con.query(sql, function (err, result) {
        if (err) throw err;
        console.log("Inserted to Table: ");
        console.log( newLight + " " + Date_and_Time)
        io.sockets.emit('duLieu', { humid: newLight, time: Date_and_Time, nguong: nguong})
    });
});

//---------------------------------------
io.on('connection', function (socket) { //kết nối vs socket
    console.log(socket.id + " connected")
    socket.on('disconnect', function () {
        console.log(socket.id + " disconnected")
    })

    // Update nguong.
    socket.on("Ngưỡng",function(data){
        var sql2="UPDATE nguong SET `nguong`='" + data + "',`thoi_gian`='" + dt.toString() + "' WHERE ID=1 "
        con.query(sql2, function (err, result) {
            if (err) throw err;
            console.log("Inserted to Table: ");
            console.log( data + " " + dt)
        });
        LayNguong();
    })

})