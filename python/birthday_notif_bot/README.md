## Subscribers Notifier

Simple bot with subscribe/unsubscribe functionality. Bot's admin can
send message to all subscribers with /broadcast command.

### Technologies:
- *telebot* - bot logic
- *sqlite3* - keep information about subscribers

### Starting bot
```
docker build -t bot .
docker run --name bot_app -d -e TOKEN=EXAMPL3 -e TGID=EXAMPLE -v $(pwd)/data:/bot/data bot
```

![image](https://github.com/user-attachments/assets/373dc92c-fce8-4008-93fc-d71c3c14dfe3)

