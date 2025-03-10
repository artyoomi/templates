import os
import telebot
import sqlite3


TOKEN    = os.getenv("TOKEN")
ADMIN_ID = os.getenv("TGID")

bot = telebot.TeleBot(TOKEN)

# Connect to sqlite3 database
users_db_conn = sqlite3.connect("data/users.db", check_same_thread=False)
db_cursor = users_db_conn.cursor()

# Create table to store subscribers
db_cursor.execute('''
    CREATE TABLE IF NOT EXISTS subscribers (
        user_id INTEGER PRIMARY KEY
        )
''')
users_db_conn.commit()

# /start
@bot.message_handler(commands=["start"])
def start(message):
    markup = telebot.types.ReplyKeyboardMarkup(resize_keyboard=True)
    btn_subscribe   = telebot.types.KeyboardButton('Subscribe')
    btn_unsubscribe = telebot.types.KeyboardButton('Unsubscribe')

    markup.add(btn_subscribe, btn_unsubscribe)
    bot.reply_to(message,
                 """
                 👋 Привет\! Я бот для рассылки\. Для подписки на рассылку используй *Subscribe*, a для отписки от неё \- *Unsubscribe*\.
                 """,
                 reply_markup=markup,
                 parse_mode="MarkdownV2")

# /subscribe button processing
@bot.message_handler(func=lambda message: message.text == "Subscribe")
def subscribe(message):
    user_id = message.from_user.id

    if user_id == int(ADMIN_ID):
        # To enable admin subscription, comment this whole block of code (with if statement)
        bot.reply_to(message, "Админ не может быть подписан на рассылку 👨‍💻 (это можно изменить в коде бота при желании).")
        return

    db_cursor.execute('SELECT user_id FROM subscribers WHERE user_id = ?', (user_id,))
    if db_cursor.fetchone():
        bot.reply_to(message, "Вы уже подписались на рассылку! 👍")
    else:
        db_cursor.execute('INSERT INTO subscribers (user_id) VALUES (?)', (user_id,))
        users_db_conn.commit()
        bot.reply_to(message, "Вы подписались на рассылку! 🎉")

# /unsubscribe button processing
@bot.message_handler(func=lambda message: message.text == "Unsubscribe")
def unsubscribe(message):
    user_id = message.from_user.id
    db_cursor.execute('SELECT user_id FROM subscribers WHERE user_id = ?', (user_id,))
    
    # Check user subscription
    if db_cursor.fetchone():
        db_cursor.execute('DELETE FROM subscribers WHERE user_id = ?', (user_id,))
        users_db_conn.commit()
        bot.reply_to(message, "Вы отписались от рассылки 🚫")
    else:
        bot.reply_to(message, "Вы не были подписаны на рассылку 😅")

# /broadcast
@bot.message_handler(commands=["broadcast"])
def broadcast(message):
    # Check that command was received from admin
    if message.from_user.id == int(ADMIN_ID):
        msg = bot.reply_to(message, "Введите сообщение для рассылки ✍️")
        bot.register_next_step_handler(msg, send_broadcast)
    else:
        bot.reply_to(message, "У вас нет прав для выполнения этой команды ⛔")

def send_broadcast(message):
    text_to_send = "📢 Рассылка!\n" + message.text
    if text_to_send:
        db_cursor.execute('SELECT user_id FROM subscribers')
        subscribers = db_cursor.fetchall()
        for subscriber in subscribers:
            try:
                bot.send_message(subscriber[0], text_to_send)
            except Exception as e:
                print(f"Не удалось отправить сообщение пользователю {subscriber[0]} 😔: {e}")
        bot.reply_to(message, "Рассылка завершена! ✅")
    else:
        bot.reply_to(message, "Сообщение не может быть пустым ❌")


def main():
    print("Bot was started...")

    try:
        bot.polling(none_stop=True)
    except KeyboardInterrupt:
        print("\nStopping bot...")

if __name__ == "__main__":
    main()
