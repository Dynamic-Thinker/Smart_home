#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <Arduino.h>

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>

    <meta charset="UTF-8">

    <meta
        name="viewport"
        content="width=device-width, initial-scale=1.0">

    <title>Gwen Home</title>

    <style>
/* ===========================================================
   GWEN HOME
   =========================================================== */

*{
    margin:0;
    padding:0;
    box-sizing:border-box;
}

:root{

    --bg:#0f172a;
    --card:#1b2535;
    --header:#172135;

    --cyan:#00d4ff;
    --green:#00d26a;
    --orange:#ffb347;
    --red:#ff4d4d;

    --shadow:0 12px 35px rgba(0,0,0,.35);

}

body{

    background:var(--bg);

    font-family:
    "Segoe UI",
    Arial,
    sans-serif;

    color:white;

    min-height:100vh;

}


/* ===========================================================
   HEADER
   =========================================================== */

header{

    display:flex;

    justify-content:space-between;

    align-items:center;

    padding:24px 40px;

    background:var(--header);

    box-shadow:var(--shadow);

}

.logo h1{

    color:var(--cyan);

    font-size:34px;

    letter-spacing:2px;

}

.logo p{

    opacity:.7;

    margin-top:5px;

}

#connection{

    display:flex;

    align-items:center;

    gap:10px;

    background:#243349;

    padding:10px 18px;

    border-radius:30px;

    transition:.3s;

}

.dot{

    width:10px;

    height:10px;

    border-radius:50%;

    background:var(--green);

    box-shadow:

    0 0 10px var(--green);

    animation:pulse 1.8s infinite;

}

.connected{

    background:#243349;

}

.disconnected{

    background:#4d2020;

}

.disconnected .dot{

    background:var(--red);

    box-shadow:0 0 10px var(--red);

}

@keyframes pulse{

0%{

transform:scale(1);

}

50%{

transform:scale(.7);

}

100%{

transform:scale(1);

}

}


/* ===========================================================
   GRID
   =========================================================== */

.container{

    width:95%;

    max-width:850px;

    margin:40px auto;

    padding:15px;

    display:grid;

    gap:15px;

    grid-template-columns:

    repeat(3,1fr);

}

.mainPower{

    grid-column:1/-1;

}


/* ===========================================================
   CARD
   =========================================================== */

.card{

    background:var(--card);

    border-radius:15px;

    padding:15px;

    box-shadow:var(--shadow);

    position:relative;

    overflow:hidden;

    transition:.3s;

    opacity:0;

    animation:fade .6s forwards;

}

.card:nth-child(1){

animation-delay:.1s;

}

.card:nth-child(2){

animation-delay:.2s;

}

.card:nth-child(3){

animation-delay:.3s;

}

.card:nth-child(4){

animation-delay:.4s;

}

.card:hover{

transform:translateY(-6px);

}

.card::before{

content:"";

position:absolute;

left:0;

top:0;

width:100%;

height:1px;

background:

rgba(255,255,255,.15);

}

@keyframes fade{

to{

opacity:1;

}

}

.card h2{

    text-align:center;

    margin-bottom:15px;
    
    font-size:18px;

}


/* ===========================================================
   MCB
   =========================================================== */

.mcbBox{

    width:60px;

    height:100px;

    margin:auto;

    border-radius:10px;

    background:#333;

    display:flex;

    justify-content:center;

    align-items:center;
    
    box-shadow: inset 0 0 10px rgba(0,0,0,0.5);

}

.mcbLever{

    width:40px;

    height:40px;

    border-radius:8px;

    background:var(--red);

    transition:.35s;

    transform:translateY(22px);
    
    box-shadow: 0 4px 10px rgba(0,0,0,0.4);

}

.mcbLever.on{
    
    background:var(--green);

    transform:translateY(-22px);

}


/* ===========================================================
   TUBE
   =========================================================== */

.tubeContainer{

display:flex;

justify-content:center;

margin:20px 0;

}

.tube{

width:120px;

height:8px;

background:#666;

border-radius:10px;

transition:.35s;

position:relative;

}

.tube::before{

content:"";

position:absolute;

left:-6px;

top:-1px;

width:6px;

height:10px;

background:#999;

}

.tube::after{

content:"";

position:absolute;

right:-6px;

top:-1px;

width:6px;

height:10px;

background:#999;

}

.tube.on{

background:white;

box-shadow:

0 0 6px white,

0 0 12px cyan,

0 0 25px cyan,

0 0 45px cyan;

}


