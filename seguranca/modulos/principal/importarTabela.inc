<?php
function importarTabela()
{
	global $db;
		
	$esquema = $_POST['esquema'];
	$tabela  = $_POST['tabela'];
	
	$sql = "SELECT
				count(*)
			FROM 
				pg_class
			JOIN 
				pg_namespace ON pg_namespace.oid = pg_class.relnamespace AND pg_namespace.nspname NOT LIKE 'pg_%'
			WHERE 
				pg_class.relname = '$tabela'
			AND
				pg_namespace.nspname = '$esquema'";
	$existe = $db->pegaUm($sql);
	if($existe){
		$_SESSION['seguranca']['importar_tabela']['msg'] = "Não foi possível importar a tabela, pois já existe a tabela $esquema.$tabela.";
		header("Location: seguranca.php?modulo=principal/importarTabela&acao=A");
		exit;
	}
	
	$csv = file($_FILES['arquivo']['tmp_name']);
		
	if($csv){
		foreach($csv as $n => $linha){
			if($n == 0)
			{
				$arrColunas = explode(";",trim($linha));
			}else
			{
				$arrValores[] = $linha;
			}		
		}
	}
	
	if($arrColunas){
		$sql = "CREATE TABLE $esquema.$tabela
				(
				  \"".implode("\" text, \"",$arrColunas)."\" text
				)
				WITH (
				  OIDS=FALSE
				);";
	}
	
	if($arrValores){
		foreach($arrValores as $valor){
			$sql.="insert into $esquema.$tabela values ('".implode("','",explode(";",str_replace(array("\"","'"),array("\\\"","\'"),trim($valor))))."');<br/>";
		}
	}
	//dbg($sql,1);
	if($sql){
		if($db->executar($sql)){
			$db->commit();
			$_SESSION['seguranca']['importar_tabela']['msg'] = "Tabela importada com sucesso.";
			header("Location: seguranca.php?modulo=principal/importarTabela&acao=A");
			exit;
		}else{
			$_SESSION['seguranca']['importar_tabela']['msg'] = "Não foi possível importar a tabela.";
			header("Location: seguranca.php?modulo=principal/importarTabela&acao=A");
			exit;
		}
	}
	
}

if($_POST['requisicao']){
	$_POST['requisicao']();
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
$titulo = str_replace("...","",$_SESSION['mnudsc']);
monta_titulo( $titulo, '&nbsp' );

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
jQuery.noConflict();

jQuery(function() {
	<?php if($_SESSION['seguranca']['importar_tabela']['msg']): ?>
		alert('<?php echo $_SESSION['seguranca']['importar_tabela']['msg'] ?>');
		<?php unset($_SESSION['seguranca']['importar_tabela']['msg']) ?>
	<?php endif; ?>
});
	
function importarTabela()
{
	var erro = 0;
	if(!jQuery("[name='esquema']").val())
	{
		alert('Favor informar o esquema.');
		erro = 1;
		return false;
	}
	if(!jQuery("[name='tabela']").val())
	{
		alert('Favor informar a tabela.');
		erro = 1;
		return false;
	}
	if(!document.getElementById('arquivo').value){
		alert('Favor selecionar o arquivo!');
		erro = 1;
		return false;
	}

	if(erro == 0)
	{
		document.getElementById('btn_importar').value = "Importando...";
		document.getElementById('btn_importar').disabled = "disabled";
		document.getElementById('form_importacao').submit();
	}
	
}
</script>
<form name="form_importacao" id="form_importacao"  method="post" action="" enctype="multipart/form-data" >
	<input type="hidden" name="requisicao" value="importarTabela" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubTituloDireita" >Observações:</td>
			<td>
				<p>A primeira linha do arquivo '.csv' deve conter o nome das colunas da tabela.</p>
				<p>O arquivo deve conter os dados separados por ponto e vírgula (;).</p>
				<p>Um exemplo de arquivo válido pode ser visualizado <a href="../csv_exemplo.csv" >aqui</a>.</p>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Esquema:</td>
			<td><?php echo campo_texto("esquema","S","S","",40,100,"","")?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Tabela:</td>
			<td><?php echo campo_texto("tabela","S","S","",40,100,"","")?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Arquivo (.csv):</td>
			<td><input type="file" name="arquivo" id="arquivo" /></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" ></td>
			<td>
				<input type="button" id="btn_importar" name="btn_importar" value="Importar Tabela" onclick="importarTabela()" />
			</td>
		</tr>
	</table>
</form>