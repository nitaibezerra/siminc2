<?
include "includes/cabecalho.inc";

define("TIPO_ODBC", "unixODBC");
define("ODBC_DSN", "emdcamara");
define("MIN_UPLOAD_SIZE", "23MB");
define("DB_DIR", "/var/www/html/simec/temp/emd/");
define("DB_FILE_NAME", "db1.mdb");

?>
<br>
<?
$testes = new Testes;

$uploadmaxsize = ini_get("upload_max_filesize");
$postmaxsize = ini_get("post_max_size");

$teste = new Teste("Módulo ODBC", TIPO_ODBC, function_exists("odbc_connect") && ODBC_TYPE == TIPO_ODBC, ODBC_TYPE);
$testes->addTeste($teste);

$teste = new Teste("Permissão de escrita no diretório", DB_DIR, is_writable(DB_DIR));
$testes->addTeste($teste);

if(file_exists(DB_DIR . DB_FILE_NAME)) {
	$teste = new Teste("Permissão de escrita no arquivo", DB_DIR . DB_FILE_NAME, is_writable(DB_DIR . DB_FILE_NAME));
	$testes->addTeste($teste);

	$teste = new Teste("Conexão através do DSN", ODBC_DSN, (bool)odbc_connect(ODBC_DSN, "",""));
	$testes->addTeste($teste);
}

$teste = new Teste("Tamanho mínimo para upload", MIN_UPLOAD_SIZE, (int)MIN_UPLOAD_SIZE <= (int)$uploadmaxsize, $uploadmaxsize);
$testes->addTeste($teste);

$teste = new Teste("Tamanho mínimo para post", MIN_UPLOAD_SIZE, (int)MIN_UPLOAD_SIZE <= (int)$postmaxsize, $postmaxsize);
$testes->addTeste($teste);

//
// tratar envio da base de dados
if($testes->resultado() && is_uploaded_file($_FILES["arquivo"]["tmp_name"]) && !$_FILES["arquivo"]["error"] && $_FILES["arquivo"]["size"]) {
	//
	// se já existir uma versão do banco, fazer backup
	if (file_exists(DB_DIR . DB_FILE_NAME)) {
		$nomeBkpBanco = date("Ymd His") . "-" . $_SESSION["usucpf"] . ".mdb";
		$_SESSION["nomeBkpBanco"] = $nomeBkpBanco;
		copy(DB_DIR . DB_FILE_NAME, DB_DIR . $nomeBkpBanco);
		
	}
	
	move_uploaded_file($_FILES["arquivo"]["tmp_name"], DB_DIR . DB_FILE_NAME);
	
	$odbc = odbc_connect(ODBC_DSN, "", "");
	
	//
	// executar atualização
	dbg($_REQUEST["atualizar"]);
	foreach ($_REQUEST["atualizar"] as $tipo=>$atualizar) {
		switch ($tipo) {
			case "execucao":
				
				break;
			case "gnd":
				$query = "SELECT * FROM orzare_gnd";
				$rs = odbc_exec($odbc, $query);
				while($row = odbc_fetch_row($rs)) {
					dbg($row);
				}
				dbg($rs);
				break;
			case "elementosdespesa":
				
				break;		
			default:
				break;
		}
	}
	
}


$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Envio dos dados';
monta_titulo($titulo_modulo,'');
?>
<table width="95%" align="center" border="0" cellpadding="0" cellspacing="0" bgcolor="#E9E9E9" style="border-left: 1px solid #DCDCDC; border-right: 1px solid #DCDCDC">
  <tr>
	<td align="center"><table width='500px' align='center' border="0" cellspacing="0" cellpadding="2" bgcolor="#E9E9E9" style="border: 1px solid #DCDCDC;">
    <tr>
      <td valign="top" colspan="4" bgcolor="#DCDCDC"><b>Verificação de pré-requisitos:</b></td>
    </tr>
	<?php
	foreach ($testes as $teste) {
		?>
		<tr>
			<td width="70%">&middot; <?= $teste->nome ?></td>
			<td width="20%"><?= $teste->info ?></td>
			<td width="5%"><?= $teste->getImagem() ?></td>
			<td width="5%"><?= !$teste->status ? $teste->msg : "" ?></td>			
		</tr>
		<?
	}
	?>
</table></td>
  </tr>
  <tr>
	<td align="center">&nbsp;</td>
  </tr>
  <tr>
	<td align="center"><?
		if (!$testes->resultado()) {
		?>
	<table width='500px' align='center' border="0" cellspacing="0" cellpadding="2" bgcolor="#E9E9E9" style="border: 1px solid #DCDCDC;">
    <tr>
      <td bgcolor="#DCDCDC"><?= Teste::IMGERRO ?></td>
      <td width="100%" bgcolor="#DCDCDC"><b>Erro na verificação de pré-requisitos:</b></td>
    </tr>
    <tr>
      <td align="center" colspan="2">É preciso que todos os testes executados tenham resultado positivo para enviar a base de dados.</td>
    </tr>
    </table>
		<?
		}
		else {
		?>
		<form method="post" enctype="multipart/form-data">
		<table width='500px' align='center' border="0" cellspacing="0" cellpadding="2" bgcolor="#E9E9E9" style="border: 1px solid #DCDCDC;">
    <tr>
      <td valign="top" colspan="2" bgcolor="#DCDCDC"><b>Envio do banco de dados:</b></td>
    </tr>
	<?php if(file_exists(DB_DIR . DB_FILE_NAME)) { ?>
	<tr>
	  <td colspan="2" align="">Data de modificação do banco access: <?= date("d/m/Y H:i:s", filemtime(DB_DIR . DB_FILE_NAME)) ?></td>
	</tr>
	<? } ?>
    <tr>
      <td colspan="2" align="center"><input type="file" name="arquivo"/></td>
    </tr>
    <tr>
      <td align="center"><input type="checkbox" name="atualizar[execucao]" value="1" checked="checked"/></td>
      <td width="100%" align="left">Atualizar dados de execução</td>
    </tr>
    <tr>
      <td align="center"><input type="checkbox" name="atualizar[gnd]" value="1"/></td>
      <td width="100%" align="left">Atualizar GNDs (somente execute se tiver certeza)</td>
    </tr>
    <tr>
      <td align="center"><input type="checkbox" name="atualizar[elementosdespesa]" value="1"/></td>
      <td width="100%" align="left">Atualizar Elementos de Despesa (somente execute se tiver certeza)</td>
    </tr>
    <tr>
      <td colspan="2" align="center"><input type="submit" name="enviar" value="Enviar"/></td>
    </tr>
    </table></form><?
		}
		?></td>
  </tr>
  <tr>
	<td align="center">&nbsp;</td>
  </tr>
</table>
<br/>
<?
include "includes/rodape.inc";
?>