/* ===========================================================
   PLUG
   =========================================================== */

.plugContainer{

height:110px;

display:flex;

flex-direction:column;

justify-content:center;

align-items:center;

position:relative;

gap: 10px;

}

.socket{

width:50px;

height:30px;

background:#ddd;

border-radius:8px;

position:relative;

}

.socket::before{

content:"";

position:absolute;

left:12px;

top:10px;

width:6px;

height:10px;

background:#333;

}

.socket::after{

content:"";

position:absolute;

right:12px;

top:10px;

width:6px;

height:10px;

background:#333;

}

.plug{

width:36px;

height:28px;

background:#222;

border-radius:8px;

position:relative;

transition:.35s;

}

.plug::before{

content:"";

position:absolute;

width:4px;

height:16px;

background:silver;

left:6px;

top:-16px;

}

.plug::after{

content:"";

position:absolute;

width:4px;

height:16px;

background:silver;

right:6px;

top:-16px;

}

.plug.in{

transform:translateY(-38px);

}


/* ===========================================================
   FAN
   =========================================================== */

.fanHolder{

display:flex;

justify-content:center;

align-items:center;

height:110px;

}

.fan{

width:70px;

height:70px;

position:relative;

}

.blade{

position:absolute;

left:29px;

top:0;

width:12px;

height:40px;

background:var(--cyan);

border-radius:10px;

transform-origin:6px 35px;

}

.b2{

transform:rotate(120deg);

}

.b3{

transform:rotate(240deg);

}

.hub{

position:absolute;

left:50%;

top:50%;

margin-left:-8px;

margin-top:-8px;

width:16px;

height:16px;

background:white;

border-radius:50%;

z-index:10;

}

.spin{

animation:spin 1s linear infinite;

}

@keyframes spin{

to{

transform:rotate(360deg);

}

}


/* ===========================================================
   SPEED
   =========================================================== */

.speedBox{

margin-top:25px;

}

.speedBox label{

display:flex;

justify-content:space-between;

margin-bottom:12px;

}

#speedSlider{

width:100%;

height:8px;

appearance:none;

background:#34455d;

border-radius:20px;

outline:none;

}

#speedSlider::-webkit-slider-thumb{

appearance:none;

width:22px;

height:22px;

border-radius:50%;

background:var(--cyan);

cursor:pointer;

box-shadow:

0 0 10px var(--cyan);

}


/* ===========================================================
   SWITCH
   =========================================================== */

.switch{

display:block;

margin:15px auto 0;

position:relative;

width:54px;

height:28px;

}

.switch input{

display:none;

}

.slider{

position:absolute;

inset:0;

background:#555;

border-radius:30px;

transition:.3s;

}

.slider::before{

content:"";

position:absolute;

left:3px;

top:3px;

width:22px;

height:22px;

background:white;

border-radius:50%;

transition:.3s;

}

input:checked + .slider{

background:var(--green);

}

input:checked + .slider::before{

transform:translateX(26px);

}

.switch:active{

transform:scale(.95);

}


/* ===========================================================
   MINI COOLER
   =========================================================== */

.mini-cooler-container{
    position:fixed;
    bottom:15px;
    right:15px;
    z-index:100;
    display:flex;
    flex-direction:column;
    align-items:center;
    gap:5px;
    background:rgba(20,30,45,.9);
    padding:6px 10px;
    border-radius:10px;
    box-shadow:var(--shadow);
    border:1px solid rgba(255,255,255,.05);
}

.cooler-switch{
    display:flex;
    background:#111;
    border-radius:15px;
    position:relative;
    padding:3px;
    box-shadow:inset 0 2px 4px rgba(0,0,0,.6);
}

.cooler-option{
    width:24px;
    height:16px;
    display:flex;
    justify-content:center;
    align-items:center;
    font-size:7px;
    font-weight:bold;
    color:#777;
    z-index:2;
    cursor:pointer;
    user-select:none;
    transition:.3s;
}

.cooler-option.active{
    color:white;
}

.cooler-thumb{
    position:absolute;
    top:3px;
    left:3px;
    width:24px;
    height:16px;
    background:var(--cyan);
    border-radius:8px;
    z-index:1;
    transition:.3s cubic-bezier(.4,0,.2,1);
    box-shadow:0 0 8px rgba(0,212,255,.5);
}

