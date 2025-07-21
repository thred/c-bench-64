export type Theme = "c64" | "c128" | "vic20" | "cbm";

export const themes: Theme[] = ["c64", "c128", "vic20", "cbm"];

export const themeNames: Record<Theme, string> = {
    c64: "C64",
    c128: "C128",
    vic20: "VIC20",
    cbm: "CBM",
};
