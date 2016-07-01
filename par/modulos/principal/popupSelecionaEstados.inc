<?php
$inuid = $_SESSION['par']['inuid'];
$qrpid = $_REQUEST['qrpid'];
$perid = QUESTOES_PONTUAIS_ESCOLAS;
$estuf = $_SESSION['par']['estuf'];
if( $_SESSION['par']['itrid'] == 2 ){
	$where = "AND ende.muncod = '{$_SESSION['par']['muncod']}'";
	$oMuninipio = new Municipio();
	$estuf = $oMuninipio->recuperarUF($_SESSION['par']['muncod']);
}

if ($_REQUEST['grava']){
//	if($pflcod == SECRETARIA_ESTADUAL || $pflcod == SECRETARIA_MUNICIPAL || $pflcod == SUPER_USUARIO){
		$qrpid = $_REQUEST['qrpid'];
		$ent = $_REQUEST['entid'];
		$perid = QUESTOES_PONTUAIS_ESCOLAS;
		$sql = "INSERT INTO par.questoespontuaisescolas ( qrpid, entid, perid ) VALUES ( $qrpid, $ent, $perid )";
		$db->executar($sql);
		$db->commit();
//	}
}

if ($_REQUEST['deleta']){
//	if($pflcod == SECRETARIA_ESTADUAL || $pflcod == SECRETARIA_MUNICIPAL || $pflcod == SUPER_USUARIO){
		$qrpid = $_REQUEST['qrpid'];
		$ent = $_REQUEST['entid'];
		$perid = QUESTOES_PONTUAIS_ESCOLAS;
		$sql = "DELETE FROM par.questoespontuaisescolas WHERE qrpid = $qrpid AND entid = $ent AND perid = $perid";
		$db->executar($sql);
		$db->commit();
//	}
}

echo "<table style='border-bottom: 0px' class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>";
echo "<tr bgcolor='e3e3e3'>";
echo "<td align='center'>";
echo "<b>Selecione as Escolas que tem CE implantado.</b>";
echo "</td>";
echo "</tr>";
echo "<tr>";
echo "<td align='center'>";
echo "<b>Lista de Escolas.</b>";
echo "</td>";
echo "</tr>";

// REGRAS DE NEGÓCIO PARA O SQL 
	// REGRA DE ESFERA  -> itrid = 1 (Estatual)
	//					-> itrid = 2 (Municipal)
 $tpcid = '';				
if( $_SESSION['par']['itrid'] == 1 ){
	$tpcid = "AND tpcid = 1";
}else if($_SESSION['par']['itrid'] == 2){
	$tpcid = "AND tpcid = 3";
}

$sql = "SELECT DISTINCT
			case when qpe.entid IS NOT NULL then
				'<center>
					<input type=checkbox checked value='||t.entid||' name=check_'||t.entid||' id=check_'||t.entid||' onclick=\"gravaEsc(\''||  t.entid ||'\',\''||  {$qrpid} ||'\');\">
				</center>'
			else
				'<center>
					<input type=checkbox value='||t.entid||' name=check_'||t.entid||' id=check_'||t.entid||' onclick=\"gravaEsc(\''||  t.entid ||'\',\''||  {$qrpid} ||'\');\">
				</center>'
			end
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
		LEFT JOIN par.questoespontuaisescolas       qpe  ON qpe.entid   = t.entid AND qpe.qrpid = {$qrpid} AND perid = {$perid}
		LEFT JOIN questionario.questionarioresposta qr   ON qr.qrpid    = qpe.qrpid       
		LEFT JOIN par.questoespontuais              qp   ON qp.qrpid    = qr.qrpid     
		LEFT JOIN par.instrumentounidade            iu   ON iu.inuid    = qp.inuid      
		WHERE 
		-- ende.muncod = '5103304' -- pode fazer a busca por inuid = 1 no lugar do muncod
		est.estuf = '{$estuf}'
		$where
		$tpcid
		AND t.entcodent IS NOT NULL
		AND t.entstatus = 'A'
		ORDER BY
			m.mundescricao, entnome";
$cabecalho = array("Selecione","Município","Código Inep","Nome");
$db->monta_lista($sql,$cabecalho,50,5,'N','95%',$par2);
?>
<script type="text/javascript">
function gravaEsc(entid, qrpid){
	campo = document.getElementById( 'check_' + entid );
	if (campo.checked){
		jQuery.ajax({
			type: "POST",
			url: "par.php?modulo=principal/popupSelecionaEstados&acao=A",
			data: "&grava=true&entid="+entid+"&qrpid="+qrpid ,
			success: function(msg){
				qntEsc = Number(eval($('#totalEscola', window.opener.document).val())) ; 
				soma = (qntEsc + 1);
				$('#totalEscola', window.opener.document).val(soma);
			}
		});
	} else {
		jQuery.ajax({
			type: "POST",
			url: "par.php?modulo=principal/popupSelecionaEstados&acao=A",
			data: "&deleta=true&entid="+entid+"&qrpid="+qrpid ,
			success: function(msg){
				qntEsc = Number(eval($('#totalEscola', window.opener.document).val())) ; 
				sub = (qntEsc - 1);
				$('#totalEscola', window.opener.document).val(sub);
			}
		});
	}
}

</script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>