.cooler-thumb.pos-0 { transform:translateX(0); background:var(--red); box-shadow:0 0 8px rgba(255,77,77,.5); }
.cooler-thumb.pos-1 { transform:translateX(24px); background:var(--green); box-shadow:0 0 8px rgba(0,210,106,.5); }
.cooler-thumb.pos-2 { transform:translateX(48px); background:var(--cyan); box-shadow:0 0 8px rgba(0,212,255,.5); }

.cooler-text{
    font-size:9px;
    font-weight:bold;
    color:#aaa;
    letter-spacing:1px;
}

/* ===========================================================
   MOBILE
   =========================================================== */

@media(max-width:900px){

header{

flex-direction:column;

gap:15px;

}

.container{

grid-template-columns:1fr;

width:95%;

padding:15px;

margin:20px auto;

}

.mainPower{

grid-column:auto;

}

}
</style>

</head>

<body>

<header>

    <div class="logo">

        <h1>GWEN HOME</h1>

        <p>Smart Home Dashboard</p>

    </div>

    <div id="connection">

        <span class="dot"></span>

        <span id="connectionText">

            CONNECTED

        </span>

    </div>

</header>


<main class="container">

    <!-- ================= MAIN POWER ================= -->

    <section class="card mainPower">

        <h2>Master Switch</h2>

        <div class="mcbBox">

            <div
                id="mcbLever"
                class="mcbLever">
            </div>

        </div>

        <label class="switch">

            <input
                id="mainSwitch"
                type="checkbox">

            <span class="slider"></span>

        </label>

    </section>


    <!-- ================= TUBE ================= -->

    <section class="card">

        <h2>Tube Light</h2>

        <div class="tubeContainer">

            <div
                id="tube"
                class="tube">
            </div>

        </div>

        <label class="switch">

            <input
                id="tubeSwitch"
                type="checkbox">

            <span class="slider"></span>

        </label>

    </section>


    <!-- ================= PLUG ================= -->

    <section class="card">

        <h2>Plug Point</h2>

        <div class="plugContainer">

            <div class="socket"></div>

            <div
                id="plug"
                class="plug">

            </div>

        </div>

        <label class="switch">

            <input
                id="plugSwitch"
                type="checkbox">

            <span class="slider"></span>

        </label>

    </section>


    <!-- ================= FAN ================= -->

    <section class="card">

        <h2>Fan</h2>

        <div class="fanHolder">

            <div
                id="fan"
                class="fan">

                <div class="blade b1"></div>
                <div class="blade b2"></div>
                <div class="blade b3"></div>

                <div class="hub"></div>

            </div>

        </div>

        <label class="switch">

            <input
                id="fanSwitch"
                type="checkbox">

            <span class="slider"></span>

        </label>

        <div class="speedBox">

            <label>

                Speed

                <span id="speedValue">

                    0

                </span>

            </label>

            <input

                id="speedSlider"

                type="range"

                min="0"

                max="100"

                value="0">

        </div>

    </section>

</main>

<!-- ================= MINI COOLER ================= -->
<div class="mini-cooler-container">
    <div class="cooler-switch">
        <div class="cooler-thumb pos-2" id="coolerThumb"></div>
        <div class="cooler-option" onclick="setCooler(0)" id="cOpt0">OFF</div>
        <div class="cooler-option" onclick="setCooler(1)" id="cOpt1">ON</div>
        <div class="cooler-option active" onclick="setCooler(2)" id="cOpt2">AUTO</div>
    </div>
    <div class="cooler-text" id="coolerText">FAN: AUTO</div>
</div>

<script>
/******************************************************************************
 * GWEN HOME
 ******************************************************************************/

const $ = id => document.getElementById(id);

const ui = {

    connection: $("connection"),
    connectionText: $("connectionText"),

    main: $("mainSwitch"),
    tube: $("tubeSwitch"),
    plug: $("plugSwitch"),
    fan: $("fanSwitch"),

    tubeAnim: $("tube"),
    plugAnim: $("plug"),
    fanAnim: $("fan"),
    mcb: $("mcbLever"),

    speed: $("speedSlider"),
    speedValue: $("speedValue"),

    coolerThumb: $("coolerThumb"),
    coolerText: $("coolerText"),
    cOpt0: $("cOpt0"),
    cOpt1: $("cOpt1"),
    cOpt2: $("cOpt2")
};

let state = {

    connected: false,

    main: false,
    tube: false,
    plug: false,
    fan: false,

    speed: 0,
    
    cooler: 2

};

