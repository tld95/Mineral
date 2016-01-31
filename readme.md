# MINERAL
> Pebble's first personal assistant!

[![MINERAL](http://img.youtube.com/vi/cwL0inVQiSM/0.jpg)](http://www.youtube.com/watch?v=cwL0inVQiSM)

## What is it?
- Mineral is a personal assistant for the Pebble, which scrapes Google Search and returns insightful textual information to the user, based on a query.
- Mineral samples data in the background and will launch a foreground app when a sudden motion is detected in the y axis, indicating the flick of a wrist.
- Mineral then utilizes Pebble's built in dication software, generating a Google search query from the user's voice.
- The search query is then sent to a Python server, which executes a Google search, using the user's transcribed dictation as a query.
- The result of the query, in the form of a Google Now card, is then scraped for essential information, which is returned to and displayed on the Pebble as text.

## How do I use it?
1. Simply flick your wrist while wearing the Pebble smartwatch to activate the dictation software.
2. When the microphone icon appears on the screen, speak your query to the watch.
3. Once your voice has been processed by Mineral, click the center button of the Pebble to confirm your query.
4. The watch will then display an answer to your query.

## APIs Used
- [Pebble C API](https://developer.getpebble.com/docs/c/)
  - Accelerometer
  - Dictation
  - User Interface

## Future Ideas
- [ ] Twitter intergration; Allowing the user to tweet by speaking to Mineral.
- [ ] Displaying weather information.
- [ ] Displaying images from Google Images.
- [ ] Intergration with internet of things.
- [ ] Integration with Houndify api, rather than scraping Google

## Contribute

Contributions are always welcome!

## License

[![CC0](https://licensebuttons.net/p/zero/1.0/88x31.png)](http://creativecommons.org/publicdomain/zero/1.0/)
