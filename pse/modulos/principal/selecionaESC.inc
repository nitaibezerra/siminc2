<?php
include_once( APPRAIZ. "pse/classes/EstadoMunicipioPSE.class.inc" );
include  APPRAIZ . 'pse/modulos/principal/permissao.inc';

	global $db;

$muncod = $_SESSION['pse']["muncod"];
$obEstadomunicipiopse = new EstadoMunicipioPSE('',$_SESSION['pse']["muncod"] );
$empid = $obEstadomunicipiopse->empid;
$check = new cls_banco();

$nmMunEst = $_SESSION['pse']['flagmun'];

if ($_REQUEST['grava']){
	if($pflcod == SECRETARIA_ESTADUAL || $pflcod == SECRETARIA_MUNICIPAL || $pflcod == SUPER_USUARIO){
		$emp = $_REQUEST['empid'];
		$ent = $_REQUEST['entid'];
		$sql = "select esmid from pse.escolamunpse2009 where empid = $emp AND entid = $ent";
		$teste = $db->pegaUm($sql);
		if($teste==''){
			$sql = "insert into pse.escolamunpse2009 (empid, entid) values ($emp, $ent)";
			$db->executar($sql);
			$sql = "UPDATE pse.estadomunicipiopse SET empquantescolapse2009 = (SELECT count(*) from pse.escolamunpse2009 WHERE empid={$empid}) WHERE empid = {$empid}";
			$db->executar($sql);
			$db->commit();
		}
	}
}

if ($_REQUEST['deleta']){
	if($pflcod == SECRETARIA_ESTADUAL || $pflcod == SECRETARIA_MUNICIPAL || $pflcod == SUPER_USUARIO){
		$emp = $_REQUEST['empid'];
		$ent = $_REQUEST['entid'];
		//verifica se a escola que ele esta querendo deletar já esta sendo usada na questão 03.
		$sql = "select entid from pse.pseanoescolamun where empid = $emp AND pamanoreferencia = 2009 AND entid = $ent";
		$teste = $db->pegaUm($sql);
		if($teste==''){
			$sql = "delete from pse.escolamunpse2009 where empid = $emp AND entid = $ent";
			$db->executar($sql);
			$sql = "UPDATE pse.estadomunicipiopse SET empquantescolapse2009 = (SELECT count(*) from pse.escolamunpse2009 WHERE empid={$empid}) WHERE empid = {$empid}";
			$db->executar($sql);
			$db->commit();
		} else {
			echo "1";
			exit;
		}
	}
}
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

	$sql = "SELECT distinct
						case when e.entid = esc.entid then
						'<center>
							<input type=checkbox checked value='||e.entid||' name=check_'||e.entid||' id=check_'||e.entid||' onclick=\"gravaEsc(\''||  e.entid ||'\',\''||  $empid ||'\');\">
						</center>'
						else
						'<center>
							<input type=checkbox value='||e.entid||' name=check_'||e.entid||' id=check_'||e.entid||' onclick=\"gravaEsc(\''||  e.entid ||'\',\''||  $empid ||'\');\">
						</center>'
						end as acao, e.entcodent, e.entnome, en.estuf, m.mundescricao 
				     from entidade.entidade e
				     LEFT JOIN pse.escolamunpse2009 esc ON esc.entid = e.entid AND esc.empid = $empid
				     --INNER JOIN entidade.entidadedetalhe entd ON e.entid=entd.entid 
				     INNER JOIN entidade.endereco en ON e.entid = en.entid and en.tpeid = 1
				     INNER JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
				     INNER JOIN territorios.municipio m ON m.muncod = en.muncod
				     where (e.entescolanova = false or e.entescolanova is null)
				     and e.entnome <> '' 
				     and e.entcodent <> '' 
				     and fe.funid = 3
				     and e.tpcid IN (1, 3)
				     AND e.entid=en.entid AND
				     en.muncod='{$muncod}'
					 
	                order by
	                	e.entnome";	
			$cabecalho 		= array("Ação", "INEP","Escola","UF","Município");
			$db->monta_lista( $sql, $cabecalho, 500000, 10, 'N', 'center', '', '', '','');
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>