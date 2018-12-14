/****************************************************************************
* GRAVITATE Desktop Client                                                  *
*                                                                           *
* Consiglio Nazionale delle Ricerche                                        *
* Istituto di Matematica Applicata e Tecnologie Informatiche                *
* Sezione di Genova                                                         *
* IMATI-GE / CNR                                                            *
*                                                                           *
* Authors: Andrea Repetto                                                   *
* Copyright(C) 2018: IMATI-GE / CNR                                         *
* All rights reserved.                                                      *
*                                                                           *
* This program is dual-licensed as follows:                                 *
*                                                                           *
* (1) You may use GRAVITATE Desktop Client as free software; you can        *
* redistribute it and/or modify it under the terms of the GNU General       *
* Public License as published by the Free Software Foundation; either       *
* version 3 of the License, or (at your option) any later  version.         *
* In this case the program is distributed in the hope that it will be       *
* useful, but WITHOUT ANY WARRANTY; without even the implied warranty of    *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
* (2) You may use GRAVITATE Desktop Client as part of a commercial          *
* software. In this case a proper agreement must be reached with the        *
* Authors and with IMATI-GE/CNR based on a proper licensing contract.       *
*                                                                           *
****************************************************************************/
/**
 * NOTE: This file is auto-generated from the QUDT specification with a
 * script contained in the qudt directory.
 * Please do not modify this file directly, or undefined behaviour may happen.
 */
#ifndef QUDT_UNITS_H
#define QUDT_UNITS_H

#include <string>

