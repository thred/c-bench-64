# TOC Separator Component

The `app-toc-separator` component provides a visual separator within TOC sections, optionally with a small label.

## Usage

### Basic separator (horizontal line only)

```html
<app-toc-section label="Settings">
    <app-toc-item label="Option 1" />
    <app-toc-separator />
    <app-toc-item label="Option 2" />
</app-toc-section>
```

### Separator with label

```html
<app-toc-section label="Settings">
    <app-toc-item label="Option 1" />
    <app-toc-separator label="Theme" />
    <app-toc-item label="Light Theme" />
    <app-toc-item label="Dark Theme" />
</app-toc-section>
```

## Properties

- `label` (optional): A small text label that appears before the separator line. The label is styled with reduced opacity, uppercase text, and small font size.

## Styling

The separator automatically adapts to the TOC theme using CSS custom properties:

- `--header-text-color` for label and line color
- Opacity is reduced for subtle appearance
- Line stretches to fill available width
- Compatible with all existing TOC themes (C64, Day, Night)
