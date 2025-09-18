import { useEffect, useState } from "react"
import { useNavigate } from "react-router"
import mqtt from "mqtt"

export default function Device() {

    const nav = useNavigate()
    const [isDeviceOnline, setIsDeviceOnline] = useState(false)
    const [isLedOn, setIsLedOn] = useState(false)
    const [isMqttConnected, setIsMqttConnected] = useState(false)
    const [mqttClient, setMqttClient] = useState<mqtt.MqttClient | null>(null)

    useEffect(() => {
        // connect mqtt server with auto-reconnect options
        const client = mqtt.connect("ws://1.15.13.162:8083", {
            reconnectPeriod: 5000, // Try to reconnect every 5 seconds
            connectTimeout: 30000, // Timeout after 30 seconds
            clean: true
        })
        
        client.on('connect', () => {
            console.log('Connected to MQTT broker')
            setIsMqttConnected(true)
            // Subscribe to device status and LED status updates
            client.subscribe('device/status')
            client.subscribe('device/led/status')
        })

        client.on('message', (topic, message) => {
            const status = message.toString()
            console.log(topic,status)
            if (topic === 'device/status') {
                setIsDeviceOnline(status === 'online')
            } else if (topic === 'device/led/status') {
                setIsLedOn(status === 'on')
            }
        })

        client.on('close', () => {
            console.log('Disconnected from MQTT broker')
            setIsMqttConnected(false)
        })

        client.on('error', (err) => {
            console.error('MQTT error:', err)
            setIsMqttConnected(false)
        })

        client.on('reconnect', () => {
            console.log('Attempting to reconnect to MQTT broker...')
        })

        client.on('offline', () => {
            console.log('MQTT client is offline')
            setIsMqttConnected(false)
        })

        setMqttClient(client)

        return () => {
            // disconnect mqtt server
            client.end()
        }
    }, [])

    const handlerOpenLed = () => {
        if (mqttClient && mqttClient.connected && isDeviceOnline) {
            mqttClient.publish('device/led/control', 'on', { qos: 1 })
        }
    }

    const handlerCloseLed = () => {
        if (mqttClient && mqttClient.connected && isDeviceOnline) {
            mqttClient.publish('device/led/control', 'off', { qos: 1 })
        }
    }

    const handlerIndexClick = () => {
        nav("/")
    }

    return (
        <div className="p-4">
            <div 
                onClick={handlerIndexClick}
                className="cursor-pointer mb-4 text-blue-600 hover:text-blue-800"
            >
                返回首页
            </div>
            
            <div className="mb-4">
                <span className="font-bold mr-2">连接状态:</span>
                <span className={`inline-flex items-center ${isMqttConnected ? 'text-green-600' : 'text-yellow-600'}`}>
                    {isMqttConnected ? "已连接" : "连接中..."}
                    <span className={`ml-2 h-2 w-2 rounded-full ${isMqttConnected ? 'bg-green-600' : 'bg-yellow-600'}`}></span>
                </span>
            </div>

            <div className="mb-4">
                <span className="font-bold mr-2">设备状态:</span>
                <span className={`inline-flex items-center ${isDeviceOnline ? 'text-green-600' : 'text-red-600'}`}>
                    {isDeviceOnline ? "在线" : "离线"}
                    <span className={`ml-2 h-2 w-2 rounded-full ${isDeviceOnline ? 'bg-green-600' : 'bg-red-600'}`}></span>
                </span>
            </div>

            <div className="mb-6">
                <span className="font-bold mr-2">LED状态:</span>
                <span className={`inline-flex items-center ${isLedOn ? 'text-green-600' : 'text-gray-600'}`}>
                    {isLedOn ? "开启" : "关闭"}
                    <span className={`ml-2 h-2 w-2 rounded-full ${isLedOn ? 'bg-green-600' : 'bg-gray-600'}`}></span>
                </span>
            </div>

            <div className="flex flex-row gap-4">
                <button
                    onClick={handlerOpenLed}
                    disabled={!isMqttConnected || !isDeviceOnline}
                    className={`px-4 py-2 rounded ${
                        isMqttConnected && isDeviceOnline
                            ? 'bg-blue-600 hover:bg-blue-700 text-white' 
                            : 'bg-gray-300 text-gray-500 cursor-not-allowed'
                    }`}
                >
                    开灯
                </button>
                <button
                    onClick={handlerCloseLed}
                    disabled={!isMqttConnected || !isDeviceOnline}
                    className={`px-4 py-2 rounded ${
                        isMqttConnected && isDeviceOnline
                            ? 'bg-blue-600 hover:bg-blue-700 text-white' 
                            : 'bg-gray-300 text-gray-500 cursor-not-allowed'
                    }`}
                >
                    关灯
                </button>
            </div>
        </div>
    )

}