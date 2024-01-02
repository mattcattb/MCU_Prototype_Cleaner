
float no_load_V_truck(float Vt_0, float Pd_bias, float R_line);
float one_load_V_truck(float Vt_1, float Pd_bias, float Pd_LEDL, float R_line);
float two_load_V_truck(float Vt_2, float Pd_bias, float Pd_LEDL, float Pd_LEDR, float R_line);

float Pd_bias_one_two(float Vt0, float Vt1, float Pd_LEDL, float R_line);

float Pd_bias_one_three(float Vt0, float Vt2, float Pd_LEDL, float Pd_LEDR, float R_line);
float Pd_bias_two_three(float Vt1, float Vt2, float Pd_LEDL, float Pd_LEDR, float R_line);

float R_LINE_EQ_One(float vt_0, float vt_1, float Pd_LEDL, float Pd_bias);
float R_LINE_EQ_Two(float vt_0, float vt_1, float vt_2, float Pd_LEDL, float Pd_LEDR);

float zero_buffer = 0.001;  // add to denominator to make sure never divide 0

// ======================== Electrical Equations ==============================

// ====== Voltage Truck Equations ========

float no_load_V_truck(float Vt_0, float Pd_bias, float R_line){
    return (Vt_0 + (Pd_bias/(Vt_0 + zero_buffer))*R_line);
}

float one_load_V_truck(float Vt_1, float Pd_bias, float Pd_LEDL, float R_line){
    return (Vt_1 + ((Pd_LEDL + Pd_bias)*R_line)/(Vt_1+zero_buffer));
}

float two_load_V_truck(float Vt_2, float Pd_bias, float Pd_LEDL, float Pd_LEDR, float R_line){
    return (Vt_2 + (Pd_LEDL + Pd_LEDR + Pd_bias)*R_line/(Vt_2 + zero_buffer));
}

// ====== Power bias Equations

float Pd_bias_one_two(float Vt0, float Vt1, float Pd_LEDL, float R_line){
    // setting no load voltage truck equation equal to 1 load voltage truck to get Pd
    // pdbias = ((Vt0 - Vt1)/Rline - PdLED.l/Vt1)*(Vt0 * Vt1)/(Vt0 - Vt1) 

    float val_1 = (Vt0 - Vt1)/(R_line + zero_buffer);
    float val_2 = Pd_LEDL/(Vt1 + zero_buffer);
    float val_3 = (Vt0*Vt1)/(Vt0 - Vt1 + zero_buffer);

    return (val_1 - val_2)*val_3;
}

float Pd_bias_one_three(float Vt0, float Vt2, float Pd_LEDL, float Pd_LEDR, float R_line){
    // setting no load voltage truck equation equal to 2 load voltage truck

    float val1 = (Vt0 - Vt2)/(R_line + zero_buffer);
    float val2 = (Pd_LEDL + Pd_LEDR)/(Vt2 + zero_buffer);
    float val3 = (Vt0 * Vt2)/(Vt0 - Vt2 + zero_buffer);

    return (val1 - val2)*val3;
}

float Pd_bias_two_three(float Vt1, float Vt2, float Pd_LEDL, float Pd_LEDR, float R_line){
    // setting 1 load voltage truck equal to 2 load voltage truck equation
	float val1 = (Vt1 - Vt2)/(R_line + zero_buffer);
	float val2 = (Pd_LEDL+Pd_LEDR)/(Vt2 + zero_buffer);
	float val3 = Pd_LEDL/(Vt1 + zero_buffer);
	float denom = (Vt1 - Vt2)/(Vt1*Vt2 + zero_buffer);

	return (val1 - val2 + val3)/(denom + zero_buffer);

}

// ====== Resistance Line Equations

float R_LINE_EQ_One(float vt_0, float vt_1, float Pd_LEDL, float Pd_bias){
    // RLine = (Vt0 * VT1)/(PdLED.L * Vt0/(Vt0-Vt1) + Pdbias)
    return (vt_0 * vt_1)/(Pd_LEDL * vt_0/(vt_0-vt_1 + zero_buffer) + Pd_bias + zero_buffer);
}

float R_LINE_EQ_Two(float vt_0, float vt_1, float vt_2, float Pd_LEDL, float Pd_LEDR){
    // vt0, vt1, vt2, PdL, PdR
    float val1 = vt_2 - vt_1;
    float val2 = (Pd_LEDL + Pd_LEDR)/(vt_0-vt_2 + zero_buffer);
    float val3 = (Pd_LEDL)/(vt_0-vt_1 + zero_buffer);

    float rline = val1/(val2 - val3 + zero_buffer);
    return rline;
}