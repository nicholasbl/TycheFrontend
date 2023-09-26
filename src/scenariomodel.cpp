#include "scenariomodel.h"

#include <QUuid>

ScenarioModel::ScenarioModel(QObject* parent) : StructTableModel(parent) { }


void ScenarioModel::add_placeholder() {
    replace({
        ScenarioRecord {
            .name = "Scenario 1",
            .uuid = QUuid::createUuid().toString(),
            .description =
                "This is the first test scenario, and has a lot going on",
            .image = "https://placehold.co/600x400/000000/FFF.png",

            .metrics = {
                {
                    .name        = "GHG",
                    .description = "This is an alpha desc",
                    .image       = "https://placehold.co/100.png",
                },{
                    .name        = "LCOE",
                    .description = "This is an beta desc",
                    .image       = "https://placehold.co/100.png",
                },{
                    .name        = "Labor",
                    .description = "This is an delta desc",
                    .image       = "https://placehold.co/100.png",
                },{
                    .name        = "WIA",
                    .description = "This is an gamma desc",
                    .image       = "https://placehold.co/100.png",
                },
            },
            
            .categories = {
                {
                    .name        = "BoS (R&D)",
                    .description = "This is an alpha desc",
                    .image       = "https://placehold.co/100.png",
                    .investment  = 350526,
                },{
                    .name        = "Inverter (R&D)",
                    .description = "This is an beta desc",
                    .image       = "https://placehold.co/100.png",
                    .investment  =  650101,
                },{
                    .name        = "Module (R&D)",
                    .description = "This is an delta desc",
                    .image       = "https://placehold.co/100.png",
                    .investment  =  593949,
                },{
                    .name        = "Stand (R&D)",
                    .description = "This is an gamma desc",
                    .image       = "https://placehold.co/100.png",
                    .investment  =  348276,
                },
            }
        },
        ScenarioRecord {
            .name = "Scenario 2",
            .uuid = QUuid::createUuid().toString(),
            .description =
                "This is the second test scenario, and has a lot going on",
            .image = "https://placehold.co/600x400/orange/FFF.png",
                         
             .metrics = {
                 {
                     .name        = "GHG 2",
                     .description = "This is an alpha desc",
                     .image       = "https://placehold.co/100.png",
                 },{
                     .name        = "LCOE 2",
                     .description = "This is an beta desc",
                     .image       = "https://placehold.co/100.png",
                 },{
                     .name        = "Labor 2",
                     .description = "This is an delta desc",
                     .image       = "https://placehold.co/100.png",
                 },{
                     .name        = "WIA 2",
                     .description = "This is an gamma desc",
                     .image       = "https://placehold.co/100.png",
                 },
             },
             
             .categories = {
                 {
                     .name        = "BoS (R&D) 2",
                     .description = "This is an alpha desc",
                     .image       = "https://placehold.co/100.png",
                                 .investment  =   958978,
                 },{
                     .name        = "Inverter (R&D) 2",
                     .description = "This is an beta desc",
                     .image       = "https://placehold.co/100.png",
                                 .investment  =   345379,
                 },{
                     .name        = "Module (R&D) 2",
                     .description = "This is an delta desc",
                     .image       = "https://placehold.co/100.png",
                                 .investment  =  920064,
                 },{
                     .name        = "Stand (R&D) 2",
                     .description = "This is an gamma desc",
                     .image       = "https://placehold.co/100.png",
                                 .investment  =   92774,
                 },
             }
        },
    });

    set_current_scenario(0);
}

int ScenarioModel::current_scenario() const {
    return m_current_scenario;
}

void ScenarioModel::set_current_scenario(int index) {
    if (m_current_scenario == index) return;
    m_current_scenario = index;
    emit current_scenario_changed();
}
