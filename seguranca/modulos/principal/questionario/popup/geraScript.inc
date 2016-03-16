<?php
include_once APPRAIZ . "includes/classes/modelo/seguranca/Sistema.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QQuestionario.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QGrupo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QPergunta.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QItemPergunta.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QResposta.class.inc";
include_once APPRAIZ . "includes/classes/questionario/QGeraScript.class.inc";
include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/tabelas/Tabela.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/tabelas/Grupocoluna.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/tabelas/Grupolinha.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/tabelas/Linha.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/tabelas/Coluna.class.inc";

ini_set("memory_limit","1000M");
set_time_limit(0);

$obGera = new QGeraScript();
echo $obGera->montaArvoreQ();
?>