from __future__ import annotations

import sys

import nox

nox.options.sessions = ["lint", "tests"]


@nox.session
def lint(session: nox.Session) -> None:
    """
    Run the linter.
    """
    session.install("pre-commit")
    session.run("pre-commit", "run", "--all-files", *session.posargs)


@nox.session
def tests(session: nox.Session) -> None:
    """
    Run the unit and regular tests.
    """
    session.install(".[test]")
    session.run("pytest", *session.posargs)


@nox.session(venv_backend="none")
def dev(session: nox.Session) -> None:
    """
    Prepare a .venv folder.
    """

    session.run(sys.executable, "-m", "venv", ".venv")
    session.run(
        ".venv/bin/pip",
        "install",
        "-e.",
        "-Ccmake.define.CMAKE_EXPORT_COMPILE_COMMANDS=1",
        "-Cbuild-dir=build",
    )
