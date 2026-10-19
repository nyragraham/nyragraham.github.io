# nyragraham.github.io

Engineering portfolio, built with Jekyll on GitHub Pages.

- About page: `index.md`
- Projects: one file per project in `_projects/`, media in `assets/projects/<project>/`

Project front matter:

```yaml
---
title: Project Name
subtitle: Course or context, dates   # optional
summary: One or two sentences shown on the Projects page.
thumbnail: /assets/projects/project-name/thumb.jpg
order: 1                             # position on the Projects page
---
```

Optional: `tags: [SolidWorks, Arduino]` shows small labels under the title.
Wrap images in `<div class="gallery" markdown="1">...</div>` to show them side by side.
