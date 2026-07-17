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