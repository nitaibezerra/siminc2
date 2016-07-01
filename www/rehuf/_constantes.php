<?

define('TPG_LFIXAS_SN',     1); // Linhas fixas sem subniveis
define('TPG_LFIXAS_CN',     2); // Linhas fixas com subniveis

define('TPG_CFIXAS_SN',     3); // Colunas fixas sem subniveis
define('TPG_CFIXAS_CN',     4); // Colunas fixas com subniveis

define('TPG_CFIXAS_PA',     7); // Colunas fixas por ano

define('TPG_LDINAM_OPCOES', 5);
define('TPG_LDINAM_TEXT',   6);

define('HOSPITALUNIV', 16);
define('HOSPITALFEDE', 93);

define('SISID', 27);


// Constantes para os perfis
define('PRF_ADMREHUF', 189);
define('PRF_CONSULTAHU', 194);
define('PRF_EQTECMEC', 190);
define('PRF_CONSULTAMEC', 191);
define('PRF_EQAPOIOHU', 192);
define('PRF_GESTORHU', 193);
define('PRF_GESTORHF', 533);
define('PRF_SUPERUSUARIO', 187);


// 	Constantes pada os documentos
//define('DOC_CADHU', 71);
//define('DOC_APROVACAOHU', 72);
// 	Constantes pada os documentos (PRODUÇÃO)
define('DOC_CADHU', 24);
define('DOC_APROVACAOHU', 25);

// Funções dos hospitais
$_funcoes = array(22,26,27,28,49);

/* Tabela rehuf.tipoitem */
define('TPIID_PD', 1); // Tipo de coluna padrão, utilizada na criação automatica da colunas fixas referenciando anos
define('TPIID_NUMERO', 2); // Tipo de coluna número, não possui formatação (inteiro)
define('TPIID_COMBO', 4); // Tipo de coluna combobox, mostrar os grupos cadastrados

/* TRATAMENTO DO LEGADO (mudança de regras a partir de 2008) */
define("MUDANCA_ANO1", "2008");
/* FIM TRATAMENTO DO LEGADO (mudança de regras a partir de 2008) */

// Constante de status //
define("ATIVO", "A");
define("INATIVO", "I");
define("PREENCHIMENTO_PREGAO", "N");
// FIM Constante status //

$_ANOS = array("2004","2005","2006","2007","2008","2009","2010","2011","2012","2013","2014","2015");

// QUESTIONARIO
if( $_SESSION['sisbaselogin'] == 'simec_desenvolvimento' ){
	//define("QUESTIONARIO_REHUF", 70);
	define("QUESTIONARIO_REHUF", 81);
	define("QUESTIONARIO_REHUF_AVALIADOR", 81);
} else {
	define("QUESTIONARIO_REHUF", 74);
	define("QUESTIONARIO_REHUF_AVALIADOR", 75);
}


/*
 * Fluxo do tipo do worflow, prestação de contas
 */

define("TIPO_DOCUMENTO_PRESTACAO_CONTAS",114);
//define("TIPO_DOCUMENTO_PRESTACAO_CONTAS",47);


/*
 * ID da tabela questionario.questionario usada na tela 'formularioPrestacaoContas.inc' 
 */
define("ID_QUESTIONARIO",85);
//define("ID_QUESTIONARIO",3);

/*
 * Estados do documento usana na tela 'formularioPrestacaoContas' 
 */

define("ESTADO_EM_ELABORACAO",709);
define("ESTADO_EM_ANALISE",711);
define("ESTADO_EM_AJUSTE",712);
define("ESTADO_BLOQUEADO",714);
define("ESTADO_ENCERRADO",716);
define("WF_ACAO_DESBLOQUEADO",1667);
define("ESTADO_NAO_INICIADO",6);



/*
 * Estados do documento usana na tela 'formularioPrestacaoContas' 
 */

//define("ESTADO_EM_ELABORACAO",323);
//define("ESTADO_EM_ANALISE",324);
//define("ESTADO_EM_AJUSTE",325);
//define("ESTADO_BLOQUEADO",326);
//define("ESTADO_ENCERRADO",327);

//constantes de CERTIFICADO
define("REHUF_HOSPITAL_ENSINO",1);
define("REHUF_HOSPITAL_AMIGO_CRIANCA",2);
define("REHUF_HOSPITAL_ACREDITACAO_CERTIFICACAO_QUALIDADE",3);
define("REHUF_HOSPITAL_OUTRAS",4);

?>