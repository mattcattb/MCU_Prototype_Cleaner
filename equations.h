// Electical Equation Functions


double R_LINE_EQ(double vt_0, double vt_1, double Pd_LEDL, double Pd_bias){
    
}

// ====== Voltage Truck Equations ========

double no_load_V_truck(double Vt_0, double Pd_bias, double R_line){
    return (Vt_0 + (Pd_bias/Vt_0)*R_line);
}

double one_load_V_truck(double Vt_1, double Pd_bias, double Pd_LEDL, double R_line){
    return (Vt_1 + ((Pd_LEDL + Pd_bias)*R_line)/Vt_1);
}

double two_load_V_truck(double Vt_2, double Pd_bias, double Pd_LEDL, double Pd_LEDR, double R_line){
    return (Vt_2 + (Pd_LEDL + Pd_LEDR + Pd_bias)*R_line/Vt_2);
}

// ====== Power bias Equations

double Pd_bias_one_two(double Vt0, double Vt1, double Pd_LEDL, double R_line){
    // setting no load voltage truck equation equal to 1 load voltage truck to get Pd
    double val_1 = (Vt0 - Vt1)/R_line;
    double val_2 = Pd_LEDL/Vt1;
    double val_3 = (Vt0*Vt1)/(Vt0 - Vt1);

    return (val_1 - val_2)*val_3;
}

double Pd_bias_one_three(double Vt0, double Vt2, double Pd_LEDL, double Pd_LEDR, double R_line){
    // setting no load voltage truck equation equal to 2 load voltage truck

    double val1 = (Vt0 - Vt2)/R_line;
    double val2 = (Pd_LEDL + Pd_LEDR)/Vt2;
    double val3 = (Vt0 * Vt2)/(Vt0 - Vt2);

    return (val1 - val2)*val3;
}

double Pd_bias_two_three(double Vt1, double Vt2, double Pd_LEDL, double Pd_LEDR, double R_line){
    // setting 1 load voltage truck equal to 2 load voltage truck equation

}

// ====== Resistance Line Equations
