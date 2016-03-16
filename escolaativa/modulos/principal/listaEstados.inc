<?php
/**
 * Sistema Simec
 * Setor responsável: MEC
 * Desenvolvedor: Equipe Consultores Simec
 * Analista: Luiz Fernando Mendonça
 * Programador: Gustavo Fernandes da Guarda
 */

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

if($_SESSION['escolaativa']['boPerfilSupervisor']){
	if(!$_SESSION['escolaativa']['entid'] || !$_SESSION['escolaativa']['tecid']){
		header( "Location: escolaativa.php?modulo=principal/professorMultiplicador&acao=A&cpf=".$_SESSION['usucpf'] );
		exit;
	} elseif( (!isset($_SESSION['escolaativa']['arEstuf']) && !count($_SESSION['escolaativa']['arEstuf'])) && $_SESSION['escolaativa']['estuf'] ){
		header( "Location: escolaativa.php?modulo=principal/estruturaAvaliacao&acao=A" );
		exit;
	}
}

if($_SESSION['escolaativa']['boPerfilPesquisador']){
	if( (!isset($_SESSION['escolaativa']['arEstuf']) && !count($_SESSION['escolaativa']['arEstuf'])) && $_SESSION['escolaativa']['estuf'] ){
		header( "Location: escolaativa.php?modulo=principal/estruturaAvaliacao&acao=A" );
		exit;
	}
}

$_SESSION['escolaativa']['montipo'] = "E";

echo montarAbasArray( criaAbaEscolaAtiva(), "escolaativa.php?modulo=principal/listaEstados&acao=A" );

$texto = "<div style=\"font-size:12px\" >Senhores (as) Usuários (as),<br /><br />
Informamos que os Cadernos de Ensino e Aprendizagem, com adequações, do Programa Escola Ativa, constante deste módulo, encontra-se disponível para download.<br/>
Cadernos de Ensino e Aprendizagem, com adequações, do Programa Escola Ativa: <a target=\"_blank\" href=\"escolaativa.php?modulo=sistema/geral/downloads_usu&acao=A\" >clique aqui</a>.</div>";
popupAlertaGeral($texto,'450px',"250px");


monta_titulo( $titulo_modulo, '' );

$where = array();
if($_SESSION['escolaativa']['arEstuf']){
	array_push($where, "  estuf in ('". implode("','", $_SESSION['escolaativa']['arEstuf'])."') ");
}

$sql = "select '<center>
					<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abreArvore(\''|| estuf ||'\')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
				</center>' as acao, 
				estuf, estdescricao 
		from territorios.estado 
		" . ( is_array($where) && count($where) ? ' where ' . implode(' AND ', $where) : '' ) ."
		order by estdescricao
		";
$cabecalho = array("Ação","Sigla","Unidade da Federação");
$db->monta_lista($sql,$cabecalho,50,5,'N','95%',$par2);
?>
<script type="text/javascript">
function abreArvore(estuf){
	if(estuf){
		window.location.href = 'escolaativa.php?modulo=principal/estruturaAvaliacao&acao=A&estuf='+estuf;
	}
}
</script>