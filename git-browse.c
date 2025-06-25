#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <linux/limits.h>

#include <git2.h>

struct termios old_termios = {0};

const char *editor = NULL;

git_repository *repo        = NULL;
git_object     *revision    = NULL;
git_commit     *commit      = NULL;
git_commit     *parent      = NULL;
git_tree       *commit_tree = NULL;
git_tree       *parent_tree = NULL;
git_diff       *diff        = NULL;

char *input = NULL;
size_t inputsize = 0;

int current_line = -1;
char current_file[PATH_MAX];
char next_file[PATH_MAX];

void cleanup(void);
void die_with_git_error();
int hunk_callback(const git_diff_delta *delta, const git_diff_hunk *hunk, void *payload);
int file_callback(const git_diff_delta *delta, float progress, void *payload);
int line_callback(const git_diff_delta *delta, const git_diff_hunk *hunk,
                  const git_diff_line *line, void *payload);
void prompt_edit(void);

int main(int argc, char **argv)
{
  git_libgit2_init();

  atexit(cleanup);

  editor = getenv("EDITOR");
  if (editor == NULL)
    editor = "vi";

  char repo_path[PATH_MAX];
  if (git_repository_open(&repo, getcwd(repo_path, sizeof(repo_path))) < 0)
    die_with_git_error();

  const char *refname = argc > 1 ? argv[1] : "HEAD";
  if (git_revparse_single(&revision, repo, refname))
    die_with_git_error();

  if (git_commit_lookup(&commit, repo, git_object_id(revision)) < 0)
    die_with_git_error();

  if (git_commit_parent(&parent, commit, 0) < 0)
    die_with_git_error();

  if (git_commit_tree(&commit_tree, commit) < 0)
    die_with_git_error();

  if (git_commit_tree(&parent_tree, parent) < 0)
    die_with_git_error();

  if (git_diff_tree_to_tree(&diff, repo, parent_tree, commit_tree, NULL) < 0)
    die_with_git_error();

  if (git_diff_foreach(diff, file_callback, NULL, hunk_callback, line_callback, NULL) < 0)
    die_with_git_error();

  if (current_line > 0)
    prompt_edit();
}

void die_with_git_error()
{
  const git_error *error = git_error_last();
  fprintf(stderr, "Error: %s\n", error->message);
  exit(EXIT_FAILURE);
}

void prompt_edit(void)
{
  for (;;) {
    printf("\x1b[1;34mOpen editor at the current hunk [y/n/q]? \x1b[m");

    int res = getline(&input, &inputsize, stdin);

    if (res == 0)
      continue;

    if (res < 0) {
      printf("\n");
      exit(EXIT_FAILURE);
    }

    if (strlen(input) > 2)
      continue;

    switch (input[0]) {
    case 'Y':
    case 'y':
      break;

    case 'N':
    case 'n':
      return;
      break;

    case 'Q':
    case 'q':
      exit(EXIT_FAILURE);

    default:
      continue;
    }

    break;
  }

  char cmd_buf[PATH_MAX + 512];
  snprintf(cmd_buf, sizeof(cmd_buf), "%s %s +%d", editor,
           current_file, current_line);

  errno = 0;
  int ret = system(cmd_buf);

  if (ret != 0) {
    if (errno != 0) {
      fprintf(stderr, "Error: failed to run '%s': %s\n", cmd_buf, strerror(errno));
    } else {
      fprintf(stderr, "Error: failed to run '%s': exit status %d\n", cmd_buf, ret);
    }
    exit(EXIT_FAILURE);
  }
}

int hunk_callback(const git_diff_delta *delta, const git_diff_hunk *hunk, void *payload)
{
  (void) delta;
  (void) payload;

  if (current_line > 0)
    prompt_edit();

  printf("\x1b[1;34m%s\x1b[0m\n", hunk->header);

  current_line = hunk->new_start;
  strcpy(current_file, next_file);

  return 0;
}

int file_callback(const git_diff_delta *delta, float progress, void *payload)
{
  (void) progress;
  (void) payload;

  strcpy(next_file, delta->new_file.path);

  return 0;
}

int line_callback(const git_diff_delta *delta, const git_diff_hunk *hunk,
                  const git_diff_line *line, void *payload)
{
  (void) delta;
  (void) hunk;
  (void) payload;

  const char *prefix;

  switch (line->origin) {
  case GIT_DIFF_LINE_ADDITION:
    prefix = "\x1b[32m+";
    break;
  case GIT_DIFF_LINE_DELETION:
    prefix = "\x1b[31m-";
    break;
  default:
    prefix = " ";
    break;
  }

  printf("%s%.*s\x1b[0m", prefix, (int) line->content_len, line->content);
  return 0;
}

void cleanup(void)
{
  if (repo != NULL)
    git_repository_free(repo);
  if (revision != NULL)
    git_object_free(revision);
  if (commit != NULL)
    git_commit_free(commit);
  if (parent != NULL)
    git_commit_free(parent);
  if (commit_tree != NULL)
    git_tree_free(commit_tree);
  if (parent_tree != NULL)
    git_tree_free(parent_tree);
  if (diff != NULL)
    git_diff_free(diff);

  free(input);

  git_libgit2_shutdown();
}
