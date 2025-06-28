# 🔐 KeyAuth C++ - HWID Based Authentication Library

Une bibliothèque légère et rapide d'authentification en C++, utilisant le **HWID (Hardware ID)** comme clé unique d'identification au lieu du SID. Pensée pour des applications natives nécessitant une protection efficace contre l'usurpation d'identité utilisateur.

## 🚀 Fonctionnalités

- Authentification basée sur le **Hardware ID**
- Génération et vérification de clés d'API
- Aucune dépendance externe lourde
- Compatible Windows (support Linux partiel en développement)

## ⚙️ Pourquoi HWID et non SID ?

Le **SID** peut être facilement manipulé ou réinitialisé par des outils d'administration système ou via une VM. En revanche, le **HWID** (ex: UUID de la carte mère, disque dur, etc.) fournit une empreinte plus stable et difficile à falsifier pour identifier une machine.

## 🛠️ Installation

1. Clone le repo :

```bash
git clone https://github.com/metixud/keyauth-hwid-lib.git
cd keyauth-hwid-lib
