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
def_init -path ./result/iPL_result.def

report_wirelength -path  "./result/report/eval/iPL_result_wirelength.rpt"
report_congestion -path "./result/report/eval/iPL_result_congestion.rpt"

#===========================================================
##   Exit 
#===========================================================
flow_exit
