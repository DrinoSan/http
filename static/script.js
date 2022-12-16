document.addEventListener('DOMContentLoaded', () => setInterval(() => {
    let time = document.getElementById('time');
    time.innerHTML = Date.now().toLocaleString();
}, 1000));

function handleSubmit(event) {

    const data = new FormData(event.target);
    const value = Object.fromEntries(data.entries());

    return value;
}

const form = document.querySelector('.form');

form.addEventListener('submit', handleSubmit);