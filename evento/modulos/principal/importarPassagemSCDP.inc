<?php 
include  APPRAIZ."www/evento/geral/_funcoes_fatura_passagem.php";
require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if($_FILES){
	$tipo = explode('.',$_FILES['arquivo']['name']);
	$tipo = $tipo[1];
	if( $tipo == 'txt' ){
		## UPLOAD DE ARQUIVO
		$campos	= array("axsdescricao" => "'cargaSCDP'||now()");
			
		$file = new FilesSimec("anexofaturamento", $campos, 'evento');
			
		$file->setUpload('cargaSCDP');	
		
		$arq = APPRAIZ."arquivos/evento/".floor($file->getIdArquivo()/1000).'/'.$file->getIdArquivo();
		$fp = fopen($arq, 'r');

    	$texto = fread($fp,filesize($arq));
    	
		$linhas = explode(chr(10),$texto);
		$passagens = Array();
		$sql = "";
	    foreach($linhas as $k => $linha){
	    	if( $k != 0 ){
		        $dados = explode("|",$linha);
		        
		        if(!existePassagem($dados[0])&&$dados[0]!=''){
		        	$dados[0] = "'".$dados[0]."'";
			        $dados[1] = "'".$dados[1]."'";
			        $dados[2] = "'".$dados[2]."'";
			        $dados[3] = "'".$dados[3]."'";
			        $dados[4] = "'".$dados[4]."'";
			        $dados[5] = str_replace(Array(".",","),Array("","."),$dados[5]);
			        $dados[6] = explode('/',$dados[6]);
			        $dados[6] = "'".$dados[6][2].'/'.$dados[6][1].'/'.$dados[6][0]."'";
			        $dados[7] = "'".$dados[7]."'";
			        $dados[8] = str_replace(Array(".",","),Array("","."),$dados[8]);
			        $dados[9] = str_replace(Array(".",","),Array("","."),$dados[9]);
			        $dados[11] = str_replace(Array(".",","),Array("","."),$dados[11]);
			        $dados[12] = "'".$dados[12]."'";
			        $dados[13] = "'".$dados[13]."'";
			        foreach($dados as $k=>$dado){
			        	if($dado==''){
			        		$dados[$k]='null';
			        	}
			        }
			        unset($dados[15]);
//			        ver($dados,d);
			    	array_push($passagens,$dados);
			    	$sql .= "INSERT INTO evento.passagemscpd(
			    					psdnumpcdp, 
				    				psdnoproposto, 
				    				aeroidori,
			  						aeroiddes,
				    				psdnuvoo, 
							        psdtarifapraticada, 
							        psddtemissaobilhete, 
							        psdnubilhete, 
							        psdvlrmulta, 
							        psdvlrdifremarcacao_, 
							        psdanoempenho, 
							        psdnuempenho, 
							        psddsempenho, 
							        psdnoorgao, 
							        psdcontrole) 
							 VALUES(".implode(',',$dados).");
							 ";
		        }
	    	}
	    }
	    if($sql != ''){
	    	$db->executar($sql);
	    	$db->commit();
	    	echo "<script>
					alert('".count($passagens)." passagens adicionadas.');
					window.location = window.location;
				  </script>";	
	    }else{
	    	echo "<script>
					alert('Nenhuma nova passagem a adicionar.');
					window.location = window.location;
				  </script>";	
	    }
		
	}else{
		echo "<script>
				alert('Tipo de arquivo inválido!');
				window.location = window.location;
			  </script>";	
	}
}

if($_REQUEST['req']){
	$_REQUEST['req']($_REQUEST);
	die();
}

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Importar Passagens SCDC', '' );

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="geral/funcoes_curso.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript">

$(document).ready(function() {

	$('#importar').click(function(){
		if( $('#arquivo').val()!='' ){
			$('#formCargaPass').submit();
		}
	});

	$('#pesquisar').click(function(){

		jQuery('#pesquisar').attr('disabled',true);
		
		$.ajax({
			type: "POST",
			url: window.location,
			data: "req=listaPassagensSCDP&"+jQuery('#formListaPass').serialize(),
			async: false,
			success: function(msg){
//			alert(msg);
//			return false;
				jQuery('#listaPassagemSCDP').html(msg);
				jQuery('#pesquisar').attr('disabled',false);
			}
		});
	});

	$('#pesquisar').click();
});

</script>
<form method="post" enctype="multipart/form-data" name="formCargaPass" id="formCargaPass" action="">
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="30%">Arquivo de importação</td>
			<td>
				<input type="file" name="arquivo" id="arquivo" <?php echo $stAtivo ?>>	
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2" >
				<input type="button" id="importar" value="Importar" />
			</td>
		</tr>
	</table>
</form>
<br>
<?php monta_titulo( 'Pesquisar Passagens', '' );?>
<form method="post" name="formListaFat" id="formListaFat" action="">
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="30%">Código PCDC</td>
			<td>
				<?php 
					$numPCDC = $_POST['numPCDC']; 
					echo campo_texto( 'numPCDC', 'N', 'S', '', 16, 15, '[#]', '','','','','id="numPCDC"' ); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2" >
				<input type="button" id="pesquisar" value="Pesquisar" />
			</td>
		</tr>
	</table>
</form>
<div id="listaPassagemSCDP" >
</div>