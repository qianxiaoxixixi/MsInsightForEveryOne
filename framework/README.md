# MindStudio Insight GUI Test

## Project Setup

```sh
npm install
```

### Run End-to-End Tests with [Playwright](https://playwright.dev)

```sh
# Install browsers for the first run
npx playwright install

# When testing on CI, must build the project first
npm run build

# Runs the end-to-end tests
npm run test
# Runs the tests only on Chromium
npm run test -- --project=chromium
# Runs the tests of a specific file
npm run test -- tests/example.spec.ts
# Runs the tests in debug mode
npm run test -- --debug
```

### Lint with [ESLint](https://eslint.org/)

```sh
npm run lint
```
