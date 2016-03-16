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

monta_titulo( $titulo_modulo, '' );
$where = array();
if($_SESSION['escolaativa']['arEstuf']){
	array_push($where, "  estuf in ('". implode("','", $_SESSION['escolaativa']['arEstuf'])."') ");
} elseif ($_SESSION['escolaativa']['estuf']){
	header( "Location: escolaativa.php?modulo=principal/listaEstadoProfSupervisor&acao=A&estuf=".$_SESSION['escolaativa']['estuf'] );
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
		window.location.href = 'escolaativa.php?modulo=principal/listaEstadoProfSupervisor&acao=A&estuf='+estuf;
	}
}
</script>