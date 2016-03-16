<?php

header('Content-Type: text/html; charset=utf-8');

// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );   
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

// carrega as funções gerais
include_once "config.inc";
include "verificasistema.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();


$uploaddir = 'uploaded_files/';
$uploadfile = $uploaddir . $_FILES['file']['name'];

$planilha = 1;


$script_select_all = '$(\'.chk\').prop( \'checked\' , \'checked\' );';
$script_unselect_all = '$(\'.chk\').removeProp( \'checked\' );';
$script_criar_query = '$(\'#form_lista\').submit()';

$select_all = '<a style=\'cursor:pointer\' href=\'#\' onclick="javascript:'. $script_select_all .'">Selecionar Todos</a>';
$unselect_all = '<a style=\'cursor:pointer\' href=\'#\' onclick="javascript:'. $script_unselect_all .'">Desselecionar Todos</a>';
$criar_query = '<a style=\'cursor:pointer\' href=\'#\' onclick="javascript:'. $script_criar_query .'">Criar query</a>';

$menu = '<ul>';

$menu .= '<li>'.$select_all.'</li>';

$menu .= '<li>'.$unselect_all.'</li>';

$menu .= '<li>'.$criar_query.'</li>';

$menu .= '</ul>';


$content = '';


if (move_uploaded_file($_FILES['file']['tmp_name'], $uploaddir . $_FILES['file']['name'])) {
    
	$arquivo = $uploaddir . $_FILES['file']['name'];

	$row = 0;
	$handle = fopen ($arquivo,"r");

	$content .= $menu;

	$content .= '<form name=\'form_lista\' method=\'post\' id=\'form_lista\' action=\'query_programas_de_trabalho.php\'><table>';
	while (($data = fgetcsv($handle, 1000, ";")) !== FALSE) {
	    
		$num = count ($data);
	    $content .= '<tr>';

	    if( $row != 0 ){
	    	$content .= '<td><input/ class=\'chk\' type=\'checkbox\' name=\'chk_'. $row .'\'></td>';
	    }else{
			$content .= '<td>&nbsp;</td>';	    	
	    }

	    for ($c=0; $c < $num; $c++) {

	    	if( $c == 2 && $row != 0 ){
	        	$content .= '<td style=\'border:1px solid #000\'><input name=\'dotacao_inicial[]\' value=\'' . $data[$c] . '\'/></td>';
	        }else{
	        	$content .= '<td style=\'border:1px solid #000\'>' . $data[$c] . '</td>';
	        }

	    }

		if( $row == 0 ){
     	   	$content .= '<td style=\'border:1px solid #000\'>Função</td>';
     	   	$content .= '<td style=\'border:1px solid #000\'>Sub Função</td>';
     	   	$content .= '<td style=\'border:1px solid #000\'>Programa</td>';
     	   	$content .= '<td style=\'border:1px solid #000\'>Ação</td>';
     	   	$content .= '<td style=\'border:1px solid #000\'>Localizador</td>';
    	}else{
    		$content .= '<td style=\'border:1px solid #000\'> <input type=\'text\' name=\'funcao[]\' style=\'width:30px\' value=\''. substr($data[0],0,2) .'\'/></td>';
    		$content .= '<td style=\'border:1px solid #000\'> <input type=\'text\' name=\'subfuncao[]\' style=\'width:40px\' value=\''. substr($data[0],2,3) .'\'/></td>';
    		$content .= '<td style=\'border:1px solid #000\'> <input type=\'text\' name=\'programa[]\' style=\'width:50px\' value=\''. substr($data[0],5,4) .'\'/></td>';
    		$content .= '<td style=\'border:1px solid #000\'> <input type=\'text\' name=\'acao[]\' style=\'width:50px\' value=\''. substr($data[0],9,4) .'\'/></td>';
    		$content .= '<td style=\'border:1px solid #000\'> <input type=\'text\' name=\'localizador[]\' style=\'width:50px\' value=\''. substr($data[0],13,4) .'\'/></td>';
    	}

	    $content .= '</tr>';

	    $row++;
	}
	$content .= '</table></form>';
	fclose ($handle);

} else {
    print "Erro ao fazer upload de arquivo:\n";
    print_r($_FILES);
}

?>

<!DOCTYPE html>
<html>
<head>
	<script type='text/javascript' src='http://code.jquery.com/jquery-1.9.1.min.js'></script>
</head>

<body>
	<?= $content ?>
</body>
</html>
