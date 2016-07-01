<?php 
// empreendimento || obra || orgao
verificaSessao( 'obra' );

function excluir(){
	
	global $db;
	
	$obrasArquivos	= new ObrasArquivos( $_REQUEST['oarid'] );
	$obrasArquivos->popularDadosObjeto( Array('oarstatus' => 'I') )
				  ->salvar();
	$obrasArquivos->commit();
	
	echo "<script type=\"text/javascript\">
			alert('Operação realizada com sucesso.');
			document.location.href = document.location.href;
		  </script>";
}

function download(){
	
	require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
	$obraArquivo = new ObrasArquivos();
	$arDados = $obraArquivo->buscaDadosPorArqid( $_REQUEST['arqid'] );
	$eschema = ($arDados[0]['obrid_1'] ? 'obras' : 'obras2');
	
	$file = new FilesSimec(null,null,$eschema);
	return $file->getDownloadArquivo($_REQUEST['arqid']);
	
	echo '<script type="text/javascript">
			document.location.href = document.location.href;
		  </script>';
}

function salvar(){
	
	global $db;
	
	require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

	$campos	= array("obrid"		=> $_SESSION['obras2']['obrid'],
					"oardesc" 	=> "'".$_POST['oardesc']."'",
					"oardata" 	=> "'".formata_data($_POST['oardata'])."'",
					"tpaid" 	=> TIPO_OBRA_ARQUIVO_OUTROS,
					"oardata" 	=> " now()");
	
	$file = new FilesSimec("obras_arquivos", $campos, 'obras2');
	if($_FILES["arquivo"]){	
		
		$arquivoSalvo = $file->setUpload($_POST['oardesc']);	
		if($arquivoSalvo){
			echo '<script type="text/javascript"> 
						alert("Operação realizada com sucesso.");
						document.location.href = document.location.href;
				  </script>';
		}
	}
}


if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	die();
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
$_SESSION['obras2']['obrid'] = $_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid'];
$obrid = $_SESSION['obras2']['obrid'];
if( !$_SESSION['obras2']['obrid'] && !$_SESSION['obras2']['empid'] ){
	$db->cria_aba(ID_ABA_CADASTRA_OBRA_EMP,$url,$parametros);
}elseif( $_SESSION['obras2']['obrid'] ){
	if( $_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ){
		$db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros);
	}else{
		$db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros);
	}
}else{
	$db->cria_aba(ID_ABA_CADASTRA_OBRA,$url,$parametros);
}

$obrasArquivos 	= new ObrasArquivos();
$orgid 			= $_SESSION['obras2']['orgid'];

//$empreendimento = new Empreendimento( $_SESSION['obras2']['empid'] );
//$empreendimento->montaCabecalho();
echo cabecalhoObra($obrid);
echo "<br>";
monta_titulo( 'Galeria de Fotos', '' );

$habilitado = true;
$habilita = 'S';

if( possui_perfil( array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL, PFLCOD_CALL_CENTER, PFLCOD_CONSULTA_TIPO_DE_ENSINO) ) ){
	$habilitado = false;
	$habilita = 'N';
}
?>
<style>
.div:hover{
background-color:#FFFFCC;
}
</style>
<script src="/includes/calendario.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
 
$(document).ready(function(){
	$('[type="text"]').keyup();
	$('.incluir').click(function(){
		var stop = false;
		$('.obrigatorio').each(function(){
			if( $(this).val() == '' ){
				stop = true;
				alert('Campo obrigatório.');
				$(this).focus();
				return false;
			}
		});
		if( stop ){
			return false;
		}
		$('#req').val('salvar');
		$('#formObraArquivos').submit();
	});
	$('.download').click(function(){
		$('#req').val('download');
		$('#arqid').val( $(this).attr('id') );
		$('#formObraArquivos').submit();
	});
	$('.excluir').click(function(){
		if( confirm('Deseja excluir esta foto?') ){
			$('#req').val('excluir');
			$('#oarid').val( $(this).attr('id') );
			$('#formObraArquivos').submit();
		}
	});
});

function abrirGaleria(arqid, schema)
{
	window.open("../slideshow/slideshow/obras2_galeriaGaleriaFotos.php?tipo=abaGaleria&arqid=" + arqid ,"imagem","width=850,height=600,resizable=yes");
}
</script>
<form method="post" name="formObraArquivos" id="formObraArquivos" enctype="multipart/form-data">
	<input type="hidden" name="req"  	id="req"	value="" />
	<input type="hidden" name="arqid" 	id="arqid"	value="" />
	<input type="hidden" name="oarid"	id="oarid"	value="" />
	<input type="hidden" name="demid" 	value="<?=$_SESSION['obras2']['demid']?>" />
<?php 
	if( $habilitado ): 
?>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="20%">Descrição:</td>
			<td><?=campo_texto('oardesc','S',$habilita,'',43,100,'','', '', '', '', 'id="oardesc"', '');?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data da Foto:</td>
			<td><?=campo_data2('oardata','S',$habilita,'','##/##/####','','', '','', '', 'oardata' );?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Arquivo (foto):</td>
			<td>
				<input type="file" name="arquivo" id="arquivo" class="obrigatorio"/>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td style="background-color:#DCDCDC" colspan="3" align="center">
				<input type="button" name="salvar" class="incluir" value="Anexar Foto"/>
			</td>
		</tr>
	</table>
