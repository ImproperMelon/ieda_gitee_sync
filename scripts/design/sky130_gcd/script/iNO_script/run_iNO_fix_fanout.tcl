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
##   reset lib
#===========================================================
source ./script/DB_script/db_init_lib_fixfanout.tcl

#===========================================================
##   reset sdc
#===========================================================
source ./script/DB_script/db_init_sdc.tcl

#===========================================================
##   read lef
#===========================================================
source ./script/DB_script/db_init_lef.tcl

#===========================================================
##   read def
#===========================================================
def_init -path ./result/iFP_result.def

#===========================================================
##   run TO to fix fanout
#===========================================================
run_no_fixfanout -config ./iEDA_config/no_default_config_fixfanout.json

#===========================================================
##   save def 
#===========================================================
def_save -path ./result/iTO_fix_fanout_result.def

#===========================================================
##   save netlist 
#===========================================================
netlist_save -path ./result/iTO_fix_fanout_result.v -exclude_cell_names {}

#===========================================================
##   report db summary
#===========================================================
report_db -path "./result/report/fixfanout_db.rpt"

#===========================================================
##   Exit 
#===========================================================
flow_exit