var socket = io("http://localhost:5005");
// var socket = io("http:/10.21.109.89:5005");

var btn1 = document.getElementById('btn1');
var led1 = document.getElementById('led1');

var slider=document.getElementById("customRange");
var nguong=document.getElementById("nguong");

// ------------------------------------------openChart-----------------------------------------------------
textOff1 = "Đóng biều đồ? \nNhấn OK để đóng CANCEL để Hủy.";
function openChart() {
  if (chartBtn.innerText == 'Open Chart') {
      document.getElementById('chartBtn').innerText = 'Close Chart';
      document.getElementById('chart').style.display = 'block';
  }
  else {
    if (confirm(textOff1) == true){
      document.getElementById('chartBtn').innerText = 'Open Chart';
      document.getElementById('chart').style.display = 'none';
    }
  }
}
openChart();  

// ------------------------------chartJS------------------------------------------------
const pickChart = document.getElementById('myChart');
const data = {
  datasets: [
    {
      label: 'Độ ẩm',
      data: [],
      borderColor: '#00DBDE',
      pointRadius: 5,
      pointStyle: 'circle',
    },
  ],
};
const drawChart = new Chart(pickChart, {
  type: 'line', // kiểu biểu đồ phần tán, cần có các điểm dữ liệu để nó nối ()
  data: data
});

//-------------------------------------------------
socket.on("duLieu", function (data) {
   
  let doam = data.humid;
  let timee = data.time;
  slider.value=data.nguong;

  nguong.innerHTML = slider.value;
  slider.oninput = function(){
    nguong.innerHTML=this.value;
    socket.emit("Ngưỡng", nguong.innerHTML);
  }
  

  //Cập nhật nội dung của ID
  $("#humid").html(doam + " %")

  if (drawChart.data.labels.length >= 10) {
    drawChart.data.datasets[0].data.shift();
    drawChart.data.labels.shift();
  }
  // Push data to chart
  drawChart.data.labels.push(timee);
  drawChart.data.datasets[0].data.push(doam);
  drawChart.update(); //cập nhật thang đo

});
//------cap nhat trang thai may bom-------
socket.on("Trạng thái",function(data){
  if(data=="1"){
    led1.style.color = 'yellow';
    btn1.classList.remove('btn-danger');
    btn1.classList.add('btn-success');
    btn1.innerText = 'ON';
  }
  else{
    led1.style.color = '';
    btn1.classList.remove('btn-success');
    btn1.classList.add('btn-danger');
    btn1.innerText = 'OFF';
  }
});


// ------------------------------btn------------------------------------------------
// var btn1 = document.getElementById('btn1');
// var led1 = document.getElementById('led1');
// textOn = "Bật máy bơm? \nNhấn OK để Bật CANCEL để Hủy.";
// textOff = "Tắt máy bơm? \nNhấn OK để Tắt CANCEL để Hủy.";

// function MayBom() {
//   if (btn1.innerText == 'ON') {
//     if (confirm(textOn) == true) {
//       led1.style.color = 'yellow';

//       btn1.classList.remove('btn-success');
//       btn1.classList.add('btn-danger');
//       btn1.innerText = 'OFF';
//       socket.emit("Máy bơm", "1")
//     }

//   } else {
//     if (confirm(textOff) == true) {
//       led1.style.color = '';

//       btn1.innerText = 'ON';
//       btn1.classList.remove('btn-danger');
//       btn1.classList.add('btn-success');
//       socket.emit("Máy bơm", "0")
//     }
//   }
// }

