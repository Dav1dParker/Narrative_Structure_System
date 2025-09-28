# Narrative Structure System

A system for structuring narrative design and integrating data into Unreal Engine 5. It consists of two independent components:

* `desktop-app/` — Qt application for narrative designers (create characters, locations, events, export JSON/XML).
* `unreal-plugin/` — UE5 plugin for importing JSON and linking entities to in-engine objects.

```
repo/
├── desktop-app/    # Qt application source code
├── unreal-plugin/  # plugin source code and .uplugin
├── docs/           # detailed documentation, diagrams
└── README.md
```

## Build & Run

### Desktop App

```bash
cd desktop-app
cmake -B build
cmake --build build
./build/NarrativeApp
```

Requires Qt 6+ and CMake 3.20+.

### Unreal Plugin

Copy `unreal-plugin` into the `Plugins/` folder of your UE project.

Open the project in UE5 and enable **NSP** in the Plugins section.

## Documentation

Detailed documentation is available in the `docs/` directory.

Demo video of the system in action:
[https://youtu.be/YNYRDfXT-Jc](https://youtu.be/YNYRDfXT-Jc)


---

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
