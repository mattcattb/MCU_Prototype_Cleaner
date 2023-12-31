#pragma once
// Electical Equation Functions

double zero_buffer = 0.001; // add to denominator to make sure never divide 0

// ====== Voltage Truck Equations ========

double no_load_V_truck(double Vt_0, double Pd_bias, double R_line){
    return (Vt_0 + (Pd_bias/(Vt_0 + zero_buffer))*R_line);
}

double one_load_V_truck(double Vt_1, double Pd_bias, double Pd_LEDL, double R_line){
    return (Vt_1 + ((Pd_LEDL + Pd_bias)*R_line)/(Vt_1+zero_buffer));
}

double two_load_V_truck(double Vt_2, double Pd_bias, double Pd_LEDL, double Pd_LEDR, double R_line){
    return (Vt_2 + (Pd_LEDL + Pd_LEDR + Pd_bias)*R_line/(Vt_2 + zero_buffer));
}

// ====== Power bias Equations

double Pd_bias_one_two(double Vt0, double Vt1, double Pd_LEDL, double R_line){
    // setting no load voltage truck equation equal to 1 load voltage truck to get Pd
    // pdbias = ((Vt0 - Vt1)/Rline - PdLED.l/Vt1)*(Vt0 * Vt1)/(Vt0 - Vt1) 

    double val_1 = (Vt0 - Vt1)/(R_line + zero_buffer);
    double val_2 = Pd_LEDL/(Vt1 + zero_buffer);
    double val_3 = (Vt0*Vt1)/(Vt0 - Vt1 + zero_buffer);

    return (val_1 - val_2)*val_3;
}

double Pd_bias_one_three(double Vt0, double Vt2, double Pd_LEDL, double Pd_LEDR, double R_line){
    // setting no load voltage truck equation equal to 2 load voltage truck

    double val1 = (Vt0 - Vt2)/(R_line + zero_buffer);
    double val2 = (Pd_LEDL + Pd_LEDR)/(Vt2 + zero_buffer);
    double val3 = (Vt0 * Vt2)/(Vt0 - Vt2 + zero_buffer);

    return (val1 - val2)*val3;
}

double Pd_bias_two_three(double Vt1, double Vt2, double Pd_LEDL, double Pd_LEDR, double R_line){
    // setting 1 load voltage truck equal to 2 load voltage truck equation

}

// ====== Resistance Line Equations

double R_LINE_EQ_One(double vt_0, double vt_1, double Pd_LEDL, double Pd_bias){
    // RLine = (Vt0 * VT1)/(PdLED.L * Vt0/(Vt0-Vt1) + Pdbias)
    return (vt_0 * vt_1)/(Pd_LEDL * vt_0/(vt_0-vt_1 + zero_buffer) + Pd_bias + zero_buffer);
}

double R_LINE_EQ_Two(double vt_0, double vt_1, double vt_2, double Pd_LEDL, double Pd_LEDR){
    // vt0, vt1, vt2, PdL, PdR
    double val1 = vt_2 - vt_1;
    double val2 = (Pd_LEDL + Pd_LEDR)/(vt_0-vt_2 + zero_buffer);
    double val3 = (Pd_LEDL)/(vt_0-vt_1 + zero_buffer);

    double rline = val1/(val2 - val3 + zero_buffer);
    return rline;
}
