$VERSION 0.2.3
	{
		10500 5900 679 430
		1.95 0.23 0.01
		0.012 2000.0
		0.3 0.3 1.8
		0.02 0.08 0.03

		0.065 0.065 0.38
		7.0 12.0
		12.0 12.0 11.0
		0.12 0.12 .35

		130 15.0 0.01 40
		0.0 0.0
		6.0
		0.01 0.003
		0.0 6.0
		8.0 0.8
		210
		<0.0 0.56 0.6> 0.00056
		1 1
		F-86
	}
	is >> fs.weight >> fs.max_thrust >> fs.max_speed >> fs.corner_speed;
	is >> fs.lift_factor >> fs.max_aoa >> fs.wing_aoa;
	is >> fs.drag_factor >> fs.idrag_factor ;
	is >> fs.drag_aoa >> fs.drag_yaw >> fs.drag_wb;
	is >> fs.drag_gr >> fs.drag_ab >> fs.drag_whls;
	is >> fs.control_pitch >> fs.control_yaw >> fs.control_roll;
	is >> fs.return_pitch >> fs.return_yaw;
	is >> fs.pitch_drag >> fs.yaw_drag >> fs.roll_drag;
	is >> fs.pitch_damp >> fs.yaw_damp >> fs.roll_damp;
	is >> fs.lspeed >> fs.l_z >> fs.l_aoa_max >> fs.max_wb;
	is >> fs.adv_yaw >> fs.adv_roll >> fs.g_height ;
	is >> fs.flap_lift >> fs.flap_drag;
	is >> fs.damp_time >> fs.wb_damp;
	is >> fs.g_limit >> fs.stall_spin;
	is >> fs.max_damage;
	is >> fs.view_point >> fs.max_aoa_factor;
	is >> fs.engine_type >> fs.flags;
	is >> fs.model;
	fs.model[8] = '\0';
 #define PROP_TYPE 0
 #define JET_TYPE 1
 #define JET_AB_TYPE 2
 #define TURBO_JET_TYPE 3
 #define RADAR_TYPE_MASK 0x07L
 #define PROP_TYPE 0
 #define JET_TYPE 1
 #define JET_AB_TYPE 2
 #define TURBO_JET_TYPE 3
 #define RADAR_TYPE_MASK 0x07L
 class Flight_Specs
 {
public:
	float weight;      /* weight in pounds */
	float mass;        /* mass in slugs (calculated, not in spec file) */
	float max_thrust;  /* maximum thrust in lbs/ft */
	float max_speed;   /* maximum speed in kts */
	float corner_speed; /* optimal control speed */
	float drag_factor; /* drag constant */
	float idrag_factor; /* constant for induced drag */
	float lift_factor; /* lift constant */
	float wing_aoa;  /* aoa of wing */
	float max_aoa;     /* beyond which, lift is 0 */
	/* Factors increasing drag */
	float drag_aoa, // drag produced by angle of attack
			drag_yaw, // drag produced by yaw
			drag_wb,  // wheel brakes drag
			drag_gr,  // landing gear drag
			drag_ab,  // air brakes drag
			drag_whls; // drag of wheels on ground
	/* Rotational factors */
	// Control factors
	float control_pitch;    // pitching moment produced by elevators/trim,
								// proportional to elevator positon (-30...+30)
	float control_yaw;   // yawing moment produced by rudders
								// proportional to rudder position (-30...+30)
	float control_roll;     // rolling moment produced by ailerons
								// proportional to ailerons (-30...+30)
	// Longitudinal & lattitidunal stability are
	// effected by these
	float return_pitch;     // pitching moment produced to return to 0 angle of attack,
								// proportional to angle of attack
	float return_yaw;               // yawing moment produced to return to 0 yaw
								// proportional to yaw
	// Rotational drag factors
	float pitch_drag;
	float yaw_drag;
	float roll_drag;
	// Damping limits
	float pitch_damp;
	float yaw_damp;
	float roll_damp;
	// landing limits
	float lspeed;                                   // maximum speed on ground
	float l_z;                                      // minimum vertical velocity
	float l_aoa_max;              // max aoa
	float max_wb;                                           // max vel for wheel brakes
	// Adverse yaw & roll factors
	float adv_yaw;                                          // Adverse yaw produced by roll
	float adv_roll;                                 // Adverse roll produced by yaw
	// More stuff!
	float g_height;               // height above ground, w/ gear down
	float flap_lift;                           // how much flaps contrib to lift
	float flap_drag;                                        //   and how much to drag
	float damp_time;                                        // how much time before applying damp ...
	float wb_damp;                                          // damping factor for wheel brakes
	float g_limit;                                          // limit on g's pulled
	float stall_spin;                                       // adverse spin produced by stalling
	int max_damage;                                 // Maximum amount of punishment
	// And even more stuff!!
	R_3DPoint view_point;                   // Where we look from when we're in
											// the cockpit (in 'port' coords)
	float max_aoa_factor;                   // for calc'ing max aoa as a function of vel
	int engine_type;                                        // 0 = prop, 1 = jet, 2 = jet with afterburner
	long flags;                                                     // general-purpose flags
	char model[9];

