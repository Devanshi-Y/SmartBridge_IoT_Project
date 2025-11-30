// ======================================
// LOG SYSTEM
// ======================================
const logs = document.getElementById("logs");
const log = (msg) => {
    const time = new Date().toLocaleTimeString();
    logs.innerHTML = `<li>${time} — ${msg}</li>` + logs.innerHTML;
};

// ======================================
// CHART INIT
// ======================================
let ctx = document.getElementById("waterChart");
let chart = new Chart(ctx, {
    type: "line",
    data: {
        labels: [],
        datasets: [
            {
                label: "Water Level (Raw Value)",
                data: [],
                borderColor: "#00eaff",
                borderWidth: 2,
                tension: 0.3,
                pointRadius: 0
            }
        ]
    },
    options: {
        responsive: true,
        scales: {
            y: { beginAtZero: true, suggestedMax: 4095 },
            x: { ticks: { display: false } }
        }
    }
});

// ======================================
// THINGSPEAK SETTINGS
// ======================================
const channelID = 3176166;                    
const readAPIKey = "KHG2ZQD7NSP3Q25A";        

function fetchData() {
    fetch(`https://api.thingspeak.com/channels/${channelID}/feeds/last.json?api_key=${readAPIKey}`)
        .then(res => res.json())
        .then(data => {
            const moisture = parseFloat(data.field1);
            const distance = parseFloat(data.field2);
            const ir = parseInt(data.field3);
            const bridge = parseInt(data.field4);
            const buzzer = parseInt(data.field5);
            const flood = parseInt(data.field6);

            updateWater(moisture);
            updateShip(distance < 20 ? 1 : 0 || ir);
            updateBridge(bridge);
            updateBuzzer(buzzer);
            updateFlood(flood);
        })
        .catch(() => log("⚠ Error fetching ThingSpeak"));
}

setInterval(fetchData, 5000);
fetchData();

// ======================================
// UI UPDATE FUNCTIONS
// ======================================
function updateWater(val) {
    document.getElementById("water-level-value").innerText = val;
    chart.data.labels.push("");
    chart.data.datasets[0].data.push(val);
    if (chart.data.labels.length > 30) {
        chart.data.labels.shift();
        chart.data.datasets[0].data.shift();
    }
    chart.update();
}

function updateShip(val) {
    const shipValueEl = document.getElementById("ship-value");
    const shipDistanceEl = document.getElementById("ship-distance");
    const blip = document.getElementById("ship-blip");
    if (val === 1) {
        shipValueEl.innerText = "Ship Detected";
        shipDistanceEl.innerText = "Nearby";
        blip.style.opacity = 1;
        log("🚢 Ship detected");
    } else {
        shipValueEl.innerText = "No Ship";
        shipDistanceEl.innerText = "—";
        blip.style.opacity = 0;
    }
}

function updateBridge(val) {
    if (val === 1) log("⏫ Bridge Lifted");
    else log("⏬ Bridge Lowered");
}

function updateBuzzer(val) {
    if (val === 1) log("🔴 Alarm ON");
}

function updateFlood(val) {
    if (val === 1) log("⚠ FLOOD ALERT!");
}
