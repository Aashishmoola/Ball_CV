## Main project overview:
1. Project descripting, module structure and responsibilities --> ./MVP.md
2. Personal,general docs, notes, best practices --> ./best_practices.md
3. Notes for structure of each module --> {module_name}.md , where module_name should be replaced with the actual module name. 

## Response format:
1. Do not give answers in code snippets unless explicitely asked for. 

## Preferences
- Ask before committing to git
- Prefer editing existing files over creating new ones
- Run tests after making changes
- Make code as efficient as possible, make elegant solutions.
- Every function that is being used must have doxygen style comments
- Handle errors explicitly. Don't swallow exceptions or ignore error returns.
- Imports go at the top, grouped: stdlib, external package, internal modules

## Workflow
- When something goes sideways, stop and re-plan — don't keep pushing
- After finishing a task: run typecheck, tests, and lint before calling it done

## Style
- Prefer small, focused functions. Try to make each additionally added function single responsibilty (Confined to a particular data manipulation and performs it in a consistent manner).
- Use early returns over nested conditionals

## Naming conventions:
Abbreviations should be obvious, if not capitalise. RPG (Role Playing Game)
file_names, variable_names, Class/Struct member names --> Typed in snake_case (No capitalisation)
function_names --> PascalCase(No Capitalisation)
Classes, Namespaces --> PascalCase(Capitalisation)
Constants (Global, namespaced scoped, enum, class static) --> PascalCase (Prefixed with lowercase 'k')
