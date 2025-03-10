import time
from google.oauth2 import service_account
from googleapiclient.discovery import build
from googleapiclient.errors import HttpError
import requests

# Path to service account json
SERVICE_ACCOUNT_FILE = ""

# ID of Google Sheets table
SPREADSHEET_ID = ""

# Scopes setup for Google Sheets
SCOPES = [
    'https://www.googleapis.com/auth/spreadsheets.readonly'
]

CHECK_DELAY = 180

# Authorization
credentials = service_account.Credentials.from_service_account_file(
    SERVICE_ACCOUNT_FILE, scopes=SCOPES)

# Initialization of API client
sheets_service = build('sheets', 'v4', credentials=credentials)

telegram_bot_token = ""
telegram_chat_ids = ["", ""]

# Send message in bot
def send_telegram_message(message):
    url = f"https://api.telegram.org/bot{telegram_bot_token}/sendMessage"
    for id in telegram_chat_ids:
        payload = {"chat_id": id, "text": message}
        requests.post(url, data=payload)

# Google lists monitoring
def monitor_spreadsheet():
    initial_sheets = set()
    
    try:
        spreadsheet = sheets_service.spreadsheets().get(spreadsheetId=SPREADSHEET_ID).execute()
        for sheet in spreadsheet['sheets']:
            initial_sheets.add(sheet['properties']['title'])
        print("Monitoring started...")
        
        while True:
            time.sleep(CHECK_DELAY)

            # Get current lists state
            current_sheets = set()
            spreadsheet = sheets_service.spreadsheets().get(spreadsheetId=SPREADSHEET_ID).execute()
            for sheet in spreadsheet['sheets']:
                current_sheets.add(sheet['properties']['title'])

            # Check for new lists
            new_sheets = current_sheets - initial_sheets
            if new_sheets:
                for sheet_title in new_sheets:
                    subject = f"New Sheet Added: {sheet_title}"
                    body = f"A new sheet titled '{sheet_title}' has been added to the spreadsheet {SPREADSHEET_ID}!"
                    send_telegram_message('\n'.join([subject, body,
                                                    "Link to spreadsheet: https://docs.google.com/spreadsheets/d/example/edit?gid=0#gid=0"]))
                    print(f"Notification sent for new sheet: {sheet_title}")
                
                # Update initial state
                initial_sheets = current_sheets

    except HttpError as _:
        print("An error occurred, trying to restart monitoring after 1 minute!")
        time.sleep(60)
        monitor_spreadsheet()

# Start monitoring
while (True):
    try:
        monitor_spreadsheet()
    except KeyboardInterrupt:
        print("Monitoring stopped.")
        break
    except TimeoutError:
        print("=" * 20 + "\nI am broken again :(\n" + 20 * "=")
