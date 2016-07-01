<?
define("TIPOENSINO_DEFAULT", 1);
define("SISID", 17);

define("PERFIL_ATUALIZACAO", 202);
define("PERFIL_ATUALIZACAO_UNI", 228);

define("PERFIL_CONSULTA", 203);
define("PERFIL_ADMINISTRADOR", 201);
define("PERFIL_ATENDENTE", 408);
define("PERFIL_ENCAMINHADOR", 407);

define("TIPOITEM_QTD", 3);

//Item
define("ITM_VAGAS_SUP",2);
define("ITM_INVESTIMENTO_SUP",5);
define("ITM_MAT_OFERTATUAL_PROF", 15);
define("ITM_MAT_PREVISTA_PROF", 11);
define("ITM_INVS_PREVISTO_PROF", 14);
define("ITM_INVS_REALIZADO_PROF", 18);

// tabela tipoensino
define("TPENSSUP", 1);
define("TPENSPROF", 2);

define("SITSOL_NINICIADO", 1);
define("SITSOL_EMATENDIMENTO", 2);
define("SITSOL_FINALIZADO", 3);


// lista do tipo de entidade universidades, ou centros profissionalizantes
$_tipoentidade = array(TPENSSUP => 12,
					   TPENSPROF => array(11,14));


// lista de funcoes (cargos) por tipo de ensino "1"-> Superior, "2"->"Profissional"
$_funcoes = array(TPENSSUP => array('campus' => 24, 'unidade' => 21),
				  TPENSPROF => array('campus' => 23, 'unidade' => 21)
				  );
/*
 * ALTERAÇÃO SOLICITADA POR WESLEY LIRA (19/05/09)
 * EFETUADA POR ALEXANDRE DOURADO
 * MUDANÇA DO CARGO DE DIRIGENTE DA UNIDADE DE DIRETOR GERAL
 * PARA REITOR 
 * 
$_funcoes = array(TPENSSUP => array('campus' => 24, 'unidade' => 21),
				  TPENSPROF => array('campus' => 23, 'unidade' => 22)
				  );
 */

// lista de funcoes (tipo de entidade, se é universidade ou centro tecnologico) por tipo de ensino "1"-> Superior, "2"->"Profissional"
$_funcoesentidade = array(TPENSSUP => array('campus' => 18, 'unidade' => 12),
				  		  TPENSPROF => array('campus' => 17, 'unidade' => 11)
				  		  );

define("INTERLOCUTORINS", 40);

// anos analisados por tipo de ensino
$anosanalisados[TPENSSUP] = array(2006,2007,2008,2009,2010,2011,2012);
$anosanalisados[TPENSPROF] = array(2006,2007,2008,2009,2010,2011,2012);
// anos analisados por default
$anosanalisados['default'] = array(2008,2009,2010,2011,2012);

//Títulos dos itens
$tituloitens[TPENSSUP] = "REUNI (Pactuação)";// tipo ensino: superior
$tituloitens[TPENSPROF] = "Expansão (Previsão)";// tipo ensino: profissional
$tituloitens['default'] = "Previsão";// tipo ensino: default

// Títulos dos cursos
$titulocursos[TPENSSUP] = "Detalhamento de vagas por curso";
$titulocursos[TPENSPROF] = "Detalhamento de matrículas por curso";
$titulocursos['default'] = "Vagas de curso";

?>