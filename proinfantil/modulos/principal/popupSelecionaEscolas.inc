<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<?php
$qrpid = $_REQUEST['qrpid'];
$perid = QUESTAO_02;
$estuf = $_SESSION['estuf'];

if ($_REQUEST['grava']){
//	if($pflcod == SECRETARIA_ESTADUAL || $pflcod == SECRETARIA_MUNICIPAL || $pflcod == SUPER_USUARIO){
		
		$ent   = $_REQUEST['entid'];
		$perid = QUESTAO_02;
		$sql = "SELECT pinid FROM proinfantil.questionario WHERE qrpid = ".$qrpid;
		$pinid = $db->pegaUm( $sql );
		$sql = "SELECT count(pinid) FROM proinfantil.proinfantilescola WHERE pinid = ".$pinid;
		$conta = $db->pegaUm( $sql );
		if( $conta ){
			$sql = "DELETE FROM proinfantil.proinfantilescola WHERE pinid = ".$pinid;
			$db->executar( $sql );
		}
		$sql = "INSERT INTO proinfantil.proinfantilescola ( pinid, entid ) VALUES ( $pinid, $ent )";
		$db->executar($sql);
		$db->commit();
		ob_clean();
		$sql = "SELECT entnome FROM entidade.entidade WHERE entid = ".$ent;
		echo $db->pegaUm( $sql );
		exit();
}

echo "<table style='border-bottom: 0px' class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>";
echo "<tr bgcolor='e3e3e3'>";
echo "<td align='center'>";
echo "<b>Selecione a Escola.</b>";
echo "</td>";
echo "</tr>";
echo "<tr>";
echo "<td align='center'>";
echo "<b>Lista de Escolas.</b>";
echo "</td>";
echo "</tr>";

$muni = $_SESSION ['proinfantil']['muncod'];
$sql = "SELECT DISTINCT
				'<center>
					<img src=\"/imagens/alterar.gif\" style=\"cursor:pointer;\" border=0 title=\"Selecionar Escola\"  onclick=\"gravaEsc(\''||  t.entid ||'\',\''||  {$qrpid} ||'\');\">
				</center>'
			as acao,
			m.mundescricao    AS descricaoMunicipio,
            t.entcodent       AS codigoescola,
            entnome           AS nomeescola
		FROM
			entidade.entidade t
		INNER JOIN entidade.endereco                ende ON t.entid     = ende.entid
		INNER JOIN territorios.municipio            m    ON m.muncod    = ende.muncod  
		INNER JOIN territorios.estado               est  ON est.estuf   = ende.estuf  
		LEFT JOIN entidade.entidadedetalhe          entd ON t.entcodent = entd.entcodent
		WHERE 
		m.muncod = '$muni'
		AND t.entcodent IS NOT NULL
		AND t.entstatus = 'A'
		ORDER BY
			m.mundescricao, entnome";
$cabecalho = array("Selecione","Município","Código Inep","Nome");
$db->monta_lista($sql,$cabecalho,50,5,'N','95%',$par2);
?>
<script type="text/javascript">
function gravaEsc(entid, qrpid){
		jQuery.ajax({
			type: "POST",
			url: "proinfantil.php?modulo=principal/popupSelecionaEscolas&acao=A",
			data: "&grava=true&entid="+entid+"&qrpid="+qrpid ,
			success: function(msg){
				$('#nomeescola', window.opener.document).val(msg);
				window.close();
			}
		});
}

</script>