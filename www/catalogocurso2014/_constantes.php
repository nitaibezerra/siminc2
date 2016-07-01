<?php
// Estados Workflow
define("WF_EM_ELABORACAO", 1039);
define("WF_EM_ANALISE_GESTOR_CURSO", 1040);
define("WF_EM_VALIDADO_GESTOR", 1041);

// Perfil
define("PERFIL_ADMINISTRADOR", 1098);
define("PERFIL_CONSULTA", 1099);
define("PERFIL_SUPERUSUARIO", 1096);
define("PERFIL_GESTOR", 1097);
define("PERFIL_COORDENADOR", 1095);

//Modalidadedo Curso
define("MODALIDADE_PRESENCIAL", 	1);
define("MODALIDADE_SEMIPRESENCIAL", 2);
define("MODALIDADE_DISTANCIA", 		3);


// Tipo de Organizaчуo
define("TO_CRITERIO_IES", 1);

//Categoria Membro Equipe
define("CME_EQUIPE_UAB", 8);

//Funчуo Exercida - Publico Alvo
define("FE_DOCENTE", 1);

//Ano CENSO
if($_SESSION['exercicio']=='2014'){
	define("ANO_CENSO", 2013);
} else {
	define("ANO_CENSO", 2013);
} 

define("ANO_EXERCICIO_2014", 2014);
define("ANO_EXERCICIO_2015", 2015);

define("ABA_ABRAGENCIA", 14690);
?>