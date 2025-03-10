FROM python:3.10-alpine

WORKDIR /bot
VOLUME /bot/data

# Setup dependencies
COPY requirements.txt /bot/requirements.txt
RUN pip install --no-cache-dir -r requirements.txt

# Make data directory if needed
RUN mkdir -p data

COPY bot.py /bot/bot.py

ENTRYPOINT [ "python3" ]
CMD [ "bot.py" ]
