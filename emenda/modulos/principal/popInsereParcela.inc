<?

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

if($_REQUEST["submetido"]) {
	$data = $_REQUEST["ano"]."-".$_REQUEST["mes"]."-01";
	
	$sql = "SELECT 
			  count(prdid) as total
			FROM 
			  emenda.ptparceladesembolso
			WHERE
				prddata = '$data'
			    AND ptrid = ".($_SESSION["emenda"]["ptrid"] ? $_SESSION["emenda"]["ptrid"] : $_SESSION["emenda"]["ptridAnalise"])."
			    AND prdtipo = '{$_REQUEST["tipo"]}'
			    AND prdminuta = '{$_REQUEST['estado']}'";
			    
	$total = $db->pegaUm($sql);
	
	if($total == 0){
		
		if( $_POST['prdid'] ){
			$sql = "UPDATE emenda.ptparceladesembolso SET
					  prddata = '{$data}'					 
					WHERE 
					  prdid = {$_POST['prdid']}";
			$db->executar($sql);
		} else {
			$sql = "INSERT INTO emenda.ptparceladesembolso(ptrid, prddata, prdtipo, prdminuta)
					VALUES (".($_SESSION["emenda"]["ptrid"] ? $_SESSION["emenda"]["ptrid"] : $_SESSION["emenda"]["ptridAnalise"]).", '".$data."', '".$_REQUEST["tipo"]."','".$_REQUEST['estado']."' )
					RETURNING prdid";
			$prdid = $db->pegaUm($sql);
			
			$sql = "INSERT INTO emenda.ptparcelainiciativa(prdid, ptiid, privalor) 
					select ".$prdid.",ptiid,null from emenda.ptiniciativa where ptrid = ".($_SESSION["emenda"]["ptrid"] ? $_SESSION["emenda"]["ptrid"] : $_SESSION["emenda"]["ptridAnalise"]);
			$db->executar($sql);
		}
		$db->commit();
		
		echo "<script type=\"text/javascript\">
				alert(\"Parcela criada com sucesso.\");
				window.opener.location.reload();
				self.close();
			  </script>";
		die;
	} else {
		echo "<script type=\"text/javascript\">
				alert(\"Já exite parcela cadastrada para este período!.\");
				//window.opener.location.reload();
			  </script>";
	}
}

$lebelButton = 'Criar Parcela';
if( $_REQUEST['prdid'] ){
	$lebelButton = 'Alterar Parcela';
	$sql = "SELECT prdid, ptrid, prddata, prdtipo, prdminuta
			FROM 
			  emenda.ptparceladesembolso WHERE prdid = {$_REQUEST['prdid']}";
	$arParcela = $db->pegaLinha( $sql );
	
	$mes = substr($arParcela['prddata'], 5, 2);
	$ano = substr($arParcela['prddata'], 0, 4);
}
?>

<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<form id="formInsereParcela" method="post" action="">
		<input type="hidden" name="submetido" value="1" />
		<input type="hidden" name="tipo" value="<?=$_REQUEST["tipo"]?>" />
		<input type="hidden" name="estado" value="<?=$_REQUEST["estado"]?>" />
		<input type="hidden" name="prdid" value="<?=$_REQUEST['prdid'] ?>" />
		<table width="100%" border="0" cellspacing="5" cellpadding="5" class="listagem">
			<thead>
				<tr>
					<td align="center" colspan="2" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Dados da Parcela</strong></td>
				</tr>
			</thead>
			<tbody>
				<tr bgcolor="#f4f4f4" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='#f4f4f4';">
					<td align="right"><b>Participante:</b></td>
					<td>
						<?=(($_REQUEST["tipo"]=='C') ? 'Concedente' : 'Proponente')?>
					</td>
				</tr>
				<tr bgcolor="#f4f4f4" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='#f4f4f4';">
					<td align="right"><b>Mês/Ano da Parcela:</b></td>
					<td>
					<?
						$sql = "SELECT mescod as codigo,mesdsc as descricao FROM public.meses ORDER BY mescod";
						$db->monta_combo("mes", $sql, 'S', "Selecione...", '', '', '', '100', 'N', '');
					?>
					&nbsp;/&nbsp;
					<select name="ano" class="CampoEstilo" style="width:100px;">
						<option value="">Selecione...</option>
					<?
						for($i=2009; $i<=(date('Y')+1); $i++) {
							if( $i == $ano ){
								echo '<option selected="selected" value="'.$i.'">'.$i.'</option>';
							} else {
								echo '<option value="'.$i.'">'.$i.'</option>';
							}
						}
					?>
					</select>
					</td>
				</tr>
				<tr bgcolor="#c0c0c0" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='#c0c0c0';">
					<td align="center" colspan="2">
						<input type="button" id="bt_criarparcela" value="<?=$lebelButton; ?>" onclick="criarParcela();" />
						&nbsp;
						<input type="button" id="bt_fechar" value="Fechar Janela" onclick="self.close();" />
					</td>
				</tr>
			</tbody>
		</table>
		</form>
	</body>
</html>

<script type="text/javascript">

function criarParcela() {
	var mes				= document.getElementsByName("mes")[0];
	var ano				= document.getElementsByName("ano")[0];
	var btCriarparcela	= document.getElementById("bt_criarparcela");
	var btFechar		= document.getElementById("bt_fechar");
	var form			= document.getElementById("formInsereParcela");
	
	btCriarparcela.disabled = true;
	btFechar.disabled 		= true;
	
	if(mes.value == "") {
		alert('O mês deve ser informado.');
		mes.focus();
		btCriarparcela.disabled = false;
		btFechar.disabled 		= false;
		return;
	}
	if(ano.value == "") {
		alert('O ano deve ser informado.');
		ano.focus();
		btCriarparcela.disabled = false;
		btFechar.disabled 		= false;
		return;
	}
	
	form.submit();
}

</script>