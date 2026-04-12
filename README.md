## Static Site Generator

A custom static site generator written in C.

## Usage

```bash
scripts/generate
```

### Dependencies

The following dependencies are required to run this program:

- [`gcc`](https://gcc.gnu.org)
- [`pandoc`](https://pandoc.org)

## Development

```bash
scripts/serve
```

There is a script to make recompilation easier for editing content. It will start a dev server on `http://localhost:8080` and rebuild any pages that change under `content` or `static` directories.

### TODO

```
Development

- [x] HTML Linter
- [x] CSS Linter

Pages

- [x] Blog
- [ ] Experience
- [ ] Projects
- [ ] Certifications

Polish

- [ ] Favicon
- [ ] Dark Mode

CI

- [ ] Test
- [ ] Spellcheck
- [ ] Linter
- [ ] GitHub Pages
```
