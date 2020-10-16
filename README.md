# Matt_daemon
Un daemon, qui ne se traduit pas par démon, désigne un type de programme informatique, un processus ou un ensemble de processus qui s’exécute en arrière-plan plutôt que sous le contrôle direct d’un utilisateur. Le terme daemon semble avoir été introduit, en 1963, par les concepteurs de CTSS du MIT, en réponse au « dragon », terme employé par les concepteurs d’ITS. Le rétro- acronyme Disk And Execution MONitor (moniteur de disque et d’exécution) a été inventé pour justifier le terme daemon après sa démocratisation. Les daemons sont souvent démarrés lors du chargement du système d’exploitation, et servent en général à répondre à des requêtes du réseau, à l’activité du matériel ou à d’autres programmes en exécutant certaines tâches. Sous Microsoft Windows, ces fonctions sont exécutées par des programmes appelés « services ».

- [x] Daemon acceptant 3 connexions simultané
- [x] Client communiquant avec le Daemon sur un port donnée
- [x] Uname du systeme sur lequelle le Daemon est lancé
- [x] Lancement d'un bind shell
- [x] Log de toutes les touches clavier envoyé au daemon
- [x] Chiffremment du tunnel de communication
- [x] Authentification
- [ ] Envoi du fichier de log par mail

Compile
```bash
make
```
Launch Daemon
```bash
sudo ./Matt_daemon
```

Launch Client
```bash
./BEN_AFK <HOST> <PORT>
```
