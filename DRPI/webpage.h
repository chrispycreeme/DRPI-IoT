const char webserver[] PROGMEM = R"=====(
  <!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>DRPI Device Page</title>
    <script src="https://unpkg.com/boxicons@2.1.4/dist/boxicons.js"></script>
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@100..900&display=swap" rel="stylesheet">
    <style>
        body {
            font-family: 'Inter', sans-serif;
            margin: 0 auto;
        }

        .container {
            border-radius: 25px;
            margin: 25px auto 0;
            width: 90%;
            height: 100px;
        }

        .logo {
            float: left;
            margin-left: 45px;
        }

        .logo h1 {
            font-size: 30px;
            margin-bottom: 0;
            color: #1D4EFB;
        }

        .logo p {
            font-weight: 400;
            margin: 0;
        }

        /* Menu styles */
        .menu ul {
            float: right;
            margin-top: 35px;
        }

        .menu ul li {
            display: inline-block;
            font-size: 17px;
            margin: 0 20px 20px 0;
        }

        .menu ul li a {
            text-decoration: none;
            color: #1D4EFB;
            position: relative;
            overflow: hidden;
        }

        .menu ul li a::before,
        .menu ul li a.activated::before {
            content: "";
            position: absolute;
            width: 100%;
            height: 2px;
            bottom: -5px;
            left: 0;
            background-color: #1D4EFB;
            visibility: hidden;
            transform: scaleX(0);
            transition: all 0.3s ease-in-out 0s;
        }

        .menu ul li a.activated::before,
        .menu ul li a:hover::before {
            visibility: visible;
            transform: scaleX(1);
        }

        .sensor-data {
            margin-left: auto;
            margin-right: auto;
            width: 50%;
            margin-top: 80px;
        }

        .device-information {
            border-radius: 15px;
            width: 50%;
            text-align: left;
            height: 140px;
        }

        h1.device-name {
            font-size: 30px;
            margin-bottom: 0px;
        }

        p.device-description {
            margin-bottom: 0;
            margin-top: 5px;
        }

        h2.sensor-name {
            display: inline-block;
            margin-left: 15px;
            margin-right: 25px;
        }

        span {
            display: flex;
            align-items: center;
            margin-left: 20px;
        }

        .sensor-cont {
            width: 100%;
            background-color: rgb(247, 247, 247);
            border: 1px solid rgb(196, 196, 196);
            border-radius: 10px;
            margin-bottom: 10px;
        }
    </style>
</head>

<body>
    <section class="topnav">
        <div class="container">
            <div class="logo">
                <h1>DRPI - IoT</h1>
                <p>Disaster Resilience and Protection Initatiave</p>
                <p>Based on <span style="color: #1D4EFB; font-weight: 700;">Internet of Things</span></p>
            </div>
            <div class="menu">
                <ul>
                    <li><a href="https://drpiiot.000webhostapp.com/">View DRPI Devices</a></li>
                    <li><a href="https://drpiiot.000webhostapp.com/register.html">Register your DRPI</a></li>
                    <li><a href="#">About Us</a></li>
                </ul>
            </div>
        </div>
    </section>
    <section class="sensor-data">
        <div class="device-information">
            <h1 class="device-name">test-device-123</h1>
            <p class="device-description">DRPI-ID: DRPI-20240425</p>
            <p class="device-description" style="font-style: italic;">Approximate Location; Olongapo City, Zambales,
                Philippines</p>
        </div>
        <div class="sensor-cont">
            <span>
                <box-icon name='cloud-rain' type='solid' color='#1d4efb' size='lg'></box-icon>
                <h2 class="sensor-name">Rain Intensity:</h2>
                <p id="rain-sensor">Rain Status & Sensor Value: Loading...</p>
            </span>
        </div>
        <div class="sensor-cont">
            <span>
                <box-icon name='thermometer' type='solid' color='#1d4efb' size='lg'></box-icon>
                <h2 class="sensor-name">Temperature:</h2>
                <p id="temperature">Temperature: Loading...</p>
            </span>
        </div>
        <div class="sensor-cont">
            <span>
                <box-icon name='eyedropper' type='solid' color='#1d4efb' size='lg'></box-icon>
                <h2 class="sensor-name">Soil Moisture and <br>Vibration (Error Type):</h2>
                <p id="error-type">Error Type: Loading...</p>
            </span>
        </div>
    </section>
    <script>
    // Function to generate mock data
    function generateMockData() {
        // Generate mock data
        var mockData = {
            temperature: 25,
            rainStatus: 1,
            sensorValue: 1023,
            errorType: 0
        };

        return mockData;
    }

    // Function to update HTML elements with mock data
    function updateData() {
        // Get mock data
        var mockData = generateMockData();

        // Update HTML elements with mock data
        document.getElementById("rain-sensor").textContent = "Rain Status & Sensor Value: " + getRainSensor(mockData.rainStatus, mockData.sensorValue);
        document.getElementById("temperature").textContent = "Temperature: " + mockData.temperature + " °C";
        document.getElementById("error-type").textContent = "Error Type: " + getErrorType(mockData.errorType);
    }

    // Helper function to combine rain status and sensor value
    function getRainSensor(rainStatus, sensorValue) {
        var rainStatusText = "";
        switch (rainStatus) {
            case 1:
                rainStatusText = "No Rain Detected";
                break;
            case 2:
                rainStatusText = "Light Rain Detected";
                break;
            case 3:
                rainStatusText = "Medium Rain Detected";
                break;
            case 4:
                rainStatusText = "Heavy Rain Detected";
                break;
            default:
                rainStatusText = "Unknown";
        }

        return rainStatusText + ", Sensor Value: " + sensorValue;
    }

    // Helper function to convert error type code to text
    function getErrorType(errorType) {
        switch (errorType) {
            case 0:
                return "No Error";
            case 1:
                return "Level 1 Error";
            case 2:
                return "Level 2 Error";
            case 3:
                return "Level 3 Error";
            default:
                return "Unknown";
        }
    }

    // Update data periodically (e.g., every 5 seconds)
    setInterval(updateData, 5000);
</script>

</body>

</html>
)=====";