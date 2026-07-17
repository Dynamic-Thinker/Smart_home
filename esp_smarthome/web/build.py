from pathlib import Path

html = Path("index.html").read_text(encoding="utf-8")
css = Path("style.css").read_text(encoding="utf-8")
js = Path("app.js").read_text(encoding="utf-8")

html = html.replace(
    '<link rel="stylesheet" href="style.css">',
    f"<style>\n{css}\n</style>",
    1
)

html = html.replace(
    '<script src="app.js"></script>',
    f"<script>\n{js}\n</script>",
    1
)

output = f'''#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <Arduino.h>

const char INDEX_HTML[] PROGMEM = R"rawliteral(
{html}
)rawliteral";

#endif
'''

Path("../webpage.h").write_text(output, encoding="utf-8")

print("../webpage.h generated successfully.")