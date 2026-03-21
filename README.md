# watopoly


## Getting started

```
ssh <watiamID>@linux.student.cs.uwaterloo.ca
cd cs246/<repo_location> (e.g. cs246/w26/a5/)
git clone https://git.uwaterloo.ca/<username>/watopoly.git
cd watopoly
```

If there is a permission error when trying to clone the repo try the following inside the ssh server:
```
ssh-keygen -t ed25519 -C "your_email@uwaterloo.ca"
cat ~/.ssh/id_ed25519.pub (copy the entire output)
```
Paste the key into GitLab -> SSH Keys -> Add new key.
Try cloning again, and it should work!

## Daily Workflow

Inside your repository run the following:

```
git pull origin main (First thing to see any new changes)
git add <file> (Include changes made to file)
git commit -m "your message" (Save changes locally with a message)
git push (Submit changes to repo)
```

## Working on new Features

To ensure we always have one version of the game working, we will code new features on feature branches and keep the main
branch untouched until we have tested that the new features works. Once new features are tested (and tests passed), we can
merge the feature branch into the main branch.

Useful commands:

```
git checkout -b <feature/feature_name> (creates a new branch and automatically changes your working directory)
git checkout <branch_name> (switch to a different branch)
```

Merging new feature after code compiles and runs as expected:
```
git checkout main
git merge feature/<your-feature>
git push origin main
```

## Authors and acknowledgment
Thank you all for the hard work :)