let busy = false;

let eventSource = null;

/******************************************************************************
 * CONNECTION
 ******************************************************************************/

function setConnection(connected)
{
    state.connected = connected;

    if(connected)
    {
        ui.connection.classList.remove("disconnected");
        ui.connection.classList.add("connected");

        ui.connectionText.innerHTML = "CONNECTED";
    }
    else
    {
        ui.connection.classList.remove("connected");
        ui.connection.classList.add("disconnected");

        ui.connectionText.innerHTML = "DISCONNECTED";
    }
}

/******************************************************************************
 * UI
 ******************************************************************************/

function updateUI()
{
    ui.main.checked = state.main;
    ui.tube.checked = state.tube;
    ui.plug.checked = state.plug;
    ui.fan.checked = state.fan;

    ui.speed.value = state.speed;
    ui.speedValue.innerHTML = state.speed;

    ui.tubeAnim.classList.toggle(
        "on",
        state.tube
    );

    ui.plugAnim.classList.toggle(
        "in",
        state.plug
    );

    if(state.fan)
        ui.fanAnim.classList.add("spin");
    else
        ui.fanAnim.classList.remove("spin");

    ui.fanAnim.style.animationDuration =
        Math.max(0.3, 2.0 - state.speed * 0.017) + "s";

    if(state.main)
        ui.mcb.classList.add("on");
    else
        ui.mcb.classList.remove("on");

    ui.coolerThumb.className = "cooler-thumb pos-" + state.cooler;
    
    ui.cOpt0.classList.toggle("active", state.cooler === 0);
    ui.cOpt1.classList.toggle("active", state.cooler === 1);
    ui.cOpt2.classList.toggle("active", state.cooler === 2);
    
    if (state.cooler === 0) ui.coolerText.innerHTML = "FAN: OFF";
    else if (state.cooler === 1) ui.coolerText.innerHTML = "FAN: ON";
    else ui.coolerText.innerHTML = "FAN: AUTO";
}

/******************************************************************************
 * HTTP
 ******************************************************************************/

async function post(url,data)
{
    if(busy)
        return;

    busy=true;

    try{

        await fetch(url,{

            method:"POST",

            headers:{
                "Content-Type":
                "application/x-www-form-urlencoded"
            },

            body:data

        });

    }
    catch(e){

        console.log(e);

    }

    // Wait for Arduino → ESP → SSE

}

/******************************************************************************
 * INITIAL STATE
 ******************************************************************************/

async function requestState()
{
    try{

        const response=
            await fetch("/status");


        state=
            await response.json();

        setConnection(true);

        updateUI();

    }

    catch(e){

        setConnection(false);

    }
}

/******************************************************************************
 * SSE
 ******************************************************************************/

function connectEvents()
{
    if(eventSource)
        eventSource.close();

    eventSource=
        new EventSource("/events");

    eventSource.addEventListener(
        "state",
        function(e){

            state=
                JSON.parse(e.data);

            busy=false;

            setConnection(true);

            updateUI();

        }
    );

    eventSource.addEventListener(
        "ping",
        function(){ }
    );

    eventSource.onerror=function(){

        setConnection(false);

        busy=false;

        setTimeout(
            connectEvents,
            2000
        );

    };

}

/******************************************************************************
 * SWITCHES
 ******************************************************************************/

ui.main.onchange=function(){

    post(
        "/toggle",
        "d=main"
    );

};

ui.tube.onchange=function(){

    post(
        "/toggle",
        "d=tube"
    );

};

ui.plug.onchange=function(){

    post(
        "/toggle",
        "d=plug"
    );

};

ui.fan.onchange=function(){

    post(
        "/toggle",
        "d=fan"
    );

};

/******************************************************************************
 * FAN SPEED
 ******************************************************************************/

ui.speed.oninput=function(){

    ui.speedValue.innerHTML=
        ui.speed.value;

};

ui.speed.onchange=function(){

    post(
        "/speed",
        "v="+ui.speed.value
    );

};

/******************************************************************************
 * COOLER
 ******************************************************************************/

function setCooler(val) {
    state.cooler = val;
    updateUI(); // optimistic update
    post(
        "/cooler",
        "v=" + val
    );
}

/******************************************************************************
 * STARTUP
 ******************************************************************************/

window.onload=function(){

    requestState();

    connectEvents();

};
</script>

</body>

</html>
)rawliteral";

#endif
