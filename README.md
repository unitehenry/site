## Static Site Generator

```
blog/
├── content/
│   ├── index.md
│   ├── blog/
│   │   └── first-post.md
│   └── about.md
├── templates/
│   └── base.html
└── static/
    └── css/style.css
```

## Refactor

Modules
    - main
    - strings
    - content
    - generate

```
main
get_content_files -> content
generate_pages -> generate
create_subdirectories -> generate (private)
path_root -> generate (private)
run_pandoc -> generate (private)
to_write_path -> genrate (private)
string_list -> strings
string_replace -> strings
trim_whitespace -> strings
```

TODO: main constants -> env vars
