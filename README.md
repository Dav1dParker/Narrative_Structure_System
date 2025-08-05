# Narrative Structure System

# English language will be added to project within next update

Система для структурирования нарративного дизайна и интеграции данных в Unreal Engine 5. Состоит из двух независимых компонентов:

* `desktop-app/` — Qt-приложение для нарративных дизайнеров (создание персонажей, локаций, событий, экспорт JSON/XML).  
* `unreal-plugin/` — плагин UE5 для импорта JSON и привязки сущностей к игровым объектам.

```
repo/
├── desktop-app/ # исходники Qt-приложения
├── unreal-plugin/ # исходники и .uplugin
├── docs/ # детальная документация, диаграммы
└── README.md
```

## Сборка и запуск

### Desktop-App
```bash
cd desktop-app
cmake -B build
cmake --build build
./build/NarrativeApp
```
Требуется Qt 6+ и CMake 3.20+.

### Unreal-Plugin
Скопируйте unreal-plugin в папку Plugins/ вашего UE-проекта.

Откройте проект в UE5 и включите NSP в разделе plugins

## Документация
Подробная документация представлена в каталоге docs/.

Видео с демонстрацией работы системы:
https://youtu.be/YNYRDfXT-Jc
