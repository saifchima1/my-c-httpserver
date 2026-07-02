const button = document.getElementById("button");
let score = 0;
button.addEventListener('click', () => {
  score += 1;
  button.innerText = score;
})
