<?php 
function excluir(){
	
	global $db;
	
	$demandaArquivos	= new DemandaArquivos();
	$demandaArquivos->excluirArquivo();
	
	echo "<script type=\"text/javascript\">
			alert('Arquivo Excluído');
			document.location.href = document.location.href;
		  </script>";
}

function download(){
	
	require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
	$file = new FilesSimec(null,null,'obras2');
	return $file->getDownloadArquivo($_REQUEST['arqid']);
	
	echo '<script type="text/javascript">
			document.location.href = document.location.href;
		  </script>';
}

function salvar(){
	
	global $db;
	
	require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

	$campos	= array("demid"	=> $_SESSION['obras2']['demid'],
					"darnome" => "'".$_POST['darnome']."'");
	
	$file = new FilesSimec("demanda_arquivos", $campos, 'obras2');
	if($_FILES["arquivo"]){	
		
		$arquivoSalvo = $file->setUpload($_POST['darnome']);	
		if($arquivoSalvo){
			echo '<script type="text/javascript"> 
						alert("Arquivo anexado com sucesso.");
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
$_SESSION['obras2']['demid'] = $_REQUEST['demid'] ? $_REQUEST['demid'] : $_SESSION['obras2']['demid'];
$arAba = getArAba('caddemandas');
echo montarAbasArray($arAba, "?modulo=principal/preobra/cadDemandaArquivos&acao=A&demid=" . $_SESSION['obras2']['demid']);

$demandaArquivos 	= new DemandaArquivos();
$orgid 				= $_SESSION['obras2']['orgid'];

$orgid = $_SESSION['obras2']['orgid'];

monta_titulo( 'Arquivos da Demanda', '' );

$habilitado = true;
$habilita = 'S';

?>
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
		$('#formDemandaArquivos').submit();
	});
	$('.download').click(function(){
		$('#req').val('download');
		$('#arqid').val( $(this).attr('id') );
		$('#formDemandaArquivos').submit();
	});
	$('.excluir').click(function(){
		$('#req').val('excluir');
		$('#arqid').val( $(this).attr('id') );
		$('#formDemandaArquivos').submit();
	});
});
</script>
<form method="post" name="formDemandaArquivos" id="formDemandaArquivos" enctype="multipart/form-data">
	<input type="hidden" name="req"  	id="req"	value="" />
	<input type="hidden" name="arqid" 	id="arqid"	value="" />
	<input type="hidden" name="demid" value="<?=$_SESSION['obras2']['demid']?>" />
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="20%">Titulo do Arquivo:</td>
			<td><?=campo_texto('darnome','S',$habilita,'',43,100,'','', '', '', '', 'id="darnome"', '');?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Arquivo:</td>
			<td>
				<input type="file" name="arquivo" id="arquivo" class="obrigatorio"/>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td style="background-color:#DCDCDC" colspan="3" align="center">
				<?php if( $habilitado ){ ?>
				<input type="button" name="salvar" class="incluir" value="Anexar Arquivo"/>
				<?php }?>
			</td>
		</tr>
	</table>
</form>
<?php 
$param = Array("dartipo" => "D");
$sql    	= $demandaArquivos->listaSql( $param );
$cabecalho 	= array("Ação","Arquivo", "Data de Inclusão");
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario");
?>