namespace qudt {

enum Unit {
    ACRE,
    ACRE_FOOT,
    ANGSTROM,
    ARE,
    BARN,
    BOARD_FOOT,
    BUSHEL_UK,
    BUSHEL_US,
    CENTIMETER,
    CHAIN,
    CORD,
    CUBIC_CENTIMETER,
    CUBIC_FOOT,
    CUBIC_FOOT_PER_MINUTE,
    CUBIC_FOOT_PER_SECOND,
    CUBIC_INCH,
    CUBIC_INCH_PER_MINUTE,
    CUBIC_KILOMETER_PER_SECOND_SQUARED,
    CUBIC_METER,
    CUBIC_METER_PER_HOUR,
    CUBIC_METER_PER_KILOGRAM,
    CUBIC_METER_PER_SECOND,
    CUBIC_METER_PER_SECOND_SQUARED,
    CUBIC_MILE,
    CUBIC_MILLIMETER,
    CUBIC_YARD,
    CUBIC_YARD_PER_MINUTE,
    DECIMETER,
    DRY_GALLON_US,
    FATHOM,
    FEMTOMETER,
    FERMI,
    FOOT,
    FURLONG,
    HECTARE,
    IMPERIAL_GALLON,
    IMPERIAL_PINT,
    INCH,
    INTERNATIONAL_MILE,
    KILOMETER,
    LIGHT_YEAR,
    LITER,
    METER,
    MICROINCH,
    MICROMETER,
    MILE_US_STATUTE,
    MILLIMETER,
    NAUTICAL_MILE,
    PARSEC,
    PLANCK_AREA,
    PLANCK_LENGTH,
    PLANCK_VOLUME,
    POINT,
    REGISTER_TON,
    ROD,
    SQUARE_CENTIMETER,
    SQUARE_FOOT,
    SQUARE_INCH,
    SQUARE_METER,
    SQUARE_MILE,
    SQUARE_YARD,
    STERE,
    TABLESPOON,
    TEASPOON,
    US_DRY_PINT,
    US_DRY_QUART,
    US_GALLON,
    US_GALLON_PER_DAY,
    US_GALLON_PER_MINUTE,
    US_LIQUID_CUP,
    US_LIQUID_OUNCE,
    US_LIQUID_PINT,
    US_LIQUID_QUART,
    US_PECK,
    US_SURVEY_FOOT,
    YARD,
    UNKNOWN
};

struct UnitInfo {
    Unit unit;
    std::string uri;
    std::string label;
    std::string abbreviation;
    double conversionMultiplier;
};

inline UnitInfo getInfo(Unit unit) {
    switch(unit)
    {
    case ACRE: return { ACRE, "http://qudt.org/vocab/unit/AC", "Acre", "acre", 4046.8564224 };
    case ACRE_FOOT: return { ACRE_FOOT, "http://qudt.org/vocab/unit/AC-FT", "Acre Foot", "ac-ft", 1233.4818375475202 };
    case ANGSTROM: return { ANGSTROM, "http://qudt.org/vocab/unit/ANGSTROM", "Angstrom", "angstrom", 1e-10 };
    case ARE: return { ARE, "http://qudt.org/vocab/unit/ARE", "are", "are", 100.0 };
    case BARN: return { BARN, "http://qudt.org/vocab/unit/B", "Barn", "b", 1e-28 };
    case BOARD_FOOT: return { BOARD_FOOT, "http://qudt.org/vocab/unit/FBM", "Board Foot", "BF", 0.00236 };
    case BUSHEL_UK: return { BUSHEL_UK, "http://qudt.org/vocab/unit/BUI", "bushel (UK)", "bui", 0.03636872 };
    case BUSHEL_US: return { BUSHEL_US, "http://qudt.org/vocab/unit/BUA", "bushel (US)", "bua", 0.03523907 };
    case CENTIMETER: return { CENTIMETER, "http://qudt.org/vocab/unit/CM", "Centimeter", "cm", 0.01 };
    case CHAIN: return { CHAIN, "http://qudt.org/vocab/unit/CH", "chain", "ch", 20.1168 };
    case CORD: return { CORD, "http://qudt.org/vocab/unit/CORD", "Cord", "cord", 3.62 };
    case CUBIC_CENTIMETER: return { CUBIC_CENTIMETER, "http://qudt.org/vocab/unit/CM3", "cubic centimeter", "cubic-cm", 1e-06 };
    case CUBIC_FOOT: return { CUBIC_FOOT, "http://qudt.org/vocab/unit/FT3", "Cubic Foot", "ft^{3}", 0.028316846592 };
    case CUBIC_FOOT_PER_MINUTE: return { CUBIC_FOOT_PER_MINUTE, "http://qudt.org/vocab/unit/FT3-PER-MIN", "Cubic Foot per Minute", "ft^{3}/min", 0.0004719474432000001 };
    case CUBIC_FOOT_PER_SECOND: return { CUBIC_FOOT_PER_SECOND, "http://qudt.org/vocab/unit/FT3-PER-SEC", "Cubic Foot per Second", "ft^{3}/s", 0.028316846592000004 };
    case CUBIC_INCH: return { CUBIC_INCH, "http://qudt.org/vocab/unit/IN3", "Cubic Inch", "in^{3}", 1.6387064e-05 };
    case CUBIC_INCH_PER_MINUTE: return { CUBIC_INCH_PER_MINUTE, "http://qudt.org/vocab/unit/IN3-PER-MIN", "Cubic Inch per Minute", "in^{3}/min", 2.7311773333333333e-07 };
    case CUBIC_KILOMETER_PER_SECOND_SQUARED: return { CUBIC_KILOMETER_PER_SECOND_SQUARED, "http://qudt.org/vocab/unit/KM3-PER-SEC2", "Cubic Kilometer per Second Squared", "km^3/s^2", 1000000000.0 };
    case CUBIC_METER: return { CUBIC_METER, "http://qudt.org/vocab/unit/M3", "Cubic Meter", "m^{3}", 1.0 };
    case CUBIC_METER_PER_HOUR: return { CUBIC_METER_PER_HOUR, "http://qudt.org/vocab/unit/M3-PER-HR", "Cubic Meter per Hour", "m^{3}/h", 0.0002777777777777778 };
    case CUBIC_METER_PER_KILOGRAM: return { CUBIC_METER_PER_KILOGRAM, "http://qudt.org/vocab/unit/M3-PER-KG", "Cubic Meter per Kilogram", "m^{3}/kg", 1.0 };
    case CUBIC_METER_PER_SECOND: return { CUBIC_METER_PER_SECOND, "http://qudt.org/vocab/unit/M3-PER-SEC", "Cubic Meter per Second", "m^{3}/s", 1.0 };
    case CUBIC_METER_PER_SECOND_SQUARED: return { CUBIC_METER_PER_SECOND_SQUARED, "http://qudt.org/vocab/unit/M3-PER-SEC2", "Cubic Meter per Second Squared", "m^3/s^2", 1.0 };
    case CUBIC_MILE: return { CUBIC_MILE, "http://qudt.org/vocab/unit/MI3", "Cubic Mile", "mi^{3}", 4168181830.0 };
    case CUBIC_MILLIMETER: return { CUBIC_MILLIMETER, "http://qudt.org/vocab/unit/MilliM3", "Cubic Millimeter", "mm^{3}", 1e-09 };
    case CUBIC_YARD: return { CUBIC_YARD, "http://qudt.org/vocab/unit/YD3", "Cubic Yard", "yd^{3}", 0.764554857984 };
    case CUBIC_YARD_PER_MINUTE: return { CUBIC_YARD_PER_MINUTE, "http://qudt.org/vocab/unit/YD3-PER-MIN", "Cubic Yard per Minute", "yd^{3}/min", 0.0127425809664 };
    case DECIMETER: return { DECIMETER, "http://qudt.org/vocab/unit/DeciM", "Decimeter", "decimeter", 0.1 };
    case DRY_GALLON_US: return { DRY_GALLON_US, "http://qudt.org/vocab/unit/US_DRY_GAL", "Dry Gallon US", "dry_gal", 0.0440488377 };
    case FATHOM: return { FATHOM, "http://qudt.org/vocab/unit/FATH", "Fathom", "fath", 1.8288 };
    case FEMTOMETER: return { FEMTOMETER, "http://qudt.org/vocab/unit/FemtoM", "Femtometer", "fm", 1e-15 };
    case FERMI: return { FERMI, "http://qudt.org/vocab/unit/FM", "fermi", "fm", 1e-15 };
    case FOOT: return { FOOT, "http://qudt.org/vocab/unit/FT", "Foot", "ft", 0.3048 };
    case FURLONG: return { FURLONG, "http://qudt.org/vocab/unit/FUR", "Furlong", "fur", 201.168 };
    case HECTARE: return { HECTARE, "http://qudt.org/vocab/unit/HA", "Hectare", "ha", 10000.0 };
    case IMPERIAL_GALLON: return { IMPERIAL_GALLON, "http://qudt.org/vocab/unit/GAL_IMP", "Imperial Gallon", "galIMP", 0.00454609 };
    case IMPERIAL_PINT: return { IMPERIAL_PINT, "http://qudt.org/vocab/unit/PINT", "Imperial Pint", "pint", 0.0056826125 };
    case INCH: return { INCH, "http://qudt.org/vocab/unit/IN", "Inch", "in", 0.0254 };
    case INTERNATIONAL_MILE: return { INTERNATIONAL_MILE, "http://qudt.org/vocab/unit/MI", "International Mile", "mi", 1609.344 };
    case KILOMETER: return { KILOMETER, "http://qudt.org/vocab/unit/KM", "Kilometer", "km", 1000.0 };
    case LIGHT_YEAR: return { LIGHT_YEAR, "http://qudt.org/vocab/unit/LY", "Light Year", "ly", 9460730472580800.0 };
    case LITER: return { LITER, "http://qudt.org/vocab/unit/L", "Liter", "L", 0.001 };
    case METER: return { METER, "http://qudt.org/vocab/unit/M", "Meter", "m", 1.0 };
    case MICROINCH: return { MICROINCH, "http://qudt.org/vocab/unit/MicroIN", "Microinch", "microinch", 2.54e-08 };
    case MICROMETER: return { MICROMETER, "http://qudt.org/vocab/unit/MicroM", "Micrometer", "microm", 1e-06 };
    case MILE_US_STATUTE: return { MILE_US_STATUTE, "http://qudt.org/vocab/unit/MI_US", "Mile US Statute", "miUS", 1609.347 };
    case MILLIMETER: return { MILLIMETER, "http://qudt.org/vocab/unit/MilliM", "Millimeter", "mm", 0.001 };
    case NAUTICAL_MILE: return { NAUTICAL_MILE, "http://qudt.org/vocab/unit/NMI", "Nautical Mile", "nmi", 1852.0 };
    case PARSEC: return { PARSEC, "http://qudt.org/vocab/unit/PARSEC", "Parsec", "parsec", 3.085678e+16 };
    case PLANCK_AREA: return { PLANCK_AREA, "http://qudt.org/vocab/unit/M2_P", "Planck Area", "planckarea", 2.61223e-70 };
    case PLANCK_LENGTH: return { PLANCK_LENGTH, "http://qudt.org/vocab/unit/L_P", "Planck Length", "plancklength", 1.616252e-35 };
    case PLANCK_VOLUME: return { PLANCK_VOLUME, "http://qudt.org/vocab/unit/PlanckVolume", "Planck Volume", "planckvolume", 4.22419e-105 };
    case POINT: return { POINT, "http://qudt.org/vocab/unit/PT", "Point", "pt", 2.54e-05 };
    case REGISTER_TON: return { REGISTER_TON, "http://qudt.org/vocab/unit/RT", "Register Ton", "RT", 2.8316846592 };
    case ROD: return { ROD, "http://qudt.org/vocab/unit/ROD", "Rod", "rd", 5.02921 };
    case SQUARE_CENTIMETER: return { SQUARE_CENTIMETER, "http://qudt.org/vocab/unit/CM2", "Square Centimeter", "sqcm", 0.0001 };
    case SQUARE_FOOT: return { SQUARE_FOOT, "http://qudt.org/vocab/unit/FT2", "Square Foot", "ft^{2}", 0.09290304 };
    case SQUARE_INCH: return { SQUARE_INCH, "http://qudt.org/vocab/unit/IN2", "Square Inch", "in^{2}", 0.00064516 };
    case SQUARE_METER: return { SQUARE_METER, "http://qudt.org/vocab/unit/M2", "Square Meter", "sq-m", 1.0 };
    case SQUARE_MILE: return { SQUARE_MILE, "http://qudt.org/vocab/unit/MI2", "Square Mile", "square-mile", 2589988.11 };
    case SQUARE_YARD: return { SQUARE_YARD, "http://qudt.org/vocab/unit/YD2", "Square Yard", "sqyd", 0.83612736 };
    case STERE: return { STERE, "http://qudt.org/vocab/unit/ST", "Stere", "st", 1.0 };
    case TABLESPOON: return { TABLESPOON, "http://qudt.org/vocab/unit/TBSP", "Tablespoon", "tbsp", 0.000147867656 };
    case TEASPOON: return { TEASPOON, "http://qudt.org/vocab/unit/TSP", "Teaspoon", "tsp", 4.92892187e-05 };
    case US_DRY_PINT: return { US_DRY_PINT, "http://qudt.org/vocab/unit/US_DRY_PINT", "US Dry Pint", "dry_pt", 0.00550610471 };
    case US_DRY_QUART: return { US_DRY_QUART, "http://qudt.org/vocab/unit/US_DRY_QT", "US Dry Quart", "dry_qt", 0.00110122094 };
    case US_GALLON: return { US_GALLON, "http://qudt.org/vocab/unit/GAL_US", "US Gallon", "gallonus", 0.003785412 };
    case US_GALLON_PER_DAY: return { US_GALLON_PER_DAY, "http://qudt.org/vocab/unit/GAL_US-PER-DAY", "US Gallon per Day", "gal/d", 4.38126389e-08 };
    case US_GALLON_PER_MINUTE: return { US_GALLON_PER_MINUTE, "http://qudt.org/vocab/unit/GAL_US-PER-MIN", "US Gallon per Minute", "gal/min", 6.30902e-05 };
    case US_LIQUID_CUP: return { US_LIQUID_CUP, "http://qudt.org/vocab/unit/CUP", "US Liquid Cup", "cup", 0.0023658825 };
    case US_LIQUID_OUNCE: return { US_LIQUID_OUNCE, "http://qudt.org/vocab/unit/OZ_US", "US Liquid Ounce", "oz", 2.95735296e-05 };
    case US_LIQUID_PINT: return { US_LIQUID_PINT, "http://qudt.org/vocab/unit/PT_US", "US Liquid Pint", "pt", 0.004731765 };
    case US_LIQUID_QUART: return { US_LIQUID_QUART, "http://qudt.org/vocab/unit/QT_US", "US Liquid Quart", "qt", 0.00946353 };
    case US_PECK: return { US_PECK, "http://qudt.org/vocab/unit/PK", "US Peck", "pk", 0.00880976754 };
    case US_SURVEY_FOOT: return { US_SURVEY_FOOT, "http://qudt.org/vocab/unit/FT_US", "US Survey Foot", "ftUS", 0.3048006 };
    case YARD: return { YARD, "http://qudt.org/vocab/unit/YD", "Yard", "yd", 0.9144 };
    default: return { UNKNOWN, "", "Unknown", "??", 0.0 };
    }
}

inline Unit fromUri(std::string uri) {
    if(uri == "http://qudt.org/vocab/unit/AC") return ACRE;
    if(uri == "http://qudt.org/vocab/unit/AC-FT") return ACRE_FOOT;
    if(uri == "http://qudt.org/vocab/unit/ANGSTROM") return ANGSTROM;
    if(uri == "http://qudt.org/vocab/unit/ARE") return ARE;
    if(uri == "http://qudt.org/vocab/unit/B") return BARN;
    if(uri == "http://qudt.org/vocab/unit/BUA") return BUSHEL_US;
    if(uri == "http://qudt.org/vocab/unit/BUI") return BUSHEL_UK;
    if(uri == "http://qudt.org/vocab/unit/CH") return CHAIN;
    if(uri == "http://qudt.org/vocab/unit/CM") return CENTIMETER;
    if(uri == "http://qudt.org/vocab/unit/CM2") return SQUARE_CENTIMETER;
    if(uri == "http://qudt.org/vocab/unit/CM3") return CUBIC_CENTIMETER;
    if(uri == "http://qudt.org/vocab/unit/CORD") return CORD;
    if(uri == "http://qudt.org/vocab/unit/CUP") return US_LIQUID_CUP;
    if(uri == "http://qudt.org/vocab/unit/DeciM") return DECIMETER;
    if(uri == "http://qudt.org/vocab/unit/FATH") return FATHOM;
    if(uri == "http://qudt.org/vocab/unit/FBM") return BOARD_FOOT;
    if(uri == "http://qudt.org/vocab/unit/FM") return FERMI;
    if(uri == "http://qudt.org/vocab/unit/FT") return FOOT;
    if(uri == "http://qudt.org/vocab/unit/FT2") return SQUARE_FOOT;
    if(uri == "http://qudt.org/vocab/unit/FT3") return CUBIC_FOOT;
    if(uri == "http://qudt.org/vocab/unit/FT3-PER-MIN") return CUBIC_FOOT_PER_MINUTE;
    if(uri == "http://qudt.org/vocab/unit/FT3-PER-SEC") return CUBIC_FOOT_PER_SECOND;
    if(uri == "http://qudt.org/vocab/unit/FT_US") return US_SURVEY_FOOT;
    if(uri == "http://qudt.org/vocab/unit/FUR") return FURLONG;
    if(uri == "http://qudt.org/vocab/unit/FemtoM") return FEMTOMETER;
    if(uri == "http://qudt.org/vocab/unit/GAL_IMP") return IMPERIAL_GALLON;
    if(uri == "http://qudt.org/vocab/unit/GAL_US") return US_GALLON;
    if(uri == "http://qudt.org/vocab/unit/GAL_US-PER-DAY") return US_GALLON_PER_DAY;
    if(uri == "http://qudt.org/vocab/unit/GAL_US-PER-MIN") return US_GALLON_PER_MINUTE;
    if(uri == "http://qudt.org/vocab/unit/HA") return HECTARE;
    if(uri == "http://qudt.org/vocab/unit/IN") return INCH;
    if(uri == "http://qudt.org/vocab/unit/IN2") return SQUARE_INCH;
    if(uri == "http://qudt.org/vocab/unit/IN3") return CUBIC_INCH;
    if(uri == "http://qudt.org/vocab/unit/IN3-PER-MIN") return CUBIC_INCH_PER_MINUTE;
    if(uri == "http://qudt.org/vocab/unit/KM") return KILOMETER;
    if(uri == "http://qudt.org/vocab/unit/KM3-PER-SEC2") return CUBIC_KILOMETER_PER_SECOND_SQUARED;
    if(uri == "http://qudt.org/vocab/unit/L") return LITER;
    if(uri == "http://qudt.org/vocab/unit/LY") return LIGHT_YEAR;
    if(uri == "http://qudt.org/vocab/unit/L_P") return PLANCK_LENGTH;
    if(uri == "http://qudt.org/vocab/unit/M") return METER;
    if(uri == "http://qudt.org/vocab/unit/M2") return SQUARE_METER;
    if(uri == "http://qudt.org/vocab/unit/M2_P") return PLANCK_AREA;
    if(uri == "http://qudt.org/vocab/unit/M3") return CUBIC_METER;
    if(uri == "http://qudt.org/vocab/unit/M3-PER-HR") return CUBIC_METER_PER_HOUR;
    if(uri == "http://qudt.org/vocab/unit/M3-PER-KG") return CUBIC_METER_PER_KILOGRAM;
    if(uri == "http://qudt.org/vocab/unit/M3-PER-SEC") return CUBIC_METER_PER_SECOND;
    if(uri == "http://qudt.org/vocab/unit/M3-PER-SEC2") return CUBIC_METER_PER_SECOND_SQUARED;
    if(uri == "http://qudt.org/vocab/unit/MI") return INTERNATIONAL_MILE;
    if(uri == "http://qudt.org/vocab/unit/MI2") return SQUARE_MILE;
    if(uri == "http://qudt.org/vocab/unit/MI3") return CUBIC_MILE;
    if(uri == "http://qudt.org/vocab/unit/MI_US") return MILE_US_STATUTE;
    if(uri == "http://qudt.org/vocab/unit/MicroIN") return MICROINCH;
    if(uri == "http://qudt.org/vocab/unit/MicroM") return MICROMETER;
    if(uri == "http://qudt.org/vocab/unit/MilliM") return MILLIMETER;
    if(uri == "http://qudt.org/vocab/unit/MilliM3") return CUBIC_MILLIMETER;
    if(uri == "http://qudt.org/vocab/unit/NMI") return NAUTICAL_MILE;
    if(uri == "http://qudt.org/vocab/unit/OZ_US") return US_LIQUID_OUNCE;
    if(uri == "http://qudt.org/vocab/unit/PARSEC") return PARSEC;
    if(uri == "http://qudt.org/vocab/unit/PINT") return IMPERIAL_PINT;
    if(uri == "http://qudt.org/vocab/unit/PK") return US_PECK;
    if(uri == "http://qudt.org/vocab/unit/PT") return POINT;
    if(uri == "http://qudt.org/vocab/unit/PT_US") return US_LIQUID_PINT;
    if(uri == "http://qudt.org/vocab/unit/PlanckVolume") return PLANCK_VOLUME;
    if(uri == "http://qudt.org/vocab/unit/QT_US") return US_LIQUID_QUART;
    if(uri == "http://qudt.org/vocab/unit/ROD") return ROD;
    if(uri == "http://qudt.org/vocab/unit/RT") return REGISTER_TON;
    if(uri == "http://qudt.org/vocab/unit/ST") return STERE;
    if(uri == "http://qudt.org/vocab/unit/TBSP") return TABLESPOON;
    if(uri == "http://qudt.org/vocab/unit/TSP") return TEASPOON;
    if(uri == "http://qudt.org/vocab/unit/US_DRY_GAL") return DRY_GALLON_US;
    if(uri == "http://qudt.org/vocab/unit/US_DRY_PINT") return US_DRY_PINT;
    if(uri == "http://qudt.org/vocab/unit/US_DRY_QT") return US_DRY_QUART;
    if(uri == "http://qudt.org/vocab/unit/YD") return YARD;
    if(uri == "http://qudt.org/vocab/unit/YD2") return SQUARE_YARD;
    if(uri == "http://qudt.org/vocab/unit/YD3") return CUBIC_YARD;
    if(uri == "http://qudt.org/vocab/unit/YD3-PER-MIN") return CUBIC_YARD_PER_MINUTE;

    return UNKNOWN;
}

}

#endif /* QUDT_UNITS_H */
