<?php

define('APPRAIZ_VIEW', APPRAIZ . 'pet/classes/views/');

if ($_SESSION['baselogin'] == "simec_desenvolvimento") {
    define("PET_PERFIL_SUPER_USUARIO", 1319);
    define("PET_PERFIL_CLAA", 1327);
    define("PET_PERFIL_GESTOR_MEC", 1329 );
    define("PET_PERFIL_ADMINISTRADOR", 1328  );
} else {
	define("PET_PERFIL_SUPER_USUARIO", 1422);
	define("PET_PERFIL_CLAA", 1425);
	define("PET_PERFIL_GESTOR_MEC", 1423 );
	define("PET_PERFIL_ADMINISTRADOR", 1424  );
}