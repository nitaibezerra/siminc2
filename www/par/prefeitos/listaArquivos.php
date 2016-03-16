<?php
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

if( !$db ){
	$db = new cls_banco();
}

if( $_REQUEST['requisicao'] == 'excluir' ){
	chmod($_REQUEST['dir'], 0666);
    unlink($_REQUEST['dir']);
    echo "<script>
    			alert('Operação Realizada com Sucesso!');
    			window.location.href = 'listaArquivos.php';
    		</script>";
    exit();
}


// pega o endereço do diretório
$diretorio = getcwd();
$diretorio = str_ireplace('\\', '/', $diretorio);

$ponteiro  = opendir($diretorio.'/pdfs');

while ($nome_itens = readdir($ponteiro)) {
    $itens[] = $nome_itens;
}

sort($itens);
$arrDir = array();
$arquivos = array();
$chave = 0;
foreach ($itens as $key => $listar) {
   if ($listar!="." && $listar!=".."){ 

   		if (is_dir($listar)) { 
   			//array_push($arrDir, array('pasta' => $listar));
			$pastas[]=$listar; 
		} else{ 
			$arquivos[$chave]=array('excluir' => '<input type="button" name="excluir" value="excluir" onclick="excluir(\''.$diretorio.'/pdfs/'.$listar.'\')">',
									'codigo' => $chave+1, 
									'arquivos' => '<a href="'.$diretorio.'/pdfs/'.$listar.'">'.$listar.'</a>', 
									'data' => date ("j/n/Y H:i:s", filectime($diretorio.'/pdfs/'.$listar))
									);
			$chave++;
			//array_push($arrDir, array('arquivos' => $listar));
		}
   }
}


?>
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<title>SIMEC - Municípios Fortes, Brasil Sustentável</title>
	<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
	<style type="text/css">
		.quebra    { page-break-before: always }
	</style>
</head>
<script>
function excluir(dir){
	window.location.href = 'listaArquivos.php?requisicao=excluir&dir='+dir;
}
</script>
<body>

		<?
		$count = 1;
		$cabecalho = array('Ações', 'Codigo', 'Descrição', 'Data');
		monta_titulo('Lista de Arquivos','');
		$db->monta_lista($arquivos, $cabecalho, 5000, 20, 'N','Center');
		/*foreach ($arquivos as $key => $v) {
			$key % 2 ? $cor = "#dedfde" : $cor = "";
			?>
			<tr bgcolor="<?=$cor ?>" id="tr_<?=$key; ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
				<td><?=$count ?></td>
				<td><a href="<?=$diretorio?>/pdfs/<?=$v['arquivos']?>"><?=$v['arquivos']?></a></td>
				<td><?=$v['data']?></td>
			</tr>
		<?
			$count++;
		}*/		
		?>
</body>
</html>