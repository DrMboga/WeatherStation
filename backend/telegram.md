# Here are some tricks how to use telegram bot api.

- [Pushing messages to some Telegram channel](#pushing-messages-to-some-telegram-channel)
- [Using web hooks for Telegram bot interaction](#using-web-hooks-for-telegram-bot-interaction)

## Pushing messages to some Telegram channel

### 1. Create a chat bot

1. Search for the `botfather` telegram bot in the Telegram client.
2. Type `/newbot` to create a new bot. You need to specify the bot's screen name and username. If the bot is successfully created, you will see the bot's API token like `123456789:aBcEfGKXX5SsYHDRDj34oa3YE`. You must not share this token with anyone.

### 2. Create a telegram channel.

1. Create a _public_ Telegram channel with a suitable name (the chat name started with `@` we will need to get chat id in step 5).
2. Add your bot (by bot name started from `@`) to the list of administrators of the created channel. At least, the bot must have permission to post messages.
3. Get the chat ID. To do so, you can use a `Postman` and call the [sendMessage](https://core.telegram.org/bots/api#sendmessage) method like this: HTTP POST https://api.telegram.org/bot[your-bot-token]/sendMessage with this body as JSON:

```json
{
  "parse_mode": "Markdown",
  "text": "_Hello there_\r\nFrom *Postman*\r\nTa da",
  "chat_id": "[public chat id started with @]"
}
```

Response will contain the numeric chat id like "id": -1001234567890,

4. Put the `bot API token` and numeric `chat id` into some secure place and use it to call further Telegram APi calls.
5. As soon as we got the numeric chat id, we can make a created in step 1 channel as private. We don't need that public name started from @ anymore.

## Using web hooks for Telegram bot interaction

There is another useful thing in Telegram chat bots. You can interact with bot itself like sending to the bot a command started from `/` symbol and getting some response.
To do so, you need to:

- Create [Chat bot](#1-create-a-chat-bot).
- Have some WEB API hosted in some public space.
- Create a set of commands using BotFather: `/mybots` -> select your bot from the list -> `Edit bot` -> `Edit Commands`
- Create web hook using [this API method](https://core.telegram.org/bots/api#setwebhook)

Like this:

```bash
curl -X POST "https://api.telegram.org/bot<YourBotToken>/setWebhook" \
-d "url=https://<your-public-url>/telegram/update"
```

Replace `YourBotToken` with your bot token and `your-public-url` with your exposed API endpoint.

- In your WEB API make something like this:

```csharp
[ApiController]
[Route("telegram")]
public class TelegramController : ControllerBase
{
    [HttpPost("update")]
    public async Task<IActionResult> ReceiveUpdate([FromBody] TelegramUpdate update)
    {
        if (update?.Message?.Text == "/forecast") // This is the command from telegram
        {
            // Call the forecast endpoint and return the result.
            var forecast = await GetForecastAsync();
            await SendMessage(update.Message.Chat.Id, forecast);
        }

        return Ok();
    }

    private async Task<string> GetForecastAsync()
    {
        // Call your forecast endpoint here.
        return "Weather is sunny with 25°C."; // Example response.
    }

    private async Task SendMessage(long chatId, string text)
    {
        var botToken = "<YourBotToken>";
        var apiUrl = $"https://api.telegram.org/bot{botToken}/sendMessage";

        using var httpClient = new HttpClient();
        var response = await httpClient.PostAsJsonAsync(apiUrl, new
        {
            chat_id = chatId,
            text = text
        });
    }
}

// Model
public class TelegramUpdate
{
    public Message Message { get; set; }
}

public class Message
{
    public long MessageId { get; set; }
    public Chat Chat { get; set; }
    public string Text { get; set; }
}

public class Chat
{
    public long Id { get; set; }
}
```
