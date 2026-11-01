---
name: cpp-topic-notes
description: "Create C++ learning notes from a topic name using Prompts.txt. Use for requests to teach or write notes on a C++, OOP, or low-level design topic, including a bare topic name in this user's learning context. Do not treat incidental names or questions about existing code as requests to create a new note."
---

# C++ Topic Notes

Turn the supplied topic into a complete Markdown learning note. A topic name such as `Composition` or `RAII` is enough when the user is requesting learning content; do the work rather than asking them to repeat the teaching instructions.

## Read the teaching rules

Find and read `Prompts.txt` in the current repository root or the relevant workspace directory. Accept filename capitalization differences. Use that file as the current source of teaching requirements.

If the workspace has no prompt file, read the bundled [references/Prompts.txt](references/Prompts.txt). This is a snapshot of the user's preferences, not a replacement for a newer workspace copy. Current user instructions take precedence over either file.

Substitute the supplied topic for `<TOPIC>` and follow the chosen prompt's requirements. Read related notes before writing so terminology, example names, prices, operations, and chapter links stay consistent. Keep the museum-ticket domain for this OOP series; use a fresh simple domain only for a genuinely separate series as the prompt directs.

## Create the note

- Infer the C++ meaning of an unambiguous topic. If a name has materially different meanings and context does not resolve them, clarify the intended topic.
- Research authoritative sources using available browsing tools and cite the pages used. If browsing is unavailable, say so and distinguish unverified further-reading links from sources actually checked.
- Follow the prompt's concise teaching structure. Explain intent, design choices, and non-obvious C++ rules. Avoid narrated execution steps, repeated syntax tutorials, and redundant recaps.
- Label complete programs, partial snippets, and intentional compile errors. Keep code, Mermaid diagrams, and recall questions consistent. Reuse relevant existing concept images when they help.
- Use the prompt's output directory, normally `resources/OOPS/`, unless the user chooses another destination. For a new topic, inspect existing filenames and use the next unused numeric prefix, such as `06_Composition.md`. For a topic already covered, update its existing note instead of creating a duplicate.

## Check and finish

Check headings, contents anchors, relative links, image paths, and code fences. Compile complete C++ examples when a compiler is available, using the stated language version; do not treat partial snippets or intentional errors as complete programs.

Report the saved note path and any material verification limitation briefly. Creating learning content does not authorize a commit or push; publish only when requested.
