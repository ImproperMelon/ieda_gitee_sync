#===========================================================
##   init flow config
#===========================================================
flow_init -config ./iEDA_config/flow_config.json

#===========================================================
##   read db config
#===========================================================
db_init -config ./iEDA_config/db_default_config.json

#===========================================================
##   reset data path
#===========================================================
source ./script/DB_script/db_path_setting.tcl

#===========================================================
##   read lef
#===========================================================
source ./script/DB_script/db_init_lef.tcl

#===========================================================
##   read def
#===========================================================
def_init -path ./result/rt_0320.def

#===========================================================
##   Save def 
#===========================================================
#def_save -path ./result/data_out.def
#netlist_save -path ./result/iRT_result.v

#===========================================================
##   Exit 
#===========================================================
#flow_exit

report_db -path "./result/report/db.rpt"
#report_wirelength -path "./result/report/eval.rpt"
report_wirelength -path  "./result/report/wirelength.rpt"
report_congestion -path "./result/report/congestion.rpt"
check_all_net
