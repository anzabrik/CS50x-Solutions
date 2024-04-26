function wrong(id) {
  document.getElementById("result").innerHTML = "Wrong :(";
  document.getElementById(id).style.backgroundColor = 'red';
}
function right(id) {
  document.getElementById("result").innerHTML = "Right!";
  document.getElementById(id).style.backgroundColor = 'green';
}
function boy() {
}
document.addEventListener('DOMContentLoaded', function() {
  document.querySelector('#msb').addEventListener('click', function() {
      let input = document.querySelector('input');
      if (input.value === 'Jupiter') {
          input.style.backgroundColor = 'green';
          document.querySelector('#result1').innerHTML = 'Right!';
      } else {
          input.style.backgroundColor = 'red';
          document.querySelector('#result1').innerHTML = 'Wrong :(';
      }
  });
});