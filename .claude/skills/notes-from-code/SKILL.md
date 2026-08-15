---
name: notes-from-code
description: Convert a practice/example code file into a topic-wise Markdown notes file. Use when the user asks to "generate notes from this file", "make a notes md from my practice programs", "convert this .py/.js/... to markdown notes", "create an interview-style md for this code", or similar. Reads the source file, extracts each topic/concept demonstrated, and writes a companion .md into resources/<mirrored-subfolder>/.
---

# notes-from-code

Turn a single code file (or a small set) into a topic-wise Markdown notes file structured for study / interview review. The output follows the same style as `resources/Basics/*.md` and `resources/DataStructures/*.md` in this user's Python project, but the skill is language-agnostic.

## When to invoke

Trigger on requests like:
- "Create an md file for this code"
- "Convert this practice file into notes"
- "Generate topic-wise markdown from this file"
- "Make an interview-ready md from these examples"
- The user hands over a code file (any language) and asks for study notes

## Inputs

- **Source file(s)** — one or more code files under the project (e.g. `Basics/9_new_topic.py`).
- Optional: **topic title** (defaults to a title inferred from the file name, e.g. `9_new_topic.py` → "New Topic").
- Optional: **output folder** (defaults to `resources/<mirror-of-source-parent>/`).

If the user hasn't specified which file, ask which one — do not guess.

## Steps

1. **Read the source file end-to-end.** Do not skim. Every commented block, function, and example should be represented in the notes.

2. **Group the code into topics.** Use the code's own comments / structure as section boundaries. If a topic is split across the file, merge it in the notes.

3. **Pick a filename.**
   - Mirror the numeric prefix of the source file if present (e.g. `9_dict.py` → `09_Dict.md`).
   - Otherwise use `NN_TopicName.md` where `NN` is the next unused number in the target folder.
   - Place under `resources/<same-subfolder-as-source>/`. Create the folder if missing.

4. **Write the Markdown** using this template:

   ```markdown
   # <Topic Title>

   Source: `<relative/path/to/source.py>`

   ## What is <topic>?
   <2–3 sentence high-level definition suitable for an interview answer.
    Cover the concept, one key property, and one gotcha or notable trait.>

   ## <Concept 1>
   <Short prose intro, then a fenced code block reproducing / cleaning up
    the relevant snippet from the source. Add a one-line comment on the
    interesting behavior when it's non-obvious.>

   ## <Concept 2>
   ...

   ## Common methods / operators / patterns  (optional — include when applicable)
   <Table or bullet list of the most-used APIs for the topic.>

   ## Gotchas  (optional)
   <Bulleted list of pitfalls. Only include when there are real ones.>
   ```

   Style rules:
   - Match the tone of existing files in `resources/` — concise, code-heavy, minimal fluff.
   - Preserve the user's original examples verbatim where they illustrate the concept well; clean up only obvious typos.
   - If the source uses a concept without explaining it (e.g. `enumerate`, `LEGB`, `dunder methods`), add a brief note — the goal is a self-contained study reference.
   - Use tables for method/operator/keyword catalogs.
   - Add fenced code blocks with the correct language tag.
   - Do **not** invent APIs or examples that aren't in the source, unless they're a directly-adjacent, widely-known concept worth including for interview prep — and mark those additions naturally, not with disclaimers.

5. **Update the resources index (if one exists).** If `resources/README.md` has a topic table, append a row for the new file. Keep the existing formatting.

6. **Review with the `notes-reviewer` agent.** After writing the file (and updating the index), invoke the `notes-reviewer` subagent (defined in `.claude/agents/notes-reviewer.md`) on the newly created / modified `.md` file(s). Pass the source file path too so it can cross-check.
   - If the reviewer returns Critical or Important findings (confidence ≥ 80), apply the fixes directly to the file. Do not argue with high-confidence findings.
   - If it returns only Nits, apply the unambiguous ones (typos, formatting drift) and skip anything stylistic.
   - If it reports no issues, move on.
   - Re-run the reviewer once after applying fixes to confirm the file is clean. Do not loop more than twice — if issues persist after one fix pass, surface them to the user.
   - Fallback: if the `notes-reviewer` agent is not yet loaded in the current session (e.g. just created), invoke `general-purpose` and paste in the review criteria from `.claude/agents/notes-reviewer.md`.

7. **Report** the file(s) written, any notable additions beyond the source, and a short summary of what the reviewer found and what was fixed (e.g. "Reviewer flagged wrong precedence tier — corrected.").

## Language-agnostic behavior

Default examples in this project are Python, but the skill works for any language. Detect from the file extension and:
- Use the matching fenced-block language tag (`python`, `js`, `ts`, `go`, `java`, ...).
- Adjust the "What is …?" definition for the target language's idioms (e.g. "variable" means something different in JS vs Rust).

## Existing conventions to follow

Look at these for reference formatting before writing new files:
- `resources/Basics/02_Variables.md`
- `resources/Basics/08_Functions.md`
- `resources/DataStructures/09_Strings.md`

Mirror their section ordering (short definition → concrete examples → methods table → gotchas), heading depth, and tone.

## Do NOT

- Do not overwrite an existing `.md` without confirming.
- Do not add Claude/AI attribution anywhere in the file.
- Do not create a new top-level folder — always place notes under `resources/<mirror-of-source-parent>/`.
- Do not write speculative "advanced" sections that aren't in the source **and** aren't standard interview material.
