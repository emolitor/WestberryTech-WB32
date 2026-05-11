"""Claude vision-based PDF page extraction (fallback only).

This module exists for future use when a recipe page explicitly sets
``use_vision: true``. With current vendor PDFs (LaTeX/Foxit text-based) the
programmatic pdfplumber extractor handles everything; this path is here as a
safety net for future scanned material.
"""

from __future__ import annotations

import base64
import hashlib
import os
from pathlib import Path
from typing import Any

try:
    import anthropic
except ImportError:  # pragma: no cover
    anthropic = None     # type: ignore[assignment]


DEFAULT_MODEL = "claude-sonnet-4-6"


def call_vision_extractor(
    page_png: Path,
    prompt: str,
    *,
    model: str = DEFAULT_MODEL,
    cache_dir: Path | None = None,
) -> dict[str, Any] | None:
    """Send a rendered page PNG to Claude with the given prompt.

    Returns the parsed JSON object the model emits, or None if no API key is
    set. Caches results keyed on (png-sha256, prompt-sha256, model).
    """
    api_key = os.environ.get("ANTHROPIC_API_KEY")
    if not api_key or anthropic is None:
        return None

    png_hash = _hash_file(page_png)
    prompt_hash = hashlib.sha256(prompt.encode("utf-8")).hexdigest()[:16]
    cache_path = (cache_dir / f"{png_hash}.{prompt_hash}.{model}.json") if cache_dir else None
    if cache_path is not None and cache_path.exists():
        import json
        return json.loads(cache_path.read_text("utf-8"))

    img_b64 = base64.b64encode(page_png.read_bytes()).decode("ascii")
    client = anthropic.Anthropic(api_key=api_key)
    response = client.messages.create(
        model=model,
        max_tokens=4096,
        messages=[
            {
                "role": "user",
                "content": [
                    {"type": "image", "source": {"type": "base64", "media_type": "image/png", "data": img_b64}},
                    {"type": "text", "text": prompt + "\n\nReply with JSON only, no prose."},
                ],
            }
        ],
    )
    text = "".join(b.text for b in response.content if hasattr(b, "text"))
    try:
        import json
        result = json.loads(text)
    except json.JSONDecodeError:
        result = {"raw": text}

    if cache_path is not None:
        cache_path.parent.mkdir(parents=True, exist_ok=True)
        cache_path.write_text(__import__("json").dumps(result, indent=2))
    return result


def _hash_file(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as fh:
        for chunk in iter(lambda: fh.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()[:32]