<?php
	endif;
?>
</form>

<?php
        $objObras = new Obras($obrid);
        $blockEdicao = $objObras->verificaObraVinculada();
        if($blockEdicao){
            echo '<script type="text/javascript">';
            echo " setTimeout(bloqueiaForm('formObraArquivos'), 500);
                   function bloqueiaForm(idForm){
                      jQuery('#'+idForm).find('input, textarea, button, select').attr('disabled','disabled');
                      jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                   }
                 ";
            echo '</script>';
        }
?>

<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>

		<td colspan="2" class="SubTituloCentro">
			<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
			<script type="text/javascript" src="../includes/remedial.js"></script>
			<script type="text/javascript" src="../includes/superTitle.js"></script>
			<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
			<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
			<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
			<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
			<script type="text/javascript">
			messageObj = new DHTML_modalMessage();	// We only create one object of this class
			messageObj.setShadowOffset(5);	// Large shadow
			
			function displayMessage(url) {
				messageObj.setSource(url);
				messageObj.setCssClassMessageBox(false);
				messageObj.setSize(690,400);
				messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
				messageObj.display();
			}
			function displayStaticMessage(messageContent,cssClass) {
				messageObj.setHtmlContent(messageContent);
				messageObj.setSize(600,150);
				messageObj.setCssClassMessageBox(cssClass);
				messageObj.setSource(false);	// no html source since we want to use a static message here.
				messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
				messageObj.display();
			}
			function closeMessage() {
				messageObj.close();	
			}

			</script>
			<?
			$arParam = array('obrid' => $_SESSION['obras2']['obrid']);
			$sql = $obrasArquivos->listaFotosSQL( $arParam );
			$fotos = ($db->carregar($sql));				

			if( $fotos ){
//				$_SESSION['imgparams'] = array("filtro" => "obrid = ".$_SESSION['obras2']['obrid'], 
//											   "tabela" => "obras2.obras_arquivos");
				
				for( $k=0; $k < count($fotos); $k++ ){

					$obraArquivo = new ObrasArquivos();
					$arDados = $obraArquivo->buscaDadosPorArqid( $fotos[$k]["arqid"] );
					
					$pathObras 	= APPRAIZ."arquivos/obras/".floor($fotos[$k]["arqid"]/1000)."/";
					$pathObras2 = APPRAIZ."arquivos/obras2/".floor($fotos[$k]["arqid"]/1000)."/";
					
					if( $_SESSION['usucpf'] == '' ){
						if( !is_file($pathObras2.$fotos[$k]["arqid"])
							&&  is_file($pathObras.$fotos[$k]["arqid"]) ){
							if( !is_dir($pathObras2) ){
								mkdir($pathObras2, 0777);
							}
							link( $pathObras.$fotos[$k]["arqid"], $pathObras2.$fotos[$k]["arqid"]);
						}
					}
					
//					$eschema = ($arDados[0]['obrid_1'] ? 'obras' : 'obras2');
					
					$divExcluir = "";
					
					if( $habilitado && $blockEdicao == false){
						$divExcluir = "<div class=\"excluir\" id=\"".$fotos[$k]["oarid"]."\" style=\"float:right;\">
				    						<img style=\"cursor:pointer; position:relative; 
				    									 z-index:10; top:0px; left:0px; \" 
				    							 width=\"20px\" src=\"../imagens/excluir_2.gif\" border=0 title=\"Excluir\" />
				    					</div>";
					}
					
					echo "<div class=\"div\" style=\"float:left; width:100px; height:122px; text-align:center; margin:10px;\" >
							<img title=\"".$fotos[$k]["arqdescricao"]."\" border='1px' id='".$fotos[$k]["arqid"]."' 
								 src='../slideshow/slideshow/verimagem.php?newwidth=64&newheight=64&arqid=".$fotos[$k]["arqid"]."&_sisarquivo=" . $eschema . "' 
								 hspace='0' vspace='0' style='position:relative; z-index:5; float:left; width:97px; height:97px;cursor:pointer;' 
								 onmouseover=\"return escape( '". $fotos[$k]["arqdescricao"] ."' );\" 
								 onclick=\"abrirGaleria(".$fotos[$k]["arqid"].")\"/> 
							<br>
							<div class=\"download\" id=\"".$fotos[$k]["arqid"]."\" style=\"float:left;margin-top:2px;\">
	    						<img style=\"cursor:pointer; position:relative; 
	    									 z-index:10; top:0px; left:0px; float:left;\" 
	    							 src=\"../imagens/salvar.png\" border=0 title=\"Doanload da Imagem\" />
	    					</div>
	    					<div style=\"float:left;margin-left:7px;margin-top:7px;\">
							" . $fotos[$k]["data"] . " 
							</div>
	    					$divExcluir
						  </div>";
					
				}
				
			}else {
				echo "Não existem fotos cadastradas";
			}
			?>
		</td>
	</tr>
</table>