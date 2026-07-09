# Sponsor Module

A full C++/Qt sponsor-management application with database CRUD, analytics, multilingual support, AI assistance, theming, and device integration.

## Overview

This project centers on a `Sponsor` model and its database operations, and the packaged Qt app also includes several advanced user-facing features:

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

- `Atelier_Connexion.pro` - Qt project file
- `main.cpp` - application entry point
- `mainwindow.cpp` / `mainwindow.h` / `mainwindow.ui` - main UI and logic
- `sponsor.cpp` / `sponsor.h` - sponsor model and database operations
- `connection.cpp` / `connection.h` - database connection helper
- `geminiai.cpp` / `geminiai.h` - Gemini chatbot integration
- `arduino.cpp` / `arduino.h` - Arduino serial helper
- `keycardmanager.cpp` / `keycardmanager.h` - key card serial handling
- `styles/` - dark and light theme stylesheets
- `translations/` - English, French, and Arabic translations
- `sponsor.cbp` - legacy Code::Blocks project file

## Requirements

- C++ compiler with Qt support
- Qt Core
- Qt SQL module
- Qt Network module
- Qt Charts module
- Qt SerialPort module
- A database containing a `sponsors` table
- Environment variable `GEMINI_API_KEY` for chatbot access

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

If you are integrating this app into a larger Qt project, open the database connection first, load `Atelier_Connexion.pro`, and run the application from Qt Creator or Code::Blocks with the Qt toolchain configured.

## Notes

- The module uses bound SQL parameters, which is a good baseline for safer queries.
- Translation files and stylesheets are loaded relative to the application directory, so the packaged `translations/` and `styles/` folders should stay alongside the executable.
- Error reporting is intentionally minimal right now; production apps should surface `QSqlError` details to help with debugging.
- The Gemini API key is read from `GEMINI_API_KEY` instead of being hardcoded.
- The `main.cpp` file launches the full Qt UI and database connection flow.

## License

No license file is currently included. Add one before publishing publicly if needed.
