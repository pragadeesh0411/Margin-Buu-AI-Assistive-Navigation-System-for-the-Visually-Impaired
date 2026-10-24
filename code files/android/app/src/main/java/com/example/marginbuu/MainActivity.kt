package com.example.marginbuu

import android.app.Activity
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.speech.tts.TextToSpeech
import android.view.Gravity
import android.view.ViewGroup
import android.widget.Button
import android.widget.LinearLayout
import android.widget.TextView
import org.json.JSONObject
import java.net.HttpURLConnection
import java.net.URL
import java.util.Locale
import java.util.concurrent.Executors

class MainActivity : Activity(), TextToSpeech.OnInitListener {
    companion object {
        // Replace these with the IPs printed by the firmware serial monitors.
        private const val SENSOR_BASE_URL = "http://192.168.1.50"
        private const val CAMERA_CAPTURE_URL = "http://192.168.1.51/capture"
        private const val POLL_INTERVAL_MS = 1500L
    }

    private val executor = Executors.newSingleThreadExecutor()
    private val mainHandler = Handler(Looper.getMainLooper())
    private lateinit var statusText: TextView
    private lateinit var tts: TextToSpeech
    private var ttsReady = false
    private var lastSpoken = ""
    private var lastSpokenAt = 0L
    private var running = true

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        tts = TextToSpeech(this, this)

        val layout = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            gravity = Gravity.CENTER_HORIZONTAL
            setPadding(28, 36, 28, 28)
        }
        val title = TextView(this).apply {
            text = "MarginBuu"
            textSize = 30f
            gravity = Gravity.CENTER
        }
        statusText = TextView(this).apply {
            text = "Connecting to sensor controller…"
            textSize = 18f
            setPadding(0, 28, 0, 28)
        }
        val speakButton = Button(this).apply {
            text = "Repeat guidance"
            setOnClickListener {
                val message = statusText.text.toString()
                speak(message, force = true)
            }
        }
        val snapshotButton = Button(this).apply {
            text = "Check camera connection"
            setOnClickListener { checkCamera() }
        }
        val stopButton = Button(this).apply {
            text = "Stop voice guidance"
            setOnClickListener { if (ttsReady) tts.stop() }
        }
        layout.addView(title, ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT)
        layout.addView(statusText, ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT)
        layout.addView(speakButton)
        layout.addView(snapshotButton)
        layout.addView(stopButton)
        setContentView(layout)

        pollStatus()
    }

    override fun onInit(status: Int) {
        ttsReady = status == TextToSpeech.SUCCESS
        if (ttsReady) tts.language = Locale.getDefault()
    }

    private fun pollStatus() {
        if (!running) return
        executor.execute {
            val result = getText("$SENSOR_BASE_URL/status")
            mainHandler.post {
                if (result == null) {
                    statusText.text = "Sensor connection unavailable. Do not assume the path is clear."
                    speak("Sensor connection unavailable. Do not assume the path is clear.")
                } else {
                    try {
                        val json = JSONObject(result)
                        val guidance = json.optString("guidance", "UNKNOWN")
                        val direction = json.optString("nearest_direction", "unknown")
                        val distance = json.optDouble("nearest_cm", -1.0)
                        val message = when (guidance) {
                            "STOP" -> "Stop. Obstacle very close, $direction."
                            "CAUTION" -> "Caution. Obstacle $direction, about ${distance.toInt()} centimeters."
                            "CLEAR" -> "No nearby obstacle detected by the sensors."
                            else -> "Obstacle status unknown. Check your surroundings."
                        }
                        statusText.text = "$message\n\nL: ${fmt(json.optDouble("left_cm", -1.0))} cm  " +
                            "F: ${fmt(json.optDouble("front_cm", -1.0))} cm  " +
                            "R: ${fmt(json.optDouble("right_cm", -1.0))} cm"
                        speak(message)
                    } catch (_: Exception) {
                        statusText.text = "Invalid sensor response. Check firmware and network."
                    }
                }
                mainHandler.postDelayed({ pollStatus() }, POLL_INTERVAL_MS)
            }
        }
    }

    private fun checkCamera() {
        executor.execute {
            val connection = try {
                (URL(CAMERA_CAPTURE_URL).openConnection() as HttpURLConnection).apply {
                    connectTimeout = 2500
                    readTimeout = 3500
                    requestMethod = "GET"
                }
            } catch (_: Exception) {
                null
            }
            val message = if (connection != null) {
                try {
                    val type = connection.contentType ?: "unknown"
                    if (connection.responseCode == 200 && type.contains("image/jpeg")) {
                        "Camera responded with a JPEG snapshot. Image recognition is not implemented in this starter."
                    } else {
                        "Camera response: HTTP ${connection.responseCode}, type $type"
                    }
                } catch (_: Exception) {
                    "Camera request failed. Check the camera IP and Wi-Fi."
                } finally {
                    connection.disconnect()
                }
            } else {
                "Camera connection unavailable. Check the camera IP and Wi-Fi."
            }
            mainHandler.post {
                statusText.text = message
                speak(message, force = true)
            }
        }
    }

    private fun getText(address: String): String? {
        var connection: HttpURLConnection? = null
        return try {
            connection = (URL(address).openConnection() as HttpURLConnection).apply {
                connectTimeout = 2000
                readTimeout = 2500
                requestMethod = "GET"
            }
            if (connection.responseCode == 200) connection.inputStream.bufferedReader().use { it.readText() }
            else null
        } catch (_: Exception) {
            null
        } finally {
            connection?.disconnect()
        }
    }

    private fun fmt(value: Double): String =
        if (value < 0) "unknown" else value.toInt().toString()

    private fun speak(message: String, force: Boolean = false) {
        val now = System.currentTimeMillis()
        if (!force && message == lastSpoken && now - lastSpokenAt < 5000L) return
        if (!force && now - lastSpokenAt < 1800L) return
        if (ttsReady) {
            tts.speak(message, TextToSpeech.QUEUE_FLUSH, null, "marginbuu_guidance")
            lastSpoken = message
            lastSpokenAt = now
        }
    }

    override fun onDestroy() {
        running = false
        if (::tts.isInitialized) {
            tts.stop()
            tts.shutdown()
        }
        executor.shutdownNow()
        super.onDestroy()
    }
}
