// document.getElementById("time").innerHTML = "01:00";
document.getElementById("player1").innerHTML = 0;
document.getElementById("player2").innerHTML = 0;

var isTimerRunning = false;
var myTimer;
var oneMinute = 59;
var buzzer = new Audio("files/Sounds/airhorn.mp3");

const updateScreen = () => {
  fetchStart();
  getPlayerOneScore();
  getPlayerTwoScore();
};

const fetchStart = async () => {
  const response = await fetch(
    "http://blynk-cloud.com/Twl7IpLaed9RZPZkiSJUEi-hM03Ju1bg/get/V2"
  );
  const res = await response.json();
  console.log(res[0]);

  if (res[0] == 1 && !isTimerRunning) {
    console.log("working");

    timerStart();
  } else if (res[0] == 0) {
    timerStop();
  }
};

const getPlayerOneScore = async () => {
  const response = await fetch(
    "http://blynk-cloud.com/Twl7IpLaed9RZPZkiSJUEi-hM03Ju1bg/get/V0"
  );
  const res = await response.json();
  console.log(res[0]);

  document.getElementById("player1").innerHTML = res[0];
};

const getPlayerTwoScore = async () => {
  const response = await fetch(
    "http://blynk-cloud.com/Twl7IpLaed9RZPZkiSJUEi-hM03Ju1bg/get/V1"
  );
  const res = await response.json();
  console.log(res[0]);

  document.getElementById("player2").innerHTML = res[0];
};

const stopBlynkTimer = async () => {
  await fetch(
    "http://blynk-cloud.com/Twl7IpLaed9RZPZkiSJUEi-hM03Ju1bg/update/V2?value=0"
  );
};

const timerStart = () => {
  isTimerRunning = true;
  startCounting(oneMinute);
};

function startCounting(duration) {
  var timer = duration,
    minutes,
    seconds;
  myTimer = setInterval(async function() {
    minutes = parseInt(timer / 60, 10);
    seconds = parseInt(timer % 60, 10);

    minutes = minutes < 10 ? "0" + minutes : minutes;
    seconds = seconds < 10 ? "0" + seconds : seconds;

    oneMinute = seconds;

    document.getElementById("time").innerHTML = minutes + ":" + seconds;

    if (--timer < 0) {
      buzzer.load();
      setTimeout(function() {
        buzzer.play();
      }, 0);
      timerStop();
      oneMinute = 60;
      document.getElementById("time").innerHTML = "01:00";
      await stopBlynkTimer();
    }
  }, 1000);
}

const timerStop = () => {
  isTimerRunning = false;
  clearInterval(myTimer);
};

updateScreen();
window.setInterval(updateScreen, 1000);
