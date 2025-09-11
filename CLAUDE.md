# Claude Code Configuration

## ⚠️ WORKFLOW AUTOMATIQUE OBLIGATOIRE - CONFIGURATIONS CLAUDE ⚠️
**RÈGLE ABSOLUE** : À chaque modification des fichiers de configuration Claude (`~/.claude/`) :
1. **IMMÉDIATEMENT** après toute modification : `git add . && git commit && git push`
2. **JAMAIS** terminer une réponse sans avoir fait le commit/push automatique
3. **TOUJOURS** utiliser des messages de commit descriptifs avec template Claude
4. **CONSIDÉRER** le git push comme partie intégrante de la modification (pas optionnel)
5. **NE PAS ATTENDRE** de commande utilisateur - le faire automatiquement

**RAPPEL** : Si tu modifies un fichier dans `~/.claude/`, tu DOIS finir par git add/commit/push dans la même réponse.

## Test Auto-push
Test effectué le 2025-09-12 pour vérifier le fonctionnement du workflow automatique.