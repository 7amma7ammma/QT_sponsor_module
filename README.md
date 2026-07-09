# Sponsor Module

A lightweight C++/Qt module for managing sponsor records in a database-backed application.

## Overview

This module defines a `Sponsor` model with the core sponsor fields used by the application:

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

- `main.cpp` - simple entry point
- `sponsor.h` - sponsor class declaration
- `sponsor.cpp` - sponsor class implementation
- `sponsor.cbp` - Code::Blocks project file

## Requirements

- C++ compiler with Qt support
- Qt Core
- Qt SQL module
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

## Build notes

### Code::Blocks

1. Open `sponsor.cbp`
2. Configure the Qt toolchain in Code::Blocks
3. Build the `Debug` or `Release` target

### Manual integration

If you are integrating this class into a larger Qt project, include `sponsor.h`, create a `Sponsor` instance, and call the CRUD methods after opening a valid Qt SQL database connection.

## Notes

- The module uses bound SQL parameters, which is a good baseline for safer queries.
- Error reporting is intentionally minimal right now; production apps should surface `QSqlError` details to help with debugging.
- The `main.cpp` file is currently only a placeholder entry point.

## License

No license file is currently included. Add one before publishing publicly if needed.
