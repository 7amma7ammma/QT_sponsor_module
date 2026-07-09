# Sponsor Module

A C++/Qt sponsor-management module with database CRUD and a richer application layer for analytics, multilingual support, and AI assistance.

## Overview

This project centers on a `Sponsor` model and its database operations, but the full Qt application in the workspace also includes several advanced user-facing features:

- sponsor CRUD management
- live table display and record selection
- sorting and search tools
- sponsor statistics with charts
- PDF export
- multilingual UI support
- dark and light theme switching
- Gemini-powered chatbot integration
- serial/Arduino connectivity support

The `Sponsor` model defines the core sponsor fields used by the application:

- `id`
- `name`
- `organization`
- `email`
- `phone`
- `contribution_amount`
- `sponsored_projects`
- `contract_details`
- `renewal_date`

It also provides basic CRUD operations through Qt SQL:

- `add()` inserts a sponsor record
- `read()` loads all sponsor records into a `QSqlQueryModel`
- `update()` modifies an existing sponsor
- `remove(int id)` deletes a sponsor by identifier

## Repository contents

- `main.cpp` - entry point
- `sponsor.h` - sponsor class declaration
- `sponsor.cpp` - sponsor class implementation
- `sponsor.cbp` - Code::Blocks project file
- `test/mainwindow.cpp` - main UI logic for the advanced Qt app
- `test/geminiai.cpp` - Gemini chat integration
- `test/arduino.cpp` - Arduino serial helper
- `test/connection.cpp` - database connection helper

## Requirements

- C++ compiler with Qt support
- Qt Core
- Qt SQL module
- Qt Network module
- Qt Charts module
- Qt SerialPort module
- A database containing a `sponsors` table

## Expected database table

The code expects a table similar to the following:

```sql
CREATE TABLE sponsors (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    organization TEXT,
    email TEXT,
    phone TEXT,
    contribution_amount REAL,
    sponsored_projects TEXT,
    contract_details TEXT,
    renewal_date TEXT
);
```

## Advanced features

The richer sponsor application found in the workspace adds these features on top of the model layer:

### Multilingual interface

The UI includes language switching for:

- English
- French
- Arabic

### Chatbot assistant

The project includes a `GeminiHandler` class that sends messages to the Gemini API and displays responses in the interface.

### Analytics and reporting

- sponsor counts grouped by organization
- pie-chart statistics
- sponsor list export to PDF
- sorting by name, ID, and amount
- record search by ID

### UI themes

- dark mode stylesheet
- light mode stylesheet

### Device integration

- Arduino serial connection support
- key card serial reading support

## Build notes

### Code::Blocks

1. Open `sponsor.cbp`
2. Configure the Qt toolchain in Code::Blocks
3. Build the `Debug` or `Release` target

### Manual integration

If you are integrating this class into a larger Qt project, include `sponsor.h`, create a `Sponsor` instance, and call the CRUD methods after opening a valid Qt SQL database connection.

## Notes

- The module uses bound SQL parameters, which is a good baseline for safer queries.
- The full UI code in `test/` and `Atelier_Connexion/` shows the advanced feature set; this folder contains the lighter sponsor model and project files.
- Error reporting is intentionally minimal right now; production apps should surface `QSqlError` details to help with debugging.
- The Gemini API key should never be hardcoded in production; move it to an environment variable or secure config file.
- The `main.cpp` file is currently only a placeholder entry point.

## License

No license file is currently included. Add one before publishing publicly if needed.
