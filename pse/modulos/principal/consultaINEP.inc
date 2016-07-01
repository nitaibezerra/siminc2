<?php
// monta cabeçalho
//include APPRAIZ . 'includes/cabecalho.inc';
include_once( APPRAIZ. "pse/classes/EstadoMunicipioPSE.class.inc" );
include  APPRAIZ . 'pse/modulos/principal/permissao.inc';

	global $db;

$muncod = $_SESSION['pse']["muncod"];
$nmMunEst = $_SESSION['pse']['flagmun'];
$obEstadomunicipiopse = new EstadoMunicipioPSE('',$_SESSION['pse']["muncod"] );
$empid = $obEstadomunicipiopse->empid;
	
	echo "<table style='border-bottom: 0px' class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>";
	echo "<tr>";
	echo "<td align='center'>";
	echo "<b>Pressione Ctrl+F para localizar a escola.</b>";
	echo "</td>";
	echo "</tr>";

	if($nmMunEst=='m'){
		$empflagestmun = '3';
	} else {
		$empflagestmun = '1';
	}
	
	if($_REQUEST["ano"] && $empid) {		
		$sql = "SELECT  distinct
				'<center>
					<a style=\"cursor:pointer;\" onclick=\"selecionaINEP(\''||  ent.entcodent ||'\',\''||  ent.entid ||'\');\">
						<img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\">
					</a>
				</center>' as acao, ent.entcodent, ent.entnome
			FROM 
				pse.escolamunpse2009 as esm, entidade.entidade  ent
			WHERE 
				esm.empid = {$empid} AND
				esm.entid =  ent.entid";
	} else {
		$sql = "SELECT distinct
					'<center>
						<a style=\"cursor:pointer;\" onclick=\"selecionaINEP(\''||  e.entcodent ||'\',\''||  e.entid ||'\');\">
							<img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
						</a>
					</center>' as acao, e.entcodent, e.entnome 
			     from entidade.entidade e, entidade.endereco en, entidade.funcaoentidade fe, territorios.municipio m
			     where (e.entescolanova = false or e.entescolanova is null) 
			     and e.entid = en.entid and en.tpeid = 1
			     and m.muncod = en.muncod
			     and fe.entid = e.entid
			     --and e.entid=entd.entid
			     and e.entnome <> '' 
			     and fe.funid = 3
			     and e.tpcid IN (1, 3)
			     AND e.entid=en.entid AND
			     en.muncod='{$muncod}'
				 
                order by
                	e.entnome";
	}					
		$cabecalho 		= array("Ação", "INEP","Escola");
		$db->monta_lista( $sql, $cabecalho, 500000, 10, 'N', 'center', '', '', '','');
	//}

?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>